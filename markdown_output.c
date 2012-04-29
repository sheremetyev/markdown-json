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

/**********************************************************************

  Parameterized function for printing an Element.

 ***********************************************************************/

void print_element_list(GString *out, element *elt, int exts) {
    g_string_append_printf(out, "[\"text\",\n");
    print_json_element_list(out, elt);
    out->str[(out->currentStringLength)-=2] = '\0'; // HACK: remove last comma
    g_string_append_printf(out, "\n]\n");
}
