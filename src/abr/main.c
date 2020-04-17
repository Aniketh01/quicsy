#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

//#include <libxml2/parser.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

#define MAXURLLENGTH 512
#define MAX_SUPPORTED_BITRATE_LEVELS 24

typedef struct
{
    long bitrate;
    char **segments;
} level;


typedef struct
{
    /*DASH params*/
    int num_of_segments;
    int num_of_levels;
    int segment_dur_ms;
    uint8_t init; /*boolean - init segment (with no media data) exists or not*/ 
    level bitrate_level[MAX_SUPPORTED_BITRATE_LEVELS];
    
}manifest;

/*http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
 * Function returns NULL when there is no replacement to be made.
 */

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements
//	cout<<"woah boy replacing "<<rep<<" with "<<with;

    if (!orig)
    {
       // printf(" not found 1\n");
        return NULL;
    }
    if (!rep || !(len_rep = strlen(rep)))
    {
      //  printf(" not found 2 \n");
        return NULL;
    }
    if (!(ins = strstr(orig, rep)))
    {
       // printf(" not found 3\n");
        return NULL;
    }
    if (!with)
        with[0] = '\0';
    len_with = strlen(with);
    tmp = strstr(ins, rep);
    for (count = 0; tmp ; ++count) {
        ins = tmp + len_rep;
        tmp = strstr(ins, rep);
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = (char *)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
    {
    	//printf(" not found \n");
        return NULL;
    }
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
//	cout<<" done "<<endl;

    return result;
}

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
    manifest *m = {0};
    xmlNode *root, *first_child, *node, *second_child, *node2, *third_child, *node3;
    xmlAttr *attribute;
    char duration[25] = "\0";
    float dur, segdur = 0, timescale = 0;
    int num_of_rates = 0, height;
    char            segnum[5];
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
            second_child = node->children;
            for (node2 = second_child; node2; node2 = node2->next) {
                fprintf(stdout, "\t\t Child is <%s> (%i)\n", node2->name, node2->type);
                if(xmlStrcmp(node2->name, (const xmlChar *) "AdaptationSet")==0) {
                    third_child = node2->children;
                    for (node3 = third_child; node3; node3 = node3->next) {
                        if(xmlStrcmp(node3->name, (const xmlChar *) "SegmentTemplate")==0) {
                            fprintf(stdout, "\t\t Child is <%s> (%i)\n", node3->name, node3->type);
                            attribute = node3->properties;
                            while(attribute) {
                                fprintf(stdout, ">>>>>>>>>>>>>>>>%s\n", (char *)attribute->name);
                                if(xmlStrcmp(attribute->name, (const xmlChar *) "duration")==0) {
                                    xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                    segdur=atoi((char *)value);
                                    xmlFree(value);
                                } else if(xmlStrcmp(attribute->name, (const xmlChar *) "timescale")==0) {
                                    xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                    timescale=atoi((char *)value);
                                    xmlFree(value);
                                } else if(xmlStrcmp(attribute->name, (const xmlChar *) "startNumber")==0) {
                                    xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                    sn=atoi((char *)value);
                                    xmlFree(value);
                                } else if(xmlStrcmp(attribute->name, (const xmlChar *) "initialization")==0 
                                        || xmlStrcmp(attribute->name, (const xmlChar *) "index")==0) {
                                    xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                    strcpy(init_url_template, base_url);
                                    strcat(init_url_template, (char *)value);
                                    fprintf(stdout, "Init_url being filled here!!! %s (%s)\n", init_url_template, (char *)value); fflush(stdout);
                                    xmlFree(value);
                                } else if(xmlStrcmp(attribute->name, (const xmlChar *) "media")==0) {
                                    xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
                                    strcpy(media_url_template, base_url);
                                    strcat(media_url_template, (char *) value);
                                    fprintf(stdout,"Media_url is %s \n", media_url_template);
                                    xmlFree(value);
                                }

                                attribute = attribute->next;
                            }

                            if (strlen(init_url_template)>0)
                    			m->init = 1; 
		                    else 
                                m->init = 0; 
                            m->num_of_segments = ceil(dur/(segdur/timescale)) + m->init;
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
                    if(num_of_rates>=MAX_SUPPORTED_BITRATE_LEVELS) {
                        fprintf(stdout, "Number of rate levels exceeds the maximum allowed value\n");
                        return -1;
                    }
                }
            }
        }
    }
    if( m->num_of_segments < 0) {
        fprintf(stdout, "Number of rate levels / segments is negative, check mpd. \n");
        return -1;
    }
    m->num_of_levels = num_of_rates;
    m->segment_dur_ms = (segdur/timescale);

    int j, k;
        for (j = 0; j < num_of_rates; j++)
    {
        level * next_level = &m->bitrate_level[j];
        next_level->segments = (char **) malloc (m->num_of_segments * sizeof(char *));
        
        for (k = 0; k < m->num_of_segments ; k++)
            next_level->segments[k] = (char *)malloc ( MAXURLLENGTH * sizeof (char));
        
        next_level->bitrate = atoi(bandwidth[j]);
        if( strlen(init_url[j])!=0)
	    {
            newurl = str_replace(init_url[j], keyword_bw, bandwidth[j]);
            if(newurl == NULL)
            {
                newurl = str_replace(init_url[j], keyword_id, id[j]);
                if(newurl == NULL)
                    strcpy(next_level->segments[0], init_url[j]);
                else
                {
                    strcpy(next_level->segments[0], newurl);
                    free(newurl);
                }
            }
            else
            {
                //printdebug(READMPD,"Replaced bandwidth \n");
                strcpy(next_level->segments[0], newurl);
                free(newurl);
            }
        }

	    sn = startNumber[j];
        for (k = m->init; k < m->num_of_segments; k++)
        {
            sprintf(segnum,"%d", sn);
            sn++; 
            tmp = str_replace(media_url[j], keyword_bw, bandwidth[j]);
            if ( tmp == NULL )
            {
                tmp = str_replace(media_url[j], keyword_id, id[j]);
                if(tmp == NULL)
                {
                    newurl = str_replace(media_url[j], keyword_num, segnum);
                    if(newurl == NULL)
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
                    if(newurl == NULL)
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
                if(newurl == NULL)
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
            fprintf(stdout, "%s\n", next_level->segments[k]);

        }
    }

    fprintf(stdout, "...\n");
    return 0;
}