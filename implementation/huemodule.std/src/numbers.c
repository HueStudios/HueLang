#if INTERFACE
#include "environment.h"
#include <stdint.h
#define I8WORD "i8"
#define I16WORD "i16"
#define I32WORD "i32"
#define I64WORD "i64"
#define U8WORD "u8"
#define U16WORD "u16"
#define U32WORD "u32"
#define U64WORD "u64"
#define F32WORD "f32"
#define F64WORD "f64"
#define INTWORD "int"
#define SINTWORD "sint"
#define UINTWORD "uint"
#define FLOATWORD "float"
#define NUMBERWORD "number"
#endif

#include "numbers.h"

void Numbers_Initialize(Environment *env) {
  Word numberword = DefinitionTable_TokToWord(env->definition_table, NUMBERWORD);
  Types_RegisterAtomicType(env, numberword, 0);
  
}
