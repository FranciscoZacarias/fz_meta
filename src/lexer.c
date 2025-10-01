
function Token_Array
load_all_tokens(Arena* arena, Lexer* lexer, String8 file_path)
{
  lexer = push_array(arena, Lexer, 1);
  lexer->arena = arena_alloc();

  // Load file into memory (pseudo)
  lexer->file              = os_file_load(lexer->arena, file_path); 
  lexer->file_start        = lexer->file.data.str;
  lexer->file_end          = lexer->file.data.str + lexer->file.data.size;
  lexer->current_character = lexer->file_start;
  lexer->line              = 1;
  lexer->column            = 0;

  Token_Array array = {0};
  array.tokens = push_array(lexer->arena, Token, TOKEN_ARRAY_SIZE);

  for(;;)
  {
    Token token = lex_token(lexer);
    if (array.count < TOKEN_ARRAY_SIZE)
    {
      array.tokens[array.count++] = token;
    }
    if (token.type == Token_End_Of_File) 
    {
      break;
    }
  }

  return array;
}

function Token
make_token(Lexer* lexer, Token_Type type, u8* start, u8* end)
{
  Token token        = {0};
  token.type         = type;
  token.value        = (String8){(u64)(end - start), start};
  token.start_offset = (u32)(start - lexer->file_start);
  token.end_offset   = (u32)(end - lexer->file_start);
  token.line         = lexer->line;
  token.column       = lexer->column;
  return token;
}

function void
advance(Lexer* lexer) 
{
  if (*lexer->current_character == '\n')
  {
    lexer->line++;
    lexer->column = 0;
  }
  else
  {
    lexer->column++;
  }
  lexer->current_character++;
}

function Token
lex_identifier_or_keyword(Lexer* lexer)
{
  u8* start = lexer->current_character;

  // must start with alpha or underscore
  if (!(char8_is_alpha(*lexer->current_character) || *lexer->current_character == '_'))
  {
    advance(lexer);
    return make_token(lexer, Token_Unknown, start, lexer->current_character);
  }

  while (char8_is_alpha(*lexer->current_character) ||
         char8_is_digit(*lexer->current_character) ||
         *lexer->current_character == '_')
  {
    advance(lexer);
  }

  u8* end = lexer->current_character;

  // reject if it's only underscores
  b32 only_underscores = 1;
  for (u8* c = start; c < end; c++)
  {
    if (*c != '_')
    {
      only_underscores = 0;
      break;
    }
  }

  if (only_underscores)
    return make_token(lexer, Token_Unknown, start, end);

  return make_token(lexer, Token_Identifier, start, end);
}


function Token
lex_number(Lexer* lexer)
{
  u8* start = lexer->current_character;
  while (char8_is_digit(*lexer->current_character)) 
  {
    advance(lexer);
  }
  return make_token(lexer, Token_Int_Literal, start, lexer->current_character);
}

function Token
lex_string(Lexer* lexer)
{
  advance(lexer); // skip opening quote
  u8* start = lexer->current_character;
  while (*lexer->current_character != '"' && *lexer->current_character != 0)
  {
    advance(lexer);
  }
  u8* end = lexer->current_character;
  if (*lexer->current_character == '"')
  {
    advance(lexer); // skip closing quote
  }
  return make_token(lexer, Token_String_Literal, start, end);
}

function Token
lex_token(Lexer* lexer)
{
  while (char8_is_space(*lexer->current_character))
  {
    advance(lexer);
  }

  u8 c = *lexer->current_character;
  if (c == 0)
  {
    return make_token(lexer, Token_End_Of_File, lexer->current_character, lexer->current_character);
  }

  switch (c)
  {
    case '@': advance(lexer); return make_token(lexer, Token_At,                lexer->current_character - 1, lexer->current_character);
    case '(': advance(lexer); return make_token(lexer, Token_Open_Parenthesis,  lexer->current_character - 1, lexer->current_character);
    case ')': advance(lexer); return make_token(lexer, Token_Close_Parenthesis, lexer->current_character - 1, lexer->current_character);
    case '{': advance(lexer); return make_token(lexer, Token_Open_Brace,        lexer->current_character - 1, lexer->current_character);
    case '}': advance(lexer); return make_token(lexer, Token_Close_Brace,       lexer->current_character - 1, lexer->current_character);
    case ',': advance(lexer); return make_token(lexer, Token_Comma,             lexer->current_character - 1, lexer->current_character);
    case '`': advance(lexer); return make_token(lexer, Token_Backtick,          lexer->current_character - 1, lexer->current_character);
    case '$': advance(lexer); return make_token(lexer, Token_Dollar,            lexer->current_character - 1, lexer->current_character);
    case '"': return lex_string(lexer);
  }

  if (char8_is_alpha(c))
  {
    return lex_identifier_or_keyword(lexer);
  }
  if (char8_is_digit(c))
  {
    return lex_number(lexer);
  }

  // Unknown token
  advance(lexer);
  return make_token(lexer, Token_Unknown, lexer->current_character - 1, lexer->current_character);
}
