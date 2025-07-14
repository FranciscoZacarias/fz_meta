#ifndef MAIN_H
#define MAIN_H

#define FZ_ENABLE_CONSOLE 1
#include "fz_include.h"

// *.h
#include "lexer.h"

// *.c
#include "lexer.c"

#define Table_Flag_Enum    (1 << 0)
#define Table_Flag_String8 (1 << 1)
#define Table_Flag_Cstring (1 << 2)

typedef struct Table_Gen {
  Arena* arena;
  u32 tables_flags; // Which tables to generate
  String8 table_name;
  String8* values;  // Values for each table
  u32 values_count;
} Table_Gen;

internal void parse_table_arguments(Lexer* lexer, Table_Gen* table);
internal void parse_table_values(Lexer* lexer, Table_Gen* table);
internal void print_table(Table_Gen table);

#endif // MAIN_H