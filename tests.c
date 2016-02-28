#include <stdio.h>
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
static void test_template_can_be_created_from_a_string() {
  printf("\t\tcan be created from a string");
  custache_template_t *t = custache_load_template("Hello, {{foo}}", "{{", "}}");
  assert(sizeof(*t) == sizeof(custache_template_t));
  assert(!strcmp(t->otag, "{{"));
  assert(!strcmp(t->ctag, "}}"));
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "Hello, "));
  assert(t->next);
  assert(t->next->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(t->next->content, "foo"));
  custache_free_template(t);
  t = custache_load_template("Hello, BEGINfooEND", "BEGIN", "END");
  assert(sizeof(*t) == sizeof(custache_template_t));
  assert(!strcmp(t->otag, "BEGIN"));
  assert(!strcmp(t->ctag, "END"));
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "Hello, "));
  assert(t->next);
  assert(t->next->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(t->next->content, "foo"));
  custache_free_template(t);
  printf(" ✓\n");
}

static void test_template_can_be_created_from_a_stream() {
  printf("\t\tcan be created from a stream");
  FILE *f = fopen("test.mustache", "r");
  custache_template_t *t = custache_load_template_stream(f, "{{", "}}");
  fclose(f);
  assert(!strcmp(t->otag, "{{"));
  assert(!strcmp(t->ctag, "}}"));
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "Hello, "));
  assert(t->next);
  assert(t->next->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(t->next->content, "foo"));
  custache_free_template(t);
}

static void test_template_can_be_created_from_a_file() {
  custache_template_t *t = custache_load_template_file("test.mustache", "{{", "}}");
  assert(!strcmp(t->otag, "{{"));
  assert(!strcmp(t->ctag, "}}"));
  assert(t->type == CUSTACHE_TEMPLATE_STATIC);
  assert(!strcmp(t->content, "Hello, "));
  assert(t->next);
  assert(t->next->type == CUSTACHE_TEMPLATE_BASIC);
  assert(!strcmp(t->next->content, "foo"));
  custache_free_template(t);
  t = custache_load_template_file("nonexistent", "{{", "}}");
  assert(!t);
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