#include "fzgen.h"

function void
entry_point(Command_Line* command_line)
{
  Arena* arena = arena_alloc();
  os_console_init();

  Lexer* lexer = NULL;
  FZG_Token_Array token_array = fzg_lexer_load_all_tokens(arena, lexer, S("D:/work/fz_meta/test.fzg"));

  fzg_init();
  
  String8 output_file = S("D:/work/fz_meta/src/generated/text.c");

  os_file_wipe(output_file);

  fzg_generate(&token_array);
  fzg_write_generators(output_file);
}

function FZG_Token_Array
fzg_lexer_load_all_tokens(Arena* arena, Lexer* lexer, String8 file_path)
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

  FZG_Token_Array array = {0};
  array.tokens = push_array(lexer->arena, FZG_Token, FZG_TOKEN_ARRAY_SIZE);

  for(;;)
  {
    FZG_Token token = fzg_lexer_token(lexer);
    if (array.count < FZG_TOKEN_ARRAY_SIZE)
    {
      array.tokens[array.count++] = token;
    }
    if (token.type == FZG_Token_End_Of_File) 
    {
      break;
    }
  }

  return array;
}

function FZG_Token
fzg_lexer_make_token(Lexer* lexer, FZG_Token_Type type, u8* start, u8* end)
{
  FZG_Token token        = {0};
  token.type         = type;
  token.value        = (String8){(u64)(end - start), start};
  token.start_offset = (u32)(start - lexer->file_start);
  token.end_offset   = (u32)(end - lexer->file_start);
  token.line         = lexer->line;
  token.column       = lexer->column;
  return token;
}

function void
fzg_lexer_advance(Lexer* lexer) 
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

function FZG_Token
fzg_lexer_identifier_or_keyword(Lexer* lexer)
{
  u8* start = lexer->current_character;

  // must start with alpha or underscore
  if (!(char8_is_alpha(*lexer->current_character) || *lexer->current_character == '_'))
  {
    fzg_lexer_advance(lexer);
    return fzg_lexer_make_token(lexer, FZG_Token_Unknown, start, lexer->current_character);
  }

  while (char8_is_alpha(*lexer->current_character) ||
         char8_is_digit(*lexer->current_character) ||
         *lexer->current_character == '_')
  {
    fzg_lexer_advance(lexer);
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
    return fzg_lexer_make_token(lexer, FZG_Token_Unknown, start, end);

  return fzg_lexer_make_token(lexer, FZG_Token_Identifier, start, end);
}


function FZG_Token
fzg_lexer_number(Lexer* lexer)
{
  u8* start = lexer->current_character;

  // Optional leading minus
  if (*lexer->current_character == '-')
  {
    fzg_lexer_advance(lexer);
  }

  b32 has_dot = false;

  while (char8_is_digit(*lexer->current_character) || *lexer->current_character == '.')
  {
    if (*lexer->current_character == '.')
    {
      if (has_dot) break; // only allow one dot
      has_dot = true;
    }
    fzg_lexer_advance(lexer);
  }

  return fzg_lexer_make_token(lexer, FZG_Token_Number, start, lexer->current_character);
}

function FZG_Token
fzg_lexer_string(Lexer* lexer)
{
  u8* start = lexer->current_character;
  fzg_lexer_advance(lexer);
  while (*lexer->current_character != '"' && *lexer->current_character != 0)
  {
    fzg_lexer_advance(lexer);
  }
  if (*lexer->current_character == '"')
  {
    fzg_lexer_advance(lexer);
  }
  return fzg_lexer_make_token(lexer, FZG_Token_String_Literal, start, lexer->current_character);
}

function FZG_Token
fzg_lexer_token(Lexer* lexer)
{
  while (char8_is_space(*lexer->current_character))
  {
    fzg_lexer_advance(lexer);
  }

  u8 c = *lexer->current_character;
  if (c == 0)
  {
    return fzg_lexer_make_token(lexer, FZG_Token_End_Of_File, lexer->current_character, lexer->current_character);
  }

  switch (c)
  {
    case '@': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_At,                lexer->current_character - 1, lexer->current_character);
    case '(': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_Open_Parenthesis,  lexer->current_character - 1, lexer->current_character);
    case ')': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_Close_Parenthesis, lexer->current_character - 1, lexer->current_character);
    case '{': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_Open_Brace,        lexer->current_character - 1, lexer->current_character);
    case '}': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_Close_Brace,       lexer->current_character - 1, lexer->current_character);
    case ',': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_Comma,             lexer->current_character - 1, lexer->current_character);
    case '`':
    {
      fzg_lexer_advance(lexer);
      return fzg_lexer_backtick_block(lexer);
    }
    case '$': fzg_lexer_advance(lexer); return fzg_lexer_make_token(lexer, FZG_Token_Dollar,            lexer->current_character - 1, lexer->current_character);
    case '"': return fzg_lexer_string(lexer);
  }

  if (char8_is_alpha(c))
  {
    return fzg_lexer_identifier_or_keyword(lexer);
  }
  if (char8_is_digit(c) || c == '-')
  {
    return fzg_lexer_number(lexer);
  }

  // Unknown token
  fzg_lexer_advance(lexer);
  return fzg_lexer_make_token(lexer, FZG_Token_Unknown, lexer->current_character - 1, lexer->current_character);
}

function FZG_Token
fzg_lexer_backtick_block(Lexer* lexer)
{
  // Assume we just consumed the opening backtick
  u8* start = lexer->current_character;

  while (*lexer->current_character != '`' && *lexer->current_character != 0)
  {
    // Consume everything literally, including spaces/newlines
    fzg_lexer_advance(lexer);
  }

  u8* end = lexer->current_character;
  if (*lexer->current_character == '`')
  {
    fzg_lexer_advance(lexer); // skip closing backtick
  }

  return fzg_lexer_make_token(lexer, FZG_Token_String_Literal, start, end); 
}

function void
fzg_init()
{
  AssertNoReentry();

  fzg_context = (FZG_Context){0};
  fzg_context.arena            = arena_alloc();
  fzg_context.tables           = push_array(fzg_context.arena, FZG_Table, FZG_MAX_TABLES);
  fzg_context.tables_count     = 0;
  fzg_context.generators       = push_array(fzg_context.arena, FZG_Generator, FZG_MAX_GENERATORS);
  fzg_context.generators_count = 0;
}

function FZG_Context*
fzg_generate(FZG_Token_Array* token_array)
{
  if (token_array->count == 0)
  {
    emit_warn(S("No tokens were parsed."));
    return NULL;
  }

  Scratch scratch = scratch_begin(0,0);

  u32 token_index = 0;
#define next_token() &token_array->tokens[token_index++]
  while (token_index < token_array->count)
  {
    FZG_Token* token = next_token();

    switch (token->type)
    {
      case FZG_Token_At:
      {
        token = next_token();
        if (token->type == FZG_Token_Identifier)
        {
          if (string8_match(S("table"), token->value, false))
          {
            if (fzg_context.tables_count >= FZG_MAX_TABLES)
            {
              emit_fatal(S("Too many tables."));
            }

            FZG_Table* table = &fzg_context.tables[fzg_context.tables_count++];

            // Table name
            token = next_token();
            fzg_expect_token(token, FZG_Token_Identifier);
            table->name = string8_copy(fzg_context.arena, token->value);

            // Get headers
            String8_List header_list = {0};
            fzg_expect_token(next_token(), FZG_Token_Open_Parenthesis);
            for (;;)
            {
              FZG_Token* token = next_token();
              if (token->type == FZG_Token_Close_Parenthesis)
              {
                break;
              }
              if (token->type == FZG_Token_Comma)
              {
                continue;
              }
              fzg_expect_token(token, FZG_Token_Identifier);
              string8_list_push(fzg_context.arena, &header_list, token->value);
            }
            if (header_list.node_count == 0)
            {
              emit_fatal(S("Tables require at least 1 header"));
            }
            table->headers = push_array(fzg_context.arena, String8, header_list.node_count);
            for (String8_Node* header_node = header_list.first; header_node != NULL; header_node = header_node->next)
            {
              table->headers[table->header_count++] = string8_copy(fzg_context.arena, header_node->value);
            }

            // Rows
            FZG_Table_Row_List row_list = {0};
            token = next_token();
            fzg_expect_token(token, FZG_Token_Open_Brace);

            for (;;)
            {
              token = &token_array->tokens[token_index];
              if (token->type == FZG_Token_Close_Brace)
              {
                token_index++;
                break;
              }

              // Start a row
              fzg_expect_token(next_token(), FZG_Token_Open_Brace);

              Scratch scratch = scratch_begin(0,0);
              String8_List field_list = {0};

              for (;;)
              {
                token = next_token();
                if (token->type == FZG_Token_Comma)
                {
                  continue;
                }
                else if (token->type == FZG_Token_Close_Brace)
                {
                  break;
                }
                string8_list_push(scratch.arena, &field_list, token->value);
              }

              // Build row node
              FZG_Table_Row_Node* node = push_array(fzg_context.arena, FZG_Table_Row_Node, 1);
              node->next = 0;
              node->value.field_count = (u32)field_list.node_count;
              node->value.fields = push_array(fzg_context.arena, String8, node->value.field_count);

              u32 i = 0;
              for (String8_Node* string_node = field_list.first; string_node != NULL; string_node = string_node->next, i++)
              {
                node->value.fields[i] = string_node->value;
              }

              if (row_list.last)
              {
                row_list.last->next = node;
              }
              else
              {
                row_list.first = node;
              }

              row_list.last = node;
              row_list.count++;
            }

            if (row_list.count == 0)
            {
              emit_warn(Sf(fzg_context.arena, "Empty table: "S_FMT"", S_ARG(table->name)));
            }
            table->rows = push_array(fzg_context.arena, FZG_Table_Row, row_list.count);
            for (FZG_Table_Row_Node* row_node = row_list.first; row_node != NULL; row_node = row_node->next)
            {
              table->rows[table->row_count++] = fzg_row_copy(fzg_context.arena, row_node->value);
            }
          }
          else if (string8_match(S("generate"), token->value, false))
          {
            FZG_Generator* generator = &fzg_context.generators[fzg_context.generators_count++];

            token = next_token();
            fzg_expect_token(token, FZG_Token_Open_Brace);
            token = next_token();
            
            while (token->type != FZG_Token_Close_Brace)
            {
              fzg_expect_token(token, FZG_Token_At);
            
              token = next_token();
              if (string8_match(token->value, S("foreach"), false))
              {
                token = next_token();
                fzg_expect_token(token, FZG_Token_Open_Parenthesis);

                token = next_token();
                fzg_expect_token(token, FZG_Token_Identifier);
                generator->table = fzg_get_table_by_name(token->value);
              
                token = next_token();
                fzg_expect_token(token, FZG_Token_Close_Parenthesis);

                token = next_token();
                fzg_expect_token(token, FZG_Token_String_Literal);

                FZG_Command* command = &generator->command_queue[generator->command_count++];
                command->kind = FZG_Command_Foreach;
                command->template_text = token->value;
                fzg_parse_template_text(generator->table, command);
              }
              else if (string8_match(token->value, S("inline"), false))
              {
                token = next_token();
                fzg_expect_token(token, FZG_Token_String_Literal);

                FZG_Command* command = &generator->command_queue[generator->command_count++];
                command->kind = FZG_Command_Inline;
                command->template_text = token->value;
                fzg_parse_template_text(generator->table, command);
              }
              else
              {
                emit_fatal(Sf(scratch.arena, "Unhandled command "S_FMT" on generate", S_ARG(token->value)));
              }
              token = next_token();
            }
          }
          else
          {
            emit_fatal(Sf(fzg_context.arena, "Unexpected global identifier: "S_FMT"", S_ARG(token->value)));
          }
        }
        else
        {
          emit_fatal(Sf(fzg_context.arena, "Expected FZG_Token_Type: %d. Expected FZG_Token_Identifier after @", token->type));
        }
      }
      break;
    }
  }
#undef next_token
  scratch_end(&scratch);
  return &fzg_context;
}

function FZG_Table*
fzg_get_table_by_name(String8 name)
{
  FZG_Table* result = NULL;
  for (u32 i = 0; i < fzg_context.tables_count; i += 1)
  {
    if (string8_match(name, fzg_context.tables[i].name, true))
    {
      result = &fzg_context.tables[i];
      break;
    }
  }
  if (result == NULL)
  {
    emit_fatal(Sf(fzg_context.arena, "Unable to find table:"S_FMT"", S_ARG(name)));
  }
  return result;
}

function void
fzg_expect_token(FZG_Token* token, FZG_Token_Type expected_type)
{
  if (token->type != expected_type)
  {
    emit_fatal(Sf(fzg_context.arena, "Expected token type %d but got %d", expected_type, token->type));
  }
}

function FZG_Table_Row
fzg_row_copy(Arena* arena, FZG_Table_Row source)
{
  FZG_Table_Row result;
  result.field_count = source.field_count;
  result.fields = push_array(arena, String8, source.field_count);

  for (u32 i = 0; i < source.field_count; i++)
  {
    result.fields[i] = string8_copy(arena, source.fields[i]);
  }

  return result;
}

function void
fzg_parse_template_text(FZG_Table* table, FZG_Command* command)
{
  Scratch scratch = scratch_begin(0, 0);

  u8* text = command->template_text.str;
  u32 size = command->template_text.size;

  #define advance() Statement(i += 1; if (i >= size) { emit_fatal(S("Error parsing template.")); } c = text[i];)

  for (u32 i = 0; i < size; i += 1)
  {
    u8 c = text[i];
    if (c == '$')
    {
      u32 var_begin = i;

      advance();
      if (c != '(')
      {
        emit_fatal(S("Expected ( after $ in string template."));
      }

      advance();
      u32 var_end = i;

      for (;;)
      {
        advance();
        var_end = i;
        if (c == ')')
        {
          var_end += 1;
          break;
        }
      }

      FZG_Template_Parameter* parameter = &command->template_parameters[command->template_parameters_count++];

      parameter->variable.size = var_end - var_begin;
      parameter->variable.str  = push_array(fzg_context.arena, u8, parameter->variable.size);
      MemoryCopy(parameter->variable.str, text + var_begin, parameter->variable.size);

      // trim off "$(" prefix and ")" suffix
      String8 trimmed = string8_slice(parameter->variable, 2, parameter->variable.size - 1);

      s32 header_index = -1;
      for (u32 j = 0; j < table->header_count; j += 1)
      {
        if (string8_match(table->headers[j], trimmed, true))
        {
          header_index = (s32)j;
          break;
        }
      }

      if (header_index < 0)
      {
        emit_fatal(Sf(fzg_context.arena,
                      "Could not find variable " S_FMT " in table " S_FMT,
                      S_ARG(parameter->variable), S_ARG(table->name)));
      }

      parameter->header_index = (u32)header_index;
    }
  }

  #undef advance
  scratch_end(&scratch);
}

function void
fzg_write_generators(String8 output_file)
{
  String8_List buffer = {0};

  for (u32 gen_i = 0; gen_i < fzg_context.generators_count; gen_i += 1)
  {
    FZG_Generator *generator = &fzg_context.generators[gen_i];
    FZG_Table *table = generator->table;

    for (u32 cmd_i = 0; cmd_i < generator->command_count; cmd_i += 1)
    {
      FZG_Command *cmd = &generator->command_queue[cmd_i];

      switch (cmd->kind)
      {
        case FZG_Command_Inline:
        {
          // Inline just writes the template directly (no row loop)
          String8 text = string8_copy(fzg_context.arena, cmd->template_text);

          for (u32 param_i = 0; param_i < cmd->template_parameters_count; param_i += 1)
          {
            FZG_Template_Parameter *param = &cmd->template_parameters[param_i];
            // Replace with header name itself (Inline has no row context)
            String8 replacement = table->headers[param->header_index];
            text = string8_replace_first(fzg_context.arena, text, param->variable, replacement);
          }

          text = string8_replace_all(fzg_context.arena, text, S("\\n"), S("\n"));
          os_file_append(output_file, text.str, text.size);
        } break;

        case FZG_Command_Foreach:
        {
          // Foreach expands once per row
          for (u32 row_i = 0; row_i < table->row_count; row_i += 1)
          {
            FZG_Table_Row row = table->rows[row_i];
            String8 text = string8_copy(fzg_context.arena, cmd->template_text);

            for (u32 param_i = 0; param_i < cmd->template_parameters_count; param_i += 1)
            {
              FZG_Template_Parameter *param = &cmd->template_parameters[param_i];
              String8 replacement = row.fields[param->header_index];
              text = string8_replace_first(fzg_context.arena, text, param->variable, replacement);
            }

            text = string8_replace_all(fzg_context.arena, text, S("\\n"), S("\n"));
            os_file_append(output_file, text.str, text.size);
          }
        } break;

        default:
        {
          emit_fatal(S("Unknown command kind in generator command queue."));
        } break;
      }
    }
  }
}
