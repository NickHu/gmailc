#include <string.h>
#include <stdbool.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "util.h"

int xml_fullcount(struct XML xml)
{
  bool failed = false;
  xmlDocPtr doc;
  xmlNode *root_element = NULL;
  doc = xmlReadMemory(xml.data, xml.size, "noname.xml", NULL, 0);
  free(xml.data);
  if(doc == NULL)
  {
    fprintf(stderr, "Failed to parse XML.\n");
    failed = true;
  }
  root_element = xmlDocGetRootElement(doc);

  // A HTML 401 is returned if authentication is invalid
  if(!strncmp((char *) root_element->name, "HTML", sizeof("HTML")))
    if(!strncmp(
      (char *) xmlNodeGetContent(
        // TITLE tag
        xmlFirstElementChild(xmlFirstElementChild(root_element))),
      "Unauthorized",
      sizeof("Unauthorized")))
    {
      fprintf(stderr, "Authentication failed\n");
      failed = true;
    }

  int fullcount;
  xmlNode *cur_node = NULL;

  for(cur_node = xmlFirstElementChild(root_element);
      cur_node;
      cur_node = cur_node->next)
  {
    if(cur_node->type == XML_ELEMENT_NODE)
    {
      if(!strncmp((char *) cur_node->name,
          "fullcount", sizeof("fullcount")))
      {
        xmlChar *xml_count = xmlNodeGetContent(cur_node);
        sscanf((char *) xml_count, "%d", &fullcount);
        xmlFree(xml_count);
      }
    }
  }
  xmlFreeDoc(doc);
  xmlCleanupParser();

  if(failed)
    exit(EXIT_FAILURE);

  return fullcount;
}

