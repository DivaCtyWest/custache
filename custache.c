#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cJSON/cJSON.h"

/**
 * Types of template nodes
 */
typedef enum {
  STATIC,
  BASIC,
  SECTION,
  INVERT,
  CLOSING
} token_type_t;

/**
 * A Template node
 */
typedef struct Template {
  unsigned char type;
  struct Template *next;
  struct Template *child;
  char *content;
} template_t;

/**
 * A View node
 */
typedef struct View {
  unsigned char type;
  char *name;
  void *content;
} view_t;

static char *parse(template_t *, char *, char *, char *);
static char *parse_static(template_t *, char *, char *, char *);
static char *parse_mustache(template_t *, char *, char *, char *);

static void _dump_template(template_t *t, unsigned int indent) {
  int i; for (i = indent; i; i--) printf(" ");
  printf("[%p %u %p %p '%s']\n", t, t->type, t->child, t-> next, t->content);
  if (t->child) _dump_template(t->child, indent + 2);
  if (t->next)  _dump_template(t->next, indent);
}

static char *render(template_t *t, view_t *v) {
  char *c = "";
  switch (t->type) {
  case STATIC: /* duplicate t->content into c */
    break;
  case BASIC: /* copy v.content to c */
    break;
  case SECTION: /* based on v's truthiness, start a new section */
  case INVERT: /* based on v's untruthiness, start a new section */
    break;
  case CLOSING: /* end the current section */
    break;
  }
  if (t->child) {
    render(t->child, v);
  }
  if (t->next) {
    render(t->next, v);
  }
  return c;
}

static void dump_template(template_t *t) {
  _dump_template(t, 0);
}

/**
 * Frees the template, by freeing all children, then all following nodes, then
 * its content, and finally the template itself.
 */
static void free_template(template_t *t) {
  if (t->child)   free_template(t->child);
  if (t->next)    free_template(t->next);
  if (t->content) free(t->content);
  free(t);
}

/**
 * Fills the template at t from the string at s. otag and ctag are used as
 * tag delimiters. Returns NULL if s has been parsed completely, otherwise
 * returns a pointer to the next unprocessed character in s.
 *
 * t must contain enough space for the template.
 */
static char *parse(template_t *t, char * s, char * otag, char * ctag) {
  char *p = strstr(s, otag);

  if (p == s)
    p = parse_mustache(t, p + strlen(otag), otag, ctag);
  else
    p = parse_static(t, s, otag, ctag);

  return p;
}

/**
 * Fills the template t with static content from the string s. Returns a
 * pointer to the next character in s.
 */
static char *parse_static(template_t *t, char * s, char * otag, char *ctag) {
  char *p = strstr(s, otag);
  size_t n = p ? p - s : strlen(s);

  t->type = STATIC;
  t->content = strndup(s, n);

  if (p) {
    t->next = calloc(1, sizeof *t->next);
    p = parse(t->next, p, otag, ctag);
  }

  return p;
}

/**
 * Fills the template t with mustache content. Returns a pointer to the next
 * character in s.
 */
static char *parse_mustache(template_t *t, char * s, char * otag, char * ctag) {

  char *p = strstr(s, ctag);
  assert(p);

  switch (*s) {
  case '#':
  case '^':
    t->type = (*s == '#' ? SECTION : INVERT);
    t->content = strndup(s + 1, p - s - 1);
    p += strlen(ctag);
    t->child = calloc(1, sizeof *t->child);
    p = parse(t->child, p, otag, ctag);
    break;
  case '/':
    t->type = CLOSING;
    t->content = strndup(s + 1, p - s - 1);
    p += strlen(ctag);
    return p;
  default:
    t->type = BASIC;
    t->content = strndup(s, p - s);
    p += strlen(ctag);
    break;
  }

  if (p) {
    t->next = calloc(1, sizeof *t->next);
    p = parse(t->next, p, otag, ctag);
  }

  return p;
}

static char *parse_json() {

}

/**
 * Makes a template from the string s, and uses it to render content with the
 * view context. Returns the rendered result, which should then be freed.
 */
char *custache(char *s, void *context) {
  template_t *t = calloc(1, sizeof *t);
  view_t *v = calloc(1, sizeof *v);
  render(t, v);
  parse(t, s, "{{", "}}");
  dump_template(t);
  free_template(t);
  return "";
}

int main(int argc, char **argv) {
  char *result;

  result = custache(argv[1], argv[2]);

  return 0;
}
