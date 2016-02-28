#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "custache.h"

typedef custache_template_t template_t;
typedef custache_view_t view_t;

#define TSTATIC CUSTACHE_TEMPLATE_STATIC
#define TBASIC CUSTACHE_TEMPLATE_BASIC
#define TSECTION CUSTACHE_TEMPLATE_SECTION
#define TINVERT CUSTACHE_TEMPLATE_INVERT
#define TCLOSING CUSTACHE_TEMPLATE_CLOSING

typedef enum {
  VNULL, VSTRING, VNUMBER, VOBJECT, VARRAY, VTRUE, VFALSE
} view_type_t;

static char *read_file(FILE *f);
static template_t *load_template_file(const char *, const char *, const char *);
static template_t *load_template_stream(FILE *, const char *, const char *);
static template_t *load_template(const char *, const char *, const char *);
static char *compile_template(template_t *, const char *);
static char *compile_mustache(template_t *, const char *);
static char *compile_static(template_t *, const char *);
static void free_template(template_t *t);
static view_t *load_view_file(const char *);
static view_t *load_view_stream(FILE *);
static view_t *load_view(const char *);
static char *compile_view(view_t *, const char *);

static char *read_file(FILE *f) {
  assert(f);
  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  char *s = (char *)malloc(filesize + 1);
  fseek(f, 0, SEEK_SET);
  fread(s, filesize, 1, f);
  s[(unsigned int) filesize] = 0;
  return s;
}

static template_t *load_template_file(const char *s, const char *otag, const char *ctag) {
  FILE *f = fopen(s, "r");
  if (!f) return NULL;
  template_t *t = load_template_stream(f, otag, ctag);
  fclose(f);
  return t;
}

static template_t *load_template_stream(FILE *f, const char *otag, const char *ctag) {
  if (!f) return NULL;
  char *s = read_file(f);
  template_t *t = load_template(s, otag, ctag);
  free(s);
  return t;
}

static template_t *load_template(const char *s, const char *otag, const char *ctag) {
  template_t *t = calloc(1, sizeof *t);
  t->otag = strdup(otag);
  t->ctag = strdup(ctag);
  compile_template(t, s);
  return t;
}

static char *compile_template(template_t *t, const char *s) {
  if (!t->otag) t->otag = "{{";
  if (!t->ctag) t->ctag = "}}";
  char *p = strstr(s, t->otag);
  if (p == s)
    p = compile_mustache(t, p + strlen(t->otag));
  else
    p = compile_static(t, s);
  return p;
}

static char *compile_mustache(template_t *t, const char *s) {
  char *p = strstr(s, t->ctag);
  assert(p);

  switch (*s) {
  case '#':
  case '^':
    t->type = (*s == '#' ? TSECTION : TINVERT);
    t->content = strndup(s + 1, p - s - 1);
    p += strlen(t->ctag);
    t->child = calloc(1, sizeof *t->child);
    t->child->otag = strdup(t->otag);
    t->child->ctag = strdup(t->ctag);
    p = compile_template(t->child, p);
    break;
  case '/':
    t->type = TCLOSING;
    t->content = strndup(s + 1, p - s - 1);
    p += strlen(t->ctag);
    return p;
  default:
    t->type = TBASIC;
    t->content = strndup(s, p - s);
    p += strlen(t->ctag);
    break;
  }

  if (p) {
    t->next = calloc(1, sizeof *t->next);
    t->next->otag = strdup(t->otag);
    t->next->ctag = strdup(t->ctag);
    p = compile_template(t->next, p);
  }

  return p;
}

static char *compile_static(template_t *t, const char *s) {
  char *p = strstr(s, t->otag);
  size_t n = p ? p - s : strlen(s);

  t->type = TSTATIC;
  t->content = strndup(s, n);

  if (p) {
    t->next = calloc(1, sizeof *t->next);
    t->next->otag = t->otag;
    t->next->ctag = t->ctag;
    p = compile_template(t->next, p);
  }

  return p;
}

static void free_template(template_t *t) {
  if (t->child) free_template(t->child);
  if (t->next) free_template(t->next);
  if (t->content) free(t->content);
  free(t);
}

static char *inspect_template(template_t *v) {
  return "INSPECT TEMPLATE";
}

static view_t *load_view_file(const char *s) {
  FILE *f = fopen(s, "r");
  view_t *v = load_view_stream(f);
  fclose(f);
  return v;
}

static view_t *load_view_stream(FILE *f) {
  char *s = read_file(f);
  view_t *v = load_view(s);
  free(s);
  return v;
}

static view_t *load_view(const char *s) {
  view_t *v = calloc(1, sizeof v);
  compile_view(v, s);
  return v;
}

static char *compile_view(view_t *v, const char *s) {
  strcpy(v->content, s);
  return v->content;
}

static void free_view(view_t *v) {
  if (v->child) free_view(v->child);
  if (v->next) free_view(v->next);
  if (v->content) free_view(v->content);
  free(v);
}

static char *inspect_view(view_t *v) {
  return "INSPECT VIEW";
}

void render(template_t *t, view_t *v) {
  printf("template:[%s], view:[%s]\n", inspect_template(t), inspect_view(v));
}

custache_view_t *custache_load_view(const char *s) {
  return load_view(s);
}

custache_view_t *custache_load_view_stream(FILE *f) {
  return load_view_stream(f);
}

custache_view_t *custache_load_view_file(const char *s) {
  return load_view_file(s);
}

void custache_free_view(view_t *v) {
  free_view(v);
}

custache_template_t *custache_load_template(const char *s, const char *otag, const char *ctag) {
  return load_template(s, otag, ctag);
}

custache_template_t *custache_load_template_stream(FILE *f, const char *otag, const char *ctag) {
  return load_template_stream(f, otag, ctag);
}

custache_template_t *custache_load_template_file(const char *s, const char *otag, const char *ctag) {
  return load_template_file(s, otag, ctag);
}

const char *custache_render(const custache_template_t *t, const custache_view_t *v) {
  printf("Write me...");
  return "";
}

void custache_free_template(template_t *t) {
  free_template(t);
}
