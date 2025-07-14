
#define FZ_ENABLE_ASSERT 1
#include "main.h"

#define META_FILE Str8("D:\\work\\fz_meta\\token_table.fz_meta")
#define OUTPUT_DIR Str8("D:\\work\\fz_meta\\generated\\")

void entry_point() {
  Arena_Temp scratch = scratch_begin(0,0);
  win32_enable_console(true);

  String8 input_file  = META_FILE;
  String8 output_dir  = OUTPUT_DIR;
  String8 output_file = Str8("generated");

  u64 last_separator_index = 0;
  u64 dot_index = input_file.size;
  string8_find_last(input_file, Str8("\\"), &last_separator_index);
  string8_find_last(input_file, Str8("."),  &dot_index);
  output_file = string8_slice(input_file, last_separator_index+1, dot_index);

  String8 full_name = string8_concat(scratch.arena, output_file, Str8(".gen.c"));
  String8 file_path = string8_concat(scratch.arena, output_dir, full_name);
  file_wipe(file_path);
  file_create(file_path);

  Lexer lexer;
  lexer_init(&lexer, META_FILE);

  while (!at_eof(&lexer)) {
    advance_token(&lexer);
    Token token = current_token(&lexer);

    if (token.type == Token_Declaration) {
      advance_token(&lexer);
      token = current_token(&lexer);
      switch (token.type) {
        case Token_Table: {
          generate_table(&lexer, file_path);
        } break;
      }
    }

  }

  system("pause");
}