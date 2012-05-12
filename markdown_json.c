/**********************************************************************

  markdown_json.c - TextJSON output for Markdown parser.
  Copyright (c) 2012 Fyodor Sheremetyev

  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT license. See LICENSE for details.

 ***********************************************************************/

#include <assert.h>
#include "markdown_json.h"

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

void print_json_element_tag(GString *out, char *str) {
    g_string_append_printf(out, "\"");
    g_string_append_printf(out, str);
    g_string_append_printf(out, "\"");
}

void print_json_element_level(GString *out, int level) {
    g_string_append_printf(out, ", { \"level\": ");
    g_string_append_printf(out, "%d", level);
    g_string_append_printf(out, " }");
}

void print_json_literal_element_list(GString *out, element *list);

void print_json_literal_element(GString *out, element *elt) {
    switch (elt->key) {
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
        print_json_literal_element_list(out, elt->children);
        g_string_append_printf(out, "'");
        break;
    case DOUBLEQUOTED:
        g_string_append_printf(out, "\\\"");
        print_json_literal_element_list(out, elt->children);
        g_string_append_printf(out, "\\\"");
        break;

    /* transparent grouping elements */
    case LIST:
        print_json_literal_element_list(out, elt->children);
        break;

    case RAW:
        /* Shouldn't occur - these are handled by process_raw_blocks() */
        assert(elt->key != RAW);
        break;
    default: 
        fprintf(stderr, "unexpected literal element key = %d\n", elt->key); 
        exit(EXIT_FAILURE);
    }
}

void print_json_literal_element_list(GString *out, element *list) {
    while (list != NULL) {
        print_json_literal_element(out, list);
        list = list->next;
    }
}

void print_json_literal_elements(GString *out, element *list) {
    g_string_append_printf(out, ", \"");
    print_json_literal_element_list(out, list);
    g_string_append_printf(out, "\"");
}

void print_json_inline_element_list(GString *out, element *list);

void print_json_inline_element(GString *out, element *elt) {
    switch (elt->key) {
    case EMPH:
        print_json_element_tag(out, "emph");
        print_json_literal_elements(out, elt->children);
        break;
    case STRONG:
        print_json_element_tag(out, "strong");
        print_json_literal_elements(out, elt->children);
        break;

    /* transparent grouping elements */
    case LIST:
        print_json_inline_element_list(out, elt->children);
        break;

    case RAW:
        /* Shouldn't occur - these are handled by process_raw_blocks() */
        assert(elt->key != RAW);
        break;
    default: 
        fprintf(stderr, "unexpected inline element key = %d\n", elt->key); 
        exit(EXIT_FAILURE);
    }
}

void print_json_inline_element_list(GString *out, element *list) {
    while (list != NULL) {
        print_json_inline_element(out, list);
        list = list->next;
    }
}

bool is_literal_element(element *elt) {
    switch (elt->key) {
    case STR:
    case SPACE:
    case LINEBREAK:
    case ELLIPSIS:
    case EMDASH:
    case ENDASH:
    case APOSTROPHE:
    case SINGLEQUOTED:
    case DOUBLEQUOTED:
        return true;
    case LIST:
        return true;
    default:
        return false;
    }
}

void print_json_inline_elements(GString *out, element *list) {
    while (list != NULL) {
        g_string_append_printf(out, ", [");
        if (is_literal_element(list)) {
            print_json_element_tag(out, "plain");
            g_string_append_printf(out, ", \"");
            print_json_literal_element(out, list);
            while (list->next != NULL && is_literal_element(list->next)) {
                list = list->next;
                print_json_literal_element(out, list);
            }
            g_string_append_printf(out, "\"");
        } else {
            print_json_inline_element(out, list);
        }
        g_string_append_printf(out, "]");
        list = list->next;
    }
}

void print_json_block_element_list(GString *out, element *list, int level);

void print_json_block_element(GString *out, element *elt, int level, bool first) {
    switch (elt->key) {
    case H1: case H2: case H3: case H4: case H5: case H6:
        g_string_append_printf(out, ", [");
        print_json_element_tag(out, "heading");
        print_json_element_level(out, 2);
        print_json_inline_elements(out, elt->children);
        g_string_append_printf(out, "]");
        break;
    case PLAIN:
    case PARA:
        g_string_append_printf(out, ", [");
        if (level > 0 && first) {
            print_json_element_tag(out, "item");
        } else {
            print_json_element_tag(out, "para");
        }
        print_json_element_level(out, level);
        print_json_inline_elements(out, elt->children);
        g_string_append_printf(out, "]");
        break;

    /* transparent grouping elements */
    case HEADINGSECTION:
    case LISTITEM:
    case LIST:
        print_json_block_element_list(out, elt->children, level);
        break;

    case BULLETLIST:
        print_json_block_element_list(out, elt->children, level + 1);
        break;

    case RAW:
        /* Shouldn't occur - these are handled by process_raw_blocks() */
        assert(elt->key != RAW);
        break;
    default: 
        fprintf(stderr, "unexpected block element key = %d\n", elt->key); 
        exit(EXIT_FAILURE);
    }
}

void print_json_block_element_list(GString *out, element *list, int level) {
    bool first = true;
    while (list != NULL) {
        print_json_block_element(out, list, level, first);
        first = false;
        list = list->next;
    }
}

void print_json_tree(GString *out, element *root) {
    g_string_append_printf(out, "[\"text\"");
    print_json_block_element_list(out, root, 0);
    g_string_append_printf(out, "]");
}
