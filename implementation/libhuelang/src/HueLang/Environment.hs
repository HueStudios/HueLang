{-# LANGUAGE FlexibleInstances #-}

module HueLang.Environment
    () where

import Data.Map (Map)
import qualified Data.Map as Map
import Data.Maybe

import Data.Stack

type Transformation = (Environment -> IO Environment)

instance Show Transformation where
  show t = "transformation"

instance Show Vtype where
  show (WordT name) = name
  show (ListT _) = "complex"

instance Show Value where
  show (TransformationV inner t) = show t ++ " {" ++ show inner ++ "}"
  show (StringV inner t) = show t ++ " {" ++ show inner ++ "}"
  show (IntV inner t) = show t ++ " {" ++ show inner ++ "}"
  show (DoubleV inner t) = show t ++ " {" ++ show inner ++ "}"
  show (BoolV inner t) = show t ++ " {" ++ show inner ++ "}"
  show (ListV inner t) = show t ++ " {" ++ show inner ++ "}"
  show (EmptyV t) = show t ++ " {}"

instance Eq Transformation where
  (==) a b = True

instance Eq (Stack String) where
  (==) a b = show a == show b

instance Eq (Stack Value) where
  (==) a b = show a == show b

data Vtype = WordT String | ListT [Vtype] deriving Eq

data Value = TransformationV Transformation Vtype
           | StringV String Vtype
           | IntV Int Vtype
           | DoubleV Double Vtype
           | BoolV Bool Vtype
           | EmptyV Vtype
           | ListV [Value] Vtype deriving (Eq)

getType :: Value -> Vtype
getType (TransformationV _ t) = t
getType (StringV _ t) = t
getType (IntV _ t) = t
getType (DoubleV _ t) = t
getType (BoolV _ t) = t
getType (ListV _ t) = t
getType (EmptyV t) = t

data Environment = Environment
  { definitionTable :: Map String Value,
    executionStack :: Stack String,
    valueStack :: Stack Value
  } deriving (Eq, Show)

hasDefinition :: Environment -> String -> Bool
hasDefinition env word = Map.member word (definitionTable env)

getDefinition :: Environment -> String -> Value
getDefinition env word
  | hasDefinition env word = fromJust (Map.lookup word (definitionTable env))
  | otherwise = EmptyV $ WordT "undefined"

setDefinition :: Environment -> String -> Value -> Environment
setDefinition env word value = env {definitionTable = Map.insert word value (definitionTable env)}

evalUntilHalt :: Environment -> IO Environment
evalUntilHalt env
  | stackIsEmpty (executionStack env) = return env
  | otherwise = do
      preEnv <- evalOne $ pushExec env "preeval"
      innerEnv <- evalOne preEnv
      evalUntilHalt innerEnv

popExec :: Environment -> (Environment, String)
popExec env = (nenv, word)
  where
    (nstack, word) = fromJust (stackPop (executionStack env))
    nenv = env{executionStack=nstack}

pushExec :: Environment -> String -> Environment
pushExec env word = nenv
  where
    nenv = env{executionStack=stackPush (executionStack env) word}

pushValue :: Environment -> Value -> Environment
pushValue env val = nenv
  where
    nenv = env{valueStack=stackPush (valueStack env) val}

evalOne :: Environment -> IO Environment
evalOne rawEnv
  | defType == WordT "primary" = case def of TransformationV _ _ -> primDef rawEnv
                                             _ -> error "Primary definition must be a transformation"
  | otherwise = evalOne pushedType
  where
    primDef = case getDefinition rawEnv "primary" of TransformationV p _ -> p
                                                     _ -> error "Definition of primary must be a transformation"
    (env, word) = popExec rawEnv
    def = getDefinition env word
    defType = getType def
    pushedType = case defType of WordT w -> pushExec rawEnv w
                                 _ -> error "Definition type must be a simple word"


__primary :: Environment -> IO Environment
__primary env = case def of TransformationV trans (WordT "primary") -> trans envWithoutTop
                            _ -> error "Primary definition must be a transformation"
  where
    (envWithoutTop, stackTop) = popExec env
    def = getDefinition envWithoutTop stackTop

__undefined :: Environment -> IO Environment
__undefined env = do
  let (nenv, word) = popExec env
  putStrLn ("Word >" ++ word ++ "< is undefined")
  return nenv

__preeval :: Environment -> IO Environment
__preeval = return

__condcomp :: Environment -> IO Environment
__condcomp env = do
  inter <- pushedFirst
  secondPart inter
    where
      (popped, toEval) = popExec env -- Pop word from the top
      initFlag = setDefinition popped "__stopcomp" $ BoolV False $ WordT "flag"
      condcompDef = case getDefinition initFlag toEval of ListV l (WordT "condcomp") -> l -- Get the definition
                                                          _ -> error "Condcomp def must be a list"
      extractIndex def index = case def !! index of StringV s _ -> s -- Extract a particular index
                                                    _ -> error "Condcomp def element must be a string"
      firstWord = extractIndex condcompDef 0
      secondWord = extractIndex condcompDef 1
      pushedFirst = evalOne $ pushExec initFlag firstWord -- Push first word
      stopComp e = case getDefinition e "__stopcomp" of BoolV b (WordT "flag") -> b
                                                        _ -> error "Stopcomp flag must be a bool"
      secondPart inenv -- If the stopcomp flag is enabled, stop composing.
        | stopComp inenv = return inenv
        | otherwise = evalOne $ pushExec inenv secondWord -- Otherwise do compose.

__wordcomprehension :: Environment -> IO Environment
__wordcomprehension env = return nenv
  where
    (poppedExec, stackTop) = popExec env
    pushed = pushValue poppedExec $ StringV (tail stackTop) $ WordT "word"
    definedFlag = setDefinition pushed "__stopcomp" $ BoolV True $ WordT "flag"
    nenv
      | head stackTop == ':' = definedFlag
      | otherwise = env
      
defaultEnv :: Environment
defaultEnv = Environment
  {
    definitionTable = Map.fromList
    [
      ("primary",TransformationV __primary $ WordT "primary"),
      ("undefined",ListV [StringV "wordcomprehension" $ WordT "undefined",
                          StringV "__undefined-final" $ WordT "undefined"] $ WordT "condcomp"),
      ("__undefined-final",TransformationV __undefined $ WordT "primary"),
      ("preeval",TransformationV __preeval $ WordT "primary"),
      ("wordcomprehension",TransformationV __wordcomprehension $ WordT "primary"),
      ("condcomp",TransformationV __condcomp $ WordT "primary")
    ],
    executionStack = stackNew,
    valueStack = stackNew
  }
