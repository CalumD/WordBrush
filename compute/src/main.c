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
    while ((opt = getopt(argc, argv, ":i:o:m:hW:H:")) != -1) {
        switch (opt) {
            // print help output
            case 'h':
                display_help();
                return config;
                // Create Multiple outputs (1 per word)
            case 'm':
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

    // TODO: At THIS point - if there was an input file defined, then we should read in all the words and ADD
    //  them to the end of the config-words mapping.

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

    mkdir(cfg->output_file_path, 0700);
    char* filename = malloc(filename_length);

    FILE* current_output_file = NULL;
    unsigned long file_index = 0;

    // Do work for each 'word' (additional param not in command line arguments)
    for (char** word = cfg->words; *word; word++, file_index++) {
        snprintf(filename, filename_length, "%s/%lu.svg", cfg->output_file_path, file_index);
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
    //create output file
    FILE* output_file = fopen(cfg->output_file_path, "w");

    //Do some maths to figure out how long each 'row' of words should be before wrapping
    long row_count = count_rows_in_wrapper_svg(cfg);

    //write the start of the wrapper SVG
    svg* wrapper_svg = svg_start(
            0,
            0,
            cfg->width * cfg->single_file_column_count,
            cfg->height * row_count
    );

    char* format ="<svg xmlns='%s' x='%dpx' y='%dpx' width='%dpx' height='%dpx' viewBox='0 0 %d %d'>\n";
    char* XML_NAMESPACE = "http://www.w3.org/2000/svg";

    //Do a loop to get each individual "word's" SVG image and APPEND to the file as we go to now blow memory up
    for (int row_index = 0; row_index < row_count; row_index++) {
        for (int column_index = 0; column_index < cfg->single_file_column_count; column_index++) {
            if (((row_index * cfg->single_file_column_count) + column_index) >= cfg->word_count) {
                continue;
            }
            printf("x: %d y: %d, word: %s\n", column_index, row_index, cfg->words[(row_index * cfg->single_file_column_count) + column_index]);

            add_to_svg(wrapper_svg, format, XML_NAMESPACE, column_index * cfg->width, row_index * cfg->height, cfg->width, cfg->height, cfg->width, cfg->height);
            compute_curves(cfg, wrapper_svg, cfg->words[(row_index * cfg->single_file_column_count) + column_index]);
            svg_end(wrapper_svg);
        }
    }

    //Write the closing of the wrapper SVG
    svg_end(wrapper_svg);
    svg_write_to_file(wrapper_svg, output_file);
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
        fprintf(stderr, "Arguments provided were invalid to compute an output.");
        return -1;
    }

    debug("Input Path: %s\n", config->input_file_path);
    debug("Output Path: %s\n", config->output_file_path);
    debug("Multi-file Output: %ld\n", config->single_file_column_count);
    debug("Keyboard Width: %ld\n", config->width);
    debug("Keyboard Height: %ld\n", config->height);
    debug("Input Word Count: %ld\n", config->word_count);
    for (int i = 0; i < config->word_count; i++) {
        debug("word: %s\n", *(config->words + i));
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
