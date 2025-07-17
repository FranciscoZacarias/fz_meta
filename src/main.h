#ifndef MAIN_H
#define MAIN_H

#define FZ_ENABLE_CONSOLE 1
#include "fz_include.h"

// *.h
#include "lexer.h"
#include "table.h"

// *.c
#include "lexer.c"
#include "table.c"

internal void handle_command_line_arguments(Arena* arena, Command_Line command_line, String8* input_file, String8* input_directory, String8* output_directory);

#endif // MAIN_H