#ifndef __MARKDOWN_JSON_H__
#define __MARKDOWN_JSON_H__

#include "glib.h"
#include "markdown_peg.h"

void print_json_tree(GString *out, element *root);

#endif /* __MARKDOWN_JSON_H__ */
