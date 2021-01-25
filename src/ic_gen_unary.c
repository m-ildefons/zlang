/*
 * FILENAME: ic_gen_unary.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     unary operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_unary_minus(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);

    char* tmp_id = get_tmp_name();
    symbol* tmp = search_symbol(symbol_list_ptr, tmp_id);

    char* tmp2_id = gen_tmp_name();
    symbol* tmp2 = new_symbol(tmp2_id);
    copy_type_list(tmp, &tmp2);
    symbol_list_append(&symbol_list_ptr, &tmp2);

    quadruple* q = make_quad(fac_neg, tmp, NULL, tmp2);

    quad_list_app_quad_list(&IC, inner);
    quad_list_app_quad(&IC, q);

    free(tmp_id);
    free(tmp2_id);
    delete_symbol(&tmp2);
    return IC;
}

quad_list* ic_gen_unary_not(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
	symbol* ires = get_tmp();
	symbol* zero = new_symbol("0");
	symbol* res = gen_tmp();

	quadruple* qcmp = make_quad(fac_compare, zero, ires, NULL);
	quadruple* set_eq = make_quad(fac_sete, NULL, NULL, res);

    quad_list_app_quad_list(&IC, inner);
	quad_list_app_quad(&IC, qcmp);
	quad_list_app_quad(&IC, set_eq);

	delete_symbol(&zero);
	delete_symbol(&res);
    return IC;
}

quad_list* ic_gen_unary_compl(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
	symbol* ires = get_tmp();
	symbol* res = gen_tmp();

	quadruple* compl = make_quad(fac_compl, ires, NULL, res);

    quad_list_app_quad_list(&IC, inner);
	quad_list_app_quad(&IC, compl);

	delete_symbol(&res);
    return IC;
}

quad_list* ic_gen_reference(asn* node){
    quad_list* IC = NULL;

	symbol* inner_sym;
	if(node->op.unary_exp.val->tag != var_tag){
		quad_list* inner = ic_gen(node->op.unary_exp.val);
		inner_sym = get_tmp();

	    quad_list_app_quad_list(&IC, inner);
	} else {
		inner_sym = node->op.unary_exp.val->op.var_exp.sym;

	}

	symbol* res_sym = gen_tmp();
	declarator* ptr = new_declarator();
	ptr->type = decl_pointer;
	type_link* tl = new_type_link();
	tl->cls = cls_decl;
	tl->type.decl = ptr;
	type_link_attach(&(res_sym), tl);
	copy_type_list(inner_sym, &res_sym);

	quadruple* addr = make_quad(fac_addr, inner_sym, NULL, res_sym);

	quad_list_app_quad(&IC, addr);

	delete_symbol(&res_sym);
    return IC;
}

quad_list* ic_gen_dereference(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
    symbol* inner_sym = get_tmp();
    symbol* tmp_sym = gen_tmp();
    copy_type_list(inner_sym, &tmp_sym);
    quadruple* q = make_quad(fac_from, inner_sym, NULL, tmp_sym);
    quad_list_app_quad_list(&IC, inner);
    quad_list_app_quad(&IC, q);
    delete_symbol(&tmp_sym);
    return IC;
}

quad_list* ic_gen_incdec(asn* node){
  quad_list* IC = NULL;
  symbol* val = NULL;
  if (node->op.unary_exp.val->tag == var_tag) {
    val = node->op.unary_exp.val->op.var_exp.sym;
  }


  quad_list* inner = ic_gen(node->op.unary_exp.val);
	symbol* inner_sym = get_tmp();

	symbol* one_sym = gen_symbol("1");
  copy_type_list(inner_sym, &one_sym);

	symbol* tmp_sym = gen_tmp();
  copy_type_list(one_sym, &tmp_sym);

	symbol* res_sym = gen_tmp();
  copy_type_list(inner_sym, &res_sym);

  int fac_type;
  switch(node->tag){
    case inc_tag: fac_type = fac_add; break;
    case dec_tag: fac_type = fac_sub; break;
    default: fac_type = fac_add;
  }

  quadruple* q1 = make_quad(fac_load, one_sym, NULL, tmp_sym);
  quadruple* q2 = make_quad(fac_type, inner_sym, tmp_sym, res_sym);
  quadruple* q3;
  if(val != NULL)
      q3 = make_quad(fac_store, res_sym, NULL, val);
  else if (node->op.unary_exp.val->tag == deref_tag)
      q3 = make_quad(fac_into, res_sym, NULL, node->op.unary_exp.val->op.unary_exp.val->op.var_exp.sym);
  else
      q3 = make_quad(fac_store, res_sym, NULL, inner_sym);

  quad_list_app_quad_list(&IC, inner);
  quad_list_app_quad(&IC, q1);
  quad_list_app_quad(&IC, q2);
  quad_list_app_quad(&IC, q3);

  delete_symbol(&one_sym);
  delete_symbol(&tmp_sym);
  delete_symbol(&res_sym);
  return IC;
}

