/*
 * FILENAME: asm_gen_cond.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation of conditionals.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

const char* asm_gen_cond(asn* cond){
	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = cond->op.cond_exp.if_symbol_map;

	asn* condition = cond->op.cond_exp.cond;
	asn_list* body = cond->op.cond_exp.if_body;
	asn_list* else_body = cond->op.cond_exp.else_body;
	ca_list* key_list = symbol_map_ptr->key_list;
	pv_leaf* symbol;
	int num_vars = 0;
	size_t num_chars = 2;

	for(; key_list != NULL; key_list = key_list->next){
		symbol = pv_search(symbol_map_ptr, key_list->key);
		if(symbol->scope != 0)
			num_vars++;
	}

	printf("if statement body containing %d vars.\n", num_vars);

	const char* condition_src = asm_gen(condition);
	size_t condition_len = strlen(condition_src);

	const char* body_src = asm_gen_body(body, num_vars);
	size_t body_len = strlen(body_src);

	const char* else_body_src = NULL;
	size_t else_body_len = 0;
	if(cond->op.cond_exp.else_body != NULL){
		else_body_src = asm_gen_body(cond->op.cond_exp.else_body, num_vars);
		else_body_len = strlen(else_body_src);
	}

	const char* label_post_body = gen_label("post_body");
	char* label_post_body_line = (char*) malloc(80 * sizeof(char));
	char* label_post_body_jmp = (char*) malloc(80 * sizeof(char));
	sprintf(label_post_body_line, "%s:\n", label_post_body);
	sprintf(label_post_body_jmp, "    je     %s\n", label_post_body);

	const char* label_end = gen_label("end");
	char* label_end_line = (char*) malloc(80 * sizeof(char));
	char* label_end_jmp = (char*) malloc(80 * sizeof(char));
	sprintf(label_end_line, "%s:\n", label_end);
	sprintf(label_end_jmp, "    jmp    %s\n", label_end);

	size_t all_src_len = body_len
								+ condition_len
								+ 80 * 3;
	if(else_body != NULL)
		all_src_len += else_body_len + 80 * 2;
	char* all_src = (char*) malloc(all_src_len * sizeof(char));

	strcpy(all_src, condition_src);
	strcat(all_src, "    cmpq   $0, %rax\n");
	strcat(all_src, label_post_body_jmp);
	strcat(all_src, body_src);

	if(else_body != NULL)
		strcat(all_src, label_end_jmp);

	strcat(all_src, label_post_body_line);

	if(else_body != NULL){
		strcat(all_src, else_body_src);
		strcat(all_src, label_end_line);
	}

	symbol_map_ptr = old_symbol_map_ptr;
    return all_src;
}

