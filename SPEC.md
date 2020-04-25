# HueLang language document

_Version 1.0_

This document attempts to describe the standard philosophy, behavior and implementation details of HueLang.

## Philosophy

HueLang is an imperative stack-based programming language designed with two main goals: speed and expandability. The base of the language itself shall be as simple as possible, thus allowing both developers and users to expand it in order to fulfill specific purposes in an efficient way. 

By following these principles, HueLang aims to be a possible replacement for a lot of modern fragmented technologies, unifying them under a single syntax and fundamental specification. For example: HueLang could be used for text composition, outputting TeX source code or even compiling directly to PDF documents. HueLang could also be used to develop web server applications, videogames, among others, all of them while mantaining its efficiency.

## Concepts

### Words

The main concept of HueLang is that of the ``word``: a string of characters that may or may not have a definition at a given point in time. Everything in HueLang starts as a ``word``: it is the linguistic interface between the programmer and the runtime environment of HueLang. 

When a word is evaluated, its definition is utilized as an instruction or set of instructions for the runtime environment to execute. For example, the word ``20`` could be evaluated as the numerical value ``20``, and instruct the environment to create an integer memory section to hold this value so that it can be used in further operations. However same word ``20`` could later be redefined to something else entirely, making it so that, for example when evaluated, it would save a certain file to disk.

#### Definition types

Regarding the definitions of words, there are primary and secondary definitions:

Primary definitions are those whose implementation is written in the host language of the runtime environment. For example, the word ``printf`` could be defined in such a way that when evaluated, the runtime environment calls the C function ``printf()``. These definitions provide the foundation of the language and allow the programmer to access functionality on the host system. 

Secondary definitions are simply those composed as an ordered set of other words. The specifics of this are further explained with the concept of compilation, seen below.

### Compilation
