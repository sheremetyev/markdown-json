#ifndef __MARKDOWN_JSON_H__
#define __MARKDOWN_JSON_H__

#include "glib.h"
#include "markdown_peg.h"

void print_json_string(GString *out, char *str);
void print_json_element_list(GString *out, element *list);
void print_json_element(GString *out, element *elt);

#endif /* __MARKDOWN_JSON_H__ */
