/**********************************************************************

  markdown_json.c - TextJSON output for Markdown parser.
  Copyright (c) 2012 Fyodor Sheremetyev

  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT license. See LICENSE for details.

 ***********************************************************************/

#include <assert.h>
#include "markdown_json.h"

void print_json_string(GString *out, char* str) {
  while (*str != '\0') {
    switch (*str) {
      case '"':
        g_string_append_printf(out, "\\\"");
        break;
      case '\n':
        g_string_append_printf(out, "\\n");
        break;
      default:
        g_string_append_c(out, *str);
    }
    str++;
  }  
}

// we return constant strings only
char* key_name(int key) {
  switch (key) {

    #define KEY_CASE(name) case name: return #name;
    ELEMENT_LIST(KEY_CASE)
    #undef KEY_CASE

    default: return "?";
  }
}

void print_json_list(GString *out, element *elt, int indent);

void print_json_element(GString *out, element *elt, int indent) {
  char * key = key_name(elt->key);
  for (int i = 0; i < indent; i++)
    g_string_append_printf(out, " ");
  if ( elt->key == LINK || elt->key == IMAGE || elt->key == IMAGEBLOCK ) {
    g_string_append_printf(out, "[\"%s\", \"", key);
    print_json_string(out, elt->contents.link->url);
    g_string_append_printf(out, "\"]");
  } else if ( elt->key == STR ) {
    g_string_append_printf(out, "[\"%s\", \"", key);
    print_json_string(out, elt->contents.str);
    g_string_append_printf(out, "\"]");
  } else {
    g_string_append_printf(out, "[\"%s\"", key);
    if (elt->children) {
      print_json_list(out, elt->children, indent + 4);
      g_string_append_printf(out, "\n");
      for (int i = 0; i < indent; i++)
        g_string_append_printf(out, " ");
    }
    g_string_append_printf(out, "]", key);
  }
}

void print_json_list(GString *out, element *elt, int indent) {
  while (elt != NULL) {
    if (elt->key != NOTELABEL && elt->key != REFERENCE) {
      g_string_append_printf(out, ",\n");
      print_json_element(out, elt, indent);
    }
    elt = elt->next;
  }
}

void print_json_tree(GString *out, element *root) {
  g_string_append_printf(out, "[\"MARKDOWN\"");
  print_json_list(out, root, 4);
  g_string_append_printf(out, "\n]\n");
}
