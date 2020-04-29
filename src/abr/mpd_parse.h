#ifndef MPD_PARSE_H
#define MPD_PARSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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

} manifest;


#endif