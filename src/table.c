internal void generate_table(Lexer* lexer, String8 output_file) {
  Table_Gen table;
  MemoryZeroStruct(&table);
  table.arena = arena_init();

  // parse table arguments
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
      case Token_Enum:    { SetFlag(table.tables_flags, Table_Flag_Enum);    } break;
      case Token_Cstring: { SetFlag(table.tables_flags, Table_Flag_Cstring); } break;
      case Token_String8: { SetFlag(table.tables_flags, Table_Flag_String8); } break;
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

  // parse table values
  advance_token(lexer); // Now it's table_name
  if (lexer->current_token.type != Token_Identifier) {
    printf("parse_table_values expected table name");
    exit(1);
  }

  table.table_name= lexer->current_token.value;

  advance_token(lexer); // Now it's '{'
  if (lexer->current_token.type != Token_Open_Brace) {
    printf("parse_table_values opening braces for table values");
    exit(1);
  }

  table.values = ArenaPush(table.arena, String8, 128);  

  Token next = {0};
  while (next.type != Token_Close_Brace) {
    advance_token(lexer);
    next = lexer->current_token;

    if (next.type == Token_Identifier) {
      table.values[table.values_count] = next.value;
      table.values_count += 1;
    } else if (next.type == Token_Close_Brace) {
      break;
    }
  }

  
  // write generated c code
  if (HasFlags(table.tables_flags, Table_Flag_Enum)) {
    Arena_Temp scratch = scratch_begin(0,0);
    String8 generated = Str8("");
    MemoryZeroStruct(&generated);

    String8 start     = string8_format(scratch.arena, Str8("\ntypedef enum %s {\n"), cstring_from_string8(scratch.arena, table.table_name));
    String8 end       = string8_format(scratch.arena, Str8("\n} %s;\n"), cstring_from_string8(scratch.arena, table.table_name));

    String8 content;
    content.size = 0;
    content.str  = ArenaPush(scratch.arena, char8, Kilobytes(32));
    for (u32 i = 0; i < table.values_count; i += 1) {
      String8 value = (i == table.values_count-1) ? Str8("  %s,") : Str8("  %s,\n");
      String8 entry = string8_format(scratch.arena, value, cstring_from_string8(scratch.arena, table.values[i]));
      MemoryCopy(content.str + content.size, entry.str, entry.size);
      content.size += entry.size;
    }
    String8 start_and_content = string8_concat(scratch.arena, start, content);
    String8 full_table = string8_concat(scratch.arena, start_and_content, end);

    file_append(scratch.arena, output_file, full_table.str, full_table.size);
    scratch_end(&scratch);
  }

  if (HasFlags(table.tables_flags, Table_Flag_Cstring)) {
    Arena_Temp scratch = scratch_begin(0,0);
    String8 generated = Str8("");
    MemoryZeroStruct(&generated);

    char8* cstring_name = cstring_from_string8(scratch.arena, table.table_name);
    for (u32 i = 0; i < table.table_name.size; i += 1) {
      cstring_name[i] = char8_to_lower(cstring_name[i]);
    }

    String8 start = string8_format(scratch.arena, Str8("\nstatic const char* %s_cstring[] {\n"), cstring_name);
    String8 end   = string8_format(scratch.arena, Str8("\n};\n"));

    String8 content;
    content.size = 0;
    content.str  = ArenaPush(scratch.arena, char8, Kilobytes(32));
    for (u32 i = 0; i < table.values_count; i += 1) {
      String8 value = (i == table.values_count-1) ? Str8("  \"%s\",") : Str8("  \"%s\",\n");
      String8 entry = string8_format(scratch.arena, value, cstring_from_string8(scratch.arena, table.values[i]));
      MemoryCopy(content.str + content.size, entry.str, entry.size);
      content.size += entry.size;
    }
    String8 start_and_content = string8_concat(scratch.arena, start, content);
    String8 full_table = string8_concat(scratch.arena, start_and_content, end);

    file_append(scratch.arena, output_file, full_table.str, full_table.size);
    scratch_end(&scratch);
  }

  if (HasFlags(table.tables_flags, Table_Flag_String8)) {
    Arena_Temp scratch = scratch_begin(0,0);
    String8 generated = Str8("");
    MemoryZeroStruct(&generated);

    char8* cstring_name = cstring_from_string8(scratch.arena, table.table_name);
    for (u32 i = 0; i < table.table_name.size; i += 1) {
      cstring_name[i] = char8_to_lower(cstring_name[i]);
    }

    String8 start = string8_format(scratch.arena, Str8("\nstatic String8 %s_string8[] {\n"), cstring_name);
    String8 end   = string8_format(scratch.arena, Str8("\n};\n"));

    String8 content;
    content.size = 0;
    content.str  = ArenaPush(scratch.arena, char8, Kilobytes(32));
    for (u32 i = 0; i < table.values_count; i += 1) {
      String8 value = (i == table.values_count-1) ? Str8("  Str8(\"%s\"),") : Str8("  Str8(\"%s\"),\n");
      String8 entry = string8_format(scratch.arena, value, cstring_from_string8(scratch.arena, table.values[i]));
      MemoryCopy(content.str + content.size, entry.str, entry.size);
      content.size += entry.size;
    }
    String8 start_and_content = string8_concat(scratch.arena, start, content);
    String8 full_table = string8_concat(scratch.arena, start_and_content, end);

    file_append(scratch.arena, output_file, full_table.str, full_table.size);
    scratch_end(&scratch);
  }

  // print table
#if PRINT_TABLE
  printf("Table: ");
  string8_printf(table.table_name); 
  printf("\n");
  for (u32 i = 0; i < table.values_count; i += 1) {
    printf(" ");
    string8_printf(table.values[i]);
    printf("\n");
  }
#endif
}