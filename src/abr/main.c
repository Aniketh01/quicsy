#include <stdio.h>
#include <string.h>

//#include <libxml2/parser.h>
#include <libxml2/libxml/parser.h>

float get_duration(char *dur)
{
    float hour, min, sec;
    char *tmp2, *tmp3;
    tmp3 = dur;
    tmp2 = strstr(tmp3, "H");
    if (tmp2 == NULL)
        hour = 0.0;
    else
    {
        *tmp2 = '\0';
        hour = atof(tmp3 + 2);
        tmp3 = tmp2 + 1;
    }

    tmp2 = strstr(tmp3, "M");
    if (tmp2 == NULL)
        min = 0.0;
    else
    {
        *tmp2 = '\0';
        min = atof(tmp3);
        tmp3 = tmp2 + 1;
    }

    tmp2 = strstr(tmp3, "S");
    if (tmp2 == NULL)
        sec = 0.0;
    else
    {
        *tmp2 = '\0';
        sec = atof(tmp3);
        tmp3 = tmp2 + 1;
    }
    return (hour * 3600) + (min * 60) + sec;
}

int main(int argc, char **argv)
{
    xmlDoc *document;
    xmlNode *root, *first_child, *node;
    xmlAttr *attribute;
    char duration[25] = "\0";
    float dur;

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
        if (xmlStrcmp(node->name, (const xmlChar *)"Period") == 0)
        {
            attribute = node->properties;
            while (attribute)
            {
                if (xmlStrcmp(attribute->name, (const xmlChar *)"duration") == 0)
                {
                    xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
                    fprintf(stdout, "\t\t%s : %s\n", (char *)attribute->name, (char *)value);
                    strcpy(duration, (char *)value);
                    fprintf(stdout, "Duration is %s and %f\n", duration, dur = get_duration(duration));
                    get_duration(duration);
                    xmlFree(value);
                    break;
                }
                attribute = attribute->next;
            }
        }
    }
    fprintf(stdout, "...\n");
    return 0;
}