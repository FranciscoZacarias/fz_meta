#ifndef TABLE_H
#define TABLE_H

typedef struct FZG_Field FZG_Field;
struct FZG_Field
{
  String8 key;   // field name (from the @table(...) header)
  String8 value; // field value (from each row)
};

typedef struct FZG_Row FZG_Row;
struct FZG_Row
{
  FZG_Field* fields;  // array of fields for this row
  u32 field_count;
};

typedef struct FZG_Table FZG_Table;
struct FZG_Table
{
  String8 name;     // table name, e.g. "UI_Stack_Table"

  String8* headers; // array of field names, from @table(...)
  u32 header_count;

  FZG_Row* rows;    // rows of the table
  u32 row_count;
};

function void fzg_generate(Arena* arena, Token_Array token_array);

#endif // TABLE_H
