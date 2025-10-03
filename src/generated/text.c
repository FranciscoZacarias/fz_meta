// @Generated code 
#define MAX_STACKS_DEFAULT 32

#define UI_Context_Member_Stack_Declarations \
  struct { UI_Node* data[MAX_STACKS_DEFAULT]; u32 top_index; UI_Node* bottom_val; } node_stack; \
  struct { Vec2f32 data[MAX_STACKS_DEFAULT]; u32 top_index; Vec2f32 bottom_val; } top_left_stack; \
  struct { f32 data[MAX_STACKS_DEFAULT]; u32 top_index; f32 bottom_val; } size_x_stack; \
  struct { f32 data[MAX_STACKS_DEFAULT]; u32 top_index; f32 bottom_val; } size_y_stack; \
  struct { f32 data[MAX_STACKS_DEFAULT]; u32 top_index; f32 bottom_val; } padding_x_stack; \
  struct { f32 data[MAX_STACKS_DEFAULT]; u32 top_index; f32 bottom_val; } padding_y_stack; \
  struct { f32 data[MAX_STACKS_DEFAULT]; u32 top_index; f32 bottom_val; } spacing_x_stack; \
  struct { f32 data[MAX_STACKS_DEFAULT]; u32 top_index; f32 bottom_val; } spacing_y_stack; \
  struct { UI_Alignment_Kind data[MAX_STACKS_DEFAULT]; u32 top_index; UI_Alignment_Kind bottom_val; } alignment_kind_stack; \
  struct { UI_Width_Kind data[MAX_STACKS_DEFAULT]; u32 top_index; UI_Width_Kind bottom_val; } width_kind_stack; \
  struct { UI_Height_Kind data[MAX_STACKS_DEFAULT]; u32 top_index; UI_Height_Kind bottom_val; } height_kind_stack; \
  struct { UI_Node_Color_Scheme data[MAX_STACKS_DEFAULT]; u32 top_index; UI_Node_Color_Scheme bottom_val; } node_color_scheme_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } border_color_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } background_color_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } background_hover_color_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } background_active_color_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } text_color_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } text_hover_color_stack; \
  struct { Color data[MAX_STACKS_DEFAULT]; u32 top_index; Color bottom_val; } text_active_color_stack; \
  /* macro terminator */

#define UI_Context_Member_Stacks_Init \
  ui_context.node_stack.top_index = 0; ui_context.node_stack.bottom_val = default_val; MemoryZeroArray(ui_context.node_stack.data); \
  ui_context.top_left_stack.top_index = 0; ui_context.top_left_stack.bottom_val = default_val; MemoryZeroArray(ui_context.top_left_stack.data); \
  ui_context.size_x_stack.top_index = 0; ui_context.size_x_stack.bottom_val = default_val; MemoryZeroArray(ui_context.size_x_stack.data); \
  ui_context.size_y_stack.top_index = 0; ui_context.size_y_stack.bottom_val = default_val; MemoryZeroArray(ui_context.size_y_stack.data); \
  ui_context.padding_x_stack.top_index = 0; ui_context.padding_x_stack.bottom_val = default_val; MemoryZeroArray(ui_context.padding_x_stack.data); \
  ui_context.padding_y_stack.top_index = 0; ui_context.padding_y_stack.bottom_val = default_val; MemoryZeroArray(ui_context.padding_y_stack.data); \
  ui_context.spacing_x_stack.top_index = 0; ui_context.spacing_x_stack.bottom_val = default_val; MemoryZeroArray(ui_context.spacing_x_stack.data); \
  ui_context.spacing_y_stack.top_index = 0; ui_context.spacing_y_stack.bottom_val = default_val; MemoryZeroArray(ui_context.spacing_y_stack.data); \
  ui_context.alignment_kind_stack.top_index = 0; ui_context.alignment_kind_stack.bottom_val = default_val; MemoryZeroArray(ui_context.alignment_kind_stack.data); \
  ui_context.width_kind_stack.top_index = 0; ui_context.width_kind_stack.bottom_val = default_val; MemoryZeroArray(ui_context.width_kind_stack.data); \
  ui_context.height_kind_stack.top_index = 0; ui_context.height_kind_stack.bottom_val = default_val; MemoryZeroArray(ui_context.height_kind_stack.data); \
  ui_context.node_color_scheme_stack.top_index = 0; ui_context.node_color_scheme_stack.bottom_val = default_val; MemoryZeroArray(ui_context.node_color_scheme_stack.data); \
  ui_context.border_color_stack.top_index = 0; ui_context.border_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.border_color_stack.data); \
  ui_context.background_color_stack.top_index = 0; ui_context.background_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.background_color_stack.data); \
  ui_context.background_hover_color_stack.top_index = 0; ui_context.background_hover_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.background_hover_color_stack.data); \
  ui_context.background_active_color_stack.top_index = 0; ui_context.background_active_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.background_active_color_stack.data); \
  ui_context.text_color_stack.top_index = 0; ui_context.text_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.text_color_stack.data); \
  ui_context.text_hover_color_stack.top_index = 0; ui_context.text_hover_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.text_hover_color_stack.data); \
  ui_context.text_active_color_stack.top_index = 0; ui_context.text_active_color_stack.bottom_val = default_val; MemoryZeroArray(ui_context.text_active_color_stack.data); \
  /* macro terminator */

#define UI_Context_Member_Stacks_Debug_Top_At_Zero \
  if(ui_context.node_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("node_stack")), ui_context.node_stack.top_index); \
  if(ui_context.top_left_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("top_left_stack")), ui_context.top_left_stack.top_index); \
  if(ui_context.size_x_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("size_x_stack")), ui_context.size_x_stack.top_index); \
  if(ui_context.size_y_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("size_y_stack")), ui_context.size_y_stack.top_index); \
  if(ui_context.padding_x_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("padding_x_stack")), ui_context.padding_x_stack.top_index); \
  if(ui_context.padding_y_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("padding_y_stack")), ui_context.padding_y_stack.top_index); \
  if(ui_context.spacing_x_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("spacing_x_stack")), ui_context.spacing_x_stack.top_index); \
  if(ui_context.spacing_y_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("spacing_y_stack")), ui_context.spacing_y_stack.top_index); \
  if(ui_context.alignment_kind_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("alignment_kind_stack")), ui_context.alignment_kind_stack.top_index); \
  if(ui_context.width_kind_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("width_kind_stack")), ui_context.width_kind_stack.top_index); \
  if(ui_context.height_kind_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("height_kind_stack")), ui_context.height_kind_stack.top_index); \
  if(ui_context.node_color_scheme_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("node_color_scheme_stack")), ui_context.node_color_scheme_stack.top_index); \
  if(ui_context.border_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("border_color_stack")), ui_context.border_color_stack.top_index); \
  if(ui_context.background_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("background_color_stack")), ui_context.background_color_stack.top_index); \
  if(ui_context.background_hover_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("background_hover_color_stack")), ui_context.background_hover_color_stack.top_index); \
  if(ui_context.background_active_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("background_active_color_stack")), ui_context.background_active_color_stack.top_index); \
  if(ui_context.text_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("text_color_stack")), ui_context.text_color_stack.top_index); \
  if(ui_context.text_hover_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("text_hover_color_stack")), ui_context.text_hover_color_stack.top_index); \
  if(ui_context.text_active_color_stack.top_index != 0) emit_fatal(Sf(ui_context.arena, "UI: Stack "S_FMT" not at 0. Actual value: '%u'", S_ARG(S("text_active_color_stack")), ui_context.text_active_color_stack.top_index); \
  /* macro terminator */

