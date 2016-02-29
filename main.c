#include <stdio.h>
#include "custache.h"

int main(const int argc, const char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Syntax: custache <view> <template>\n");
    return 1;
  }
  custache_template_t *t = custache_load_template_file(argv[1], "{{", "}}");
  custache_view_t *v = custache_load_view_file(argv[2]);
  printf("Rendering...\n");
  //custache_free_view(v);
  custache_free_template(t);
  return 0;
}

