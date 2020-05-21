/*
 * FILENAME: ic_gen_const.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     definition, of literal constants.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_int_const(asn* node){
    quad_list* IC = NULL;

    char* arg1_id = salloc(12);
    sprintf(arg1_id, "%d", node->op.int_exp);
    char* res_id = gen_tmp_name();

    symbol* arg1 = new_symbol(arg1_id);
//    specifier* s = new_specifier();
//    s->type = type_int;
//    s->storage_class = sclass_auto;
//    s->output_class = oclass_auto;
//    type_link* tl = new_type_link();
//    tl->cls = cls_spec;
//    tl->type.spec = s;
//    type_link_attach(&(arg1), tl);

    symbol* res = new_symbol(res_id);
    specifier* s_res = new_specifier();
    s_res->type = type_int;
    s_res->storage_class = sclass_auto;
    s_res->output_class = oclass_auto;
    type_link* tl_res = new_type_link();
    tl_res->cls = cls_spec;
    tl_res->type.spec = s_res;
    type_link_attach(&(res), tl_res);

    symbol_list_append(&symbol_list_ptr, &res);

    quadruple* q = make_quad(fac_load, arg1, NULL, res);
    quad_list_app_quad(&IC, q);

    delete_symbol(&arg1);
    delete_symbol(&res);
    free(arg1_id);
    free(res_id);
    return IC;
}

quad_list* ic_gen_real_const(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_string_const(asn* node){
    quad_list* IC = NULL;
	char* sc_id = salloc(12);
	sprintf(sc_id, ".SC%d", node->op.int_exp);
	symbol* sc = gen_symbol(sc_id);

	symbol* res = gen_tmp();
	specifier* s_res = new_specifier();
	s_res->type = type_string;
	s_res->storage_class = sclass_auto;
	s_res->output_class = oclass_auto;
	type_link* tl_res = new_type_link();
	tl_res->cls = cls_spec;
	tl_res->type.spec = s_res;
	type_link_attach(&(res), tl_res);
	symbol_list_append(&symbol_list_ptr, &res);

	quadruple* load = make_quad(fac_load, sc, NULL, res);
	quad_list_app_quad(&IC, load);

	delete_symbol(&sc);
	delete_symbol(&res);
	free(sc_id);
    return IC;
}

