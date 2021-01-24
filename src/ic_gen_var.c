/*
 * FILENAME: ic_gen_var.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     definition, declaration and references of variables.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"


quad_list* ic_gen_var(asn* node){
    quad_list* IC = NULL;
    char* tmp_id = gen_tmp_name();
    symbol* tmp = new_symbol(tmp_id);
    copy_type_list(node->op.var_exp.sym, &tmp);
    symbol_list_append(&symbol_list_ptr, &tmp);

    quadruple* q = make_quad(fac_load, node->op.var_exp.sym, NULL, tmp);
    quad_list_app_quad(&IC, q);

    delete_symbol(&tmp);
    free(tmp_id);
    return IC;
}
