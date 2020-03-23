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

char* asm_gen_cond(asn* cond){
	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = cond->op.cond_exp.if_symbol_map;

	asn* condition = cond->op.cond_exp.cond;
	asn_list* body = cond->op.cond_exp.if_body;
	asn_list* else_body = cond->op.cond_exp.else_body;
	ca_list* key_list = symbol_map_ptr->key_list;
	pv_leaf* sym;
	int num_vars = 0;

	for(; key_list != NULL; key_list = key_list->next){
		sym = pv_search(symbol_map_ptr, key_list->key);
		if(sym->scope != 0)
			num_vars++;
	}

	printf("if statement body containing %d vars.\n", num_vars);

	char* condition_src = asm_gen(condition);

	char* body_src = asm_gen_body(body);

	char* else_body_src = NULL;
	if(cond->op.cond_exp.else_body != NULL){
		else_body_src = asm_gen_body(cond->op.cond_exp.else_body);
	}

	char* label_post_body = gen_label("post_body");
	char* label_post_body_line = salloc(80);
	char* label_post_body_jmp = salloc(80);
	sprintf(label_post_body_line, "%s:\n", label_post_body);
	sprintf(label_post_body_jmp, "    je     %s\n", label_post_body);

	char* label_end = gen_label("end");
	char* label_end_line = salloc(80);
	char* label_end_jmp = salloc(80);
	sprintf(label_end_line, "%s:\n", label_end);
	sprintf(label_end_jmp, "    jmp    %s\n", label_end);

	char* all_src = strnew();

	strapp(&all_src, condition_src);
	strapp(&all_src, "    cmpq   $0, %rax\n");
	strapp(&all_src, label_post_body_jmp);
	strapp(&all_src, body_src);

	if(else_body != NULL)
		strapp(&all_src, label_end_jmp);

	strapp(&all_src, label_post_body_line);

	if(else_body != NULL){
		strapp(&all_src, else_body_src);
		strapp(&all_src, label_end_line);
	}

	symbol_map_ptr = old_symbol_map_ptr;

    free(condition_src);
    free(body_src);
    if(else_body != NULL)
        free(else_body_src);
    free(label_post_body);
    free(label_post_body_line);
    free(label_post_body_jmp);
    free(label_end);
    free(label_end_line);
    free(label_end_jmp);

    return all_src;
}

