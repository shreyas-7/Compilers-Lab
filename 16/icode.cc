#define ICS "    \t"
#define ICs '\t'
#define AS '\t'

template class Const_Opd<int>;
template class Const_Opd<double>;
extern Local_Environment interpreter_global_table;


Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){inst_op = op; this->name = name; mnemonic = mnn; ic_symbol = ics; ic_format = icf; assem_format = af;}
Instruction_Descriptor::Instruction_Descriptor(){}
Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){symbol_entry = &se;}
Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){ register_description = rd;}
Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){ op_desc = *machine_desc_object.spim_instruction_table[inst_op]; this->opd1 = opd1; this->result = result;}
Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){ op_desc = *machine_desc_object.spim_instruction_table[inst_op]; this->opd1 = opd1; this->opd2 = opd2; this->result = result;}
Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, string label, int size){ op_desc = *machine_desc_object.spim_instruction_table[inst_op]; this->opd1 = opd1; this->opd2 = opd2; this->offset = label; actual_param_size = size;}
Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label){op_desc = *machine_desc_object.spim_instruction_table[inst_op]; this->label = label;}
Code_For_Ast::Code_For_Ast(){}
Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){ics_list = ic_l; result_register = reg;}
Print_IC_Stmt::Print_IC_Stmt(){op_desc = *machine_desc_object.spim_instruction_table[print];}//cout<<"CONSTR\n";}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){register_description = rhs.register_description; return *this;}
template <class T> Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs){this->num = rhs.num; return *this;}
Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){op_desc = rhs.op_desc; opd1= rhs.opd1; result = rhs.result; return *this;}
Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){op_desc = rhs.op_desc; opd1= rhs.opd1; opd2 = rhs.opd2; result = rhs.result; return *this;}
Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){op_desc = rhs.op_desc; opd1= rhs.opd1; return *this;}
Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs){op_desc = rhs.op_desc; label = rhs.label; return *this;}
Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){result_register = rhs.result_register; ics_list = rhs.ics_list; return *this;}


Tgt_Op Instruction_Descriptor::get_op(){return inst_op;}
string Instruction_Descriptor::get_name(){return name;}
string Instruction_Descriptor::get_mnemonic(){return mnemonic;}
string Instruction_Descriptor::get_ic_symbol(){return ic_symbol;}
Icode_Format Instruction_Descriptor::get_ic_format(){return ic_format;}
Assembly_Format Instruction_Descriptor::get_assembly_format(){return assem_format;}

Instruction_Descriptor & Icode_Stmt::get_op(){return op_desc;}

Ics_Opd * Icode_Stmt::get_opd1(){}
Ics_Opd * Icode_Stmt::get_opd2(){}
Ics_Opd * Icode_Stmt::get_result(){}
void Icode_Stmt::set_opd1(Ics_Opd*){}
void Icode_Stmt::set_opd2(Ics_Opd*){}
void Icode_Stmt::set_result(Ics_Opd*){}

Ics_Opd * Move_IC_Stmt::get_opd1(){return opd1;}
Ics_Opd * Compute_IC_Stmt::get_opd1(){return opd1;}
Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){return opd1;}
Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){return opd2;}

Ics_Opd * Compute_IC_Stmt::get_opd2(){return opd2;}
void Compute_IC_Stmt::set_opd2(Ics_Opd* io){opd2 = io;}

Register_Descriptor * Register_Addr_Opd::get_reg(){ return register_description;}
	
Ics_Opd * Move_IC_Stmt::get_result()	{return result;}
Ics_Opd * Compute_IC_Stmt::get_result() {return result;}

Instruction_Descriptor & Compute_IC_Stmt	 ::get_inst_op_of_ics(){return op_desc;}
Instruction_Descriptor & Move_IC_Stmt		 ::get_inst_op_of_ics(){return op_desc;}
Instruction_Descriptor & Label_IC_Stmt		 ::get_inst_op_of_ics(){return op_desc;}
Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){return op_desc;}


// string Control_Flow_IC_Stmt::get_label()			{return label;}
string Label_IC_Stmt::get_label()					{return label;}

template <class T> Const_Opd<T>::Const_Opd (T num){ this->num = num;}


void Move_IC_Stmt::set_opd1(Ics_Opd * io){ opd1 = io;}
void Compute_IC_Stmt::set_opd1(Ics_Opd * io){ opd1 = io;}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){ opd1 = io;}
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){ opd2 = io;}

void Move_IC_Stmt::set_result(Ics_Opd * io){result = io;}
void Compute_IC_Stmt::set_result(Ics_Opd * io){result = io;}


// void Control_Flow_IC_Stmt::set_label(string io){label = io;}
void Label_IC_Stmt::set_label(string label){this->label = label;}
void Code_For_Ast::set_reg(Register_Descriptor * reg){result_register = reg;}


void Instruction_Descriptor::print_instruction_descriptor(ostream & buf){buf << name ;}
void Mem_Addr_Opd::print_ics_opd(ostream & buf){
	buf<<symbol_entry->get_variable_name();}//<<'_';}
void Register_Addr_Opd::print_ics_opd(ostream & buf){buf<<register_description->get_name();}
template <class T> void Const_Opd<T>::print_ics_opd(ostream & buf){
	//std::cout<<constant; 
	buf<<num;}

void Mem_Addr_Opd::print_asm_opd(ostream & buf){
//	buf<<symbol_entry->get_variable_name();
	//buf<<symbol_entry->get_symbol_scope() <<' '; 
	// cout << symbol_entry->get_symbol_scope() << symbol_entry->get_variable_name() << "\n";
	if (symbol_entry->get_symbol_scope() == global) 
		buf<<symbol_entry->get_variable_name();
	else if (symbol_entry->get_ref_offset() == fp_ref) 
		buf<<symbol_entry->get_start_offset()<<"($fp)";
	else
		buf<<symbol_entry->get_start_offset()<<"($sp)";
}
void Register_Addr_Opd::print_asm_opd(ostream & buf){//$reg	
	buf<<'$'<<register_description->get_name();}
template <class T> void Const_Opd<T>::print_asm_opd(ostream & buf){buf<<num;}



void Move_IC_Stmt::print_icode(ostream & buf){
	// ld result <- opd1
	buf<<ICs<<op_desc.get_name()<<':'<<ICS;
	result->print_ics_opd(buf); buf<<" <- "; opd1->print_ics_opd(buf); buf<< endl;
}

void Compute_IC_Stmt::print_icode(ostream & buf){
	// add:    	t1 <- v0 , t0
	buf<<ICs<<op_desc.get_name()<<':'<<ICS;
	result->print_ics_opd(buf); buf<<" <- "; opd1->print_ics_opd(buf);
	if (opd2 != NULL){ buf << " , "; opd2->print_ics_opd(buf);}
	buf<<endl;
}

void Control_Flow_IC_Stmt::print_icode(ostream & buf){
// 	beq:    	t1 , zero : goto label0
	buf<<ICs; op_desc.print_instruction_descriptor(buf);  buf<<':'<<ICS;
	opd1->print_ics_opd(buf);buf<<" , zero : goto "<<offset<< endl;
}

void Label_IC_Stmt::print_icode(ostream & buf){
	// TODO --> OP DESC CORRECT??
	if (op_desc.get_op() == j) 
			buf<<ICs<<"goto "<<label<<endl ;
	else buf<<endl<<label<<':'<<ICS<<endl;
}

void Print_IC_Stmt::print_icode(ostream & buf){
	buf<<ICs<<"print"<<endl;	
//	std::cout << op_desc.get_name() << "\n";
}


// TODO CHECK
void Move_IC_Stmt::print_assembly(ostream & buf){
	// std::cout<<"Moce\n";
	Ics_Opd* f1,*f2; 
	if (op_desc.get_assembly_format() == a_op_r_o1) {f1 = result ; f2=opd1;}
	else {f2=result;f1=opd1;}
	buf<<AS<<op_desc.get_mnemonic();buf<<" ";
	f1->print_asm_opd(buf);buf<<", ";f2->print_asm_opd(buf);buf<<endl ;
}

void Compute_IC_Stmt::print_assembly(ostream & buf){
	// add $t1, $v0, $t0
		// std::cout<<"ASDasdasd\n";

	buf<<ICs<<op_desc.get_mnemonic()<<' ';
	// if (!(result) ) cout << "NONO\n";
	// else cout << "YEY\n";
	// if (result->as)
	if (result){result->print_asm_opd(buf); buf<<", ";}

	opd1->print_asm_opd(buf);
	if (opd2 != NULL){ buf << ", "; opd2->print_asm_opd(buf); }
	buf<<endl;
}

void Control_Flow_IC_Stmt::print_assembly(ostream & buf){
	// beq $t1, $zero, label0 
	if (opd1){
	// cout<<"CFIC\n";
	buf<<AS<<op_desc.get_mnemonic()<<' ';
	opd1->print_asm_opd(buf);
	buf<<", $zero, "<<offset<<AS<<endl ; }
	else {
		buf<<AS<<op_desc.get_mnemonic()<<" "<<offset<<endl; 
	}
}
void Label_IC_Stmt::print_assembly(ostream & buf){
	if (op_desc.get_op() == j) buf<<AS<<"j "<<label<<endl;
	else buf<<"\n"<<label<<":"<<endl;}

void Print_IC_Stmt::print_assembly(ostream & buf){ 
	buf<<AS<<"syscall"<<endl ; }


void Code_For_Ast::append_ics(Icode_Stmt & ics){ics_list.push_back(&ics);}
list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){return ics_list;}
Register_Descriptor * Code_For_Ast::get_reg(){return result_register;}

Register_Descriptor * Ics_Opd::get_reg(){}

