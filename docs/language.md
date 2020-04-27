# HueLang language document

_Version 1.0_

This document attempts to describe the standard philosophy, behavior and implementation details of HueLang.

## Philosophy

HueLang is an imperative stack-based programming language designed with two main goals: speed and expandability. The base of the language itself shall be as simple as possible, thus allowing both developers and users to expand it in order to fulfill specific purposes in an efficient way. 

By following these principles, HueLang aims to be a possible replacement for a lot of modern fragmented technologies, unifying them under a single syntax and fundamental specification. For example: HueLang could be used for text composition, outputting TeX source code or even compiling directly to PDF documents. HueLang could also be used to develop web server applications, videogames, among others, all of them while mantaining its efficiency.

## Concepts

### Words

_"In the beginning were the words and the words made the world. I am the words. The words are everything. Where the words end the world ends." - [The Talos Principle](https://store.steampowered.com/app/257510/The_Talos_Principle/)_

The main concept of HueLang is that of the ``word``: a symbol that may or may not have a definition at a given point in time. Everything in HueLang starts as a ``word``: it is the linguistic interface between the programmer and the runtime environment of HueLang. 

Note that the concept of word is more general than that of a textual representation. Words could be images, sounds, or any manifestation in some other medium that can be used to represent symbols. However, this document will focus in words represented as a string of characters. Further generalization can hopefully be infered, and research into a non-textual implementation of HueLang is certainly something to aim for in the future.

When a word is evaluated, its definition is utilized as an instruction or set of instructions for the runtime environment to execute. For example, the word ``20`` could be evaluated as the numerical value ``20``, and instruct the environment to create an integer memory section to hold this value so that it can be used in further operations. However, the same word ``20`` could later be redefined to something else entirely, making it so that, for example when evaluated, it would save a certain file to disk.

#### Definitions

Regarding the definitions of words, there are primary and secondary definitions:

Primary definitions are those whose implementation is written in the host language of the runtime environment. For example, the word ``printf`` could be defined in such a way that when evaluated, the runtime environment calls the C function ``printf()``. These definitions provide the foundation of the language and allow the programmer to access functionality on the host system. 

Secondary definitions are simply those composed as an ordered set of other words. The specifics of this are further explained with the concept of compilation, seen below.

### Stacks

Being a stack-based languaged, it follows that stacks are a key part of HueLang. A stack is a data structure that allows the storage and retrieval of data, following a _last in, first out_ order. 

The HueLang runtime is composed of two stacks: execution and value.

#### Execution stack

The execution stack is used by the runtime to hold the words to be evaluated. When a word with a secondary definition is evaluated, the word is said to be "expanded" into its component words, and these are pushed to the execution stack. 

#### Value stack

When a word evaluates to a value (For example how the word ``20`` could evaluate to the integer 20), it is pushed to the value stack. Primary words may then pop values from the value stack, operate on them, and optionally push a result back.

### Compilation

Compilation is the process through which new secondary words are defined by the programmer. By default, this is done by evaluating a sequence of words like the following:

```
: [word_name] [component_words ...] ;
```

For example:

```
: sayhue "Hue" println ;
```

The built-in words for compilation are ``:`` and ``;``, but it must be noted that these words can and may be redefined by the user for a particular purpose. As such, the user should avoid redefining said words unless the intention is to modify the compilation process in some way. 

The ``:`` word sets the environment into a special state called "compilation state". In this state, the first word after the ``:`` is directly interpreted as the name of the word to be defined or re-defined. Further words are not evaluated to their definition, but are instead added to a buffer. As an exception, the user can define a word with a name such as ``[word_name]_compile``. This will make it so that when ``[word_name]`` is evaluated in compile mode, it it is evaluated to the definition of ``[word_name]_compile``. 

The definition of a secondary word may contain further usages of ``:`` and ``;``, implying that the word being defined will compile other words when evaluated. 

The compilation process finishes when the number of ``;`` word in the sequence is equal to the number of ``:`` words. When this happens, the compilation state is disabled, and the buffer is saved as the definition of the word.

### Forced evaluation

The runtime environment performs a multitude of tasks whenever a word on the execution stack must be evaluated. However, under some circumstances it may be desired for the runtime to evaluate a word without any such formalities. When this is done, it is said that the word is "forcibly evaluated".

### Word resolution

Word resolution is the process through which a word is resolved to its definition. Right before a word is evaluated, the word ``!preprocess`` is forcibly evaluated first (Note that since ``!preprocess`` is forcibly evaluated, it is not added to the execution stack for this evaluation, it is instead resolved and evaluated directly. Therefore, ``!preprocess`` can not preprocess itself or any other forcibly evaluated words). The definition of this word may then alter in-place the word that is to be evaluated. Redefining the ``!preprocess`` word allows the user to change the word that was to be evaluated, replacing it with something else. 

After the pre-processing stage, the runtime environment attempts to resolve the word by looking up the definition table. The definition table is a key-value data structure that resolves a word to its primary or secondary definition. If a definition is found on the definition table, this definition is used and the word resolution process ends. If no definition is found, the word ``!defaultresolve`` is forcibly evaluated. Redefining ``!defaultresolve`` allows the user to create non-exhaustive word definitions. For example, ``!defaultresolve`` could look for words with numeric formats and perform the work required to add them to the the value stack. If ``!defaultresolve`` successfully resolved a word, it must raise a ``resolved`` flag for the runtime to be aware of this fact. 

When a word couldn't be resolved either by looking up its definition or through a non-exhaustive method, The runtime environment sets its error message to ``"Unknown word"`` and the word ``handleerror`` is forcibly evaluated.

### Word literals

Under some circumstances it is useful to refer to a word as a value, instead of as its definition. This can be achieved through the use of quotations and word literals. 

By default, ``!defaultresolve`` shall be defined in such a way that it looks for words such as ``:[token]``. When it finds one, it adds the ``[token]`` as a word literal to the value stack. The ``:`` is called "quote" in this context.

### Vocabularies

Vocabularies are extensions to HueLang that may be invoked at any moment during runtime. This can be done by evaluating a sequence like ``:[vocabulary_name] #use``. When ``#use`` is evaluated, the vocabulary ``[vocabulary_name]`` is initialized, at which point it may alter any part of the runtime environment. This means it may define words, add new behaviour, and pretty much anything else.

It is up to the implementation to inform the user of the available vocabularies as well as the specifics of them, so long as they provide enough freedom and expandability to the language as possible.