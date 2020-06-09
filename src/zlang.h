/*
 * FILENAME: zlang.h
 *
 * DESCRIPTION:
 *     Declaration of global variables and functions. These carry information
 *     that are continuously needed throughout the different steps of the
 *     compilation process.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

/* Major Version - increase with major (code-breaking) changes.
 *             0 - Never released.
 *             1 - Language barely working
 */
#define __ZLANG_MAJ__ 0

/* Minor Version - increase with minor (feature-intruducting) changes
 */
#define __ZLANG_MIN__ 2

/* Sub Version - increase with internal changes whenever a milestone has been
 *               reached.
 */
#define __SUB__ "1"
#ifndef __GIT_REV__
#define __ZLANG_SUB__ __SUB__
#else
#define __ZLANG_SUB__ __SUB__ "-" __GIT_REV__
#endif

extern char* filename;
extern char* asm_out;
extern char* elf_out;

// emit intel assembly
extern int asm_intel;

#endif

