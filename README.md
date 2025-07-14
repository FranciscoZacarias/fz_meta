# fz_meta
Generates c tables

Example:
```
@table(enum, cstring, string8)
My_Table_Name {
  Value1
  Value2
  Value3
}
```

Will generate the following tables:

```c

static const char* my_table_name_cstring[] = {
  "Value1",
  "Value2",
  "Value3",
}

typedef enum My_Table_Name {
  Value1 = 0,
  Value2,
  Value3,
} My_Table_Name;

static String8* my_table_name_string8[] = {
  Str8("Value1"),
  Str8("Value2"),
  Str8("Value3"),
}
```