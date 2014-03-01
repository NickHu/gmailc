#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "curl_handler.h"
#include "xml_handler.h"

#include "util.h"

static int verbose_flag;

void print_usage(char *bin_name)
{
  printf("Usage: %s [options] \n", bin_name);
  printf("Options:\n"
  "\t-h, --help	Print this message and exit.\n"
  "\t-p, --password	Specify a password.\n"
  "\t-u, --username	Specify a username.\n"
  "\t-v, --verbose	Enable verbose mode.\n"
  "\t--version	Display version.\n");
}

void read_conf(char *path, char *username, char *password)
{
  char buffer[MAX_LINE];
  FILE *conf = fopen(path, "r");
  if(conf != NULL)
  {
    while(fgets(buffer, MAX_LINE, conf) != NULL)
    {
      int pos = 0;
      while(buffer[pos] == ' ' || buffer[pos] == '\t')
        ++pos;
      if(buffer[pos] == '#')
        continue;

      char *option;

      option = strstr(buffer, "username");
      if(option)
      {
        strncpy(username, option + sizeof("username"), MAX_USERNAME);
        STRIPN(username);
        continue;
      }

      option = strstr(buffer, "password");
      if(option)
      {
        strncpy(password, option + sizeof("password"), MAX_PASSWORD);
        STRIPN(password);
        continue;
      }
    }
    fclose(conf);
  }
  else
  {
    if(verbose_flag)
      fprintf(stderr, "Failed to open file at: %s\n", path);
  }
}

int main(int argc, char *argv[])
{
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];

  // read config
  char conf_path[50];
  snprintf(conf_path, sizeof(conf_path)
          , "%s/%s/%s", getenv("HOME"), ".config", "gmailc.conf");
  read_conf(conf_path, username, password);

  // handle arguments
  int c;

  while(1)
  {
    static struct option long_options[] =
    {
      {"verbose", no_argument, &verbose_flag, 1},
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, 'V'},
      {"username", required_argument, 0, 'u'},
      {"password", required_argument, 0, 'p'},
      {0, 0, 0, 0}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "vhVu:p:",
        long_options, &option_index);

    if(c == -1)
      break;

    switch(c)
    {
      case 0:
        if (long_options[option_index].flag != 0)
          break;
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;
      case 'u':
        strncpy(username, optarg, MAX_USERNAME);
        break;
      case 'p':
        strncpy(password, optarg, MAX_PASSWORD);
        break;
      case 'v':
        verbose_flag = 1;
        break;
      case 'h':
        print_usage(argv[0]);
        exit(EXIT_SUCCESS);
      case 'V':
        printf("gmailc version: %s\n", VERSION);
        exit(EXIT_SUCCESS);
      case '?':
        // do nothing
        break;
      default:
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if(*username == '\0')
  {
    puts("No username");
    exit(EXIT_FAILURE);
  }

  if(*password == '\0')
  {
    puts("No password");
    exit(EXIT_FAILURE);
  }

  struct XML mail_xml;
  mail_xml = get_mail_xml(username, password);

  if(verbose_flag)
  {
    printf("username: %s\n"
           "password: %s\n"
           "\n",
          username, password);

    printf("Raw XML:\n"
           "%s\n"
           "\n"
           "Size: %d\n"
           "\n",
          mail_xml.data, (int) mail_xml.size);
  }

  if(!mail_xml.size)
  {
    fprintf(stderr, "No XML\n");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", xml_fullcount(mail_xml));

  return 0;
}

