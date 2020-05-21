/*
 * FILENAME: ic_gen_logic.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     logical operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"


quad_list* ic_gen_log_and(asn* node){
    quad_list* IC = NULL;

    quad_list* lval_code = ic_gen(node->op.binary_exp.lval);
	symbol* lval_res = get_tmp();

    quad_list* rval_code = ic_gen(node->op.binary_exp.rval);
	symbol* rval_res = get_tmp();

	symbol* zero = new_symbol(strdup("0"));
    quadruple* qcmplval = make_quad(fac_compare, zero, lval_res, NULL);
    quadruple* qcmprval = make_quad(fac_compare, zero, rval_res, NULL);

	symbol* label = gen_label("lpos");
    quadruple* qlabel = make_quad(fac_label, label, NULL, NULL);
    quadruple* jlabel = make_quad(fac_jne, label, NULL, NULL);

	symbol* end = gen_label("end");
    quadruple* qend = make_quad(fac_label, end, NULL, NULL);
    quadruple* jend = make_quad(fac_jump, end, NULL, NULL);

	symbol* res = gen_tmp();
    quadruple* qset = make_quad(fac_setne, NULL, NULL, res);
    quadruple* set_zero = make_quad(fac_load, zero, NULL, res);

    quad_list_app_quad_list(&IC, lval_code);
    quad_list_app_quad(&IC, qcmplval);
    quad_list_app_quad(&IC, jlabel);
    quad_list_app_quad(&IC, set_zero);
    quad_list_app_quad(&IC, jend);
    quad_list_app_quad(&IC, qlabel);
    quad_list_app_quad_list(&IC, rval_code);
    quad_list_app_quad(&IC, qcmprval);
    quad_list_app_quad(&IC, qset);
    quad_list_app_quad(&IC, qend);

    delete_symbol(&zero);
    delete_symbol(&label);
    delete_symbol(&end);
    delete_symbol(&res);
    return IC;
}

quad_list* ic_gen_log_xor(asn* node){
    quad_list* IC = NULL;

	quad_list* lval_code = ic_gen(node->op.binary_exp.lval);
	symbol* lval_res = get_tmp();
	quadruple* lval_store = make_quad(fac_store, lval_res, NULL, NULL); // push
	quadruple* lval_load1 = make_quad(fac_load, NULL, NULL, lval_res); // pop
	quadruple* lval_load2 = make_quad(fac_load, NULL, NULL, lval_res); // pop

	quad_list* rval_code = ic_gen(node->op.binary_exp.rval);
	symbol* rval_res = get_tmp();

	symbol* zero = new_symbol("0");
	quadruple* qcmplval1 = make_quad(fac_compare, zero, lval_res, NULL);
	quadruple* qcmplval2 = make_quad(fac_compare, zero, lval_res, NULL);
	quadruple* qcmprval = make_quad(fac_compare, zero, rval_res, NULL);

	printf("blabla %zu\n", lval_res->ref_count);
	printf("blabla %zu\n", rval_res->ref_count);

	symbol* label = gen_label("lpos");
    quadruple* qlabel = make_quad(fac_label, label, NULL, NULL);
    quadruple* jlabel = make_quad(fac_jne, label, NULL, NULL);

	symbol* end = gen_label("end");
    quadruple* qend = make_quad(fac_label, end, NULL, NULL);
    quadruple* jend = make_quad(fac_jump, end, NULL, NULL);

	symbol* res = gen_tmp();
    quadruple* qsetne = make_quad(fac_setne, NULL, NULL, res);
	quadruple* qsete = make_quad(fac_sete, NULL, NULL, res);

	quad_list_app_quad_list(&IC, lval_code);
	quad_list_app_quad(&IC, lval_store);
	quad_list_app_quad_list(&IC, rval_code);
	quad_list_app_quad(&IC, qcmprval);
	quad_list_app_quad(&IC, jlabel);
	quad_list_app_quad(&IC, lval_load1);
	quad_list_app_quad(&IC, qcmplval1);
	quad_list_app_quad(&IC, qsetne);
	quad_list_app_quad(&IC, jend);
	quad_list_app_quad(&IC, qlabel);
	quad_list_app_quad(&IC, lval_load2);
	quad_list_app_quad(&IC, qcmplval2);
	quad_list_app_quad(&IC, qsete);
	quad_list_app_quad(&IC, qend);

	delete_symbol(&zero);
    delete_symbol(&label);
    delete_symbol(&end);
    delete_symbol(&res);
    return IC;
}

quad_list* ic_gen_log_or(asn* node){
    quad_list* IC = NULL;

    quad_list* lval_code = ic_gen(node->op.binary_exp.lval);
	symbol* lval_res = get_tmp();

    quad_list* rval_code = ic_gen(node->op.binary_exp.rval);
	symbol* rval_res = get_tmp();

	symbol* zero = new_symbol("0");
	copy_type_list(lval_res, &zero);
    quadruple* qcmplval = make_quad(fac_compare, zero, lval_res, NULL);
    quadruple* qcmprval = make_quad(fac_compare, zero, rval_res, NULL);

	symbol* label = gen_label("lpos");
    quadruple* qlabel = make_quad(fac_label, label, NULL, NULL);
    quadruple* jlabel = make_quad(fac_je, label, NULL, NULL);

	symbol* end = gen_label("end");
    quadruple* qend = make_quad(fac_label, end, NULL, NULL);
    quadruple* jend = make_quad(fac_jump, end, NULL, NULL);

	symbol* res = gen_tmp();
    quadruple* qset = make_quad(fac_setne, NULL, NULL, res);

    quad_list_app_quad_list(&IC, lval_code);
    quad_list_app_quad(&IC, qcmplval);
    quad_list_app_quad(&IC, jlabel);
    quad_list_app_quad(&IC, jend);
    quad_list_app_quad(&IC, qlabel);
    quad_list_app_quad_list(&IC, rval_code);
    quad_list_app_quad(&IC, qcmprval);
    quad_list_app_quad(&IC, qset);
    quad_list_app_quad(&IC, qend);

    delete_symbol(&zero);
    delete_symbol(&label);
    delete_symbol(&end);
    delete_symbol(&res);
    return IC;
}

