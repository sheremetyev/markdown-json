/**********************************************************************

  markdown_json.c - TextJSON output for Markdown parser.
  Copyright (c) 2012 Fyodor Sheremetyev

  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT license. See LICENSE for details.

 ***********************************************************************/

#include <assert.h>
#include "markdown_json.h"

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
  if ( elt->key == STR ) {
    g_string_append_printf(out, "%p: %s   '%s'\n", elt, key, elt->contents.str);
  } else {
    g_string_append_printf(out, "%p: %s\n", elt, key);
  }
  if (elt->children)
    print_json_list(out, elt->children, indent + 4);
}

void print_json_list(GString *out, element *elt, int indent) {
  while (elt != NULL) {
    print_json_element(out, elt, indent);
    elt = elt->next;
  }
}

void print_json_tree(GString *out, element *root) {
  print_json_list(out, root, 0);
}
