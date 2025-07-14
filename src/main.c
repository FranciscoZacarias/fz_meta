
#define FZ_ENABLE_ASSERT 1
#include "main.h"

#define META_FILE Str8("D:\\work\\fz_meta\\input.fz_meta")

void entry_point() {
  Arena* arena = arena_init();
  win32_enable_console(true);
  
  Lexer lexer;
  lexer_init(&lexer, META_FILE);

  while (!at_eof(&lexer)) {
    advance_token(&lexer);
    Token token = current_token(&lexer);

    if (token.type == Token_Declaration) {
      advance_token(&lexer);
      token = current_token(&lexer);

      Table_Gen table;
      MemoryZeroStruct(&table);
      table.arena = arena_init();

      if (token.type == Token_Table) {
        parse_table_arguments(&lexer, &table);
      } else {
        printf("[Line:%u,Column:%u] Error: Expected a table declaration after '@'", lexer.line, lexer.column);
        exit(1);
      }

      parse_table_values(&lexer, &table);

      print_table(table);
    }
  }

  system("pause");
}

internal void parse_table_arguments(Lexer* lexer, Table_Gen* table) {
  advance_token(lexer); // Now it's '('
  if (lexer->current_token.type != Token_Open_Parenthesis) {
    printf("Called parse_table_arguments lexer wasn't at Token_Close_Parenthesis '('");
    exit(1);
  }

  Token token = {0};
  for (;;) {
    advance_token(lexer); // Now should be a table_type
    token = current_token(lexer);
    switch (token.type) {
      case Token_Enum:    { SetFlag(table->tables_flags, Table_Flag_Enum);    } break;
      case Token_Cstring: { SetFlag(table->tables_flags, Table_Flag_Cstring); } break;
      case Token_String8: { SetFlag(table->tables_flags, Table_Flag_String8); } break;
      default: {
        printf("[Line:%u,Column:%u] Error: Type inside table declaration not found.", lexer->line, lexer->column);
        exit(1);
      } break;
    }

    advance_token(lexer);
    token = current_token(lexer);
    if (token.type == Token_Comma) {
     /* Don't care, more table_types to come */
    } else if (token.type == Token_Close_Parenthesis) {
      break; 
    } else {
      printf("[Line:%u,Column:%u] Error: Expected either ',' or ')' after table arguments", lexer->line, lexer->column);
      exit(1);  
    }
  }
}

internal void parse_table_values(Lexer* lexer, Table_Gen* table) {
  advance_token(lexer); // Now it's table_name
  if (lexer->current_token.type != Token_Identifier) {
    printf("parse_table_values expected table name");
    exit(1);
  }

  table->table_name= lexer->current_token.value;

  advance_token(lexer); // Now it's '{'
  if (lexer->current_token.type != Token_Open_Brace) {
    printf("parse_table_values opening braces for table values");
    exit(1);
  }

  table->values = ArenaPush(table->arena, String8, 128);  

  Token next = {0};
  while (next.type != Token_Close_Brace) {
    advance_token(lexer);
    next = lexer->current_token;

    if (next.type == Token_Identifier) {
      table->values[table->values_count] = next.value;
      table->values_count += 1;
    } else if (next.type == Token_Close_Brace) {
      break;
    }
  }
}

internal void print_table(Table_Gen table) {
  printf("Table: ");
  string8_printf(table.table_name); 
  printf("\n");
  for (u32 i = 0; i < table.values_count; i += 1) {
    printf(" ");
    string8_printf(table.values[i]);
    printf("\n");
  }
}