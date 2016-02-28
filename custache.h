#ifndef CUSTACHE_H
#define CUSTACHE_H

typedef enum {
  CUSTACHE_TEMPLATE_STATIC,
  CUSTACHE_TEMPLATE_BASIC,
  CUSTACHE_TEMPLATE_SECTION,
  CUSTACHE_TEMPLATE_INVERT,
  CUSTACHE_TEMPLATE_CLOSING
} custache_template_type_t;

typedef struct custache_template {
  custache_template_type_t type;
  struct custache_template *next, *child;
  char *content, *otag, *ctag;
} custache_template_t;

typedef struct custache_view {
  unsigned char type;
  struct custache_view *next, *child;
  void *content;
} custache_view_t;

custache_template_t *
custache_load_template(
    const char *,
    const char *,
    const char *
);

custache_template_t *
custache_load_template_stream(
    FILE *,
    const char *,
    const char *
);

custache_template_t *
custache_load_template_file(
    const char *,
    const char *,
    const char *
);

void
custache_free_template(
    custache_template_t *
);

custache_view_t *
custache_load_view(
    const char *
);

custache_view_t *
custache_load_view_stream(
    FILE *
);

custache_view_t *
custache_load_view_file(
    const char *
);

void
custache_free_view(
    custache_view_t *
);

const char *
custache_render(
    const custache_template_t *,
    const custache_view_t *
);

#endif
