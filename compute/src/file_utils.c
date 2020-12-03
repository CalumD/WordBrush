#include "file_utils.h"

bool canWriteDirectory(char* pathToDirectory) {
    return access(pathToDirectory, W_OK) == 0 ? true : false;
}

bool canReadFile(char* pathToFile) {
    return access(pathToFile, F_OK | R_OK) == 0 ? true : false;
}

bool canWriteFile(char* pathToFile) {
    return access(pathToFile, W_OK) == 0 ? true : false;
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