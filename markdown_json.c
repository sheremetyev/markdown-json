/**********************************************************************

  markdown_json.c - Text JSON output for Markdown parser.
  Copyright (c) 2012 Fyodor Sheremetyev

  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT license. See LICENSE for details.

 ***********************************************************************/

#include <assert.h>
#include "markdown_json.h"

/* print_json_string - print string, escaping for JSON */
void print_json_string(GString *out, char *str) {
    char *tmp;
    while (*str != '\0') {
        switch (*str) {
        default:
            g_string_append_c(out, *str);
        }
        str++;
    }
}

/* print_json_element_list - print a list of elements as JSON */
void print_json_element_list(GString *out, element *list) {
    while (list != NULL) {
        print_json_element(out, list);
        list = list->next;
    }
}

/* print_json_element - print an element as json */
void print_json_element(GString *out, element *elt) {
    switch (elt->key) {
    /* Literals */
    case STR:
        print_json_string(out, elt->contents.str);
        break;
    case SPACE:
        g_string_append_printf(out, "%s", elt->contents.str);
        break;
    case LINEBREAK:
        g_string_append_printf(out, "\\n");
        break;
    case ELLIPSIS:
        g_string_append_printf(out, "...");
        break;
    case EMDASH:
        g_string_append_printf(out, "---");
        break;
    case ENDASH:
        g_string_append_printf(out, "--");
        break;
    case APOSTROPHE:
        g_string_append_printf(out, "'");
        break;
    case SINGLEQUOTED:
        g_string_append_printf(out, "'");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "'");
        break;
    case DOUBLEQUOTED:
        g_string_append_printf(out, "\\\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\\\"");
        break;

    /* Span elements */
    case EMPH:
        g_string_append_printf(out, "\",[\"emphasis\",\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\"],\"");
        break;
    case STRONG:
        g_string_append_printf(out, "\",[\"strong\",\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\"],\"");
        break;
    case CODE:
        g_string_append_printf(out, "\",[\"code\",\"");
        print_json_string(out, elt->contents.str);
        g_string_append_printf(out, "\"],\"");
        break;
    case MATHSPAN:
        g_string_append_printf(out, "\",[\"math\",\"");
        g_string_append_printf(out, "%s", elt->contents.str);
        g_string_append_printf(out, "\"],\"");
        break;

    case LINK:
        g_string_append_printf(out, "\",[\"link\",\"");
        print_json_string(out, elt->contents.link->url);
        print_json_string(out, elt->contents.link->title);
        print_json_element_list(out, elt->contents.link->label);
        g_string_append_printf(out, "\"],\"");
        break;

    case NOCITATION:
    case CITATION:
        g_string_append_printf(out, "\",[\"citation\",\"");
        print_json_string(out, elt->contents.str);
        g_string_append_printf(out, "\"],\"");
        break;

    case LOCATOR:
        g_string_append_printf(out, "\",[\"locator\",\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\"],\"");
        break;

    /* Block elements */
    case H1: case H2: case H3: case H4: case H5: case H6:
        g_string_append_printf(out, "\n[\"heading\",\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\"],\n");
        break;
    case PLAIN:
    case PARA:
        g_string_append_printf(out, "\n[\"para\",\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\"],\n");
        break;
    case BLOCKQUOTE:
        g_string_append_printf(out, "\n[\"quote\",\"");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "\"],\n");
        break;

    case VERBATIM:
        g_string_append_printf(out, "\n[\"verbatim\",");
        print_json_string(out, elt->contents.str);
        g_string_append_printf(out, "],\n");
        break;

    case HRULE:
        g_string_append_printf(out, "\n---\n");
        break;

    case REFERENCE:
        /* Nonprinting */
        break;
    case NOTELABEL:
        /* Nonprinting */
        break;
    case NOTE:
        g_string_append_printf(out, "\n[\"note\",");
        print_json_element_list(out, elt->children);
        g_string_append_printf(out, "],\n");
        break;

    case IMAGEBLOCK:
        g_string_append_printf(out, "\n[\"image\",");
        print_json_string(out, elt->contents.link->url);
        print_json_string(out, elt->contents.link->title);
        print_json_element_list(out, elt->contents.link->label);
        g_string_append_printf(out, "],\n");
        break;

    /* Wrapping elements */
    case HEADINGSECTION:
    case LIST:
    case BULLETLIST:
    case ORDEREDLIST:
    case LISTITEM:
        print_json_element_list(out, elt->children);
        break;

    /* Tables - not implemented */
    case TABLE:
    case TABLESEPARATOR:
    case TABLECAPTION:
    case TABLELABEL:
    case TABLEHEAD:
    case TABLEBODY:
    case TABLEROW:
    case TABLECELL:
    case CELLSPAN:
        break;

    /* Glossary - not implemented */
    case GLOSSARY:
    case GLOSSARYTERM:
    case GLOSSARYSORTKEY:
        break;

    /* Definition list - not implemented */
    case DEFLIST:
    case TERM:
    case DEFINITION:
        break;

    /* Other ignored elements */
    case FOOTER:
    case ATTRKEY:
    case HTML:
    case HTMLBLOCK:
    case IMAGE:
        break;

    case RAW:
        /* Shouldn't occur - these are handled by process_raw_blocks() */
        assert(elt->key != RAW);
        break;
    default: 
        fprintf(stderr, "print_json_element encountered unknown element key = %d\n", elt->key); 
        exit(EXIT_FAILURE);
    }
}
