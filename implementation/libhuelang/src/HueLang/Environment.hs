{-# LANGUAGE FlexibleInstances #-}

module HueLang.Environment
    () where

import Data.Map (Map)
import qualified Data.Map as Map
import Data.Maybe

import Data.Stack

type Transformation = (Environment -> IO Environment)

instance Show Transformation where
  show t = "{transformation}"

instance Eq Transformation where
  (==) a b = False

instance Eq (Stack String) where
  (==) a b = show a == show b

instance Eq (Stack Value) where
  (==) a b = show a == show b

data Vtype = Vtype
  {
    wordt :: Maybe String,
    listt :: Maybe [Vtype]
  } deriving (Eq, Show)

data Value = Value
  { transformationv :: Maybe Transformation,
    strv :: Maybe String,
    intv :: Maybe Int,
    doublev :: Maybe Double,
    boolv :: Maybe Bool,
    listv :: Maybe [Value],
    vtype :: Vtype
  } deriving (Eq, Show)

simpleType :: String -> Vtype
simpleType name = Vtype {wordt=Just name,listt=Nothing}

emptyValue :: Value
emptyValue = Value
  {
    transformationv=Nothing,
    intv=Nothing,
    doublev=Nothing,
    boolv=Nothing,
    listv=Nothing,
    strv=Nothing,
    vtype=simpleType "undefined"
  }

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
  | otherwise = emptyValue

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

evalOne :: Environment -> IO Environment
evalOne rawEnv
  | vtype def == simpleType "primary" = fromJust (transformationv (getDefinition rawEnv "primary")) rawEnv
  | otherwise = evalOne pushedType
  where
    (env, word) = popExec rawEnv
    def = getDefinition env word
    pushedType = pushExec rawEnv (fromJust (wordt (vtype def)))

__primary :: Environment -> IO Environment
__primary env = fromJust (transformationv (getDefinition env stackTop)) envWithoutTop
  where
    (envWithoutTop, stackTop) = popExec env

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
    initFlag = setDefinition popped "__stopcomp" emptyValue{boolv=Just False, vtype=simpleType "flag"}
    condcompDef = fromJust $ listv $ getDefinition initFlag toEval -- Get the definition
    extractIndex def index = fromJust $ strv $ def !! index -- Extract a particular index
    firstWord = extractIndex condcompDef 0
    secondWord = extractIndex condcompDef 1
    pushedFirst = evalOne $ pushExec initFlag firstWord -- Push first word
    secondPart inenv -- If the stopcomp flag is enabled, stop composing.
      | fromJust (boolv (getDefinition inenv "__stopcomp")) = return inenv
      | otherwise = evalOne $ pushExec inenv secondWord -- Otherwise do compose.
      
defaultEnv :: Environment
defaultEnv = Environment
  {
    definitionTable = Map.fromList
    [
      ("primary",emptyValue{transformationv=Just __primary,vtype=simpleType "primary"}),
      ("undefined",emptyValue{transformationv=Just __undefined,vtype=simpleType "primary"}),
      ("preeval",emptyValue{transformationv=Just __preeval,vtype=simpleType "primary"})
    ],
    executionStack = stackNew,
    valueStack = stackNew
  }