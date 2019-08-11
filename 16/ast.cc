#include "ast.hh"
#include <stdlib.h>
extern "C" void yyerror (char *);

template class Number_Ast<double>;
template class Number_Ast<int>;


int Ast::labelCounter ;

// Constructors
Ast::Ast(){}
Plus_Ast::Plus_Ast              (Ast * l, Ast * r, int line){ lhs = l; rhs=r; lineno=line;ast_num_child = binary_arity; lhs->is_value_zero() ? (rhs->is_value_zero() ? set_data_type(rhs->get_data_type()) : set_data_type(lhs->get_data_type())) : set_data_type(lhs->get_data_type()); }
Minus_Ast::Minus_Ast            (Ast * l, Ast * r, int line){ lhs = l; rhs=r; lineno=line;ast_num_child = binary_arity; lhs->is_value_zero() ? (rhs->is_value_zero() ? set_data_type(rhs->get_data_type()) : set_data_type(lhs->get_data_type())) : set_data_type(lhs->get_data_type()); }
Divide_Ast::Divide_Ast          (Ast * l, Ast * r, int line){ lhs = l; rhs=r; lineno=line;ast_num_child = binary_arity; lhs->is_value_zero() ? (rhs->is_value_zero() ? set_data_type(rhs->get_data_type()) : set_data_type(lhs->get_data_type())) : set_data_type(lhs->get_data_type()); }
Mult_Ast::Mult_Ast              (Ast * l, Ast * r, int line){ lhs = l; rhs=r; lineno=line;ast_num_child = binary_arity; lhs->is_value_zero() ? (rhs->is_value_zero() ? set_data_type(rhs->get_data_type()) : set_data_type(lhs->get_data_type())) : set_data_type(lhs->get_data_type()); }
UMinus_Ast::UMinus_Ast          (Ast * l, Ast * r, int line){ lhs = l; rhs=r; lineno=line;ast_num_child = unary_arity ; lhs->is_value_zero() ? (rhs->is_value_zero() ? set_data_type(rhs->get_data_type()) : set_data_type(lhs->get_data_type())) : set_data_type(lhs->get_data_type()); }
Assignment_Ast::Assignment_Ast  (Ast * l, Ast * r, int line){ lhs = l; rhs=r; lineno=line;ast_num_child = binary_arity; lhs->is_value_zero() ? (rhs->is_value_zero() ? set_data_type(rhs->get_data_type()) : set_data_type(lhs->get_data_type())) : set_data_type(lhs->get_data_type()); }
Name_Ast::Name_Ast(string & n,Symbol_Table_Entry & e, int l){ lineno = l;variable_symbol_entry = &e;  set_data_type(e.get_data_type());}
template <class T> Number_Ast<T>::Number_Ast(T num, Data_Type d, int l)   { lineno =l; set_data_type(d); constant = num ; }
Return_Ast::Return_Ast(Ast * ret_val, string name, int line) { return_value = ret_val; proc_name = name; lineno = line; if(ret_val != NULL) node_data_type = ret_val->get_data_type(); else node_data_type = void_data_type;}
Print_Ast::Print_Ast(Ast *v, int line) {var = v; lineno = line;}
Call_Ast::Call_Ast(string name, int line) {procedure_name = name; lineno = line;}

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* c, Ast* l, Ast* r, int line){cond=c;lhs=l;rhs=r;lineno=line;ast_num_child=ternary_arity;set_data_type(lhs->get_data_type());}
Relational_Expr_Ast::Relational_Expr_Ast(Ast * l, Relational_Op rop, Ast * r, int line){rel_op=rop;lhs_condition=l;lineno=line;rhs_condition=r;lineno=line;
    // Changing here, from last lab.
    // Now need to generate different symbols bc1t and beq
    set_data_type(r->get_data_type());
    ast_num_child=binary_arity;}
Logical_Expr_Ast::Logical_Expr_Ast(Ast * l, Logical_Op bop, Ast * r, int line){bool_op=bop;
        lhs_op=l;
        rhs_op=r;
        lineno=line;
        set_data_type(rhs_op->get_data_type());
        ast_num_child= (bool_op == _logical_not) ? unary_arity:binary_arity;
        }
Selection_Statement_Ast::Selection_Statement_Ast(Ast * c,Ast* t, Ast* e, int line){
        cond=c;
        then_part=t;
        else_part=e;
        lineno=line;
        set_data_type(void_data_type);
        ast_num_child=ternary_arity;
}
Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * c, Ast* b, int line, bool do_form){cond=c;body=b;lineno=line;is_do_form=do_form;set_data_type(void_data_type);ast_num_child=binary_arity;}
Sequence_Ast::Sequence_Ast(int line) {lineno = line;}


// Destructors
Ast::~Ast(){}
// Assignment_Ast::~Assignment_Ast(){}
// Name_Ast::~Name_Ast(){}
// template <class T> Number_Ast<T>::~Number_Ast(){}
// Return_Ast::~Return_Ast(){}
// Conditional_Expression_Ast::~Conditional_Expression_Ast(){}
// Relational_Expr_Ast::~Relational_Expr_Ast(){}
// Logical_Expr_Ast::~Logical_Expr_Ast(){}
// Selection_Statement_Ast::~Selection_Statement_Ast(){}
// Iteration_Statement_Ast::~Iteration_Statement_Ast(){}
// Sequence_Ast::~Sequence_Ast(){}


// Get data type functions 
Data_Type Ast::get_data_type()                               { return node_data_type; }
Data_Type Name_Ast::get_data_type()                          { return Ast::get_data_type(); }
Data_Type Arithmetic_Expr_Ast::get_data_type()               { return Ast::get_data_type(); }
template <class T> Data_Type Number_Ast<T>::get_data_type()  { return Ast::get_data_type(); }
Data_Type Logical_Expr_Ast::get_data_type()                  { return Ast::get_data_type(); }                                        
Data_Type Relational_Expr_Ast::get_data_type()               { return Ast::get_data_type(); }                 
Data_Type Selection_Statement_Ast::get_data_type()           { return Ast::get_data_type(); }           
Data_Type Iteration_Statement_Ast::get_data_type()           { return Ast::get_data_type(); }           
Data_Type Return_Ast::get_data_type()                        { return Ast::get_data_type(); }           
//TODO
Data_Type Call_Ast::get_data_type()                        { return Ast::get_data_type(); }           



// Set data type functions 
void Ast::set_data_type(Data_Type dt)                               {node_data_type = dt; }
void Name_Ast::set_data_type(Data_Type dt)                          {node_data_type = dt; }
void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)               {node_data_type = dt; }
template <class T>  void Number_Ast<T>::set_data_type(Data_Type dt) {node_data_type = dt; }
void Logical_Expr_Ast::set_data_type(Data_Type dt)                  {node_data_type = dt; }
void Relational_Expr_Ast::set_data_type(Data_Type dt)               {node_data_type = dt; }
void Selection_Statement_Ast::set_data_type(Data_Type dt)           {node_data_type = dt; }
void Iteration_Statement_Ast::set_data_type(Data_Type dt)           {node_data_type = dt; }


// is_value_zero
bool Ast::is_value_zero(){return 0;}
template <class T> bool Number_Ast<T>::is_value_zero(){return constant==0;}

//check_ast
bool Ast::check_ast(){return 0;}
bool Assignment_Ast::check_ast(){
        if(rhs->is_value_zero()){
//                rhs->set_data_type(lhs->get_data_type());
                return 1;
        }
        if(lhs->is_value_zero()){
//                lhs->set_data_type(rhs->get_data_type());
                return 1;
        }
        if(lhs->get_data_type() != rhs->get_data_type()) {
                cerr<<"cs316: Error , datatypes don't match\n";
                exit(EXIT_FAILURE);
                return 0 ; 
        } 
        return 1;
}
bool Arithmetic_Expr_Ast::check_ast(){
        if(rhs != NULL)
        if(rhs->is_value_zero()){
//                rhs->set_data_type(lhs->get_data_type());
                return 1;
        }
        if(lhs->is_value_zero()){
//                lhs->set_data_type(rhs->get_data_type());
                return 1;
        }
        if(rhs != NULL)
                if(lhs->get_data_type() != rhs->get_data_type()) {
                        cerr<<"cs316: Error , dataypes don't match\n";
                        exit(EXIT_FAILURE);
                        return 0 ; 
                } 
        return 1;
}

bool Relational_Expr_Ast::check_ast(){
        if(lhs_condition->get_data_type() != rhs_condition->get_data_type()){
                cerr << "cs316: Error , datatypes don't match\n";
                exit(EXIT_FAILURE);
                return 0;
        }
        return 1;
}

bool Logical_Expr_Ast::check_ast(){
    // Commenting this here as always we will need to convert
    // Unlike lab 4
        // if(lhs_op != NULL)
        // if(lhs_op->get_data_type() != rhs_op->get_data_type()){
        //         cerr << "cs316: Error , datatypes don't match\n";
        //         exit(EXIT_FAILURE);
        //         return 0;
        // }

        return 1;
}
bool Selection_Statement_Ast::check_ast(){
        if(else_part != NULL){
                if(cond->check_ast() && then_part->check_ast() && else_part->check_ast())
                        return 1;
                else return 1;
        }
        else{
        if(cond->check_ast() && then_part->check_ast())
                        return 1;
                else return 1;
        }
}

bool Iteration_Statement_Ast::check_ast(){return cond->check_ast() && body->check_ast();}

//get_symbol_table_entry
Symbol_Table_Entry& Ast::get_symbol_entry() {}
Symbol_Table_Entry& Name_Ast::get_symbol_entry(){return *variable_symbol_entry;} ;


// Print functions 
void Plus_Ast::print(ostream & b){
        b<<"\n"<<AST_NODE_SPACE<<"Arith: PLUS";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";        if(rhs != NULL) rhs->print(b); b<<")";
}
void Minus_Ast::print(ostream & b){
        b<<"\n"<<AST_NODE_SPACE<<"Arith: MINUS";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";        if(rhs != NULL) rhs->print(b); b<<")";
}
void Mult_Ast::print(ostream & b){
        b<<"\n"<<AST_NODE_SPACE<<"Arith: MULT";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";        if(rhs != NULL) rhs->print(b); b<<")";
}
void Divide_Ast::print(ostream & b){
        b<<"\n"<<AST_NODE_SPACE<<"Arith: DIV";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";        if(rhs != NULL) rhs->print(b); b<<")";
}
void UMinus_Ast::print(ostream & b){
        b<<"\n"<<AST_NODE_SPACE<<"Arith: UMINUS";
        b<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
}
void Return_Ast::print(ostream & b){
        b<<"\n"<<AST_SPACE<<AST_NODE_SPACE<<"RETURN ";
        return_value->print(b);
}
template <class T> void Number_Ast<T>::print(ostream & b){
        b<<"Num : "<<constant;
}
void Name_Ast::print(ostream & b){
        b<<"Name : "<<(get_symbol_entry()).get_variable_name();
}
void  Assignment_Ast::print(ostream & b){
        b<<"\n"<<AST_SPACE<<"Asgn: ";
        b<<"\n"<<AST_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
        b<<"\n"<<AST_NODE_SPACE<<"RHS (";        if(rhs != NULL) rhs->print(b); b<<")";
}


void Relational_Expr_Ast::print(ostream &b){
        b<<"\n"<<AST_NODE_SPACE<<"Condition: ";
        if(rel_op == less_equalto) b << "LE";
        else if(rel_op == less_than) b << "LT";
        else if(rel_op == greater_than) b << "GT";
        else if(rel_op == greater_equalto) b << "GE";
        else if(rel_op == equalto) b << "EQ";
        else if(rel_op == not_equalto) b << "NE";
        b << "\n"<<AST_SUB_NODE_SPACE<<"LHS ("; if(lhs_condition != NULL) lhs_condition->print(b); b<<")";
        b << "\n"<<AST_SUB_NODE_SPACE<<"RHS ("; if(rhs_condition != NULL) rhs_condition->print(b); b<<")";
}

void Logical_Expr_Ast::print(ostream &b){
        b<<"\n"<<AST_NODE_SPACE<<"Condition: ";
        if(bool_op == _logical_not) b << "NOT";
        else if(bool_op == _logical_and) b << "AND";
        else if(bool_op == _logical_or) b << "OR";
        if(lhs_op != NULL){
        b << "\n"<<AST_SUB_NODE_SPACE<<"LHS ("; if(lhs_op != NULL) lhs_op->print(b); b<<")";
        }
        b << "\n"<<AST_SUB_NODE_SPACE<<"RHS ("; if(rhs_op != NULL) rhs_op->print(b); b<<")";    
}




void Conditional_Expression_Ast::print(ostream &b){
        b<<"\n"<<AST_SPACE<<"Cond:";
        b << "\n"<<AST_NODE_SPACE<<"IF_ELSE"; if(cond != NULL) cond->print(b); b;
        b<<"\n"<<AST_NODE_SPACE<<"LHS (";        if(lhs != NULL) lhs->print(b); b<<")";
        b<<"\n"<<AST_NODE_SPACE<<"RHS (";        if(rhs != NULL) rhs->print(b); b<<")";
}

void Selection_Statement_Ast::print(ostream &b){
        b<<"\n"<<AST_SPACE<<"IF :";
        b << "\n"<<AST_SPACE<<"CONDITION ("; if(cond != NULL) cond->print(b); b<<")";
        b << "\n"<<AST_SPACE<<"THEN ("; if(then_part != NULL) then_part->print(b); b<<")";
        if (else_part != NULL){
                b << "\n"<<AST_SPACE<<"ELSE ("; if(else_part != NULL) else_part->print(b); b<<")";
        }
}

void Iteration_Statement_Ast::print(ostream &b){
        
        if(is_do_form){
                b << "\n" << AST_SPACE << "DO (";       if(body != NULL) body->print(b); b << ")";
                b << "\n" << AST_SPACE << "WHILE CONDITION ("; if(cond != NULL) cond->print(b); b << ")";
        }
        else{
                b << "\n" << AST_SPACE << "WHILE :\n";
                b << AST_SPACE << "CONDITION (";        if(cond!= NULL) cond->print(b); b <<")";
                b << "\n" << AST_SPACE <<"BODY (";      if(body != NULL) body->print(b); b << ")";
        }
}

void Sequence_Ast::print(ostream &b){
        list<Ast *>::iterator it;
        for (it = statement_list.begin(); it != statement_list.end(); it++) {
                b << "\n";
                (*(it))->print(b);
        }
}

void Print_Ast::print(ostream &b){

    b<<"\n"<<AST_SPACE<<"Print : \n"<<AST_SUB_NODE_SPACE<<"(";
	var->print(b); 	
	b<<")" ;

}

void Call_Ast::print(ostream &b){
        b<<"\n"<<AST_SPACE<<"FN CALL: "<<procedure_name<<"(";

        list<Ast *>::iterator it;
        for (it = actual_param_list.begin(); it != actual_param_list.end(); it++) {
            b<<"\n"<<AST_NODE_SPACE;
            (*(it))->print(b);
        }
        b<<")";
}

void Sequence_Ast::ast_push_back(Ast * ast){ statement_list.push_back(ast);}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){}

void Call_Ast::set_register(Register_Descriptor * reg){ return_value_reg = reg; }

//TODO
void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
    for(auto i : param_list){
        actual_param_list.push_back(i);
    }
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
    list<Symbol_Table_Entry *> params = formal_param_list.get_table();
//    cout << params.size() << " "<<actual_param_list.size() << endl ; 
    if (params.size() != actual_param_list.size()) {
        std::cerr<<"cs316: Error 1: Function definitions don't match\n";
        return;
    }
    list<Symbol_Table_Entry *>::iterator param_iter = params.begin();
    list<Ast *>::iterator real_iter = actual_param_list.begin();
    while (param_iter != params.end()) {
         // cout << (*param_iter)->get_data_type() << (*real_iter)->get_data_type() ; 
        if ((*param_iter)->get_data_type() != (*real_iter)->get_data_type()) {
            std::cerr<<"cs316: Error 2: Function definitions don't match\n";
            return;
        }
        param_iter++;
        real_iter++;
    }
}
