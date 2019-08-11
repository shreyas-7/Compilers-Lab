#include "ast.hh"
template class Number_Ast<double>;
template class Number_Ast<int>;

int is_if = 0;

//Code_For_Ast & Ast::compile(){}
//Code_For_Ast & Ast::compile_and_optimize_ast(Lra_Outcome &  lra){}


Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast result = rhs->compile();
	Register_Descriptor *reg = result.get_reg();
	list<Icode_Stmt *> ics = result.get_icode_list();

	Register_Addr_Opd* opd1 = new Register_Addr_Opd(reg);
	Mem_Addr_Opd* mem = new Mem_Addr_Opd(lhs->get_symbol_entry());

	Move_IC_Stmt* move;
	if (rhs->get_data_type() == double_data_type)
		move = new Move_IC_Stmt(store_d, opd1, mem);
	else
		move = new Move_IC_Stmt(store, opd1, mem);
	Code_For_Ast* code = new Code_For_Ast(ics, reg);
	code->append_ics(*move);

	reg->reset_use_for_expr_result();
	return *code;
}

Code_For_Ast & Name_Ast::compile(){
	Register_Descriptor *reg;	

	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();
	// cout << reg << 
	
	list<Icode_Stmt *> *ics = new list<Icode_Stmt *>();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(reg);
	Mem_Addr_Opd* mem = new Mem_Addr_Opd(get_symbol_entry());

	Move_IC_Stmt* move;
	if (get_data_type() == double_data_type)
		move = new Move_IC_Stmt(load_d, mem, opd1);
	else
		move = new Move_IC_Stmt(load, mem, opd1);

	Code_For_Ast* code = new Code_For_Ast(*ics, reg);
	code->append_ics(*move);

	//reg->reset_use_for_expr_result();
	return *code;
}

template <class T> Code_For_Ast & Number_Ast<T>::compile(){
	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();		
	// reg->set_use_for_expr_result();
	list<Icode_Stmt *>* ics = new list<Icode_Stmt *>();
	Const_Opd<T>* opd1 = new Const_Opd<T>(constant);
	Register_Addr_Opd * result = new Register_Addr_Opd(reg);

	Move_IC_Stmt* move;
	if (get_data_type() == double_data_type)
		move = new Move_IC_Stmt(imm_load_d, opd1, result);
	else
		move = new Move_IC_Stmt(imm_load, opd1, result);
	Code_For_Ast* code = new Code_For_Ast(*ics, reg);
	code->append_ics(*move);

	//reg->reset_use_for_expr_result();
	return *code;
	
}
//Code_For_Ast & Arithmetic_Expr_Ast::compile(){}
Code_For_Ast & Plus_Ast::compile(){
	
	Code_For_Ast e1 = lhs->compile();
	Register_Descriptor* l = e1.get_reg(); 
	list<Icode_Stmt *> ics1 = e1.get_icode_list();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

	Code_For_Ast e2 = rhs->compile();
	Register_Descriptor* r = e2.get_reg(); 
	list<Icode_Stmt *> ics2 = e2.get_icode_list();
	Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);
	
	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* compute;

	if (get_data_type() == double_data_type)
		compute = new Compute_IC_Stmt(add_d, opd1, opd2, result);
	else
		compute = new Compute_IC_Stmt(add, opd1, opd2, result);

	l->reset_use_for_expr_result();
	r->reset_use_for_expr_result();
	ics1.merge(ics2);
//	ics1.push_back(compute);
	Code_For_Ast* code = new Code_For_Ast(ics1, reg);

	code->append_ics(*compute);

	//reg->reset_use_for_expr_result();
	return *code;
}

Code_For_Ast & Minus_Ast::compile(){
	
	Code_For_Ast e1 = lhs->compile();
	Register_Descriptor* l = e1.get_reg(); 
	list<Icode_Stmt *> ics1 = e1.get_icode_list();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

	Code_For_Ast e2 = rhs->compile();
	Register_Descriptor* r = e2.get_reg(); 
	list<Icode_Stmt *> ics2 = e2.get_icode_list();
	Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);
	
	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();
	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* compute;
	
	if (get_data_type() == double_data_type)
		compute = new Compute_IC_Stmt(sub_d, opd1, opd2, result);
	else
		compute = new Compute_IC_Stmt(sub, opd1, opd2, result);

	ics1.merge(ics2);
	// ics1.push_back(compute);
	Code_For_Ast* code = new Code_For_Ast(ics1, reg);
	l->reset_use_for_expr_result();
	r->reset_use_for_expr_result();
	
	code->append_ics(*compute);

	//reg->reset_use_for_expr_result();
	return *code;

}

Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast e1 = lhs->compile();
	Register_Descriptor* l = e1.get_reg(); 
	list<Icode_Stmt *> ics1 = e1.get_icode_list();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

	Code_For_Ast e2 = rhs->compile();
	Register_Descriptor* r = e2.get_reg(); 
	list<Icode_Stmt *> ics2 = e2.get_icode_list();
	Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);
	
	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();

	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* compute;
	
	if (get_data_type() == double_data_type)
		compute = new Compute_IC_Stmt(div_d, opd1, opd2, result);
	else
		compute = new Compute_IC_Stmt(divd, opd1, opd2, result);

	ics1.merge(ics2);
	// ics1.push_back(compute);
	Code_For_Ast* code = new Code_For_Ast(ics1, reg);
	l->reset_use_for_expr_result();
	r->reset_use_for_expr_result();
	
	code->append_ics(*compute);

	//reg->reset_use_for_expr_result();
	return *code;
}

Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast e1 = lhs->compile();
	Register_Descriptor* l = e1.get_reg(); 
	list<Icode_Stmt *> ics1 = e1.get_icode_list();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

	Code_For_Ast e2 = rhs->compile();
	Register_Descriptor* r = e2.get_reg(); 
	list<Icode_Stmt *> ics2 = e2.get_icode_list();
	Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);
	
	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();

	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* compute;
	
	if (get_data_type() == double_data_type)
		compute = new Compute_IC_Stmt(mult_d, opd1, opd2, result);
	else
		compute = new Compute_IC_Stmt(mult, opd1, opd2, result);

	ics1.merge(ics2);
	// ics1.push_back(compute);
	Code_For_Ast* code = new Code_For_Ast(ics1, reg);
	l->reset_use_for_expr_result();
	r->reset_use_for_expr_result();
	
	code->append_ics(*compute);

	//reg->reset_use_for_expr_result();
	return *code;
}

Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast e1 = lhs->compile();
	Register_Descriptor* l = e1.get_reg(); 
	list<Icode_Stmt *> ics1 = e1.get_icode_list();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();

	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* compute;
	
	if (get_data_type() == double_data_type)
		compute = new Compute_IC_Stmt(uminus_d, opd1, NULL, result);
	else
		compute = new Compute_IC_Stmt(uminus, opd1, NULL, result);

	// ics1.push_back(compute);
	Code_For_Ast* code = new Code_For_Ast(ics1, reg);
	l->reset_use_for_expr_result();
	
	code->append_ics(*compute);

	//reg->reset_use_for_expr_result();
	return *code;
}

Code_For_Ast & Relational_Expr_Ast::compile(){
	Code_For_Ast e1 = lhs_condition->compile();
	Register_Descriptor* l = e1.get_reg(); 
	list<Icode_Stmt *> ics1 = e1.get_icode_list();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

	Code_For_Ast e2 = rhs_condition->compile();
	Register_Descriptor* r = e2.get_reg(); 
	list<Icode_Stmt *> ics2 = e2.get_icode_list();
	Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);
	
	Register_Descriptor *reg;
	if (get_data_type() == double_data_type)
		reg = machine_desc_object.get_new_register<float_reg>();
	else
		reg = machine_desc_object.get_new_register<int_reg>();
	// reg->set_use_for_expr_result();

	Register_Addr_Opd* result = new Register_Addr_Opd(reg);

	Compute_IC_Stmt* compute;
	if (rhs_condition->get_data_type() == int_data_type)
	{
		if (rel_op == less_than)
		compute = new Compute_IC_Stmt(slt, opd1, opd2, result);   
		else if(rel_op == greater_than)
			compute = new Compute_IC_Stmt(sgt, opd1, opd2, result);
		else if(rel_op == less_equalto)
			compute = new Compute_IC_Stmt(sle, opd1, opd2, result);
		else if(rel_op == greater_equalto)
			compute = new Compute_IC_Stmt(sge, opd1, opd2, result);
		else if(rel_op == equalto)
			compute = new Compute_IC_Stmt(seq, opd1, opd2, result);
		else
			compute = new Compute_IC_Stmt(sne, opd1, opd2, result);
	}
	else {
		if (rel_op == less_than)
		{compute = new Compute_IC_Stmt(slt_d, opd1, opd2, NULL); is_if=1;}
		else if(rel_op == greater_than)                                       
			{compute = new Compute_IC_Stmt(sgt_d, opd1, opd2, NULL); is_if=0;}
		else if(rel_op == less_equalto)                                       
			{compute = new Compute_IC_Stmt(sle_d, opd1, opd2, NULL); is_if=1;}
		else if(rel_op == greater_equalto)                                    
			{compute = new Compute_IC_Stmt(sge_d, opd1, opd2, NULL); is_if=0;}
		else if(rel_op == equalto)                                            
			{compute = new Compute_IC_Stmt(seq_d, opd1, opd2, NULL); is_if=1;}
		else                                                                  
		{	compute = new Compute_IC_Stmt(sne_d, opd1, opd2, NULL);	is_if=0;}
	}

	ics1.merge(ics2);
	// ics1.push_back(compute);
	Code_For_Ast* code = new Code_For_Ast(ics1, reg);
	l->reset_use_for_expr_result();
	r->reset_use_for_expr_result();
	
	code->append_ics(*compute);

	//reg->reset_use_for_expr_result();
	return *code;
}

Code_For_Ast & Logical_Expr_Ast::compile(){
	if(bool_op != _logical_not){
		Code_For_Ast e1 = lhs_op->compile();
		Register_Descriptor* l = e1.get_reg(); 
		list<Icode_Stmt *> ics1 = e1.get_icode_list();
		Register_Addr_Opd* opd1 = new Register_Addr_Opd(l);

		Code_For_Ast e2 = rhs_op->compile();
		Register_Descriptor* r = e2.get_reg(); 
		list<Icode_Stmt *> ics2 = e2.get_icode_list();
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);
		
		Register_Descriptor *reg = machine_desc_object.get_new_register<int_reg>();;
		// reg->set_use_for_expr_result();

		Register_Addr_Opd* result = new Register_Addr_Opd(reg);

		Compute_IC_Stmt* compute;
		
		if (bool_op == _logical_and)
			compute = new Compute_IC_Stmt(and_t, opd1, opd2, result);
		else
			compute = new Compute_IC_Stmt(or_t, opd1, opd2, result);

		ics1.merge(ics2);
		// ics1.push_back(compute);
		Code_For_Ast* code = new Code_For_Ast(ics1, reg);
		l->reset_use_for_expr_result();
		r->reset_use_for_expr_result();
		
		code->append_ics(*compute);

		//reg->reset_use_for_expr_result();
		return *code;
	}
	else{
			Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
			// temp->set_use_for_expr_result();
			Register_Addr_Opd * not_opd = new Register_Addr_Opd(temp);
			
			Ics_Opd * one = new Const_Opd<int>(1);
			Move_IC_Stmt * m = new Move_IC_Stmt(imm_load , one, not_opd);


		Code_For_Ast e2 = rhs_op->compile();
		Register_Descriptor* r = e2.get_reg(); 
		list<Icode_Stmt *> ics2 = e2.get_icode_list();
		Register_Addr_Opd* opd2 = new Register_Addr_Opd(r);

		ics2.push_front(m);
		
		Register_Descriptor *reg = machine_desc_object.get_new_register<int_reg>();;
		// reg->set_use_for_expr_result();

		Register_Addr_Opd* result = new Register_Addr_Opd(reg);

		Compute_IC_Stmt* compute = new Compute_IC_Stmt(not_t, opd2, not_opd, result);

		// ics2.push_back(compute);
		Code_For_Ast* code = new Code_For_Ast(ics2, reg);
		r->reset_use_for_expr_result();
		temp->reset_use_for_expr_result();
		code->append_ics(*compute);

		//reg->reset_use_for_expr_result();
		return *code;

	}
}
// search for I1.push_back
Code_For_Ast & Selection_Statement_Ast::compile(){
	// cout<<"IN IF\n";
	Code_For_Ast cond_stmt = cond->compile();
	
	Code_For_Ast then_stmt = then_part->compile();
	string label0 = get_new_label();
	string label1 = get_new_label();
	Code_For_Ast else_stmt;

	if (else_part)
		else_stmt = else_part->compile();

	// cout<<"\nLABEL 0:"<<label0;
	// cout<<"\nLABEL 1:"<<label1;
	
	
	Register_Descriptor* cond_reg = cond_stmt.get_reg();
	Register_Addr_Opd* cond_opd = new Register_Addr_Opd(cond_reg);
	Control_Flow_IC_Stmt* control_stmt;
	if (cond->get_data_type() == int_data_type)
		control_stmt = new Control_Flow_IC_Stmt(beq, cond_opd, NULL, label0);
	else if (is_if) control_stmt = new Control_Flow_IC_Stmt(bc1f,NULL, NULL, label0); 
	else 
		control_stmt = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label0);


	list<Icode_Stmt *> ics = cond_stmt.get_icode_list();
	ics.push_back(control_stmt);

	ics.splice(ics.end(),then_stmt.get_icode_list());

	if(else_part){
		Label_IC_Stmt * control_stmt2 = new Label_IC_Stmt(j, label1);
		ics.push_back(control_stmt2);
	}
	Label_IC_Stmt * label0_stmt = new Label_IC_Stmt(label, label0);
	ics.push_back(label0_stmt);

	if (else_part){
		ics.splice(ics.end(),else_stmt.get_icode_list());	

		Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label, label1);
		ics.push_back(label1_stmt);
	}
	cond_reg->reset_use_for_expr_result();

	Code_For_Ast* code = new Code_For_Ast(ics, cond_reg);

	return *code; 
}

Code_For_Ast & Iteration_Statement_Ast::compile(){
	

	Code_For_Ast body_stmt = body->compile();
	list<Icode_Stmt *> ics = body_stmt.get_icode_list();

	string label0 = get_new_label();
	string label1 = get_new_label();

	Label_IC_Stmt *label0_stmt = new Label_IC_Stmt(label,label0);
	Label_IC_Stmt *label1_stmt = new Label_IC_Stmt(label,label1);
	Label_IC_Stmt *control_stmt = new Label_IC_Stmt(j, label1);

	ics.push_front(label0_stmt);
	if(!is_do_form)
		ics.push_front(control_stmt);
	ics.push_back(label1_stmt);

	Code_For_Ast cond_stmt = cond->compile();
	int is_if=::is_if;
	Register_Descriptor* cond_reg = cond_stmt.get_reg();
	Register_Addr_Opd* opd1 = new Register_Addr_Opd(cond_reg);

	Control_Flow_IC_Stmt* branch_stmt ; 
	if (cond->get_data_type() == int_data_type)
		branch_stmt = new Control_Flow_IC_Stmt(bne,opd1, NULL, label0);
	else if (is_if)  
		branch_stmt = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label0);
	else 
		branch_stmt = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label0);

	ics.splice(ics.end(),cond_stmt.get_icode_list());
	ics.push_back(branch_stmt);

	Code_For_Ast* code = new Code_For_Ast(ics, cond_reg);

	return *code;
}

Code_For_Ast & Conditional_Expression_Ast::compile(){
	//get_new_label();


	Code_For_Ast cond_stmt = cond->compile();


	Register_Descriptor * cond_reg = cond_stmt.get_reg();
	Register_Addr_Opd * cond_opd = new Register_Addr_Opd(cond_reg);

	string label0 = get_new_label();

	Code_For_Ast lhs_stmt = lhs->compile();
	Register_Descriptor * l = lhs_stmt.get_reg();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(l);
	
	string label1 = get_new_label();
	Code_For_Ast rhs_stmt = rhs->compile();
	Register_Descriptor * r = rhs_stmt.get_reg();
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(r);
	


	// Control_Flow_IC_Stmt * control_stmt = new Control_Flow_IC_Stmt(beq, cond_opd, label0);

	
	Control_Flow_IC_Stmt* control_stmt;
	if (cond->get_data_type() == int_data_type)
		control_stmt = new Control_Flow_IC_Stmt(beq, cond_opd, NULL, label0);
	else if (is_if) control_stmt = new Control_Flow_IC_Stmt(bc1f,NULL,NULL, label0); 
	else 
		control_stmt = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label0);

	Register_Descriptor * result;
	if(get_data_type() == double_data_type) {
		result =  machine_desc_object.get_new_register<float_reg>();
	} else {
		result =  machine_desc_object.get_new_register<int_reg>();
	}
	// result->set_use_for_expr_result();

	list<Icode_Stmt *> ics = cond_stmt.get_icode_list();
	ics.push_back(control_stmt);
	ics.splice(ics.end(),lhs_stmt.get_icode_list());

	Register_Addr_Opd * reg = new Register_Addr_Opd(result);
	Register_Addr_Opd * zeroOpd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);

	Compute_IC_Stmt * compute_lhs = new Compute_IC_Stmt(or_t, opd1, zeroOpd, reg);
	Label_IC_Stmt* control_stmt2 = new Label_IC_Stmt(j, label1);
	Label_IC_Stmt * label0_stmt = new Label_IC_Stmt(label, label0);
	ics.push_back(compute_lhs);
	ics.push_back(control_stmt2);
	ics.push_back(label0_stmt);

	Compute_IC_Stmt * compute_rhs = new Compute_IC_Stmt(or_t, opd2, zeroOpd, reg);
	Label_IC_Stmt * label1_stmt = new Label_IC_Stmt(label, label1);
	ics.splice(ics.end(),rhs_stmt.get_icode_list());
	ics.push_back(compute_rhs);
	ics.push_back(label1_stmt);

	cond_reg->reset_use_for_expr_result();
	l->reset_use_for_expr_result();
	r->reset_use_for_expr_result();

	Code_For_Ast * code = new Code_For_Ast(ics, result);

	return *code;
}

Code_For_Ast & Sequence_Ast::compile() {
	// cout<<"IN SEQUENCE\n";
	//get_new_label();
	Code_For_Ast result;
    list<Ast *>::iterator it;
    list<Icode_Stmt *> *temp = new list<Icode_Stmt *>();
    Register_Descriptor *reg;
    for (it = statement_list.begin(); it != statement_list.end(); it++) {
    	result = (*it)->compile();
    	temp->splice(temp->end(),result.get_icode_list());
    	reg = result.get_reg();
    }
    Code_For_Ast *code = new Code_For_Ast(*temp,reg);
    return *code;
}

Code_For_Ast & Print_Ast::compile(){

//	Code_For_Ast result ; 
	if (var->get_data_type() == int_data_type) {	

			Register_Descriptor * call = machine_desc_object.spim_register_table[v0];
			Register_Descriptor * arg = machine_desc_object.spim_register_table[a0];

			Register_Addr_Opd * not_opd = new Register_Addr_Opd(call);
			Ics_Opd * one = new Const_Opd<int>(1);
			Move_IC_Stmt * m = new Move_IC_Stmt(imm_load , one, not_opd);

			list<Icode_Stmt *> *ics = new list<Icode_Stmt *>();
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(arg);
			Mem_Addr_Opd* mem = new Mem_Addr_Opd(var->get_symbol_entry());
			Move_IC_Stmt* move =  new Move_IC_Stmt(load, mem, opd1);
			
			Icode_Stmt* assign = new Print_IC_Stmt();
			ics->push_back(m);
			ics->push_back(move);
			ics->push_back(assign); 
			Code_For_Ast *code = new Code_For_Ast(*ics, NULL);
			//std::cout <<" ASDOIJ\n";
			return *code;
	}
	
	else if (var->get_data_type() == double_data_type) {	

			Register_Descriptor * call = machine_desc_object.spim_register_table[v0];
			Register_Descriptor * arg = machine_desc_object.spim_register_table[f12];

			Register_Addr_Opd * not_opd = new Register_Addr_Opd(call);
			Ics_Opd * one = new Const_Opd<int>(3);
			Move_IC_Stmt * m = new Move_IC_Stmt(imm_load , one, not_opd);
			list<Icode_Stmt *> *ics = new list<Icode_Stmt *>();
			Register_Addr_Opd* opd1 = new Register_Addr_Opd(arg);
			Mem_Addr_Opd* mem = new Mem_Addr_Opd(var->get_symbol_entry());
			Move_IC_Stmt* move =  new Move_IC_Stmt(load_d, mem, opd1);
			Icode_Stmt* assign = new Print_IC_Stmt;
			ics->push_back(m);
			ics->push_back(move);
			ics->push_back(assign); 
			Code_For_Ast *code = new Code_For_Ast(*ics, NULL);
			return *code;
	}
}



Code_For_Ast & Return_Ast::compile(){
	if(return_value == NULL){
	    string epilogue = "epilogue_" + proc_name;
	    Control_Flow_IC_Stmt * jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, epilogue);
	    Code_For_Ast *code = new Code_For_Ast();
	    code->append_ics(*jump_stmt);
	    return *code;
	}
    Code_For_Ast &ret = return_value->compile();

    Move_IC_Stmt * move ;
    Register_Descriptor * reg;
    if (get_data_type() == int_data_type){
	    reg = machine_desc_object.spim_register_table[v1];
	    
	    Register_Addr_Opd * opd1 = new Register_Addr_Opd(ret.get_reg());
	    Register_Addr_Opd * opd2 = new Register_Addr_Opd(reg);
	    move = new Move_IC_Stmt (mov, opd1, opd2);
    }
    else if (get_data_type() == double_data_type){
	    reg = machine_desc_object.spim_register_table[f0];
	    
	    Register_Addr_Opd * opd1 = new Register_Addr_Opd(ret.get_reg());
	    Register_Addr_Opd * opd2 = new Register_Addr_Opd(reg);
		move = new Move_IC_Stmt (move_d, opd1, opd2);
    }


    string epilogue = "epilogue_" + proc_name;
    
    list<Icode_Stmt *> ics;
    list<Icode_Stmt *> &ret_list = ret.get_icode_list();
    Control_Flow_IC_Stmt * jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, epilogue);
    ics.splice(ics.begin(), ret_list);
    ics.push_back(move);
    ics.push_back(jump_stmt);

    Code_For_Ast *code = new Code_For_Ast(ics, reg);
    ret.get_reg()->reset_use_for_expr_result();
    return *code;
}

Code_For_Ast & Call_Ast::compile(){
    Procedure * p = program_object.get_procedure_prototype(procedure_name);
    Symbol_Table & params = p->get_formal_param_list();
    
    list<Register_Descriptor*> reg_list;
    list<Icode_Stmt*> ics;

    int offset = 0;
    int n = actual_param_list.size();
    list<Ast *> copy_param_list = actual_param_list;
    copy_param_list.reverse();
    for (list<Ast*>::iterator iter = copy_param_list.begin();iter != copy_param_list.end(); iter++) {
//        (*iter)->get_symbol_entry().set_symbol_scope(formal);
        // cout<<(*iter)->get_variable_name()<<" "<<(*iter)->get_data_type()<<endl;
        // cout<<(*iter)->get_symbol_entry().get_variable_name()<<" QWERTYUIOP"<<endl;
        Code_For_Ast &c_ast = (*iter)->compile();
        ics.splice(ics.end(), c_ast.get_icode_list());
        Register_Descriptor* reg = c_ast.get_reg();
        reg_list.push_back(reg);
        Symbol_Table_Entry copy = params.get_symbol_table_entry(params.get_variable_in_formal_list(n));
        string name = copy.get_variable_name();
        Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,
        									copy.get_data_type(),copy.get_lineno(), sp_ref);
        entry->set_symbol_scope(formal);
        entry->set_start_offset(offset);
        if ((*iter)->get_data_type() == int_data_type)
            offset -= 4;
        else
            offset -= 8;


        Register_Addr_Opd * opd1 = new Register_Addr_Opd(reg);
        Mem_Addr_Opd * opd2 = new Mem_Addr_Opd(*entry);
        Move_IC_Stmt * move;
		if ((*iter)->get_data_type() == double_data_type)
			move = new Move_IC_Stmt(store_d, opd1, opd2);
		else
			move = new Move_IC_Stmt(store, opd1, opd2);
        ics.push_back(move);
        c_ast.get_reg()->reset_use_for_expr_result();
        n--;
    }

    Control_Flow_IC_Stmt * stmt = new Control_Flow_IC_Stmt(jal, NULL, NULL, procedure_name);
    if (offset != 0) {
        Const_Opd<int> * off = new Const_Opd<int>(-offset);

        Register_Descriptor * sp_reg = machine_desc_object.spim_register_table[sp];
        Register_Addr_Opd * opd1 = new Register_Addr_Opd(sp_reg);
        Compute_IC_Stmt * add_stmt = new Compute_IC_Stmt(add, opd1, off, opd1);
        Compute_IC_Stmt * sub_stmt = new Compute_IC_Stmt(sub, opd1, off, opd1);
    
        ics.push_back(sub_stmt);
        ics.push_back(stmt);
        ics.push_back(add_stmt);
    } 
    else
       ics.push_back(stmt);
    for (list<Register_Descriptor *>::iterator iter=reg_list.begin(); iter!=reg_list.end(); iter++)
        (*iter)->reset_use_for_expr_result();

    Register_Descriptor* reg = new Register_Descriptor();
    if (p->get_return_type() == int_data_type){
        reg = machine_desc_object.get_new_register<int_reg>();
	    Register_Descriptor* v1_reg = machine_desc_object.spim_register_table[v1];
        Register_Addr_Opd *opd1 = new Register_Addr_Opd(v1_reg);
        Register_Addr_Opd *opd2 = new Register_Addr_Opd(reg);
        Move_IC_Stmt * move = new Move_IC_Stmt(mov, opd1, opd2);;
	    ics.push_back(move);
    }
    else if (p->get_return_type() == double_data_type){
        reg = machine_desc_object.get_new_register<float_reg>();

	    Register_Descriptor* f0_reg = machine_desc_object.spim_register_table[f0];
        Register_Addr_Opd *opd1 = new Register_Addr_Opd(f0_reg);
        Register_Addr_Opd *opd2 = new Register_Addr_Opd(reg);
        Move_IC_Stmt * move = new Move_IC_Stmt(move_d, opd1, opd2);;
	    ics.push_back(move);
	}

	if(return_value_reg != NULL){
		cout<<"SDFKLD1SF\n";
		if (p->get_return_type() == int_data_type || p->get_return_type() == double_data_type)
			{
		cout<<"SDFKLD2SF\n";
				reg->reset_use_for_expr_result();
		cout<<"SDFKL3DSF\n";
	}
	}

    Code_For_Ast * code = new Code_For_Ast(ics, reg);
    return *code;
}


Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
template <class T> Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){}
//Code_For_Ast & Arithmetic_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
// Code_For_Ast & Conditional_Expression_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
// Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
// Code_For_Ast & Logical_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
// Code_For_Ast & Selection_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
// Code_For_Ast & Iteration_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){} 
// Code_For_Ast & Sequence_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}


Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){}


void Sequence_Ast::print_assembly(ostream & file_buffer){}
void Sequence_Ast::print_icode(ostream & file_buffer){}

