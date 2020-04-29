#include "mpd_parse.h"
#include "utils.h"


int main(int argc, char **argv)
{
    xmlDoc *document;
    manifest m[] = {0};
    xmlNode *root, *first_child, *node, *second_child, *node2, *third_child, *node3, *node4, *fourth_child;
    xmlAttr *attribute;
    char duration[25] = "\0";
    float dur, segdur = 0, timescale = 0;
    int num_of_rates = 0, height;
    char segnum[5];
    char bw[25] = "", repid[25] = "";
    char bandwidth[MAX_SUPPORTED_BITRATE_LEVELS][25];
    char id[MAX_SUPPORTED_BITRATE_LEVELS][5] = {{0}};
    char base_url[MAXURLLENGTH] = "";
    char init_url[MAX_SUPPORTED_BITRATE_LEVELS][MAXURLLENGTH] = {""};
    char media_url[MAX_SUPPORTED_BITRATE_LEVELS][MAXURLLENGTH] = {""};
    int startNumber[MAX_SUPPORTED_BITRATE_LEVELS], sn = 1;
    char init_url_template[MAXURLLENGTH] = "";
    char media_url_template[MAXURLLENGTH] = "";
    char *tmp, *newurl;
    char keyword_id[12] = "$id$";
    char keyword_bw[12] = "$Bandwidth$";
    char keyword_num[12] = "$Number$";

    char *filename;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return 1;
    }
    filename = argv[1];

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);

    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);

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
            second_child = node->children;
            for (node2 = second_child; node2; node2 = node2->next)
            {
                fprintf(stdout, "\t\t Child is <%s> (%i)\n", node2->name, node2->type);
                if (xmlStrcmp(node2->name, (const xmlChar *)"AdaptationSet") == 0)
                {
                    third_child = node2->children;
                    for (node4 = third_child; node4; node4 = node4->next)
                    {
                        fprintf(stdout, "\t\t Child is <%s> (%i)\n", node4->name, node4->type);
                        if (xmlStrcmp(node4->name, (const xmlChar *)"Representation") == 0)
                        {
                            fourth_child = node4->children;
                            for (node3 = fourth_child; node3; node3 = node3->next)
                            {
                                if (xmlStrcmp(node3->name, (const xmlChar *)"SegmentTemplate") == 0)
                                {
                                    fprintf(stdout, "\t\t Child is <%s> (%i)\n", node3->name, node3->type);
                                    attribute = node3->properties;
                                    while (attribute)
                                    {
                                        fprintf(stdout, ">>>>>>>>>>>>>>>>%s\n", (char *)attribute->name);
                                        if (xmlStrcmp(attribute->name, (const xmlChar *)"duration") == 0)
                                        {
                                            xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                            segdur = atoi((char *)value);
                                            xmlFree(value);
                                        }
                                        else if (xmlStrcmp(attribute->name, (const xmlChar *)"timescale") == 0)
                                        {
                                            xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                            timescale = atoi((char *)value);
                                            xmlFree(value);
                                        }
                                        else if (xmlStrcmp(attribute->name, (const xmlChar *)"startNumber") == 0)
                                        {
                                            xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                            sn = atoi((char *)value);
                                            xmlFree(value);
                                        }
                                        else if (xmlStrcmp(attribute->name, (const xmlChar *)"initialization") == 0 || xmlStrcmp(attribute->name, (const xmlChar *)"index") == 0)
                                        {
                                            xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                            strcpy(init_url_template, base_url);
                                            strcat(init_url_template, (char *)value);
                                            fprintf(stdout, "Init_url being filled here!!! %s (%s)\n", init_url_template, (char *)value);
                                            fflush(stdout);
                                            xmlFree(value);
                                        }
                                        else if (xmlStrcmp(attribute->name, (const xmlChar *)"media") == 0)
                                        {
                                            xmlChar *value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                            strcpy(media_url_template, base_url);
                                            strcat(media_url_template, (char *)value);
                                            fprintf(stdout, "Media_url is %s \n", media_url_template);
                                            xmlFree(value);
                                        }

                                        attribute = attribute->next;
                                    }

                                    if (strlen(init_url_template) > 0)
                                        m->init = 1;
                                    else
                                        m->init = 0;
                                    m->num_of_segments = ceil(dur / (segdur / timescale)) + m->init;
                                }
                            }
                        }
                    }

                    strcpy(media_url[num_of_rates], media_url_template);
                    strcpy(init_url[num_of_rates], init_url_template);
                    strcpy(bandwidth[num_of_rates], bw);
                    strcpy(id[num_of_rates], repid);
                    startNumber[num_of_rates] = sn;
                    fprintf(stdout, "Init segment : %s\n", init_url[num_of_rates]);
                    fprintf(stdout, "Media segment : %s\n", media_url[num_of_rates]);
                    fprintf(stdout, "Timescale : %f, Seg duration : %f\n", timescale, segdur);
                    fprintf(stdout, "Number of Segments = %d\n", m->num_of_segments);
                    num_of_rates++;
                    if (num_of_rates >= MAX_SUPPORTED_BITRATE_LEVELS)
                    {
                        fprintf(stdout, "Number of rate levels exceeds the maximum allowed value\n");
                        return -1;
                    }
                }
            }
        }
    }
    if (m->num_of_segments < 0)
    {
        fprintf(stdout, "Number of rate levels / segments is negative, check mpd. \n");
        return -1;
    }
    m->num_of_levels = num_of_rates;
    m->segment_dur_ms = (segdur / timescale);

    int j, k;
    for (j = 0; j < num_of_rates; j++)
    {
        level *next_level = &m->bitrate_level[j];
        next_level->segments = (char **)malloc(m->num_of_segments * sizeof(char *));

        for (k = 0; k < m->num_of_segments; k++)
            next_level->segments[k] = (char *)malloc(MAXURLLENGTH * sizeof(char));

        next_level->bitrate = atoi(bandwidth[j]);
        if (strlen(init_url[j]) != 0)
        {
            newurl = str_replace(init_url[j], keyword_bw, bandwidth[j]);
            if (newurl == NULL)
            {
                newurl = str_replace(init_url[j], keyword_id, id[j]);
                if (newurl == NULL)
                    strcpy(next_level->segments[0], init_url[j]);
                else
                {
                    strcpy(next_level->segments[0], newurl);
                    free(newurl);
                }
            }
            else
            {
                fprintf(stdout,"Replaced bandwidth \n");
                strcpy(next_level->segments[0], newurl);
                free(newurl);
            }
        }

        sn = startNumber[j];
        for (k = m->init; k < m->num_of_segments; k++)
        {
            sprintf(segnum, "%d", sn);
            sn++;
            tmp = str_replace(media_url[j], keyword_bw, bandwidth[j]);
            if (tmp == NULL)
            {
                tmp = str_replace(media_url[j], keyword_id, id[j]);
                if (tmp == NULL)
                {
                    newurl = str_replace(media_url[j], keyword_num, segnum);
                    if (newurl == NULL)
                    {
                        strcpy(next_level->segments[k], media_url[j]);
                    }
                    else
                    {
                        strcpy(next_level->segments[k], newurl);
                        free(newurl);
                    }
                }
                else
                {
                    newurl = str_replace(tmp, keyword_num, segnum);
                    if (newurl == NULL)
                    {
                        strcpy(next_level->segments[k], tmp);
                    }
                    else
                    {
                        strcpy(next_level->segments[k], newurl);
                        free(newurl);
                    }
                    free(tmp);
                }
            }
            else
            {
                newurl = str_replace(tmp, keyword_num, segnum);
                if (newurl == NULL)
                {
                    strcpy(next_level->segments[k], tmp);
                }
                else
                {
                    strcpy(next_level->segments[k], newurl);
                    free(newurl);
                }
                free(tmp);
            }
            // fprintf(stdout, "%s\n", next_level->segments[k]);
        }
    }

    fprintf(stdout, "...\n");
    return 0;
}