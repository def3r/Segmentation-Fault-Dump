#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "libxml/xmlstring.h"

void traverse_html(xmlNode* node) {
  if (node == nullptr) {
    return;
  }

  xmlNode* n = nullptr;
  for (n = node; n; n = n->next) {
    if (n->type == XML_ELEMENT_NODE) {
      std::cout << "Node: " << n->name << "\n";
      xmlChar* href = xmlGetProp(n, (xmlChar*)"href");
      if (href != nullptr) {
        std::cout << "href : " << href << "\n";
      }
      std::free(href);

      xmlChar* class_attr = xmlGetProp(n, (xmlChar*)"class");
      if (class_attr != nullptr) {
        std::cout << "class : " << class_attr << "\n";
      }
      std::free(class_attr);

      if (std::string((char*)n->name) == "iframe") {
        xmlChar* title = xmlGetProp(n, (xmlChar*)"title");
        xmlChar* src = xmlGetProp(n, (xmlChar*)"src");

        if (title != nullptr) {
          std::cout << "\tTitle: " << title << std::endl;
        }
        if (src != nullptr) {
          std::cout << "\tSrc  : " << src << std::endl;
        }

        std::free(src);
        std::free(title);
      }
    } else if (n->type == XML_TEXT_NODE && n->content) {
      std::cout << "Text: " << n->content << std::endl;
    }
    traverse_html(n->children);
  }
}

void traverse(xmlNode* node) {
  if (node == nullptr) {
    return;
  }

  xmlNode* n = nullptr;
  for (n = node; n; n = n->next) {
    if (n->type == XML_CDATA_SECTION_NODE) {
      std::cout << "CDATA: " << n->content << std::endl;
      xmlDoc* h = htmlReadMemory(
          (char*)n->content, std::strlen((char*)n->content), NULL, NULL,
          HTML_PARSE_RECOVER);  // htmlParseDoc is legacy?
      if (h == nullptr) {
        std::cerr << "Cant parse html" << std::endl;
        std::abort();
      }
      xmlNode* html_node = xmlDocGetRootElement(h);
      traverse_html(html_node);
      xmlFreeDoc(h);
    }
    traverse(n->children);
  }
}

int main() {
  xmlDoc* doc;
  xmlNode* node;

  doc = xmlReadFile("./xmlhtml.xml", NULL, 0);
  if (doc == nullptr) {
    std::cout << "Can't open xml" << std::endl;
  }

  node = xmlDocGetRootElement(doc);

  traverse(node);

  xmlFreeDoc(doc);
}
