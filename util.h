#ifndef _util_h
#define _util_h

#define stripn(x) (x)[strlen((x)) - 1] = '\0'

static int const MAX_LINE = 100;
static int const MAX_USERNAME = 30;
static int const MAX_PASSWORD = 18;

struct XML
{
  char *data;
  size_t size;
};

#endif
