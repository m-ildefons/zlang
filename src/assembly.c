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


void emit(char** code, const char* inst, const char* arg1, const char* arg2){
	int padlen = 10 - strlen(inst);
	if(arg1 == NULL && arg2 == NULL){
		strapp(code, "    %s\n", inst);
	} else if(arg2 == NULL) {
		strapp(code,
			"    %s%*.*s%s\n",
			inst,
			padlen, padlen, " ",
			arg1);
	} else {
		if(asm_intel){
			strapp(code,
				"    %s%*.*s%s, %s\n",
				inst,
				padlen, padlen, " ",
				arg2,
				arg1);
		} else {
			strapp(code,
				"    %s%*.*s%s, %s\n",
				inst,
				padlen, padlen, " ",
				arg1,
				arg2);
		}
	}
}
