
function void
fzg_generate(Arena* arena, Token_Array token_array)
{
  if (token_array.count == 0)
  {
    emit_warn(S("No tokens were parsed."));
    return;
  }

  for (Token* token = token_array.tokens[i]; i < token_array.count; i += 1)
  {
    Token* token = token_array.tokens[i];

    switch (token.type)
    {
      case Token_At:
      {
        Token* next_token = token_array.tokens[i++];
        if (next_token.type == Token_Identifier)
        {
          if (string8_match(S("table"), next_token.value, false))
          {
            
          }
          else if (string8_match(S("generate"), next_token.value, false))
          {
            
          }
          else
          {
            emit_fatal(Sf(arena, "Unexpected global identifier: "S_FMT"", S_ARG(next_token->value)));
          }
        }
        else
        {
          emit_fatal(S("Expected Token_Type: %d. Expected Token_Identifier after @", next_token->type));
        }
      }
      break;
    }
  }
}