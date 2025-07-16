
#define FZ_ENABLE_ASSERT 1
#define PRINT_TOKENS 0
#define PRINT_TABLE  0
#include "main.h"

#define META_FILE Str8("D:\\work\\fz_meta\\token_table.fz_meta")
#define OUTPUT_DIR Str8("D:\\work\\fz_meta\\generated\\")

void entry_point(Command_Line command_line) {
  Arena_Temp scratch = scratch_begin(0,0);
  win32_enable_console(true);

  string8_printf(get_present_working_directory());
  printf("\n");

  if (command_line.args_count == 0) {
    printf("\nUsage:\n");
    printf("  --input_file \"path/to/input/file.fz_meta\"\n");
    printf("    : Accepts a path to one single input .fz_meta file\n\n");
    printf("  --input      \"path/to/input/directory\"\n");
    printf("    : Accepts a directory of .fz_meta files\n\n");
    printf("  --output     \"path/to/output/\"\n");
    printf("    : Accepts a directory to place generated c files\n\n");
    return;  
  }

  for (u32 i = 0; i < command_line.args_count; i += 1) {
    Command_Line_Arg arg = command_line.args[i];
    if (arg.is_flag) {
    
    } else {
      if (string8_equal(arg.key, Str8("input_file"))) {
        // Handles single input file
        if (file_exists(arg.value)) {
          if (!file_has_extension(arg.value, Str8("fz_meta"))) {
            printf("File passed in input_file is not a .fz_meta file");
            return;
          } else {
            string8_printf(arg.value);
            printf("\n");
          }
        } else {
          printf("input file doesn't exist\n");
          return;
        }
      } else if (string8_equal(arg.key, Str8("input"))) {
        // Handles all input files in a given dir
        if (path_is_directory(arg.value)) {
          string8_printf(arg.value);
          printf("\n");
        } else {
          printf("input_dir is not a directory\n");
          return;
        }
      } else if (string8_equal(arg.key, Str8("output"))) {
        // Output to generated files
        if (path_is_directory(arg.value)) {
          string8_printf(arg.value);
          printf("\n");
        } else {
          printf("output is not a directory\n");
          return;
        }
      }
    }
  }

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