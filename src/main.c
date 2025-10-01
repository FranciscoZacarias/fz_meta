
#define FZ_ENABLE_ASSERT 1
#define PRINT_TOKENS 0
#define PRINT_TABLE  0
#include "main.h"

function void
entry_point(Command_Line* command_line)
{
  Arena* arena  = arena_alloc();
  os_console_init();

}

function void
handle_command_line_arguments(Arena* arena, Command_Line* command_line, String8* input_file, String8* input_directory, String8* output_directory)
{
}