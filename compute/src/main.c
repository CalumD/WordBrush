#include "main.h"

void display_help() {
    printf("TODO: FILL ME IN\n");
}

Config* initialiseConfig() {
    Config* config = malloc(sizeof(Config));
    config->successfully_initialised = false;
    config->single_file_column_count = 0;
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
    while ((opt = getopt(argc, argv, ":i:o:s:hW:H:")) != -1) {
        switch (opt) {
            // print help output
            case 'h':
                display_help();
                return config;
                // Create singular output file with rows and columns of words
            case 's':
                config->single_file_column_count = strtol(optarg, NULL, 10);
                break;
                // Input filename
            case 'i':
                config->input_file_path = optarg;
                break;
                // Output directory (will output files to here with name ~/output_<X>.svg)
            case 'o':
                config->output_file_path = optarg;
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
    config->words = argv + optind;
    config->word_count = argc - optind;

    // FIXME: The output from the 'canWrite' methods are inconsistent depending on where they are called from.
    if (config->single_file_column_count > 0) {
        if (!canWriteFile(config->output_file_path)) {
            fprintf(stderr, "Cannot write that file, check destination is free - aborting.\n");
            errored = true;
        }
    } else {
        if (!canWriteDirectory(config->output_file_path)) {
            fprintf(stderr, "Cannot write to that output directory - aborting.\n");
            errored = true;
        }
    }

    if (config->single_file_column_count > 0 && config->inputFile) {
        fprintf(stderr, "You cannot use -s and -i together - aborting.\n");
        errored = true;
    }

    // TODO: At THIS point - if there was an input file defined, then we should read in all the words and ADD
    //  them to the end of the config-words mapping.
    if (!errored && !canReadFile(config->input_file_path)) {
        fprintf(stderr, "Cannot read from the provided input file - aborting.\n");
        errored = true;
    }

    // Open the input file, if it exists, for reading.
    config->inputFile = open_input_file(config->input_file_path);

    if (!errored) {
        config->successfully_initialised = true;
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
    size_t filename_length = strlen(cfg->output_file_path) + 26;

    // Create the directory and make space for the various file names
    mkdir(cfg->output_file_path, 0700);
    char* filename = malloc(filename_length);
    FILE* current_output_file = NULL;
    unsigned long file_index = 0;

    // Initialise the re-usable svg memory
    svg* svg = svg_init();
    char* word;
    while ((word = next_word(cfg))) {
        // prepare the next output file name
        snprintf(filename, filename_length, "%s/%lu.svg", cfg->output_file_path, file_index++);
        current_output_file = fopen(filename, "w");

        //create the svg
        svg_start(svg, 0, 0, cfg->width, cfg->height);

        compute_curves(cfg, svg, word);

        // finalise and write the file.
        svg_end(svg);
        svg_flush_to_file(svg, current_output_file);
        fclose(current_output_file);
    }

    svg_free(svg);
    free(filename);
}

void single_file_output_wordbrush(Config* cfg) {

    //create output file
    FILE* output_file = fopen(cfg->output_file_path, "w");

    //Do some maths to figure out how long each 'row' of words should be before wrapping
    long row_count = count_rows_in_wrapper_svg(cfg);

    //write the start of the wrapper SVG
    svg* wrapper_svg = svg_init();
    svg_start(wrapper_svg, 0, 0, cfg->width * cfg->single_file_column_count, cfg->height * row_count);
    svg_flush_to_file(wrapper_svg, output_file);

    //create an inner SVG for the inner words
    svg* inner_word = svg_init();

    //Do a loop to get each individual "word's" SVG image and add to the file as we go to now blow memory up
    char* word;
    for (int row_index = 0; row_index < row_count; row_index++) {
        for (int column_index = 0; column_index < cfg->single_file_column_count; column_index++) {
            // If we have reached the 'wordcount', then skip the remaining cells in the 2D grid of words
            if (((row_index * cfg->single_file_column_count) + column_index) >= cfg->word_count) {
                continue;
            }
            word = next_word(cfg);

            // Initialise the boilerplate of the next word
            svg_start(inner_word, column_index * cfg->width, row_index * cfg->height, cfg->width, cfg->height);
            debug("x: %d y: %d, word: %s\n", column_index, row_index, word);

            // Calculate the next word
            compute_curves(cfg, inner_word, word);

            // End & write it to the file
            svg_end(inner_word);
            svg_flush_to_file(inner_word, output_file);
        }
    }

    //Write the closing of the wrapper SVG & free buffers
    svg_end(wrapper_svg);
    svg_flush_to_file(wrapper_svg, output_file);
    svg_free(inner_word);
    svg_free(wrapper_svg);

    fclose(output_file);
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
    if (!config->successfully_initialised) {
        free(config);
        fprintf(stderr, "Arguments provided were invalid to compute an output.\n");
        return -1;
    }

    debug("Input Path: %s\n", config->input_file_path);
    debug("Output Path: %s\n", config->output_file_path);
    debug("Multi-file Output: %ld\n", config->single_file_column_count);
    debug("Keyboard Width: %ld\n", config->width);
    debug("Keyboard Height: %ld\n", config->height);
    debug("Input Word Count: %ld\n", config->word_count);
    for (int i = 0; i < config->word_count; i++) {
        debug("cli arg word: %s\n", *(config->words + i));
    }
    debug("\n\n");

    //Execute the main purpose of the program.
    if (config->single_file_column_count > 0) {
        single_file_output_wordbrush(config);
    } else {
        multi_file_output_wordbrush(config);
    }

    free(config);
    return 0;
}
