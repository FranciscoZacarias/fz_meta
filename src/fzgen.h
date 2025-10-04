#ifndef FZG_H
#define FZG_H

#include "fz_base.h"

typedef enum FZG_Token_Type
{
  FZG_Token_Unknown = 0,

  // Identifiers (names of tables, columns, types, etc.)
  FZG_Token_Identifier,       

  // Literals
  FZG_Token_Number,      // numbers like 32
  FZG_Token_String_Literal,   // string literals if needed for code expansion

  // Symbols / punctuation
  FZG_Token_At,               // @
  FZG_Token_Open_Parenthesis, // (
  FZG_Token_Close_Parenthesis,// )
  FZG_Token_Open_Brace,       // {
  FZG_Token_Close_Brace,      // }
  FZG_Token_Comma,            // ,
  FZG_Token_Semicolon,        // ;  (maybe optional, depending on syntax)

  // Operators / special
  FZG_Token_Dollar,           // $ (for code expansion references)
  
  FZG_Token_End_Of_File,      // EOF
} FZG_Token_Type;


typedef struct FZG_Token FZG_Token;
struct FZG_Token
{
  FZG_Token_Type type;
  String8 value;
  u32 start_offset;
  u32 end_offset;
  u32 line;
  u32 column;
};

typedef struct FZG_Token_Array FZG_Token_Array;
struct FZG_Token_Array
{
  FZG_Token* tokens;
  u64 count;
};
#define FZG_TOKEN_ARRAY_SIZE 4096

typedef struct Lexer Lexer;
struct Lexer
{
  Arena* arena;

  File_Data file;
  u8* file_start;
  u8* file_end;
  u8* current_character;

  u32 line;
  u32 column;

  FZG_Token current_token;
};

typedef struct FZG_Table_Row FZG_Table_Row;
struct FZG_Table_Row
{
  String8* fields;
  u32 field_count;
};

typedef struct FZG_Table_Row_Node FZG_Table_Row_Node;
struct FZG_Table_Row_Node
{
  FZG_Table_Row_Node* next;
  FZG_Table_Row value;
};

typedef struct FZG_Table_Row_List FZG_Table_Row_List;
struct FZG_Table_Row_List
{
  FZG_Table_Row_Node* first;
  FZG_Table_Row_Node* last;
  u64 count;
};

typedef struct FZG_Table FZG_Table;
struct FZG_Table
{
  String8 name;

  String8* headers;
  u32 header_count;

  FZG_Table_Row* rows;
  u32 row_count;
};

typedef struct FZG_Template_Parameter FZG_Template_Parameter;
struct FZG_Template_Parameter
{
  u32 header_index; // E.g. For the headers (name, age, height), name -> header_index = 0, age -> header_index = 1 ...
  String8 variable;
};

typedef enum
{
  FZG_Command_None = 0,
  FZG_Command_Inline,
  FZG_Command_Foreach,
} FZG_Command_Kind;

#define FZG_MAX_TEMPLATE_PARAMETERS 16
typedef struct FZG_Command FZG_Command;
struct FZG_Command
{
  FZG_Command_Kind kind;
  String8 template_text;
  FZG_Template_Parameter template_parameters[FZG_MAX_TEMPLATE_PARAMETERS];
  u32 template_parameters_count;
};

#define MAX_COMMANDS 16
typedef struct FZG_Generator FZG_Generator;
struct FZG_Generator
{
  FZG_Table* table;
  FZG_Command command_queue[MAX_COMMANDS];
  u32 command_count;
};

#define FZG_MAX_TABLES 8
#define FZG_MAX_GENERATORS 16
typedef struct FZG_Context FZG_Context;
struct FZG_Context
{
  Arena* arena;

  String8 input_file;
  String8 output_file_name;
  String8 output_directory;

  FZG_Table* tables;
  u32 tables_count;

  FZG_Generator* generators;
  u32 generators_count;
};

// Globals
global FZG_Context fzg_context;

// Lexer
function FZG_Token_Array fzg_lexer_load_all_tokens(Arena* arena, Lexer* lexer, String8 file_path);
function FZG_Token       fzg_lexer_make_token(Lexer* lexer, FZG_Token_Type type, u8* start, u8* end);
function void            fzg_lexer_advance(Lexer* lexer);
function FZG_Token       fzg_lexer_identifier_or_keyword(Lexer* lexer);
function FZG_Token       fzg_lexer_number(Lexer* lexer);
function FZG_Token       fzg_lexer_string(Lexer* lexer);
function FZG_Token       fzg_lexer_token(Lexer* lexer);
function FZG_Token       fzg_lexer_backtick_block(Lexer* lexer);

// Generator
function void          fzg_init();
function FZG_Context*  fzg_generate(FZG_Token_Array* token_array);
function void          fzg_parse_table(FZG_Token_Array *tokens, u32 *i);
function void          fzg_parse_generator(FZG_Token_Array *tokens, u32 *i);
function void          fzg_expect_token(FZG_Token* token, FZG_Token_Type expected_type);
function FZG_Table_Row fzg_row_copy(Arena* arena, FZG_Table_Row source);
function FZG_Table*    fzg_get_table_by_name(String8 name);
function void          fzg_parse_template_text(FZG_Table* table, FZG_Command* command);
function void          fzg_write_generators();

#endif // FZG_H


