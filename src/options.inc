/*
 * FILENAME: options.inc
 *
 * DESCRIPTION:
 *     Declaration of macros for option parsing. Thanks to duskborn.com
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */


#ifndef OPTION
#error OPTION(short-name, long-name, num-args, desc) was not defined.
#endif

OPTION(h,  help,      0,  Print this help and exit.)
OPTION(o,  output,    1,  Put output into this file.)
OPTION(v,  verbose,   0,  Give more detailed output.)
OPTION(i,  asm-intel, 0,  Emit Intel assemblyu code.)

#undef OPTION
