#ifndef TABLE_H
#define TABLE_H

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

internal void generate_table(Lexer* lexer, String8 output_file);

#endif // TABLE_H
