#ifndef LEXER_H
#define LEXER_H

typedef enum Token_Type
{
  Token_Unknown = 0,

  // Identifiers (names of tables, columns, types, etc.)
  Token_Identifier,       

  // Literals
  Token_Int_Literal,      // numbers like 32
  Token_String_Literal,   // string literals if needed for code expansion

  // Symbols / punctuation
  Token_At,               // @
  Token_Open_Parenthesis, // (
  Token_Close_Parenthesis,// )
  Token_Open_Brace,       // {
  Token_Close_Brace,      // }
  Token_Comma,            // ,
  Token_Semicolon,        // ;  (maybe optional, depending on syntax)
  Token_Backtick,         // `  (for code expansion blocks)
  
  // Operators / special
  Token_Dollar,           // $ (for code expansion references)
  
  Token_End_Of_File,      // EOF
} Token_Type;


typedef struct Token Token;
struct Token
{
  Token_Type type;
  String8 value;
  u32 start_offset;
  u32 end_offset;
  u32 line;
  u32 column;
};

typedef struct Token_Array Token_Array;
struct Token_Array
{
  Token* tokens;
  u64 count;
};
#define TOKEN_ARRAY_SIZE 4096

///////////////
// Lexer
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

  Token current_token;
};

function Token_Array load_all_tokens(Arena* arena, Lexer* lexer, String8 file_path);
function Token       make_token(Lexer* lexer, Token_Type type, u8* start, u8* end);
function void        advance(Lexer* lexer);
function Token       lex_identifier_or_keyword(Lexer* lexer);
function Token       lex_number(Lexer* lexer);
function Token       lex_string(Lexer* lexer);
function Token       lex_token(Lexer* lexer);

#endif // LEXER_H