#include<iostream>

Local_Environment::Local_Environment(){}
Local_Environment::~Local_Environment(){}
Eval_Result_Value_Int::Eval_Result_Value_Int()			{this->defined = 0;}
Eval_Result_Value_Double::Eval_Result_Value_Double()	{this->defined = 0;}
Eval_Result_Value_Int::~Eval_Result_Value_Int(){}
Eval_Result_Value_Double::~Eval_Result_Value_Double(){}


int Eval_Result::get_int_value()						{}
double Eval_Result::get_double_value()					{}
int Eval_Result_Value::get_int_value()					{}
double Eval_Result_Value::get_double_value()			{}
int Eval_Result_Value_Int::get_int_value()				{return value;}
double Eval_Result_Value_Double::get_double_value()		{return value;}


void Eval_Result::set_value(int value)					{}
void Eval_Result::set_value(double value)				{}
void Eval_Result_Value::set_value(int number)			{}
void Eval_Result_Value::set_value(double number)		{}
void Eval_Result_Value_Int::set_value(int number)		{value = number; result_type = int_result; defined=1;}
void Eval_Result_Value_Double::set_value(double number)	{value = number; result_type = double_result; defined=1;}
void Eval_Result_Value_Int::set_value(double number)	{value = (int) number; result_type = int_result; defined =1;}
void Eval_Result_Value_Double::set_value(int number)	{value = (double) number; result_type = double_result; defined=1;}

	
bool Eval_Result::is_variable_defined()				{}
bool Eval_Result_Value::is_variable_defined() 		{}
bool Eval_Result_Value_Int::is_variable_defined()	{return defined;}
bool Eval_Result_Value_Double::is_variable_defined(){return defined;}


void Eval_Result::set_variable_status(bool def)				{}
void Eval_Result_Value::set_variable_status(bool def) 		{}
void Eval_Result_Value_Int::set_variable_status(bool def)	{defined=def;}
void Eval_Result_Value_Double::set_variable_status(bool def){defined=def;}


void Eval_Result::set_result_enum(Result_Enum res) {result_type = res;}
void Eval_Result_Value::set_result_enum(Result_Enum res) {result_type = res;}
void Eval_Result_Value_Int::set_result_enum(Result_Enum res){result_type = res;}
void Eval_Result_Value_Double::set_result_enum(Result_Enum res){result_type = res;}


Result_Enum Eval_Result::get_result_enum() 				{return result_type;}
Result_Enum  Eval_Result_Value::get_result_enum() 		{return result_type;}
Result_Enum Eval_Result_Value_Int::get_result_enum()	{return result_type;}
Result_Enum Eval_Result_Value_Double::get_result_enum()	{return result_type;}


////// TODO : PRINT
void Local_Environment::print(ostream & file_buffer){
	map<string, Eval_Result*>::iterator it ; 
	for (it = variable_table.begin();it!=variable_table.end();it++){
		if (it->second->is_variable_defined()){
			if (it->second->get_result_enum() == int_result)
				file_buffer << VAR_SPACE << it->first << " : " << it->second->get_int_value()<<"\n";
			else if (it->second->get_result_enum() == double_result)
				file_buffer << VAR_SPACE << it->first << " : " << it->second->get_double_value()<<"\n";
			}
		else{
			file_buffer << VAR_SPACE << it->first << " : undefined\n";
			
		}
	}
}

bool Local_Environment::is_variable_defined(string name){
	if (does_variable_exist(name)) {
		return variable_table[name]->is_variable_defined(); 
	}	
	return 0 ;
}

Eval_Result * Local_Environment::get_variable_value(string name){

	if (!does_variable_exist(name)) return NULL ;
	return variable_table[name] ;
}
void Local_Environment::put_variable_value(Eval_Result & value, string name){
	variable_table[name] = &value ; 
}
bool Local_Environment::does_variable_exist(string name){
	return variable_table.find(name) != variable_table.end() ;
}

void Symbol_Table::create(Local_Environment& table){
	list <Symbol_Table_Entry*>::iterator it ;
	for (it=variable_table.begin();it!=variable_table.end();it++){
			if((*it)->get_data_type() == int_data_type){
				Eval_Result_Value_Int* res = new Eval_Result_Value_Int;
				res->set_value(0);
 				res->set_variable_status((*it)->get_symbol_scope());
				res->set_result_enum(int_result);
				table.put_variable_value(*res, (*it)->get_variable_name());
			}
		    else if((*it)->get_data_type() == double_data_type){
				Eval_Result_Value_Double* res = new Eval_Result_Value_Double;
				res->set_value(0.0);
				res->set_variable_status((*it)->get_symbol_scope());
				res->set_result_enum(double_result);
				table.put_variable_value(*res, (*it)->get_variable_name());
			}
			else {
				std::cout<<"cs316: Error in var\n";
				exit(EXIT_FAILURE); 
			}
	}
} 




