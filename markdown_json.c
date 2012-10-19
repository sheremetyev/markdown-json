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
    case LIST:               return "LIST";
    case RAW:                return "RAW";
    case SPACE:              return "SPACE";
    case LINEBREAK:          return "LINEBREAK";
    case ELLIPSIS:           return "ELLIPSIS";
    case EMDASH:             return "EMDASH";
    case ENDASH:             return "ENDASH";
    case APOSTROPHE:         return "APOSTROPHE";
    case SINGLEQUOTED:       return "SINGLEQUOTED";
    case DOUBLEQUOTED:       return "DOUBLEQUOTED";
    case STR:                return "STR";
    case LINK:               return "LINK";
    case IMAGE:              return "IMAGE";
    case CODE:               return "CODE";
    case HTML:               return "HTML";
    case EMPH:               return "EMPH";
    case STRONG:             return "STRONG";
    case PLAIN:              return "PLAIN";
    case PARA:               return "PARA";
    case LISTITEM:           return "LISTITEM";
    case BULLETLIST:         return "BULLETLIST";
    case ORDEREDLIST:        return "ORDEREDLIST";
    case H1:                 return "H1";
    case H2:                 return "H2";
    case H3:                 return "H3";
    case H4:                 return "H4";
    case H5:                 return "H5";
    case H6:                 return "H6";
    case BLOCKQUOTE:         return "BLOCKQUOTE";
    case VERBATIM:           return "VERBATIM";
    case HTMLBLOCK:          return "HTMLBLOCK";
    case HRULE:              return "HRULE";
    case REFERENCE:          return "REFERENCE";
    case NOTE:               return "NOTE";
    case DEFLIST:            return "DEFLIST";
    default:                 return "?";
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
