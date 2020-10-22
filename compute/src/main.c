#include "main.h"

void display_help() {
    printf("TODO: FILL ME IN\n");
}

Config* initialiseConfig() {
    Config* config = malloc(sizeof(Config));
    config->successfullyInitialised = false;
    config->multiFileOutput = 0;
    config->width = 450;
    config->height = 180;

    return config;
}


/**
 * Parse the program arguments in preparation to run.
 *
 * @param argc The number of arguments provided to the application.
 * @param argv The arguments provided to the application.
 * @return A Struct representing the configuration for the program to work on.
 */
Config* get_program_arguments(int argc, char* argv[]) {
    Config* config = initialiseConfig();

    bool errored = false;

    // put ':' in the starting of the string so that program can distinguish between '?' and ':'
    int opt;
    while ((opt = getopt(argc, argv, ":i:o:m:hW:H:")) != -1) {
        switch (opt) {
            // print help output
            case 'h':
                display_help();
                return config;
                // Create Multiple outputs (1 per word)
            case 'm':
                config->multiFileOutput = strtol(optarg, NULL, 10);
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

    // optind is for the extra arguments which are not parsed
    config->extra_args = argv + optind;

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

        svg* svg = svg_start(0, 0, cfg->width, cfg->height);

        compute_curves(cfg, svg, *word);

        svg_end(svg);
        svg_write_to_file(svg, current_output_file);
        svg_free(svg);
        fclose(current_output_file);
    }


    free(filename);
}


void single_file_output_wordbrush(Config* cfg) {
    //create directory
    printf("HALLO: ");
    FILE* current_output_file = NULL;
    current_output_file = fopen(cfg->outputFilePath, "w");
    fclose(current_output_file);


    //create the file

    //write the start of the wrapper SVG

    //Do some maths to figure out how long each 'row' of words should be before wrapping

    //Do a loop to get each individual "word's" SVG image and APPEND to the file as we go to now blow memory up

    //Write the closing of the wrapper SVG

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
    debug("Multi-file Output: %ld\n", config->multiFileOutput);
    debug("Keyboard Width: %ld\n", config->width);
    debug("Keyboard Height: %ld\n", config->height);
    for (int i = 0; *(config->extra_args + i); i++) {
        debug("extra argument: %s\n", *(config->extra_args + i));
    }
    debug("\n\n");

    //Execute the main purpose of the program.
    if (config->multiFileOutput > 0) {
        single_file_output_wordbrush(config);
    } else {
        multi_file_output_wordbrush(config);
    }

    free(config);
    return 0;
}
