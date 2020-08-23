{-# LANGUAGE TypeSynonymInstances, FlexibleInstances #-}

module HueLang.Environment
    ( someFunc ) where

import Data.Map (Map)
import qualified Data.Map as Map
import Data.Maybe
import Data.Either

import Data.Stack (Stack)
import Data.Stack

someFunc :: IO ()
someFunc = putStrLn "someFun"

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
    vtype=simpleType "undefined"
  }

data Environment = Environment
  { definitionTable :: Map String Value,
    executionStack :: Stack String,
    valueStack :: Stack Value
  } deriving (Eq, Show)

hasDefinition :: Environment -> String -> Bool
hasDefinition env word = (Map.member word (definitionTable env))

getDefinition :: Environment -> String -> Value
getDefinition env word
  | hasDefinition env word = fromJust (Map.lookup word (definitionTable env))
  | otherwise = emptyValue

evalUntilHalt :: Environment -> IO Environment
evalUntilHalt env
  | stackIsEmpty (executionStack env) = return env
  | otherwise = do
      innerEnv <- evalOne env
      evalUntilHalt innerEnv

evalOne :: Environment -> IO Environment
evalOne rawEnv
  | vtype def == simpleType "primary" = (fromJust (transformationv (getDefinition rawEnv "primary"))) rawEnv
  | otherwise = evalOne pushedType
  where
    pop = (fromJust (stackPop (executionStack rawEnv)))
    word = snd pop
    env = rawEnv{executionStack=fst pop}
    def = getDefinition env word
    pushedType = rawEnv{executionStack=stackPush (executionStack rawEnv) (fromJust (wordt (vtype def)))}

__primary :: Environment -> IO Environment
__primary env = (fromJust (transformationv (getDefinition env stackTop))) envWithoutTop
  where
    pop = (fromJust (stackPop (executionStack env)))
    stackTop = snd pop
    envWithoutTop = env{executionStack=fst pop}

__test :: Environment -> IO Environment
__test env = do
  putStrLn "Hue"
  return env

defaultEnv :: Environment
defaultEnv = Environment
  {
    definitionTable = Map.fromList
    [
      ("primary",emptyValue{transformationv=Just __primary,vtype=simpleType "primary"}),
      ("test",emptyValue{transformationv=Just __test,vtype=simpleType "primary"})
    ],
    executionStack = stackNew,
    valueStack = stackNew
  }
