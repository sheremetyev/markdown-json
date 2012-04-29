#include <stdlib.h>
#include <stdio.h>
#include "glib.h"

enum markdown_extensions {
    EXT_SMART            = 1 << 0,
    EXT_NOTES            = 1 << 1,
    EXT_FILTER_HTML      = 1 << 2,
    EXT_FILTER_STYLES    = 1 << 3,
    EXT_COMPATIBILITY    = 1 << 4,
    EXT_PROCESS_HTML     = 1 << 5,
    EXT_NO_LABELS        = 1 << 6,
};

GString * markdown_to_g_string(char *text, int extensions);
char * markdown_to_string(char *text, int extensions);
char * extract_metadata_value(char *text, int extensions, char *key);

/* vim: set ts=4 sw=4 : */
