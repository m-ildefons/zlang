/*
 * FILENAME: assembly.c
 *
 * DESCRIPTION
 *     Declarations of helper functions for assembly generation.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "assembly.h"

void __emit(char** code,
            const char* inst,
            const char* arg1,
            const char* arg2,
            const char* file,
            unsigned int line) {
	int padlen = 10 - strlen(inst);
	if (arg1 == NULL && arg2 == NULL) {
		strapp(code,
           "    %s  # %s:%d\n",
           inst,
           file,
           line);
	} else if (arg2 == NULL) {
		strapp(code,
			"    %s%*.*s%s  # %s:%d\n",
			inst,
			padlen, padlen, " ",
			arg1,
      file,
      line);
	} else {
		if(asm_intel){
			strapp(code,
				"    %s%*.*s%s, %s  # %s:%d\n",
				inst,
				padlen, padlen, " ",
				arg2,
				arg1,
        file,
        line);
		} else {
			strapp(code,
				"    %s%*.*s%s, %s  # %s:%d\n",
				inst,
				padlen, padlen, " ",
				arg1,
				arg2,
        file,
        line);
		}
	}
}
