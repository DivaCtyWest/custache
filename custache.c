#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LENGTH(x) (sizeof(x) / sizeof(*x))

typedef enum {
  STATIC = 0,
  BASIC,
  SECTION,
  INVERT
} template_type_t;

typedef struct Template {
  template_type_t type;
  struct Template *next;
  struct Template *child;
  char *content;
} template_t;

typedef struct Context {
} context_t;

static template_t *compile(char *);
static void parse(template_t *, char *, char *, char *);
static void mustache(template_t *, char *, char *, char *);

static void debug_template(template_t *, unsigned int);

char *custache(char *template, void *context) {
  template_t *t = compile(template);
  char *result;
  asprintf(&result, "Template at %p\n", t);
  free(t);
  return result;
}

static template_t *compile(char *src) {
  template_t *t = malloc(sizeof *t);
  parse(t, src, "{{", "}}");
  return t;
}

static void parse(template_t *t, char *src, char *otag, char *ctag) {
  size_t size = strlen(otag);
  char *ptr = src;

  printf("PARSE   : ");
  debug_template(t, 0);

  if (!*ptr) {
    return;
  }

  ptr = strstr(src, otag);
  if (!ptr) {
    /* Blow up if we're expecting more content */
    assert(!t->type);
  }
  else if (ptr == src) {
    /* No static content - {{mustache}} */
    bzero(ptr, size); /* Terminate the string, for the preceding templates */
    ptr += size; /* Go to the start of the {{mustache}} */
    mustache(t, ptr, otag, ctag);
  }
  else {
    /* Static content - build the template */
    t->type = STATIC; /* It's boring static content... */
    t->next = malloc(sizeof *t); /* ... but possibly some additional content. */
    t->content = src; /* Remember this string... */
    parse(t->next, ptr, otag, ctag);
  }

  printf("PARSEEND: ");
  debug_template(t, 0);

}

static void mustache(template_t *t, char *src, char *otag, char *ctag) {
  size_t size = strlen(ctag);
  char *ptr = src;

  printf("MUSTACHE: ");
  debug_template(t, 0);

  switch (*ptr++) {
    case '#': /* section */
      t->type = SECTION;
      t->child = malloc(sizeof *(t->child));
      break;
    case '^': /* inverted section */
      t->type = INVERT;
      t->child = malloc(sizeof *t);
      break;
    case '/': /* end of section */
      assert(t->type == SECTION);
      assert(!strcmp(t->content, ptr + 1));
      break;
    case '=': /* delimiters */
      break;
    case '>': /* partial */
      break;
    default: /* basic */
      ptr--;
      t->type = BASIC;
      t->next = malloc(sizeof *(t->next));
  }
  t->content = ptr;         /* remember the tag name */
  ptr = strstr(ptr, ctag);  /* go to the start of the closing tag */
  assert(ptr);              /* make sure there IS a closing tag! */
  bzero(ptr, size); /* terminate the string */
  ptr += size;      /* go to the end of the closing tag */

}

static void debug_template(template_t *t, unsigned int indent) {
  int i;
  for (i = indent; i; i--) printf(" "); 
  printf("[%p TYPE %o CHILD %p NEXT %p CONTENT '%s']", t, t->type, t->child, t-> next, t->content);
  if (t->child) debug_template(t->child, indent + 2);
  if (t->next) debug_template(t->next, indent);
  printf("\n");
}

int main(int argc, char **argv) {
  char *result;

  printf("Starting...\n");

  result = custache(argv[1], argv[2]);

  printf("%s\n", result);

  free(result);

  return 0;
}

