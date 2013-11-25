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
  printf("Options:\n\
  -h, --help	Print this message and exit.\n\
  -p, --password	Specify a password.\n\
  -u, --username	Specify a username.\n\
  -v, --verbose	Enable verbose mode.\n\
  --version	Display version.\n");
}

int read_conf(char *path, char *username, char *password)
{
  char buffer[MAX_LINE];
  int count = 0;
  FILE *conf = fopen(path, "r");
  if(conf != NULL)
  {
    while(fgets(buffer, MAX_LINE, conf) != NULL)
    {
      if(buffer[0] == '#')
        continue;

      char *option;
      char value[32];

      option = strstr(buffer, "username");
      if(option)
      {
        strncpy(value, option + sizeof("username"), MAX_USERNAME);
        ++count;
        continue;
      }

      option = strstr(buffer, "password");
      if(option)
      {
        strncpy(value, option + sizeof("password"), MAX_PASSWORD);
        ++count;
        continue;
      }
    }
    fclose(conf);
    return count/2;
  }
  else
  {
    if(verbose_flag)
      fprintf(stderr, "Failed to open file at: %s\n", path);
    return 0;
  }
}

int main(int argc, char *argv[])
{
  char username[MAX_USERNAME + 1];
  char password[MAX_PASSWORD + 1];
  username[0] = '\0';
  password[0] = '\0';
  int parsed;

  // read config
  char conf_path[50];
  snprintf(conf_path, sizeof(conf_path)
          , "%s/%s/%s", getenv("HOME"), ".config", "gmailc.conf");
  parsed = read_conf(conf_path, username, password);

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
        ++parsed;
        break;
      case 'p':
        strncpy(password, optarg, MAX_PASSWORD);
        ++parsed;
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

  if(!parsed)
  {
    puts("No username or password");
    exit(1);
  }

  if(verbose_flag)
    printf("username: %s\npassword: %s\n", username, password);

  printf("%d\n", xml_fullcount(get_mail_xml(username, password)));

  return 0;
}

