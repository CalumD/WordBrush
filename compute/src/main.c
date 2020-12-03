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
    config->current_arg = config->extra_args;

    for (; optind < argc; optind++) {
        debug("extra arguments: %s\n", argv[optind]);
    }

    // Verify we either have an input/output file, or that there are additional arguments to use as input.
    if (!errored) {
        config->successfullyInitialised = true;
    }

    return config;
}

bool check_if_delim(char c, char *delims) {
    for (char *dl = delims; *dl; dl++) {
        if (c == *dl) {
            return true;
        }
    }

    return false;
}

/**
 * Returns tokens from a file.
 *
 * Similar to strtok, however this function takes the FILE* itself
 * and is able to stream the files, handling tokens that overflow onto
 * the next buffer.
 *
 * @param fp Pointer to a file stream.
 * @param delim Token delimiters.
 */
char* clever_strtok(FILE* fp, char* delim) {

    /*
     * TODO
     *
     * change to buffer that can resize if a word is too big
     */
    static char token[1024];

#define buf_size 1024
    static char buf[buf_size];

    static FILE* prev_fp;

    static size_t bytes_read = buf_size;

    static char* cur;
    static char* end;

    if (fp == NULL) {
        fp = prev_fp;

    } else {
        bytes_read = buf_size;
        cur = NULL;
        end = NULL;
    }

    prev_fp = fp;

    // skip all leading delimiters
    for (;; cur++) {
        if (cur >= end) {
            if (bytes_read < buf_size) {
                return NULL;
            } else {
                bytes_read = fread(buf, 1, buf_size, fp);
                cur = buf;
                end = buf + bytes_read;
        
                if (bytes_read == 0) {
                    return NULL;
                }
            }
        }

        bool delim_found = check_if_delim(*cur, delim);
        if (!delim_found) {
            break;
        }
    }

    char* tok = token;
    *(tok++) = *(cur++);
    for (;; cur++) {
        if (cur >= end) {
            if (bytes_read < buf_size) {
                *tok = '\0';
                return token;
            } else {
                bytes_read = fread(buf, 1, buf_size, fp);
                cur = buf;
                end = buf + bytes_read;

                if (bytes_read == 0) {
                    *tok = '\0';
                    return token;
                }
            }
        }

        bool delim_found = check_if_delim(*cur, delim);
        if (delim_found) {
            break;
        } else {
            *(tok++) = *cur;
        }
    }

    *tok = '\0';
    return token;
}

char* next_word_file(Config* cfg) {
    static bool first_time = true;
    char* res = clever_strtok(first_time ? cfg->inputFile : NULL, " \n,.-\":;");
    first_time = false;
    fprintf(stderr, "%s\n", res);
    return res;
}

char* next_word_arg(Config* cfg) {
    char* res = *(cfg->current_arg++);
    return res;
}

char* next_word(Config* cfg) {

    /*
     * The input can be either a list of command-line arguments,
     * or text from a file. This function should correctly choose
     * how to get the next word based on whether or not there was an
     * input file given at initialisation.
     */
    if (cfg->inputFile) {
        char* res = next_word_file(cfg);

        if (res) {
            return res;
        }
    }

    return next_word_arg(cfg);
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

    // Do work for each 'word'
    char* word;
    while ((word = next_word(cfg))) {
        snprintf(filename, filename_length, "%s/%lu.svg", cfg->outputFilePath, file_index);
        current_output_file = fopen(filename, "w");

        svg* svg = svg_start(cfg->width, cfg->height);

        compute_curves(cfg, svg, word);

        svg_end(svg);
        svg_write_to_file(svg, current_output_file);
        svg_free(svg);
        fclose(current_output_file);

    file_index++;
    }

    free(filename);
}


void single_file_output_wordbrush(Config* cfg) {
    // TODO Implement this...
    printf("%s\n", cfg->outputFilePath);
}

FILE* open_input_file(char* path) {
    if (!path) {
        return NULL;
    }

    if (!strcmp(path, "-")) {
        return stdin;
    }

    return fopen(path, "r");
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

    // Open the input file, if it exists, for reading.
    config->inputFile = open_input_file(config->inputFilePath);

    //Execute the main purpose of the program.
    if (config->multiFileOutput) {
        multi_file_output_wordbrush(config);
    } else {
        single_file_output_wordbrush(config);
    }

    free(config);
    return 0;
}
