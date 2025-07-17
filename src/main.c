
#define FZ_ENABLE_ASSERT 1
#define PRINT_TOKENS 0
#define PRINT_TABLE  0
#include "main.h"

global String8 input_file = {0};
global String8 input      = {0};
global String8 output     = {0};

void entry_point(Command_Line command_line) {
  Arena_Temp scratch = scratch_begin(0,0);
  win32_enable_console(true);

  if (command_line.args_count == 0) {
    printf("\nUsage:\n");
    printf("  --input_file \"path/to/input/file.fz_meta\"\n");
    printf("    : Accepts a path to one single input .fz_meta file\n\n");
    printf("  --input      \"path/to/input/directory\"\n");
    printf("    : Accepts a directory of .fz_meta files\n\n");
    printf("  --output     \"path/to/output/\"\n");
    printf("    : [OPTIONAL]\n");
    printf("    : Accepts a directory to place generated c files\n\n");
    return;  
  }

  String8 pwd = path_get_working_directory();
  String8 dir = path_get_current_directory_name(pwd);
  if (string8_equal(dir, Str8("build"))) {
    pwd = path_dirname(pwd); // move back if in build
  }

  for (u32 i = 0; i < command_line.args_count; i += 1) {
    Command_Line_Arg arg = command_line.args[i];
    if (arg.is_flag) continue;

    String8 path = path_new(scratch.arena, arg.value);
    String8 full_path = path_join(scratch.arena, pwd, path);

    if (string8_equal(arg.key, Str8("input_file"))) {
      if (!file_exists(full_path)) {
        printf("input file doesn't exist\n");
        return;
      }
      if (!file_has_extension(path, Str8("fz_meta"))) {
        printf("File passed in input_file is not a .fz_meta file");
        return;
      }
      input_file = full_path;
    } else if (string8_equal(arg.key, Str8("input"))) {
      if (!path_is_directory(full_path)) {
        printf("input_dir is not a directory\n");
        return;
      }
      input = full_path;
    } else if (string8_equal(arg.key, Str8("output"))) {
      if (!path_is_directory(full_path)) {
        if (!path_create_as_directory(full_path)) {
          printf("output is not a directory\n");
          return;
        }
        string8_printf(full_path);
        printf("\n");
      }
      output = full_path;
    }
  }

  if (input_file.size == 0 && input.size == 0) {
    printf("No input directory nor input file provided\n\n");
    return;
  }
  if (output.size == 0 || path_is_directory(output)) {
    String8 default_generated_path = path_join(scratch.arena, pwd, Str8("src\\generated"));
    if (!path_create_as_directory(default_generated_path)) {
      printf("Unable to find or create the default generated path\n");
      string8_printf(default_generated_path);
      printf("\n");
      return;
    }
    output = default_generated_path;
  }

  Lexer lexer;
  lexer_init(&lexer, input_file);

  // TODO(fz): We're assuming just 1 file given by input_file and not a directory given by input
  while (!at_eof(&lexer)) {
    advance_token(&lexer);
    Token token = current_token(&lexer);

    if (token.type == Token_Declaration) {
      advance_token(&lexer);
      token = current_token(&lexer);
      switch (token.type) {
        case Token_Table: {
          String8 output_file = path_join(scratch.arena, output, path_get_file_name_no_ext(input_file));
          generate_table(&lexer, output_file);
        } break;
      }
    }
  }
}