#ifndef _util_h
#define _util_h

#define STRIPN(x) (x)[strlen((x)) - 1] = '\0'

static const int MAX_LINE = 100;
static const int MAX_USERNAME = 30;
static const int MAX_PASSWORD = 18;

struct XML
{
  char *data;
  size_t size;
};

#endif

