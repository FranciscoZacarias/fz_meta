internal void lexer_init(Lexer* lexer, String8 file_path) {
  MemoryZeroStruct(lexer);
 
  lexer->arena             = arena_init();
  lexer->file              = file_load(lexer->arena, file_path);
  lexer->current_character = lexer->file.data.str;
  lexer->file_start        = lexer->file.data.str;
  lexer->file_end          = lexer->file.data.str + lexer->file.data.size;
  lexer->line              = 1;
  lexer->column            = 1;

  lexer->current_token.type  = Token_Unknown;
  lexer->current_token.value = Str8("");
}

internal void advance_token(Lexer* lexer) {
  if (lexer->file.data.size == 0)  return;
  MemoryZeroStruct(&lexer->current_token);

  eat_trivia(lexer); // Always skip trivia constructs

  char8 c = current_character(lexer);
  if (at_eof(lexer)) {
    make_token_current(lexer, Token_End_Of_File, 1);
    return;
  } else if (isdigit(c)) {
    make_number(lexer);
  } else if (c == '@') {
    make_token_current(lexer, Token_Declaration, 1);
    advance_character(lexer);
  } else if (char8_is_alpha(c) || c == '_') {
    make_identifier(lexer);
  } else if (c == '(') {
    make_token_current(lexer, Token_Open_Parenthesis, 1);
    advance_character(lexer);
  } else if (c == ')') {
    make_token_current(lexer, Token_Close_Parenthesis, 1);
    advance_character(lexer);
  } else if (c == '{') {
    make_token_current(lexer, Token_Open_Brace, 1);
    advance_character(lexer);
  } else if (c == '}') {
    make_token_current(lexer, Token_Close_Brace, 1);
    advance_character(lexer);
  } else if (c == ',') {
    make_token_current(lexer, Token_Comma, 1);
    advance_character(lexer);
  } else if (c == '=') {
    make_token_current(lexer, Token_Equals, 1);
    advance_character(lexer);
  } else {
    advance_character(lexer);
  }

  if (lexer->current_token.type != Token_Unknown) {
    print_current_token(lexer);
  }
}

internal void advance_character(Lexer* lexer) {
  if (at_eof(lexer))  return;
    
  if (*(lexer->current_character) == '\n' || (*(lexer->current_character) == '\r' && *(lexer->current_character+1) == '\n')) {
    lexer->line  += 1;
    lexer->column = 1;
  } else {
    lexer->column += 1;
  }
    
  lexer->current_character += 1;
}

internal void make_token_range(Lexer* lexer, Token_Type type, char8* start, char8* end) {
  lexer->current_token.type         = type;
  lexer->current_token.value.str    = start;
  lexer->current_token.value.size   = (u32)(end - start);
  lexer->current_token.start_offset = character_offset(lexer, start);
  lexer->current_token.end_offset   = character_offset(lexer, end);
}

internal void make_token_current(Lexer* lexer, Token_Type type, u32 length) {
  lexer->current_token.type         = type;
  lexer->current_token.value.str    = lexer->current_character;
  lexer->current_token.value.size   = length;
  lexer->current_token.start_offset = character_offset(lexer, lexer->current_character);
  lexer->current_token.end_offset   = character_offset(lexer, lexer->current_character + length);
}

internal char8 peek_character(Lexer* lexer, u32 offset) {
  if (lexer->current_character + offset >= lexer->file_end) {
    return '\0';
  }
  return lexer->current_character[offset];
}

internal b32 is_character_trivia(Lexer* lexer) {
  b32 result = false;
  char8 c = current_character(lexer);
  char8 next = peek_character(lexer, 1);
  if (c == ' ') {
    result = true;
  } else if (c == '\t') {
    result = true;
  } else if (c == '\n') {
    result = true;
  } else if (c == '\r') {
    if (next == '\n') {
      result = true;
    }
  } else if (c == '/') {
    if (next == '/' || next == '*') {
      result = true;
    }
  }

  return result;
}

internal b32 is_token_trivia(Token token) {
  b32 result = (token.type == Token_Space               ||
                token.type == Token_Tab                 ||
                token.type == Token_New_Line            ||
                token.type == Token_Comment_Line        ||
                token.type == Token_Comment_Block_Start ||
                token.type == Token_Comment_Block_End);
  return result;
}

internal void eat_trivia(Lexer* lexer) {
  while (is_character_trivia(lexer)) {
    if (current_character(lexer) == '/' && peek_character(lexer, 1) == '*') {
      while (1) {
        advance_character(lexer);
        if (current_character(lexer) == '*' && peek_character(lexer, 1) == '/') {
          advance_character(lexer);
          advance_character(lexer);
          break;
        }
      }
    } else if (current_character(lexer) == '/' && peek_character(lexer, 1) == '/') {
      while (1) {
        char8 c = current_character(lexer);
        if (c == '\n' || (c == '\r' && peek_character(lexer, 1) == '\n')) {
          advance_character(lexer);
          advance_character(lexer);
          break;
        }
        advance_character(lexer);
      }
    } else {
      advance_character(lexer);
    }
  }
}

internal void make_identifier(Lexer* lexer) {
  char8 c = current_character(lexer);
  if (!char8_is_alpha(c) && c != '_') return;
  
  char8* start = lexer->current_character;
  while (char8_is_alphanum(current_character(lexer)) || current_character(lexer) == '_') {
    advance_character(lexer);
  }
  make_token_range(lexer, Token_Identifier, start, lexer->current_character);
  try_to_make_keyword(lexer);
}

internal void make_number(Lexer* lexer) {
  char8 c = current_character(lexer);
  if (!isdigit(c)) return;
  char8* start = lexer->current_character;
  while (isdigit(current_character(lexer))) {
    advance_character(lexer);
  }
  make_token_range(lexer, Token_Number, start, lexer->current_character);
}

internal void try_to_make_keyword(Lexer* lexer) {
  String8 value = lexer->current_token.value;
  Token_Type type = lexer->current_token.type;
  if (string8_equal(value, Str8("enum")))    type = Token_Enum;
  if (string8_equal(value, Str8("string8"))) type = Token_String8;
  if (string8_equal(value, Str8("cstring"))) type = Token_Cstring;
  if (string8_equal(value, Str8("table")))   type = Token_Table;
  lexer->current_token.type = type;
}

internal char8 current_character(Lexer* lexer) {
  char8 result = (lexer->current_character >= lexer->file_end) ? '\0' : *(lexer->current_character);
  return result;
}

internal Token current_token(Lexer* lexer) {
  Token result = lexer->current_token;
  return result;
}

internal b32 at_eof(Lexer* lexer) {
  b32 result = (lexer->current_character >= lexer->file_end);
  return result;
}

internal u32 character_offset(Lexer* lexer, char8* character) {
  u32 result = (u32)(character - lexer->file_start);
  return result;
}

internal void print_current_token(Lexer* lexer) {
  if (!lexer) return;

  // Safety: check enum range
  if (lexer->current_token.type >= Token_Count) {
    printf("xx Token_Type out of range: %d\n", lexer->current_token.type);
    return;
  }

  // Print token type
  printf("%s: ", token_type_names[lexer->current_token.type]);
  for (u32 i = strlen(token_type_names[lexer->current_token.type]); i < 26; i += 1) {
    printf(" ");
  }

  Assert(lexer->current_token.start_offset + lexer->current_token.value.size == lexer->current_token.end_offset);

  // Print token value if it has one (non-empty string)
  if (lexer->current_token.value.size > 0 && lexer->current_token.value.str) {
    if (lexer->current_token.type == Token_New_Line) {
      printf("Token value: '\\n'");
    } else {
      printf("Token value: '%.*s'", (s32)lexer->current_token.value.size, lexer->current_token.value.str);
    }
    for (u32 i = lexer->current_token.value.size; i < 32; i += 1) {
      printf(" ");
    }
    printf("StartEnd: [%d, %d]", lexer->current_token.start_offset, lexer->current_token.end_offset);
  }
  printf("\n");
}