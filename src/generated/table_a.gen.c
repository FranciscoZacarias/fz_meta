
typedef enum Token_Type {
  Table_A_A = 0,
  Table_A_B,
  Table_A_C,
  Table_A_D,
} Token_Type;

static const char* token_type_cstring[] {
  "Table_A_A",
  "Table_A_B",
  "Table_A_C",
  "Table_A_D",
};

static String8 token_type_string8[] {
  Str8("Table_A_A"),
  Str8("Table_A_B"),
  Str8("Table_A_C"),
  Str8("Table_A_D"),
};
