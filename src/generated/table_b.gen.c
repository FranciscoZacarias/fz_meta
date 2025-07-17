
typedef enum Token_Type {
  Table_B_A = 0,
  Table_B_B,
  Table_B_C,
  Table_B_D,
} Token_Type;

static const char* token_type_cstring[] {
  "Table_B_A",
  "Table_B_B",
  "Table_B_C",
  "Table_B_D",
};

static String8 token_type_string8[] {
  Str8("Table_B_A"),
  Str8("Table_B_B"),
  Str8("Table_B_C"),
  Str8("Table_B_D"),
};
