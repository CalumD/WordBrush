#include "main.h"

const char* argp_program_bug_address = "https://github.com/CalumD/WordBrush/issues";
const char* argp_program_version = "1.0.0";

static int parse_opt(int key, char* arg, struct argp_state* state) {

    Config* config = (Config*) state->input;
    switch (key) {
        case 'i':
            debug("input: %s\n", arg);
            config->input_file_path = arg;
            break;
        case 'o':
            debug("output: %s\n", arg);
            config->output_file_path = arg;
            break;
        case 'w':
            debug("width: %s\n", arg);
            config->width = strtol(arg, NULL, 10);
            break;
        case 'h':
            debug("height: %s\n", arg);
            config->height = strtol(arg, NULL, 10);
            break;
        case 'k':
            debug("keyboard: %s\n", "true");
            config->draw_background = true;
            break;
        case 's':
            debug("columns: %s\n", arg);
            config->single_file_column_count = strtol(arg, NULL, 10);
            break;
        case ARGP_KEY_ARG:
            debug("argp_key_arg: %s\n", arg);
            argz_add(&config->words, &config->args_legnth, arg);
            config->word_count++;
            break;
    }
    return 0;
}


Config* initialiseConfig() {
    Config* config = malloc(sizeof(Config));
    config->successfully_initialised = false;
    config->input_file_path = 0;
    config->output_file_path = 0;
    config->successfully_initialised = false;
    config->draw_background = false;
    config->single_file_column_count = 0;
    config->width = 450;
    config->height = 180;
    config->word_count = 0;
    config->words = 0;
    config->args_legnth = 0;

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

    struct argp_option options[] = {
            {"input",    'i', "PATH", 0,       "Path to a text file to be used as word input.",                                                             0},
            {"output",   'o', "PATH", 0,       "Path to DIRECTORY to use if in multi-file output mode, or path to FILENAME if in single-file output mode.", 0},
            {"width",    'w', "NUM",  0,       "The width of each individual output image",                                                                 0},
            {0,          'W', 0, OPTION_ALIAS, 0,                                                                                                           0},
            {"height",   'h', "NUM",  0,       "The height of each individual output image",                                                                0},
            {0,          'H', 0, OPTION_ALIAS, 0,                                                                                                           0},
            {"keyboard", 'k', 0,      0,       "Display the keyboard in the background",                                                                    0},
            {"keeb",     0,   0, OPTION_ALIAS, 0,                                                                                                           0},
            {"sfo",      's', "NUM",  0,       "Number of columns per row in single-file output mode.",                                                     0},
            {"columns",  0,   0, OPTION_ALIAS, 0,                                                                                                           0},
            {0}
    };
    struct argp argp = {options, parse_opt, "[WORD INPUT]", 0, 0, 0, 0};

    int args_failed = argp_parse(&argp, argc, argv, 0, 0, config);

    bool errored = false;

    if (args_failed) {
        fprintf(stderr, "Something went wrong when parsing the CLI arguments.\n");
        errored = true;
    }

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

    if (config->single_file_column_count > 0 && config->input_file_path) {
        fprintf(stderr, "You cannot use -s and -i together - aborting.\n");
        errored = true;
    }

    if (!errored && config->input_file_path && !canReadFile(config->input_file_path)) {
        fprintf(stderr, "Cannot read from the provided input file - aborting.\n");
        errored = true;
    }

    // Open the input file, if it exists, for reading.
    config->input_file = open_input_file(config->input_file_path);

    if (!errored) {
        config->successfully_initialised = true;
    }

    return config;
}

void start_metadata_file(Config* cfg, char* metadata_dir_name, char* metadata_file_name) {
    char metadata_file_path[strlen(metadata_dir_name) + strlen(metadata_file_name)];
    strcpy(metadata_file_path, metadata_dir_name);
    strcat(metadata_file_path, metadata_file_name);
    if ((cfg->output_meta_file = fopen(metadata_file_path, "w")) == NULL) {
        printf("Write error occurred, check metadata file path.\n");
        exit(1);
    }
    fprintf(cfg->output_meta_file, "{\n\t");
}

void provide_metadata_output_type(Config* cfg, bool is_multi_file_output) {
    fprintf(cfg->output_meta_file,
            "\"outputType\": \"%s\",\n\t\"words\": {\n\t\t",
            is_multi_file_output ? "multi" : "single");
}

void finish_metadata_file(Config* cfg) {
    fprintf(cfg->output_meta_file,
            ",\n\t\"startTime\": \"%s\",\n\t\"stopTime\": \"%s\",\n\t\"finalised\": true\n}\n",
            cfg->start_timestamp, cfg->stop_timestamp
    );
    fclose(cfg->output_meta_file);
    free(cfg->start_timestamp);
    free(cfg->stop_timestamp);
}

void multi_file_output_wordbrush(Config* cfg) {

    /*
     * This buffer should be at least as large as the longest possible
     * filename, which can be calculated as the length of the directory's
     * path, plus a /, plus the most digits a long can have (20 assuming
     * a long is 8 bytes wide), plus ".svg", plus a zero terminator.
     */
    size_t filename_length = strlen(cfg->output_file_path) + 26;

    start_metadata_file(cfg, cfg->output_file_path, "/meta.json");
    provide_metadata_output_type(cfg, true);

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
        fprintf(cfg->output_meta_file, "\"%lu.svg\": \"%s\",\n\t\t", file_index - 1, word);
    }
    fseek(cfg->output_meta_file, -4, SEEK_CUR);
    fprintf(cfg->output_meta_file, "\n\t}");

    svg_free(svg);
    free(filename);
}

void single_file_output_wordbrush(Config* cfg) {

    //create output file
    FILE* output_file = fopen(cfg->output_file_path, "w");
    start_metadata_file(cfg, cfg->output_file_path, "-meta.json");
    provide_metadata_output_type(cfg, false);
    fprintf(cfg->output_meta_file, "\"%s\": [\n\t\t\t", basename(cfg->output_file_path));

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
        fprintf(cfg->output_meta_file, "[");
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
            fprintf(cfg->output_meta_file, "\"%s\", ", word);
        }
        fseek(cfg->output_meta_file, -2, SEEK_CUR);
        fprintf(cfg->output_meta_file, "],\n\t\t\t");
    }

    //Write the closing of the wrapper SVG & free buffers
    svg_end(wrapper_svg);
    svg_flush_to_file(wrapper_svg, output_file);
    svg_free(inner_word);
    svg_free(wrapper_svg);

    fclose(output_file);
    fseek(cfg->output_meta_file, -5, SEEK_CUR);
    fprintf(cfg->output_meta_file, "\n\t\t]\n\t}");
}

char* get_time_stamp() {
    char* timestamp = malloc(sizeof(char) * 20);
    time_t now = time(0);
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return timestamp;
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

    //Execute the main purpose of the program.
    config->start_timestamp = get_time_stamp();
    if (config->single_file_column_count > 0) {
        single_file_output_wordbrush(config);
    } else {
        multi_file_output_wordbrush(config);
    }
    config->stop_timestamp = get_time_stamp();
    finish_metadata_file(config);

    if (config->input_file) {
        fclose(config->input_file);
    }
    if (config->words) {
        free(config->words);
    }
    free(config);
    return 0;
}
