/*
 * FILENAME: fileop.h
 *
 * DESCRIPTION:
 *     Decalrations for functions dealing with files.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __FILEOP_H__
#define __FILEOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filename);
void write_file(const char* filename, char* content);

#endif

