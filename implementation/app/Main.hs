module Main where

import System.Console.Haskeline
import HueLang.Environment
import Control.Monad.IO.Class

inputEvalWord :: Environment -> String -> IO Environment
inputEvalWord env word = evalUntilHalt $ pushExec env word

main :: IO ()
main = runInputT defaultSettings (loop defaultEnv)
  where
    loop :: Environment -> InputT IO ()
    loop env = do
      minput <- getInputLine ">> "
      case minput of
        Nothing -> return ()
        Just "quit" -> return ()
        Just input -> do nenv <- liftIO $ inputEvalWord env input
                         loop nenv 
