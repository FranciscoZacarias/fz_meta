# fz_gen

Code generator heavily inspired by [MetaDesk](https://github.com/ryanfleury/metadesk) but _why use someone else's when you can do it yourself_?

This tool allows you to define tables and expand each row into a templated string.

## Commands:
- `@output_file_name`: Output file name without any extensions.
- `@output_directory`: Output directory - can be relative.
- `@table`: Define a table by name and header, then fill each row. To define a value with a space, create a string with backticks. Example:
```
@table
UI_Stacks (name, type, default_value)
{
  { node     `UI_Node*` `(UI_Node){0}`             }
  { top_left  Vec2f32   `vec2f32(0.0f,0.0f)`       }
  { size_x    f32       `g_os_window.dimensions.x` }
  { size_y    f32       `g_os_window.dimensions.y` }
}
```
- `@generate`: Define a scope where text generation will happen.
- `@inline`: Called inside a `@generate` scope. Generates one line. Value must be a string defined with backticks. Does not support variables. Example:
```
@generate
{
  @inline `// @Generated code \n`
}
```
- `@foreach`: Called inside a `@generate` scope. Provide a table name within parentheses and a string defined with backticks. generates a line for each row of that table. Within this string, you can place a member of each row with the syntax `$(header)`, and it will be replaced with the column value of that header, for each row. Example:
```
@generate
{
  @foreach(UI_Stacks) `  struct { $(type) data[32]; u32 top_index; $(type) bottom_val; } $(name_lower)_stack;\n`
}
```

## Example

```
@output_file_name `test`
@output_directory `../src/generated/`

@table
UI_Stacks (name, type, default_value)
{
  { node              `UI_Node*`            `(UI_Node){0}`                   }
  { top_left          Vec2f32              `vec2f32(0.0f,0.0f)`             }
  { size_x            f32                  `g_os_window.dimensions.x`       }
  { size_y            f32                  `g_os_window.dimensions.y`       }
  { spacing_y         f32                  `0.0f`                           }
  { alignment_kind    UI_Alignment_Kind    UI_Alignment_Kind_Y              }
  { width_kind        UI_Width_Kind        UI_Width_Kind_Fill               }
  { height_kind       UI_Height_Kind       UI_Height_Kind_Fill              }
  { node_color_scheme UI_Node_Color_Scheme `ui_context.color_scheme.window` }
  { border_color      Color                PURPLE(1)                        }
  { background_color  Color                PURPLE(1)                        }
}

@generate
{
  @inline `// @Generated code \n`
  @foreach(UI_Stacks) `typedef struct $(name)_stack { $(type) data[32]; u32 top_index; $(type) bottom_val; } $(name)_stack;\n`
}
```

Will generate a file called `../src/generated/test.fzg.c` with the following content:

```c
// @Generated code 
typedef struct node_stack { UI_Node* data[32]; u32 top_index; UI_Node* bottom_val; } node_stack;
typedef struct top_left_stack { Vec2f32 data[32]; u32 top_index; Vec2f32 bottom_val; } top_left_stack;
typedef struct size_x_stack { f32 data[32]; u32 top_index; f32 bottom_val; } size_x_stack;
typedef struct size_y_stack { f32 data[32]; u32 top_index; f32 bottom_val; } size_y_stack;
typedef struct spacing_y_stack { f32 data[32]; u32 top_index; f32 bottom_val; } spacing_y_stack;
typedef struct alignment_kind_stack { UI_Alignment_Kind data[32]; u32 top_index; UI_Alignment_Kind bottom_val; } alignment_kind_stack;
typedef struct width_kind_stack { UI_Width_Kind data[32]; u32 top_index; UI_Width_Kind bottom_val; } width_kind_stack;
typedef struct height_kind_stack { UI_Height_Kind data[32]; u32 top_index; UI_Height_Kind bottom_val; } height_kind_stack;
typedef struct node_color_scheme_stack { UI_Node_Color_Scheme data[32]; u32 top_index; UI_Node_Color_Scheme bottom_val; } node_color_scheme_stack;
typedef struct border_color_stack { Color data[32]; u32 top_index; Color bottom_val; } border_color_stack;
typedef struct background_color_stack { Color data[32]; u32 top_index; Color bottom_val; } background_color_stack;
```