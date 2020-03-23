/*
 * FILENAME: asm_gen_loop.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation for loops.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

char* asm_gen_for_loop(asn* for_loop){
	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = for_loop->op.for_loop_exp.symbol_map;

	asn* init = for_loop->op.for_loop_exp.init;
	asn* cond = for_loop->op.for_loop_exp.cond;
	asn* move = for_loop->op.for_loop_exp.move;
	asn_list* body = for_loop->op.for_loop_exp.body;

	ca_list* key_list = symbol_map_ptr->key_list;
	pv_leaf* sym;
	int num_vars = 0;

	for(; key_list != NULL; key_list = key_list->next){
		sym = pv_search(symbol_map_ptr, key_list->key);
		if(sym->scope > old_symbol_map_ptr->scope)
			num_vars++;
	}

	printf("for loop body containing %d vars.\n", num_vars);

	char* label_start = gen_label("f_start");
	char* label_start_line = salloc(80);
    assert(label_start_line != NULL);
	char* label_start_jmp = salloc(80);
    assert(label_start_jmp != NULL);
	sprintf(label_start_line, "%s:\n", label_start);
	sprintf(label_start_jmp, "    jmp    %s\n", label_start);

	char* label_end = gen_label("f_end");
	char* label_end_line = salloc(80);
    assert(label_end_line != NULL);
	char* label_end_jmp = salloc(80);
    assert(label_end_jmp != NULL);
	sprintf(label_end_line, "%s:\n", label_end);
	sprintf(label_end_jmp, "    je     %s\n", label_end);

	const char* old_loop_start_label = loop_start_label;
	const char* old_loop_end_label = loop_end_label;
	loop_start_label = label_start;
	loop_end_label = label_end;

	char* init_src = asm_gen(init);
	char* cond_src = asm_gen(cond);
	char* move_src = asm_gen(move);
	char* body_src = asm_gen_body(body);
	char* src = strnew();

	strapp(&src, init_src);
	strapp(&src, label_start_line);
	strapp(&src, cond_src);
	strapp(&src, "    cmpq   $0, %rax\n");
	strapp(&src, label_end_jmp);
	strapp(&src, body_src);
	strapp(&src, move_src);
	strapp(&src, label_start_jmp);
	strapp(&src, label_end_line);

	symbol_map_ptr = old_symbol_map_ptr;
	loop_start_label = old_loop_start_label;
	loop_end_label = old_loop_end_label;

    free(init_src);
    free(cond_src);
    free(move_src);
    free(body_src);
    free(label_start);
    free(label_start_line);
    free(label_start_jmp);
    free(label_end);
    free(label_end_line);
    free(label_end_jmp);
    return src;
}

char* asm_gen_while_loop(asn* while_loop){
	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = while_loop->op.while_loop_exp.symbol_map;

	asn* cond = while_loop->op.while_loop_exp.cond;
	asn_list* body = while_loop->op.while_loop_exp.body;

	ca_list* key_list = symbol_map_ptr->key_list;
	pv_leaf* sym;
	int num_vars = 0;

	for(; key_list != NULL; key_list = key_list->next){
		sym = pv_search(symbol_map_ptr, key_list->key);
		if(sym->scope > old_symbol_map_ptr->scope)
			num_vars++;
	}

	printf("while loop body containing %d vars.\n", num_vars);

	char* label_start = gen_label("w_start");
	char* label_start_line = salloc(80);
	char* label_start_jmp = salloc(80);
	sprintf(label_start_line, "%s:\n", label_start);
	sprintf(label_start_jmp, "    jmp    %s\n", label_start);

	char* label_end = gen_label("w_end");
	char* label_end_line = salloc(80);
	char* label_end_jmp = salloc(80);
	sprintf(label_end_line, "%s:\n", label_end);
	sprintf(label_end_jmp, "    je     %s\n", label_end);

	const char* old_loop_start_label = loop_start_label;
	const char* old_loop_end_label = loop_end_label;
	loop_start_label = label_start;
	loop_end_label = label_end;

	char* cond_src = asm_gen(cond);
	char* body_src = asm_gen_body(body);
	char* src = strnew();

	strapp(&src, label_start_line);
	strapp(&src, cond_src);
	strapp(&src, "    cmpq   $0, %rax\n");
	strapp(&src, label_end_jmp);
	strapp(&src, body_src);
	strapp(&src, label_start_jmp);
	strapp(&src, label_end_line);

	symbol_map_ptr = old_symbol_map_ptr;
	loop_start_label = old_loop_start_label;
	loop_end_label = old_loop_end_label;

    free(cond_src);
    free(body_src);
    free(label_start);
    free(label_start_line);
    free(label_start_jmp);
    free(label_end);
    free(label_end_line);
    free(label_end_jmp);
    return src;
}

