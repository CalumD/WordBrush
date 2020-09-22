#include <stdio.h>
#include <malloc.h>
#include <getopt.h>
#include <stdbool.h>

#include "main.h"
#include "wordbrush.h"
#include "fileUtils.h"
#include "debug.h"

/**
 * Parse the program arguments in preparation to run.
 *
 * @param argc The number of arguments provided to the application.
 * @param argv The arguments provided to the application.
 * @return A Struct representing the configuration for the program to work on.
 */
Config* getProgramArguments(int argc, char* argv[]) {
    Config* config = malloc(sizeof(Config));
    config->successfullyInitialised = false;
    config->multiFileOutput = false;

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
                if (!canReadFile(optarg)) {
                    printf("Failed because of input file name.\n");
                    errored = true;
                }
                break;
            // Output directory (will output files to here with name ~/output_<X>.svg)
            case 'o':
                config->outputFilePath = optarg;
                if (!canWriteDirectory(optarg)) {
                    printf("Failed because of output directory.\n");
                    errored = true;
                }
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
    for(; optind < argc; optind++){
        debug("extra arguments: %s\n", argv[optind]);
    }

    // Verify we either have an input/output file, or that there are additional arguments to use as input.
    if (!errored) {
        config->successfullyInitialised = true;
    }

    return config;
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
    Config* config = getProgramArguments(argc, argv);

    // If program arguments are not suitable, then shutdown.
    if (!config->successfullyInitialised) {
        free(config);
        fprintf(stderr, "Arguments provided were invalid to compute an output.");
        return -1;
    }

    //Execute the main purpose of the program.
    computeCurves(config);

    return 0;
}
