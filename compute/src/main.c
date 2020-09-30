#include "main.h"

void display_help() {
    printf("TODO: FILL ME IN\n");
}

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
    while ((opt = getopt(argc, argv, ":i:o:mhW:H:")) != -1) {
        switch (opt) {
            // print help output
            case 'h':
                display_help();
                return config;
                // Create Multiple outputs (1 per word)
            case 'm':
                /* TODO: change this to a number
                 * - Default behaviour should write one file per word,
                 * if this option is found, we should stitch together this number
                 * of images per ROW.
                */
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
                config->width = strtol(optarg, NULL, 10);
                break;
                // Height of the virtual keyboard (aka output height scale)
            case 'H':
                config->height = strtol(optarg, NULL, 10);
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

    for (; optind < argc; optind++) {
        debug("extra arguments: %s\n", argv[optind]);
    }

    // Verify we either have an input/output file, or that there are additional arguments to use as input.
    if (!errored) {
        config->successfullyInitialised = true;
    }

    return config;
}

void multi_file_output_wordbrush(Config* cfg) {

    /*
     * This buffer should be at least as large as the longest possible
     * filename, which can be calculated as the length of the directory's
     * path, plus a /, plus the most digits a long can have (20 assuming
     * a long is 8 bytes wide), plus ".svg", plus a zero terminator.
     */
    size_t filename_length = strlen(cfg->outputFilePath) + 26;

    mkdir(cfg->outputFilePath, 0700);
    char* filename = malloc(filename_length);

    FILE* current_output_file = NULL;
    unsigned long file_index = 0;

    // Do work for each 'word' (additional param not in command line arguments)
    for (char** word = cfg->extra_args; *word; word++, file_index++) {
        snprintf(filename, filename_length, "%s/%lu.svg", cfg->outputFilePath, file_index);
        current_output_file = fopen(filename, "w");

        svg* svg = svg_start(cfg->width, cfg->height);

        compute_curves(cfg, svg, *word);

        svg_end(svg);
        svg_write_to_file(svg, current_output_file);
        svg_free(svg);
        fclose(current_output_file);
    }


    free(filename);
}


void single_file_output_wordbrush(Config* cfg) {
    // TODO Implement this...
    printf("%s\n", cfg->outputFilePath);
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

    debug("Input Path: %s\n", config->inputFilePath);
    debug("Output Path: %s\n", config->outputFilePath);
    debug("Multi-file Output: %s\n", config->multiFileOutput ? "true" : "false");
    debug("\n\n");

    //Execute the main purpose of the program.
    if (config->multiFileOutput) {
        multi_file_output_wordbrush(config);
    } else {
        single_file_output_wordbrush(config);
    }

    free(config);
    return 0;
}
