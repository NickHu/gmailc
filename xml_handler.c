#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "util.h"

int xml_fullcount(struct XML xml)
{
  xmlDocPtr doc;
  xmlNode *root_element = NULL;
  doc = xmlReadMemory(xml.data, xml.size, "noname.xml", NULL, 0);
  free(xml.data);
  if(doc == NULL)
  {
    fprintf(stderr, "Failed to parse XML.\n");
    return 0;
  }
  root_element = xmlDocGetRootElement(doc);

  int fullcount = 0;
  xmlNode *cur_node = NULL;

  for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if(!strncmp((char *) cur_node->name, "fullcount", 9))
      {
        xmlChar *xml_count = xmlNodeGetContent(cur_node);
        sscanf((char *) xml_count, "%d", &fullcount);
        xmlFree(xml_count);
      }
    }
  }
  xmlFreeDoc(doc);
  xmlCleanupParser();
  return fullcount;
}

