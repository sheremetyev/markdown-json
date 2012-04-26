#include <assert.h>
#include "markdown_json.h"

/**********************************************************************

  Functions for printing Elements as JSON

 ***********************************************************************/

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
    case RAW:
        /* Shouldn't occur - these are handled by process_raw_blocks() */
        assert(elt->key != RAW);
        break;
    default: 
        fprintf(stderr, "print_json_element encountered unknown element key = %d\n", elt->key); 
        exit(EXIT_FAILURE);
    }
}
