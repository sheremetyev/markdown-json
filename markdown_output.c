/**********************************************************************

  markdown_output.c - functions for printing Elements parsed by 
                      markdown_peg.
  (c) 2008 John MacFarlane (jgm at berkeley dot edu).
  
  portions Copyright (c) 2012 Fyodor Sheremetyev
  portions Copyright (c) 2010-2011 Fletcher T. Penney

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License or the MIT
  license.  See LICENSE for details.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

 ***********************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "glib.h"
#include "markdown_peg.h"
#include "utility_functions.h"
#include "markdown_json.h"

static int extensions;

/* MultiMarkdown Routines */
element * metadata_for_key(char *key, element *list);
char * metavalue_for_key(char *key, element *list);

/**********************************************************************

  Parameterized function for printing an Element.

 ***********************************************************************/

void print_element_list(GString *out, element *elt, int exts) {
    extensions = exts;

    g_string_append_printf(out, "[\"text\",\n");
    print_json_element_list(out, elt);
    out->str[(out->currentStringLength)-=2] = '\0'; // HACK: remove last comma
    g_string_append_printf(out, "\n]\n");
}

/* find specified metadata key, if present */
element * metadata_for_key(char *key, element *list) {
    char *label;
    element *step = NULL;
    step = list;
    
    label = label_from_string(key,0);
    
    while (step != NULL) {
        if (step->key == METADATA) {
           /* search METAKEY children */
            step = step->children;
            while ( step != NULL) {
                if (strcmp(step->contents.str, label) == 0) {
                    free(label);
                    return step;
                }
                step = step->next;
            }
            free(label);
            return NULL;
        }
       step = step->next;
    }
    free(label);
    return NULL;
}


/* find specified metadata key, if present */
char * metavalue_for_key(char *key, element *list) {
    char *label;
    char *result;
    element *step = NULL;
    step = list;
    
    label = label_from_string(key,0);
    
    while (step != NULL) {
        if (step->key == METADATA) {
           /* search METAKEY children */
            step = step->children;
            while ( step != NULL) {
                if (strcmp(step->contents.str, label) == 0) {
                    /* Found a match */
                    if ((strcmp(label,"latexmode") == 0) ||
                        (strcmp(label,"quoteslanguage") == 0)) {
                        result = label_from_string(step->children->contents.str,0);
                    } else {
                        result = strdup(step->children->contents.str);
                    }
                    free(label);
                   return result;
                }
                step = step->next;
            }
            free(label);
            return NULL;
        }
       step = step->next;
    }
    free(label);
    return NULL;
}

/* bogus function just references a couple globals defined in utility_functions.h but not used in this source file */
static void bogus_function()
{
	static char* bogus;
	static element* bogus2;
	bogus = charbuf;
	bogus2 = parse_result;
}
