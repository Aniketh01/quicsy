#include <stdio.h>
#include <string.h>

//#include <libxml2/parser.h>
#include <libxml2/libxml/parser.h>

int main(int argc, char **argv)
{
    xmlDoc *document;
    xmlNode *root, *first_child, *node;
    xmlAttr *attribute;

    char *filename;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return 1;
    }
    filename = argv[1];

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);

    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->name);

    first_child = root->children;
    for (node = first_child; node; node = node->next)
    {
        fprintf(stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
    }
    fprintf(stdout, "...\n");
    return 0;
}