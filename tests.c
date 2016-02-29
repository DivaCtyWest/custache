#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "custache.h"

/* view tests */
static void test_view_can_be_created() {
  printf("\t\tcan be created");
  assert(1);
  printf(" ✗\n");
}

/* template tests */
static void test_template_content(custache_template_t *t) {
  custache_template_t *u;
  assert(sizeof(*t) == sizeof(custache_template_t));
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "Hello, "));
  assert(!t->child);
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(t->content, "name"));
  assert(!t->child);
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, ".\n"));
  assert(!t->child);
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_SECTION);
  assert(!strcmp(t->content, "message_count"));
  assert(u = t->child);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, "\nYou have "));
  assert(!u->child);
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(u->content, "message_count"));
  assert(!u->child);
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, " messages:\n"));
  assert(!u->child);
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_CLOSING);
  assert(!strcmp(u->content, "message_count"));
  assert(!u->next);
  assert(!u->child);
  assert(t->next);
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "\n"));
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_SECTION);
  assert(!strcmp(t->content, "messages"));
  assert(u = t->child);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, "\n- From "));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(u->content, "from"));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, ": "));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(u->content, "subject"));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, "\n"));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(u->content, "message"));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, "\n"));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_CLOSING);
  assert(!strcmp(u->content, "messages"));
  assert(!u->next);
  assert(!u->child);
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "\n"));
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_INVERT);
  assert(!strcmp(t->content, "message_count"));
  assert(u = t->child);
  assert(u->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(u->content, "\nYou have no messages.\n"));
  assert(u = u->next);
  assert(u->type == CUSTACHE_TEMPLATE_CLOSING);
  assert(!strcmp(u->content, "message_count"));
  assert(!u->next);
  assert(!u->child);
  assert(t = t->next);
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "\n"));
  assert(!t->next);
  assert(!t->child);
}

static void test_template_can_be_created_from_a_string() {
  printf("\t\tcan be created from a string");
  FILE *f = fopen("test.mustache", "r");
  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  char *s = (char *)malloc(filesize + 1);
  fseek(f, 0, SEEK_SET);
  fread(s, filesize, 1, f);
  s[(unsigned int) filesize] = 0;
  fclose(f);
  custache_template_t *t = custache_load_template(s, "{{", "}}");
  free(s);
  test_template_content(t);
  custache_free_template(t);
  printf(" ✓\n");
}

static void test_template_can_be_created_from_a_stream() {
  printf("\t\tcan be created from a stream");
  FILE *f = fopen("test.mustache", "r");
  custache_template_t *t = custache_load_template_stream(f, "{{", "}}");
  fclose(f);
  test_template_content(t);
  custache_free_template(t);
  printf(" ✓\n");
}

static void test_template_can_be_created_from_a_file() {
  printf("\t\tcan be created from a file");
  custache_template_t *t = custache_load_template_file("test.mustache", "{{", "}}");
  test_template_content(t);
  custache_free_template(t);
  printf(" ✓\n");
}

static void test_template_can_be_created() {
  test_template_can_be_created_from_a_string();
  test_template_can_be_created_from_a_stream();
  test_template_can_be_created_from_a_file();
}

static void test_template_can_be_freed() {
  printf("\t\tcan be freed");
  custache_template_t *t = custache_load_template("Hello, {{foo}}", "{{", "}}");
  custache_free_template(t);
  printf(" ✓\n");
}

/* custache tests */
static void test_custache_string_view_string_template() {
  printf("\t\tcan render a string view through a string template\n");
}

static void test_custache_string_view_stream_template() {
  printf("\t\tcan render a string view through a stream template\n");
}

static void test_custache_stream_view_string_template() {
  printf("\t\tcan render a stream view through a string template\n");
}

static void test_custache_stream_view_stream_template() {
  printf("\t\tcan render a stream view through a stream template\n");
}

/* runners */
static void run_view_tests() {
  printf("\tView\n");
  test_view_can_be_created();
}

static void run_template_tests() {
  printf("\tTemplate\n");
  test_template_can_be_created();
  test_template_can_be_freed();
}

static void run_custache_tests() {
  printf("\tCustache\n");
  test_custache_string_view_string_template();
  test_custache_string_view_stream_template();
  test_custache_stream_view_string_template();
  test_custache_stream_view_stream_template();
}

/* main */
int main(const int argc, const char **argv) {
  printf("Custache Tests\n");
  run_template_tests();
  run_view_tests();
  run_custache_tests();
  return 0;
}
