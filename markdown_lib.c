/**********************************************************************

  markdown_lib.c - markdown in C using a PEG grammar.
  (c) 2008 John MacFarlane (jgm at berkeley dot edu).

  portions Copyright (c) 2010-2011 Fletcher T. Penney

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License or the MIT
  license.  See LICENSE for details.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markdown_peg.h"
#include "markdown_json.h"

#define TABSTOP 4

/* preformat_text - allocate and copy text buffer while
 * performing tab expansion. */
static GString *preformat_text(char *text) {
    GString *buf;
    char next_char;
    int charstotab;

    int len = 0;

    buf = g_string_new("");

    charstotab = TABSTOP;
    while ((next_char = *text++) != '\0') {
        switch (next_char) {
        case '\t':
            while (charstotab > 0)
                g_string_append_c(buf, ' '), len++, charstotab--;
            break;
        case '\n':
            g_string_append_c(buf, '\n'), len++, charstotab = TABSTOP;
            break;
        default:
            g_string_append_c(buf, next_char), len++, charstotab--;
        }
        if (charstotab == 0)
            charstotab = TABSTOP;
    }
    g_string_append(buf, "\n\n");
    return(buf);
}

/* process_raw_blocks - traverses an element list, replacing any RAW elements with
 * the result of parsing them as markdown text, and recursing into the children
 * of parent elements.  The result should be a tree of elements without any RAWs. */
static element * process_raw_blocks(element *input, int extensions, element *references, element *notes, element *labels) {
    element *current = NULL;
    element *last_child = NULL;
    char *contents;
    current = input;

    while (current != NULL) {
        if (current->key == RAW) {
            /* \001 is used to indicate boundaries between nested lists when there
             * is no blank line.  We split the string by \001 and parse
             * each chunk separately. */
            contents = strtok(current->contents.str, "\001");
            current->key = LIST;
            current->children = parse_markdown(contents, extensions, references, notes, labels);
            last_child = current->children;
            while ((contents = strtok(NULL, "\001"))) {
                while (last_child->next != NULL)
                    last_child = last_child->next;
                last_child->next = parse_markdown(contents, extensions, references, notes, labels);
            }
            free(current->contents.str);
            current->contents.str = NULL;
        }
        if (current->children != NULL)
            current->children = process_raw_blocks(current->children, extensions, references, notes, labels);
        current = current->next;
    }
    return input;
}

/* markdown_to_gstring - convert markdown text to Text JSON.
 * Returns a GString, which must be freed after use using g_string_free(). */
GString * markdown_to_g_string(char *text, int extensions) {
    element *result;
    element *references;
    element *notes;
    element *labels;
    GString *formatted_text;
    GString *out;
    out = g_string_new("");

    formatted_text = preformat_text(text);

    references = parse_references(formatted_text->str, extensions);
    notes = parse_notes(formatted_text->str, extensions, references);
    labels = parse_labels(formatted_text->str, extensions, references, notes);
    result = parse_markdown(formatted_text->str, extensions, references, notes, labels);

    result = process_raw_blocks(result, extensions, references, notes, labels);

    g_string_free(formatted_text, TRUE);

    print_json_tree(out, result);

    free_element_list(result);

    return out;
}

/* markdown_to_string - convert markdown text to Text JSON.
 * Returns a null-terminated string, which must be freed after use. */
char * markdown_to_string(char *text, int extensions) {
    GString *out;
    char *char_out;
    out = markdown_to_g_string(text, extensions);
    char_out = out->str;
    g_string_free(out, FALSE);
    return char_out;
}

/* vim:set ts=4 sw=4: */
