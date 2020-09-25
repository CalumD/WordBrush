#include <stdio.h>
#include <malloc.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "main.h"
#include "wordbrush.h"
#include "file_utils.h"
#include "debug.h"

/**
 * Parse the program arguments in preparation to run.
 *
 * @param argc The number of arguments provided to the application.
 * @param argv The arguments provided to the application.
 * @return A Struct representing the configuration for the program to work on.
 */
Config* get_program_arguments(int argc, char* argv[]) {
    Config* config = malloc(sizeof(Config));
    config->successfullyInitialised = false;

    /*
     * Until we figure out a nice way to combine squiggles,
     * multi-file output should be the default.
     */
    config->multiFileOutput = true;

    bool errored = false;

    // put ':' in the starting of the string so that program can distinguish between '?' and ':'
    int opt;
    while((opt = getopt(argc, argv, ":i:o:mhW:H:")) != -1)
    {
        switch(opt)
        {
            // print help output
            case 'h':
                printf("HERE IS SOME HELP, THIS SHOULD BE IMPROVED AT SOME POINT!\n");
                return config;
            // Create Multiple outputs (1 per word)
            case 'm':
                config->multiFileOutput = true;
                break;
            // Input filename
            case 'i':
                config->inputFilePath = optarg;
                break;
            // Output directory (will output files to here with name ~/output_<X>.svg)
            case 'o':
                config->outputFilePath = optarg;
                break;
            // Width of the virtual keyboard (aka output width scale)
            case 'W':
                sscanf(optarg, "%d", &config->width);
                break;
            // Height of the virtual keyboard (aka output height scale)
            case 'H':
                sscanf(optarg, "%d", &config->height);
                break;
            // Covering when the user makes a mistake in input.
            case ':':
                fprintf(stderr, "option needs a value\n");
                errored = true;
                break;
            default:
                fprintf(stderr, "unknown option: %c\n", optopt);
                errored = true;
                break;
        }
    }

    // optind is for the extra arguments
    // which are not parsed
    config->extra_args = argv + optind;

    for(; optind < argc; optind++){
        debug("extra arguments: %s\n", argv[optind]);
    }

    // Verify we either have an input/output file, or that there are additional arguments to use as input.
    if (!errored) {
        config->successfullyInitialised = true;
    }

    return config;
}

void word_loop(Config *cfg) {
    FILE *cur_output_file = NULL;
    unsigned long file_index = 0;
    char *buf;
    size_t bufsize;

    if (cfg->multiFileOutput) {
        mkdir(cfg->outputFilePath, 0700);

        /*
         * This buffer should be at least as large as the longest possible
         * filename, which can be calculated as the length of the directory's
         * path, plus a /, plus the most digits a long can have (20 assuming
         * a long is 8 bytes wide), plus ".svg", plus a zero terminator.
         */
        bufsize = strlen(cfg->outputFilePath) + 26;
        buf = malloc(bufsize);
    } else {
        cur_output_file = fopen(cfg->outputFilePath, "w");
    }

    for (char** word = cfg->extra_args; *word; word++) {
        if (cfg->multiFileOutput) {
            if (cur_output_file) {
                fclose(cur_output_file);
            }

            snprintf(buf, bufsize, "%s/%lu.svg", cfg->outputFilePath,
                file_index);
            cur_output_file = fopen(buf, "w");
        }

        compute_curves(cfg, *word, cur_output_file);

        file_index++;
    }
}

/**
 * Main Entry point which delegates out the rest of the application functionality.
 *
 * @param argc The number of arguments provided to the application.
 * @param argv The arguments provided to the application.
 * @return The success value of this application.
 */
int main(int argc, char* argv[]) {

    // Check command line arguments
    Config* config = get_program_arguments(argc, argv);

    // If program arguments are not suitable, then shutdown.
    if (!config->successfullyInitialised) {
        free(config);
        fprintf(stderr, "Arguments provided were invalid to compute an output.");
        return -1;
    }

    //Execute the main purpose of the program.
    word_loop(config);

    return 0;
}
