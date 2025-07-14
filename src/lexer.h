global const char* token_type_names[] = {
  "Token_Unknown",

  "Token_Identifier",
  "Token_Declaration",
  "Token_Number",

  "Token_Table",
  "Token_Enum",
  "Token_String8",
  "Token_Cstring",

  "Token_Open_Parenthesis",  // (
  "Token_Close_Parenthesis", // )
  "Token_Open_Brace",        // {
  "Token_Close_Brace",       // }
  
  "Token_Equals",            // =
  "Token_Comma",             // ,

  "Token_Space",                // ' '
  "Token_Tab",                  // /t
  "Token_New_Line",              // \n, \r, or \r\n
  "Token_Comment_Line",          // //
  "Token_Comment_Block_Start",   // /*
  "Token_Comment_Block_End",     // */

  "Token_End_Of_File",

  "Token_Count",
};

typedef enum Token_Type {
  Token_Unknown = 0,

  Token_Identifier,
  Token_Declaration,
  Token_Number,

  Token_Table,
  Token_Enum,
  Token_String8,
  Token_Cstring,

  Token_Open_Parenthesis,  // (
  Token_Close_Parenthesis, // )
  Token_Open_Brace,        // {
  Token_Close_Brace,       // }
  
  Token_Equals,            // =
  Token_Comma,             // ,

  Token_Space,                // ' '
  Token_Tab,                  // /t
  Token_New_Line,              // \n, \r, or \r\n
  Token_Comment_Line,          // //
  Token_Comment_Block_Start,   // /*
  Token_Comment_Block_End,     // */

  Token_End_Of_File,

  Token_Count,
} Token_Type;

typedef struct Token {
  Token_Type type;
  String8 value;
  u32 start_offset;
  u32 end_offset;
} Token;

///////////////
// Lexer
typedef struct Lexer {
  Arena* arena;

  File_Data file;
  char8* file_start;
  char8* file_end;
  char8* current_character;

  u32 line;
  u32 column;

  Token current_token;
} Lexer;

internal void  lexer_init(Lexer* lexer, String8 file_path);
internal void  advance_token(Lexer* lexer);
internal void  advance_character(Lexer* lexer);
internal void  make_token_range(Lexer* lexer, Token_Type type, char8* start, char8* end);
internal void  make_token_current(Lexer* lexer, Token_Type type, u32 length);
internal char8 peek_character(Lexer* lexer, u32 offset);
internal b32   is_character_trivia(Lexer* lexer);
internal b32   is_token_trivia(Token token);
internal void  eat_trivia(Lexer* lexer);
internal void  make_identifier(Lexer* lexer);
internal void  make_number(Lexer* lexer); /* We only handle int */
internal void  try_to_make_keyword(Lexer* lexer);

internal char8 current_character(Lexer* lexer);
internal Token current_token(Lexer* lexer);
internal b32   at_eof(Lexer* lexer);
internal u32   character_offset(Lexer* lexer, char8* character);

internal void print_current_token(Lexer* lexer);