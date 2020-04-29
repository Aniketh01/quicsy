#include "utils.h"

/*http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
 * Function returns NULL when there is no replacement to be made.
 */

char *str_replace(char *orig, char *rep, char *with)
{
    char *result;  // the return string
    char *ins;     // the next insert point
    char *tmp;     // varies
    int len_rep;   // length of rep
    int len_with;  // length of with
    int len_front; // distance between rep and end of last rep
    int count;     // number of replacements
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
    for (count = 0; tmp; ++count)
    {
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
    while (count--)
    {
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