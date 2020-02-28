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

const char* asm_gen_for_loop(asn* for_loop){
	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = for_loop->op.for_loop_exp.symbol_map;

	asn* init = for_loop->op.for_loop_exp.init;
	asn* cond = for_loop->op.for_loop_exp.cond;
	asn* move = for_loop->op.for_loop_exp.move;
	asn_list* body = for_loop->op.for_loop_exp.body;

	ca_list* key_list = symbol_map_ptr->key_list;
	pv_leaf* symbol;
	int num_vars = 0;

	for(; key_list != NULL; key_list = key_list->next){
		symbol = pv_search(symbol_map_ptr, key_list->key);
		if(symbol->scope > old_symbol_map_ptr->scope)
			num_vars++;
	}

	printf("for loop body containing %d vars.\n", num_vars);

	const char* label_start = gen_label("f_start");
	char* label_start_line = (char*) malloc(80 * sizeof(char));
    assert(label_start_line != NULL);
	char* label_start_jmp = (char*) malloc(80 * sizeof(char));
    assert(label_start_jmp != NULL);
	sprintf(label_start_line, "%s:\n", label_start);
	sprintf(label_start_jmp, "    jmp    %s\n", label_start);

	const char* label_end = gen_label("f_end");
	char* label_end_line = (char*) malloc(80 * sizeof(char));
    assert(label_end_line != NULL);
	char* label_end_jmp = (char*) malloc(80 * sizeof(char));
    assert(label_end_jmp != NULL);
	sprintf(label_end_line, "%s:\n", label_end);
	sprintf(label_end_jmp, "    je     %s\n", label_end);

	const char* old_loop_start_label = loop_start_label;
	const char* old_loop_end_label = loop_end_label;
	loop_start_label = label_start;
	loop_end_label = label_end;

	const char* init_src = asm_gen(init);
	size_t init_len = strlen(init_src);

	const char* cond_src = asm_gen(cond);
	size_t cond_len = strlen(cond_src);

	const char* move_src = asm_gen(move);
	size_t move_len = strlen(move_src);

	const char* body_src = asm_gen_body(body, num_vars);
	size_t body_len = strlen(body_src);
	size_t total_len = init_len + cond_len + move_len + body_len + 5 * 80;
	char* src = (char*) malloc(total_len * sizeof(char));

	strcpy(src, init_src);
	strcat(src, label_start_line);
	strcat(src, cond_src);
	strcat(src, "    cmpq   $0, %rax\n");
	strcat(src, label_end_jmp);
	strcat(src, body_src);
	strcat(src, move_src);
	strcat(src, label_start_jmp);
	strcat(src, label_end_line);

	symbol_map_ptr = old_symbol_map_ptr;
	loop_start_label = old_loop_start_label;
	loop_end_label = old_loop_end_label;
    free(label_start_line);
    free(label_start_jmp);
    free(label_end_line);
    free(label_end_jmp);
    return src;
}

const char* asm_gen_while_loop(asn* while_loop){
	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = while_loop->op.while_loop_exp.symbol_map;

	asn* cond = while_loop->op.while_loop_exp.cond;
	asn_list* body = while_loop->op.while_loop_exp.body;

	ca_list* key_list = symbol_map_ptr->key_list;
	pv_leaf* symbol;
	int num_vars = 0;

	for(; key_list != NULL; key_list = key_list->next){
		symbol = pv_search(symbol_map_ptr, key_list->key);
		if(symbol->scope > old_symbol_map_ptr->scope)
			num_vars++;
	}

	printf("while loop body containing %d vars.\n", num_vars);

	const char* label_start = gen_label("w_start");
	char* label_start_line = (char*) malloc(80 * sizeof(char));
    assert(label_start_line != NULL);
	char* label_start_jmp = (char*) malloc(80 * sizeof(char));
    assert(label_start_jmp != NULL);
	sprintf(label_start_line, "%s:\n", label_start);
	sprintf(label_start_jmp, "    jmp    %s\n", label_start);

	const char* label_end = gen_label("w_end");
	char* label_end_line = (char*) malloc(80 * sizeof(char));
    assert(label_end_line != NULL);
	char* label_end_jmp = (char*) malloc(80 * sizeof(char));
    assert(label_end_jmp != NULL);
	sprintf(label_end_line, "%s:\n", label_end);
	sprintf(label_end_jmp, "    je     %s\n", label_end);

	const char* old_loop_start_label = loop_start_label;
	const char* old_loop_end_label = loop_end_label;
	loop_start_label = label_start;
	loop_end_label = label_end;

	const char* cond_src = asm_gen(cond);
	size_t cond_len = strlen(cond_src);

	const char* body_src = asm_gen_body(body, num_vars);
	size_t body_len = strlen(body_src);

	size_t total_len = body_len + cond_len + 5 * 80;
	char* src = (char*) malloc(total_len * sizeof(char));

	strcpy(src, label_start_line);
	strcat(src, cond_src);
	strcat(src, "    cmpq   $0, %rax\n");
	strcat(src, label_end_jmp);
	strcat(src, body_src);
	strcat(src, label_start_jmp);
	strcat(src, label_end_line);

	symbol_map_ptr = old_symbol_map_ptr;
	loop_start_label = old_loop_start_label;
	loop_end_label = old_loop_end_label;
    free(label_start_line);
    free(label_start_jmp);
    free(label_end_line);
    free(label_end_jmp);
    return src;
}

