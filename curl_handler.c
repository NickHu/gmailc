#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <curl/curl.h>

#include "curl_handler.h"

#include "util.h"

static size_t curl_write(char *ptr, size_t size, size_t nmemb, void *data)
{
  size_t real_size = size * nmemb;
  struct XML *xml = (struct XML *) data;

  xml->data = realloc(xml->data, xml->size + real_size);
  if(xml->data == NULL)
  {
    /* out of memory */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(xml->data[xml->size]), ptr, real_size);
  xml->size += real_size;
  xml->data[xml->size] = 0;

  return real_size;
}

struct XML get_mail_xml(char *username, char *password)
{
  struct XML xml_data;
  xml_data.data = malloc(1);
  xml_data.size = 0;

  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();

  if(curl)
  {
    char url[100];
    snprintf(url, sizeof(url),
            "https://%s:%s@mail.google.com/mail/feed/atom",
            url_encode(username), url_encode(password));
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &xml_data);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "cURL: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  assert(xml_data.data);

  return xml_data;
}

char *url_encode(char *string)
{
  // TODO: handle more than @ if this ever becomes necessary
  const char delim[] = "@";
  int at_count = 0;
  for(int i = 0; string[i]; ++i)
    if(string[i] == '@')
      ++at_count;

  // @ => %40, hence 1 character becomes 3 characters in the new buffer
  const size_t encoded_size = strlen(string) + 2 * at_count;
  char *encoded = malloc(encoded_size);
  encoded[0] = '\0';

  char *token = strtok(string, delim);

  while(token != NULL)
  {
    strncat(encoded, token, encoded_size - strlen(encoded));
    char *next = strtok(NULL, delim);
    if(next)
      strncat(encoded, "%40", sizeof("%40"));
    token = next;
  }
  return encoded;
}
