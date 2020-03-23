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

quad_list* ic_gen_var_def(asn* node){
    quad_list* IC = NULL;
    quad_list* rhs = NULL;
    if(node->op.var_def_exp.val != NULL){
        rhs = ic_gen(node->op.var_def_exp.val);
    } else {
        asn* tmp_null = make_int_exp(0);
        rhs = ic_gen(tmp_null);
        delete_exp(tmp_null);
    }

    char* reg_id = get_tmp_name();

////    pv_leaf* var = new_pv_leaf(node->op.var_def_exp.ident,
////                                node->op.var_def_exp.type,
////                                0, // TODO: replace
////                                0, // TODO: replace
////                                node->op.var_def_exp.scope);
//    pv_leaf* var = pv_search(symbol_map_ptr, node->op.var_def_exp.ident);
    symbol* reg = new_symbol(reg_id, at_void);
//
//    pv_root* osm = symbol_map_ptr;
//    symbol_map_ptr = pv_insert(symbol_map_ptr, reg_id, reg);
//    delete_trie(osm);
//
//    quadruple* q = make_quad(fac_store, reg, NULL, var);
//
    quad_list_app_quad_list(&IC, rhs);
//    quad_list_app_quad(&IC, q);
//
  delete_symbol(&reg);
    free(reg_id);
    return IC;
}

quad_list* ic_gen_var_ref(asn* node){
    quad_list* IC = NULL;

    char* reg_id = gen_tmp_name();
    symbol* reg = new_symbol(reg_id, at_void);

//    pv_root* osm = symbol_map_ptr;
//    symbol_map_ptr = pv_insert(symbol_map_ptr, reg_id, reg);
//    delete_trie(osm);
//
////    pv_leaf* var = new_pv_leaf(node->op.var_ref_exp.ident, at_void, 0, 0, 0);
//    pv_leaf* var = pv_search(symbol_map_ptr, node->op.var_ref_exp.ident);

//    quadruple* q = make_quad(fac_load, var, NULL, reg);
//    quad_list_app_quad(&IC, q);
    delete_symbol(&reg);
    free(reg_id);
    return IC;
}

