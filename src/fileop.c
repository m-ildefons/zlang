/*
 * FILENAME: fileop.c
 *
 * DESCRIPTION:
 *     Implementation of functions dealing with (source) files.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "fileop.h"

char* read_file(const char* filename){
    char* buffer = NULL;
    size_t str_size;
    size_t read_size;
    FILE* fd = fopen(filename, "r");

    if(fd){
        /* get file size */
        fseek(fd, 0, SEEK_END);
		long tell = ftell(fd);
		if(tell < 0)
			abort();
		str_size = (size_t) tell;
        rewind(fd);

        buffer = malloc((str_size + 1)* sizeof(char));
        read_size = fread(buffer, sizeof(char), str_size, fd);
        buffer[str_size] = '\0';

        if(str_size != read_size){
            /* something went wrong */
            free(buffer);
            buffer = NULL;
        }

        fclose(fd);
    }
    return buffer;
}

void write_file(const char* filename, char* content){
    size_t c_size = strlen(content);

    FILE* fd = fopen(filename, "w");
    if(fd){
        fwrite(content, 1, c_size, fd);
        fclose(fd);
    }
}

