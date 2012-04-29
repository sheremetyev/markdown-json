/**********************************************************************

  markdown.c - markdown in C using a PEG grammar.
  (c) 2008 John MacFarlane (jgm at berkeley dot edu).
  
  portions Copyright (c) 2012 Fyodor Sheremetyev
  portions Copyright (c) 2010-2011 Fletcher T. Penney
  portions Copyright (c) 2011 Daniel Jalkut
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License or the MIT
  license.  See LICENSE for details.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

 ***********************************************************************/

#include "glib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include "markdown_peg.h"

static int extensions;

/**********************************************************************

  The main program is just a wrapper around the library functions in
  markdown_lib.c.  It parses command-line options, reads the text to
  be converted from input files or stdin, converts the text, and sends
  the output to stdout or a file.  Character encodings are ignored.

 ***********************************************************************/

#define VERSION "0.0.1"
#define COPYRIGHT "portions Copyright (c) 2012 Fyodor Sheremetyev, MIT licensed.\n" \
                  "portions Copyright (c) 2010-2012 Fletcher T. Penney.\n" \
				  "portions Copyright (c) 2011 Daniel Jalkut, MIT licensed.\n" \
                  "original Copyright (c) 2008-2009 John MacFarlane.  License GPLv2+ or MIT.\n" \
                  "This is free software: you are free to change and redistribute it.\n" \
                  "There is NO WARRANTY, to the extent permitted by law."

/* print version and copyright information */
void version(const char *progname)
{
  printf("markdown-json version %s\n"
         "%s\n",
         VERSION,
         COPYRIGHT);
}

#define MD_ARGUMENT_FLAG(name, flagChar, flagValue, outPointer, desc, argPlaceholder)	{ name, no_argument, outPointer, flagValue }
#define MD_ARGUMENT_STRING(name, flagChar, outPointer, desc, argPlaceholder)	{ name, required_argument, NULL, flagChar }

/* With getopt we don't get the same fancy automatic usage (I don't think?) so for now we're faking it ... */
static void printUsage() {
	printf("Usage:\
  markdown-json [OPTION...] [FILE...]\n\
\n\
Help Options:\n\
  -h, --help              Show help options\n\
\n\
Application Options:\n\
  -v, --version           print version and exit\n\
  -o, --output=FILE       send output to FILE (default is stdout)\n\
  -x, --extensions        use all syntax extensions\n\
  --filter-html           filter out raw HTML (except styles)\n\
  --filter-styles         filter out HTML styles\n\
  -c, --compatibility     markdown compatibility mode\n\
  -b, --batch             process multiple files automatically\n\
\n\
Syntax extensions\n\
  --smart --nosmart       toggle smart typography extension\n\
  --notes --nonotes       toggle notes extension\n\
  --process-html          process MultiMarkdown inside of raw HTML\n\
  --nolabels              do not generate id attributes for headers\n\
\n\
Converts text in specified files (or stdin) from markdown to Text JSON.\n");
}

int main(int argc, char * argv[]) {
	
    int numargs;            /* number of filename arguments */
    int i;

    GString *inputbuf;
    char *out;              /* string containing processed output */

    GString *file;
    char *fake;
    FILE *input;
    FILE *output;
    char curchar;
    char *progname = argv[0];

    /* Code for command-line option parsing. */

    static gboolean opt_version = FALSE;
    static gchar *opt_output = 0;
    static gchar *opt_to = 0;
    static gboolean opt_smart = TRUE;
    static gboolean opt_no_smart = FALSE;
    static gboolean opt_notes = TRUE;
    static gboolean opt_no_notes = FALSE;
    static gboolean opt_process_html = FALSE;
    static gboolean opt_filter_html = FALSE;
    static gboolean opt_filter_styles = FALSE;
    static gboolean opt_allext = FALSE;
    static gboolean opt_compatibility = FALSE;
    static gboolean opt_batchmode = FALSE;
    static gchar *opt_extract_meta = FALSE;
    static gboolean opt_no_labels = FALSE;

	static struct option entries[] =
	{
	  MD_ARGUMENT_FLAG( "help", 'h', 1, NULL, "Show help options", NULL ),
	  MD_ARGUMENT_FLAG( "version", 'v', 1, &opt_version, "print version and exit", NULL ),
      MD_ARGUMENT_STRING( "output", 'o', &opt_output, "send output to FILE (default is stdout)", "FILE" ),
      MD_ARGUMENT_FLAG( "extensions", 'x', 1, &opt_allext, "use all syntax extensions", NULL ),
      MD_ARGUMENT_FLAG( "filter-html", 0, 1, &opt_filter_html, "filter out raw HTML (except styles)", NULL ),
      MD_ARGUMENT_FLAG( "filter-styles", 0, 1, &opt_filter_styles, "filter out HTML styles", NULL ),
      MD_ARGUMENT_FLAG( "compatibility", 'c', 1, &opt_compatibility, "markdown compatibility mode", NULL ),
      MD_ARGUMENT_FLAG( "batch", 'b', 1, &opt_batchmode, "process multiple files automatically", NULL ),
      MD_ARGUMENT_FLAG( "smart", 0, 1, &opt_smart, "use smart typography extension (on by default)", NULL ),
      MD_ARGUMENT_FLAG( "nosmart", 0, 1, &opt_no_smart, "do not use smart typography extension", NULL ),
      MD_ARGUMENT_FLAG( "notes", 0, 1, &opt_notes, "use notes extension (on by default)", NULL ),
      MD_ARGUMENT_FLAG( "nonotes", 0, 1, &opt_no_notes, "do not use notes extension", NULL ),
      MD_ARGUMENT_FLAG( "process-html", 0, 1, &opt_process_html, "process MultiMarkdown inside of raw HTML", NULL ),
      MD_ARGUMENT_FLAG( "nolabels", 0, 1, &opt_no_labels, "do not generate id attributes for headers", NULL ),
      { NULL }
    };

	char ch;
	while ((ch = getopt_long(argc, argv, "hvo:t:xcbe:", entries, NULL)) != -1) {
		 switch (ch) {
			case 'h':
				printUsage();
				return EXIT_SUCCESS;
				break;
			case 'v':
				opt_version = true;
				break;
			case 'o':
				opt_output = malloc(strlen(optarg) + 1);
				strcpy(opt_output, optarg);
				break;
			case 't':
				opt_to = malloc(strlen(optarg) + 1);
				strcpy(opt_to, optarg);
				break;
			case 'x':
				opt_allext = true;
				break;
			case 'c':
				opt_compatibility = true;
				break;
			case 'b':
				opt_batchmode = true;
				break;
			case 'e':
				opt_extract_meta = malloc(strlen(optarg) + 1);
				strcpy(opt_extract_meta, optarg);
				break;
		 }
	}

	 argc -= optind;
	 argv += optind;		 
	
	/* We expect argc and argv to still point just one below the start of remaining args */
	argc++;
	argv--;
	
    /* Process command-line options and arguments. */

    if (opt_version) {
        version(progname);
        return EXIT_SUCCESS;
    }

    extensions = 0;
    if (opt_allext)
        extensions = 0xFFFFFF;  /* turn on all extensions */
    if (opt_no_smart)
        opt_smart = FALSE;
    if (opt_smart)
        extensions = extensions | EXT_SMART;
    if (opt_no_notes)
        opt_notes = FALSE;
    if (opt_notes)
        extensions = extensions | EXT_NOTES;
    if (opt_process_html)
        extensions = extensions | EXT_PROCESS_HTML;
    if (opt_filter_html)
        extensions = extensions | EXT_FILTER_HTML;
    if (opt_filter_styles)
        extensions = extensions | EXT_FILTER_STYLES;
    if (opt_no_labels)
        extensions = extensions | EXT_NO_LABELS;

    /* Compatibility mode turns off extensions and most 
        MultiMarkdown-specific features */
    if (opt_compatibility) {
        extensions = 0x000000;
        extensions = extensions | EXT_COMPATIBILITY;
        extensions = extensions | EXT_NO_LABELS;
    }

    numargs = argc - 1;

    if (opt_batchmode && numargs != 0) {
        /* handle each file individually, and set output to filename with
            appropriate extension */
        
           for (i = 0; i < numargs; i++) {
                inputbuf = g_string_new("");   /* string for concatenated input */
                /* Read file */
                if ((input = fopen(argv[i+1], "r")) == NULL) {
                    perror(argv[i+1]);
                    exit(EXIT_FAILURE);
                }
                while ((curchar = fgetc(input)) != EOF)
                    g_string_append_c(inputbuf, curchar);
                fclose(input);
                
                file = g_string_new(argv[i+1]);

                /* open output file */
                if (!(output = fopen(file->str, "w"))) {
                    perror(opt_output);
                    return 1;
                }
               
                out = markdown_to_string(inputbuf->str, extensions);

                fprintf(output, "%s\n", out);
                fclose(output);
                g_string_free(file,true);
                free(out);
                g_string_free(inputbuf, true);
           }
        
    } else {
        /* Read input from stdin or input files into inputbuf */

        inputbuf = g_string_new("");   /* string for concatenated input */

        if (numargs == 0) {        /* use stdin if no files specified */
            while ((curchar = fgetc(stdin)) != EOF)
                g_string_append_c(inputbuf, curchar);
            fclose(stdin);
        }
        else {                  /* open all the files on command line */
           for (i = 0; i < numargs; i++) {
                if ((input = fopen(argv[i+1], "r")) == NULL) {
                    perror(argv[i+1]);
                    exit(EXIT_FAILURE);
                }
                while ((curchar = fgetc(input)) != EOF)
                    g_string_append_c(inputbuf, curchar);
                fclose(input);
           }
        }
        
       /* we allow "-" as a synonym for stdout here */
        if (opt_output == NULL || strcmp(opt_output, "-") == 0)
            output = stdout;
        else if (!(output = fopen(opt_output, "w"))) {
            perror(opt_output);
            return 1;
        }

        out = markdown_to_string(inputbuf->str, extensions);
        fprintf(output, "%s\n", out);
        free(out);
        fclose(output);
        g_string_free(inputbuf, true);
        
    }

    return(EXIT_SUCCESS);
}
