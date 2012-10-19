/**********************************************************************

  markdown_json.c - TextJSON output for Markdown parser.
  Copyright (c) 2012 Fyodor Sheremetyev

  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT license. See LICENSE for details.

 ***********************************************************************/

#include <assert.h>
#include "markdown_json.h"

void print_json_list(GString *out, element *elt, int indent);

void print_json_element(GString *out, element *elt, int indent) {
  char * key;
  switch (elt->key) {
    case LIST:               key = "LIST"; break;
    case RAW:                key = "RAW"; break;
    case SPACE:              key = "SPACE"; break;
    case LINEBREAK:          key = "LINEBREAK"; break;
    case ELLIPSIS:           key = "ELLIPSIS"; break;
    case EMDASH:             key = "EMDASH"; break;
    case ENDASH:             key = "ENDASH"; break;
    case APOSTROPHE:         key = "APOSTROPHE"; break;
    case SINGLEQUOTED:       key = "SINGLEQUOTED"; break;
    case DOUBLEQUOTED:       key = "DOUBLEQUOTED"; break;
    case STR:                key = "STR"; break;
    case LINK:               key = "LINK"; break;
    case IMAGE:              key = "IMAGE"; break;
    case CODE:               key = "CODE"; break;
    case HTML:               key = "HTML"; break;
    case EMPH:               key = "EMPH"; break;
    case STRONG:             key = "STRONG"; break;
    case PLAIN:              key = "PLAIN"; break;
    case PARA:               key = "PARA"; break;
    case LISTITEM:           key = "LISTITEM"; break;
    case BULLETLIST:         key = "BULLETLIST"; break;
    case ORDEREDLIST:        key = "ORDEREDLIST"; break;
    case H1:                 key = "H1"; break;
    case H2:                 key = "H2"; break;
    case H3:                 key = "H3"; break;
    case H4:                 key = "H4"; break;
    case H5:                 key = "H5"; break;
    case H6:                 key = "H6"; break;
    case BLOCKQUOTE:         key = "BLOCKQUOTE"; break;
    case VERBATIM:           key = "VERBATIM"; break;
    case HTMLBLOCK:          key = "HTMLBLOCK"; break;
    case HRULE:              key = "HRULE"; break;
    case REFERENCE:          key = "REFERENCE"; break;
    case NOTE:               key = "NOTE"; break;
    case DEFLIST:            key = "DEFLIST"; break;
    default:                 key = "?";
  }
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
