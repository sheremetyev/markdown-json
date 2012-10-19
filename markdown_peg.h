#ifndef __MARKDOWN_PEG_H__
#define __MARKDOWN_PEG_H__

/* markdown_peg.h */
#include "markdown_lib.h"
#include "glib.h"

#ifndef WIN32
extern char *strdup(const char *string);
#endif

/* Information (label, URL and title) for a link. */
struct Link {
    struct Element   *label;
    char             *url;
    char             *title;
    struct Element   *attr;
    char            *identifier;
};

typedef struct Link link;

/* Union for contents of an Element (string, list, or link). */
union Contents {
    char             *str;
    struct Link      *link;
};

/* Types of semantic values returned by parsers. */ 
#define ELEMENT_LIST(V)                                                        \
  V(LIST)                                                                      \
  V(RAW)                                                                       \
  V(SPACE)                                                                     \
  V(LINEBREAK)                                                                 \
  V(ELLIPSIS)                                                                  \
  V(EMDASH)                                                                    \
  V(ENDASH)                                                                    \
  V(APOSTROPHE)                                                                \
  V(SINGLEQUOTED)                                                              \
  V(DOUBLEQUOTED)                                                              \
  V(STR)                                                                       \
  V(LINK)                                                                      \
  V(IMAGE)                                                                     \
  V(IMAGEBLOCK)                                                                \
  V(CODE)                                                                      \
  V(HTML)                                                                      \
  V(EMPH)                                                                      \
  V(STRONG)                                                                    \
  V(PLAIN)                                                                     \
  V(PARA)                                                                      \
  V(LISTITEM)                                                                  \
  V(BULLETLIST)                                                                \
  V(ORDEREDLIST)                                                               \
  V(H1) V(H2) V(H3) V(H4) V(H5) V(H6) V(H7)                                    \
  V(BLOCKQUOTE)                                                                \
  V(VERBATIM)                                                                  \
  V(HTMLBLOCK)                                                                 \
  V(HRULE)                                                                     \
  V(REFERENCE)                                                                 \
  V(NOTE)                                                                      \
  V(CITATION)                                                                  \
  V(NOCITATION)                                                                \
  V(LOCATOR)                                                                   \
  V(NOTELABEL)                                                                 \
  V(DEFLIST)                                                                   \
  V(TERM)                                                                      \
  V(DEFINITION)                                                                \
  V(FOOTER)                                                                    \
  V(LABEL)                                                                     \
  V(HEADINGSECTION)                                                            \
  V(ENDHTML)                                                                   \
  V(TABLE)                                                                     \
  V(TABLEHEAD)                                                                 \
  V(TABLEBODY)                                                                 \
  V(TABLEROW)                                                                  \
  V(TABLECELL)                                                                 \
  V(CELLSPAN)                                                                  \
  V(TABLECAPTION)                                                              \
  V(TABLELABEL)                                                                \
  V(TABLESEPARATOR)                                                            \
  V(AUTOLABEL)                                                                 \
  V(ATTRIBUTE)                                                                 \
  V(ATTRKEY)                                                                   \
  V(ATTRVALUE)                                                                 \
  V(GLOSSARY)                                                                  \
  V(GLOSSARYTERM)                                                              \
  V(GLOSSARYSORTKEY)                                                           \
  V(MATHSPAN)                                                                  \

enum keys {
  #define KEY_DECLARATION(name) name,
  ELEMENT_LIST(KEY_DECLARATION)
  #undef KEY_DECLARATION
};

/* constants for managing Smart Typography */
enum smartelements {
    LSQUOTE,
    RSQUOTE,
    LDQUOTE,
    RDQUOTE,
    NDASH,
    MDASH,
    ELLIP,
    APOS,
};

enum smartoutput {
    HTMLOUT,
    LATEXOUT,
};

enum language {
    DUTCH,
    ENGLISH,
    FRENCH,
    GERMAN,
    SWEDISH,
    GERMANGUILL,
};

/* Semantic value of a parsing action. */
struct Element {
    int               key;
    union Contents    contents;
    struct Element    *children;
    struct Element    *next;
};



typedef struct Element element;

element * parse_references(char *string, int extensions);
element * parse_notes(char *string, int extensions, element *reference_list);
element * parse_labels(char *string, int extensions, element *reference_list, element *note_list);

element * parse_markdown(char *string, int extensions, element *reference_list, element *note_list, element *label_list);
void free_element_list(element * elt);
void free_element(element *elt);
void print_element_list(GString *out, element *elt, int exts);

#endif /* __MARKDOWN_PEG_H__ */
