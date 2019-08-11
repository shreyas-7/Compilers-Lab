#include "ast.hh"
#include "local-environment.hh"
#include <stdlib.h>
extern "C" void yyerror (char *);


template class Number_Ast<double>;
template class Number_Ast<int>;

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){}
Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){}
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){}
Eval_Result & Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
        string name = variable_symbol_entry->get_variable_name();
        file_buffer<<AST_SPACE<<name<<" : ";
        if (variable_symbol_entry->get_data_type() == int_data_type){
            file_buffer<<eval_env.get_variable_value(name)->get_int_value();
        }
        else if (variable_symbol_entry->get_data_type() == double_data_type){
            file_buffer<<eval_env.get_variable_value(name)->get_double_value();
        }
        file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
        return *(eval_env.get_variable_value(variable_symbol_entry->get_variable_name()));
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
        eval_env.put_variable_value(result, variable_symbol_entry->get_variable_name());
}

template <class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(node_data_type == int_data_type){
                Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
                env->set_value(constant);
                return *env;
        }
        else if(node_data_type == double_data_type){
                Eval_Result_Value_Double* env = new Eval_Result_Value_Double;
                env->set_value(constant);
                return *env;
        }
}


Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

    print(file_buffer) ;
    string name = lhs->get_symbol_entry().get_variable_name();
    
    
    if (lhs->get_data_type() == int_data_type){
	    Eval_Result* right = & (rhs->evaluate(eval_env, file_buffer));
	    if(lhs->get_symbol_entry().get_symbol_scope() == local){
	        eval_env.put_variable_value(*right, name);
	    }
	    else if(lhs->get_symbol_entry().get_symbol_scope() == global)
	        interpreter_global_table.put_variable_value(*right, name);
        file_buffer<<"\n"<<AST_SPACE<<name<<" : "<<right->get_int_value()<<"\n\n"; 
	}	
    else {
	    Eval_Result* right = & (rhs->evaluate(eval_env, file_buffer));
		if(lhs->get_symbol_entry().get_symbol_scope() == local){
		    eval_env.put_variable_value(*right, name);
        }
		else if(lhs->get_symbol_entry().get_symbol_scope() == global)
		    interpreter_global_table.put_variable_value(*right, name);
        file_buffer<<"\n"<<AST_SPACE<<name<<" : "<<right->get_double_value()<<"\n\n"; 
    }


    // if (lhs->get_variable_value().get_resuls)
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(get_symbol_entry().get_symbol_scope() != global){
        	string name = get_symbol_entry().get_variable_name();
        	if(!eval_env.get_variable_value(name)->is_variable_defined()){
        		std::cerr << "cs316: Error variable used before definition\n";
        		exit(EXIT_FAILURE);
        	}
        	else
	        	return *(eval_env.get_variable_value(name));
        }
        else{
        	return *(interpreter_global_table.get_variable_value(get_symbol_entry().get_variable_name()));
        }
        
}


Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(node_data_type == int_data_type){
                Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_int_value() + rhs->evaluate(eval_env, file_buffer).get_int_value());
                return *env;
        }
        else{
                Eval_Result_Value_Double* env = new Eval_Result_Value_Double;
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_double_value() + rhs->evaluate(eval_env, file_buffer).get_double_value());
                return *env;
        }
}

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(node_data_type == int_data_type){
                Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_int_value() - rhs->evaluate(eval_env, file_buffer).get_int_value());
                return *env;
        }
        else{
                Eval_Result_Value_Double* env = new Eval_Result_Value_Double;
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_double_value() - rhs->evaluate(eval_env, file_buffer).get_double_value());
                return *env;
        }
}

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(node_data_type == int_data_type){
                Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
                if(rhs->evaluate(eval_env, file_buffer).get_int_value() == 0 ){
                    std::cerr<<"cs316: Error division by zero\n";
                    exit(EXIT_FAILURE);
                }
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_int_value() / rhs->evaluate(eval_env, file_buffer).get_int_value());
                return *env;
        }
        else{
                Eval_Result_Value_Double* env = new Eval_Result_Value_Double;
                if(rhs->evaluate(eval_env, file_buffer).get_double_value() == 0 ){
                    std::cerr<<"cs316: Error division by zero\n";
                    exit(EXIT_FAILURE);
                }
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_double_value() / rhs->evaluate(eval_env, file_buffer).get_double_value());
                return *env;
        }
}

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(node_data_type == int_data_type){
                Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_int_value() * rhs->evaluate(eval_env, file_buffer).get_int_value());
                return *env;
        }
        else{
                Eval_Result_Value_Double* env = new Eval_Result_Value_Double;
                env->set_value(lhs->evaluate(eval_env, file_buffer).get_double_value() * rhs->evaluate(eval_env, file_buffer).get_double_value());
                return *env;
        }
}

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        if(node_data_type == int_data_type){
                Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
                env->set_value(-lhs->evaluate(eval_env, file_buffer).get_int_value());
                return *env;
        }
        else{
                Eval_Result_Value_Double* env = new Eval_Result_Value_Double;
                env->set_value(-lhs->evaluate(eval_env, file_buffer).get_double_value());
                return *env;
        }
}

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        int result = cond->evaluate(eval_env, file_buffer).get_int_value();
        if(result != 0){
                Eval_Result* env = &(lhs->evaluate(eval_env, file_buffer));
                return *env;
        }
        else{
                Eval_Result* env =&(rhs->evaluate(eval_env, file_buffer));
                return *env;
        }
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    return return_value->evaluate(eval_env,file_buffer);
}

// double value or integer value?
Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
        if(rel_op == less_equalto){
                if (lhs_condition->get_data_type() == double_data_type){
                if(lhs_condition->evaluate(eval_env, file_buffer).get_double_value() <= rhs_condition->evaluate(eval_env, file_buffer).get_double_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}
                else {
               if(lhs_condition->evaluate(eval_env, file_buffer).get_int_value() <= rhs_condition->evaluate(eval_env, file_buffer).get_int_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}                    
                
        }
        else if(rel_op == less_than){
                if (lhs_condition->get_data_type() == double_data_type){
                if(lhs_condition->evaluate(eval_env, file_buffer).get_double_value() < rhs_condition->evaluate(eval_env, file_buffer).get_double_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}
                else {
               if(lhs_condition->evaluate(eval_env, file_buffer).get_int_value() < rhs_condition->evaluate(eval_env, file_buffer).get_int_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}                    
        }
        else if(rel_op == greater_than){
                if (lhs_condition->get_data_type() == double_data_type){
                    if(lhs_condition->evaluate(eval_env, file_buffer).get_double_value() > rhs_condition->evaluate(eval_env, file_buffer).get_double_value()){
                        env->set_value(1);
                    }
                    else
                        env->set_value(0);
                }
                else {
                    if(lhs_condition->evaluate(eval_env, file_buffer).get_int_value() > rhs_condition->evaluate(eval_env, file_buffer).get_int_value()){
                       env->set_value(1);
                    }
                    else
                        env->set_value(0);
                    }                    
        }
        else if(rel_op == greater_equalto){
                if (lhs_condition->get_data_type() == double_data_type){
                if(lhs_condition->evaluate(eval_env, file_buffer).get_double_value() >= rhs_condition->evaluate(eval_env, file_buffer).get_double_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}
                else {
               if(lhs_condition->evaluate(eval_env, file_buffer).get_int_value() >= rhs_condition->evaluate(eval_env, file_buffer).get_int_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}                    
        }
        else if(rel_op == equalto){
                if (lhs_condition->get_data_type() == double_data_type){
                if(lhs_condition->evaluate(eval_env, file_buffer).get_double_value() == rhs_condition->evaluate(eval_env, file_buffer).get_double_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}
                else {
               if(lhs_condition->evaluate(eval_env, file_buffer).get_int_value() == rhs_condition->evaluate(eval_env, file_buffer).get_int_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}                    
        }
        else if(rel_op == not_equalto){
                if (lhs_condition->get_data_type() == double_data_type){
                if(lhs_condition->evaluate(eval_env, file_buffer).get_double_value() != rhs_condition->evaluate(eval_env, file_buffer).get_double_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}
                else {
               if(lhs_condition->evaluate(eval_env, file_buffer).get_int_value() != rhs_condition->evaluate(eval_env, file_buffer).get_int_value()){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
}                    
        }
        else    yyerror("ERROR : unknown relational operation");
        return *env;
}

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        Eval_Result_Value_Int* env = new Eval_Result_Value_Int;
        if(bool_op == _logical_not){
                if(rhs_op->evaluate(eval_env, file_buffer).get_int_value() == 0){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
        }
        else if(bool_op == _logical_or){
                if(lhs_op->evaluate(eval_env, file_buffer).get_int_value() > 0 || rhs_op->evaluate(eval_env, file_buffer).get_int_value() > 0){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
        }
        else if(bool_op == _logical_and){
                if(lhs_op->evaluate(eval_env, file_buffer).get_int_value() != 0 && rhs_op->evaluate(eval_env, file_buffer).get_int_value() != 0){
                       env->set_value(1);
                }
                else
                        env->set_value(0);
        }
        else
                yyerror("ERROR : unkown logical operation");
        return *env;
}


Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        Eval_Result* env = new Eval_Result_Value_Int;

        if(cond->evaluate(eval_env, file_buffer).get_int_value() != 0){
                env = &(then_part->evaluate(eval_env, file_buffer));
        }     
        else if(else_part != NULL){
                env = &(else_part->evaluate(eval_env, file_buffer));
        }
        return *env;
}

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        Eval_Result* env = new Eval_Result_Value_Int;
        if(is_do_form){
            env = &(body->evaluate(eval_env, file_buffer));
        }
        while(cond->evaluate(eval_env, file_buffer).get_int_value() != 0){
            env = &(body->evaluate(eval_env, file_buffer));
        }
        return *env;
}

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
        list<Ast *>::iterator it;
        for (it = statement_list.begin(); it != statement_list.end(); it++) {
            (*(it))->evaluate(eval_env, file_buffer);
        }
}

//TODO
Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}
