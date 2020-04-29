#include "mpd_parse.h"
#include "utils.h"

int main(int argc, char **argv) {
    manifest media_manifest = {0};
    int res = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return 1;
    }

    res = parse_mpd(argv[1], &media_manifest);
    if (res < 0) {
        fprintf(stderr, "Failed to parse mpd\n");
        return 1;
    }

    for (int i = 0; i < media_manifest.num_of_levels ; i++)
    {
        fprintf(stdout, " TEST: BITRATE LEVEL : %d (%d) Presenting 1st and last URL\n", media_manifest.bitrate_level[i].bitrate, i);
        fprintf(stdout, " TEST: %s\n", media_manifest.bitrate_level[i].segments[i]);
        fprintf(stdout, " TEST: %s\n", media_manifest.bitrate_level[i].segments[media_manifest.num_of_segments-1]);
    }
}
