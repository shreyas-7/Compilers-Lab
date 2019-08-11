%{
#include <stdio.h>
extern "C" void yyerror (char *);
extern int yylex(void);
#include <iostream>
#include <map>
extern int yylineno ; 
extern int yyparse() ; 
Symbol_Table * current_procedure;
string curproc_name ; 
Symbol_Table * global_table ; 
Symbol_Table * curr_formal_table ;
Procedure * cp;
%}




%union 
{
// #line 50 "parser.y" /* yacc.c:1909  */
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Call_Ast* c_ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
	Sequence_Ast * seq ;
	list <Symbol_Table_Entry *> * symbol_list;
	Data_Type data_type ;
	Relational_Op rop ; 
	Logical_Op lop;
	map<std::string, Procedure*> * map_proc;

// #line 79 "parser.tab.h" /* yacc.c:1909  */
};


%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token <string_value> RETURN
%token INTEGER FLOAT ASSIGN VOID
%token <rop> L_OP LE_OP G_OP GE_OP E_OP NE_OP
%token <lop> AND_OP OR_OP NOT_OP 
%token IF ELSE WHILE DO PRINT

%right '?'
%left OR_OP
%left AND_OP
%left NOT_OP
%left L_OP LE_OP G_OP GE_OP E_OP NE_OP
%left '+' '-'
%left '*' '/'
%right UMI

%nonassoc IFX
%nonassoc ELSE

%type <procedure>       procedure_definition function_decl
%type <symbol_table> program
%type <symbol_table> optional_variable_declaration_list_local
%type <symbol_table> variable_declaration_list_global
%type < symbol_list> variable_list
%type <data_type> data_type
%type <ast_list> args
%type <symbol_table> variable_declaration_list
%type <symbol_table> declaration 
//%type <procedure> procedure_definition
%type <ast_list> optional_statement_list
%type <ast> call
%type <ast> statement
%type <ast> assignment_statement
%type <ast> expr_level1
%type <ast> expr_level2
%type <ast> expr_level3
%type <ast> expr_level4
%type <ast> constant
%%

prog : {
	global_table = new Symbol_Table();
} program

program	:	declaration 
			{
				global_table->append_list(*($<symbol_table>1),yylineno) ;
			}
			program
			| function_decl program
			| 
			procedure_definitions
			{
				if(program_object.get_procedure_prototype("main") == NULL){
					cout<<"cs316: Error : Main not defined\n" ;
					 exit(EXIT_FAILURE);
				}
				program_object.set_global_table(*global_table) ;
			}

procedure_definitions : procedure_definition procedure_definitions | procedure_definition

procedure_definition	: data_type NAME 
						'(' optional_args_def ')' 
						'{'
						{
							Procedure * p;
							
							if ($2->length()==5 && $2->find("main") != std::string::npos) {
								p = new Procedure($1, "main", yylineno); 
							}
							else p = new Procedure($1, *($2), yylineno) ; 
							p->set_formal_param_list(*($<symbol_table>4)) ;
							if(program_object.is_procedure_exists(*($2))){
								if(program_object.get_procedure_prototype(*$2)->get_return_type() != p->get_return_type()){
									cout<<"cs316: Error : Function declaration and definition don't match\n" ;
									exit(EXIT_FAILURE);
								}
								if(p->is_proc_defined()){
									cout<<"cs316: Error : Function already defined\n" ;
									 exit(EXIT_FAILURE);
								}
							}
							
							p->set_proc_is_defined();
    	           			program_object.set_proc_to_map(p->get_proc_name(),p) ;
    	           			curproc_name = p->get_proc_name() ;
							curr_formal_table = &p->get_formal_param_list() ;
							cp = p;
    	           			$<procedure>$ = p ; 
						}
	
						optional_variable_declaration_list_local
						{       	           		
							current_procedure = $8;
    	           			$<procedure>7->set_local_list(*($8));
	           			}
                    	optional_statement_list
    	           		'}'
	           			{
    	           			$<procedure>7->set_ast_list(*($<ast_list>10));
    	           			list<Ast*>::iterator i = ($<ast_list>10)->begin(); 	
    	           		}
    	           		;

variable_declaration_list_global	:	/* empty */
												variable_declaration_list
											{
												$1->set_table_scope(global) ;
												$$ = $1 ;
											}
											;


function_decl			: data_type NAME
						'(' optional_args_def ')' ';'
						{       	           	
							Procedure * p;
							if ($2->length()==5 && $2->find("main") != std::string::npos) {
								p = new Procedure($1, "main", yylineno); 
							}

							else p = new Procedure($1, *($2), yylineno) ; 
														// cout << "Proc\n";
// 
    	           			program_object.set_proc_to_map(p->get_proc_name(),p) ;
							p->set_formal_param_list(*($<symbol_table>4)) ;
							$<procedure>$ = p ; 
							
						}
						|
						 data_type NAME  
						'(' decl_args ')' ';'
						{       	           	
							Procedure * p;
							if ($2->length()==5 && $2->find("main") != std::string::npos) {
								p = new Procedure($1, "main", yylineno); 
							}

							else p = new Procedure($1, *($2), yylineno) ; 

    	           			program_object.set_proc_to_map(p->get_proc_name(),p) ;
							p->set_formal_param_list(*($<symbol_table>4)) ;
							$<procedure>$ = p ; 
							
						}
						;


decl_args 				:	decl_arg
						{
                            $<symbol_table>$ = new Symbol_Table();
                            $<symbol_table>$->set_table_scope(formal);
                            $<symbol_table>$->push_symbol($<symbol_entry>1) ;
						}						
						|	decl_args 
						',' decl_arg
						{
							$<symbol_table>1->push_symbol($<symbol_entry>3);
							$<symbol_table>$ = $<symbol_table>1;
						}

						;

decl_arg 				:
						data_type 
						{
							std::string a = "param" ;
							Symbol_Table_Entry* se = new Symbol_Table_Entry(a,$1,yylineno) ;
							$<symbol_entry>$ = se ; 

						}


optional_args_def 		:
                        { 
                            $<symbol_table>$ = new Symbol_Table();
                            $<symbol_table>$->set_table_scope(formal);
                        } 
                        |	def_args
                        {
                            $<symbol_table>$ = $<symbol_table>1 ; 
                        }
                        ;

def_args 				:	def_arg
						{
                            $<symbol_table>$ = new Symbol_Table();
                            $<symbol_table>$->set_table_scope(formal);
                            $<symbol_table>$->push_symbol($<symbol_entry>1) ;
						}						
						|	def_args 
						',' def_arg
						{
							if ($<symbol_table>1->variable_in_symbol_list_check($<symbol_entry>3->get_variable_name())){
								cout<<"cs316: Error : Variable at line"<<yylineno<<" already presnt in args\n" ;
								 exit(EXIT_FAILURE);
							}
							// cout << $<symbol_entry>1->get_variable_name() << " " << $<symbol_entry>1->get_symbol_scope() << endl ; 
							$<symbol_table>1->push_symbol($<symbol_entry>3);
                            $<symbol_table>1->set_table_scope(formal);
							$<symbol_table>$ = $<symbol_table>1;
						}

						;

def_arg 				:	data_type NAME
						{
							Symbol_Table_Entry * se = new Symbol_Table_Entry(*($2),$1,yylineno) ;
							se->set_symbol_scope(formal);
							// cout << *$2 << " " << $1 << formal << endl ; 
							$<symbol_entry>$ = se ; 
						}





data_type 			:   VOID 
                            { $$ = void_data_type; }
                        |	INTEGER 
                            { $$ = int_data_type; }
                        |   FLOAT 
                            { $$ = double_data_type; }
                        ;

optional_variable_declaration_list_local	:	/* empty */
											{
												Symbol_Table * st = new Symbol_Table() ;
												st->set_table_scope(local);
												$$ = st ;
											}
											|	variable_declaration_list
											{
												$1->set_table_scope(local) ;
												$$=$1;
											}
											;

variable_declaration_list		:	declaration
								{
									$$ = $1 ;
								}
								|	variable_declaration_list declaration
								{
									$1->append_list(*($2), yylineno) ;
									$$ = $1 ;
								}
								;

declaration		:	data_type variable_list ';'
					{

						Symbol_Table * st = new Symbol_Table() ; 

						int length = $2->size() ;
						while(length--) {
							Symbol_Table_Entry * current_entry = $2->front() ;
							$2->pop_front() ; 

							Symbol_Table * s_temp = new Symbol_Table() ; 
							current_entry->set_data_type($1) ;
							s_temp->push_symbol(current_entry) ;
							st->append_list(*s_temp,yylineno);
						}



						$$ = st ; 
					}
				;

variable_list 	:	NAME
					{
						// cout<<*($1)<<"@\n";
						// cout << yylineno << "\n" ;
						Symbol_Table_Entry * current_variable = new Symbol_Table_Entry(*($1), void_data_type, yylineno);
						list <Symbol_Table_Entry *> * st = new list<Symbol_Table_Entry *> () ;
						st->push_back(current_variable) ;
						$$ = st ; 
					}
	            |   variable_list ',' NAME
					{
						Symbol_Table_Entry * current_variable = new Symbol_Table_Entry(*($3), void_data_type, yylineno);
						($1)->push_back(current_variable) ;
						$$ = $1 ; 
					}

				;


optional_statement_list	:
						{
							//cout<<"stat 1\n";
							list <Ast *> *ast_list = new list<Ast *> ();
							//cout<<"stat 2\n";
							$$ = ast_list;
							//cout<<"stat 3\n";
						}
						|	optional_statement_list statement
						{
							//cout<<"stat 4\n";
							($1)->push_back($2);
							//cout<<"stat 5\n";
							$$ = $1;
						}
						;

statement  				: assignment_statement {$<ast>$ = $<ast>1 ;}
						| if_statement		   {$<ast>$ = $<ast>1 ;}
						| while_statement 	   {$<ast>$ = $<ast>1 ;}
						| do_while_statement   {$<ast>$ = $<ast>1 ;}
						| print_statement	   {$<ast>$ = $<ast>1 ;}
						|	'{' seq_statement '}'
						{
							$<ast>$ = $<ast>2 ;
						}
						| call 					{
													$<c_ast>1->set_register(new Register_Descriptor());
													cout<<"Settng reg\n";
													$<ast>$ = $<ast>1;}
						| return_statement		{$<ast>$ = $<ast>1;}
						;

cond_expr 				:   expr_level1 L_OP expr_level1	{$<ast>$ = new Relational_Expr_Ast($<ast>1,less_than,$<ast>3,yylineno);$<ast>$->check_ast();}
						| 	expr_level1 LE_OP expr_level1	{$<ast>$ = new Relational_Expr_Ast($<ast>1,less_equalto,$<ast>3,yylineno);$<ast>$->check_ast();}
						| 	expr_level1 G_OP expr_level1	{$<ast>$ = new Relational_Expr_Ast($<ast>1,greater_than,$<ast>3,yylineno);$<ast>$->check_ast();}
						| 	expr_level1 GE_OP expr_level1	{$<ast>$ = new Relational_Expr_Ast($<ast>1,greater_equalto,$<ast>3,yylineno);$<ast>$->check_ast();}
						| 	expr_level1 NE_OP expr_level1	{$<ast>$ = new Relational_Expr_Ast($<ast>1,not_equalto,$<ast>3,yylineno);$<ast>$->check_ast();}
						| 	expr_level1 E_OP expr_level1	{$<ast>$ = new Relational_Expr_Ast($<ast>1,equalto,$<ast>3,yylineno);$<ast>$->check_ast();}
						|	cond_expr AND_OP cond_expr		{$<ast>$ = new Logical_Expr_Ast   ($<ast>1,_logical_and,$<ast>3,yylineno);$<ast>$->check_ast();}
						|	cond_expr OR_OP cond_expr		{$<ast>$ = new Logical_Expr_Ast   ($<ast>1,_logical_or,$<ast>3,yylineno);$<ast>$->check_ast();}
						|	NOT_OP cond_expr				{$<ast>$ = new Logical_Expr_Ast   (NULL,_logical_not,$<ast>2,yylineno); $<ast>$->check_ast();}
						| 	'(' cond_expr ')'				{$<ast>$ = $<ast>2;}
						;

if_statement			: IF '(' cond_expr ')' body %prec IFX {$<ast>$ = new Selection_Statement_Ast($<ast>3, $<ast>5, NULL   , yylineno);$<ast>$->check_ast();}
						| IF '(' cond_expr ')' body ELSE body {$<ast>$ = new Selection_Statement_Ast($<ast>3, $<ast>5, $<ast>7, yylineno);$<ast>$->check_ast();}
						;

body					: statement 		{$<ast>$ = $<ast>1;}
						| '{' seq_statement '}'		{$<ast>$ = $<ast>2;}
						;

seq_statement 			: // empty 
						{
							$<seq>$ = new Sequence_Ast(yylineno) ;
						}
						| seq_statement statement
						{
							$<seq>1->ast_push_back($<ast>2);
							$<seq>$ = $<seq>1;
						}
						;

return_statement 		:
						RETURN
						expr_level1 ';'
						{
							// cout << curproc_name << endl;
							if ( cp->get_return_type() != $2->get_data_type() ){
								cout<<"cs316: Error : Incorrect return type\n" ;
								exit(EXIT_FAILURE) ;
							}
							Return_Ast* return_ast = new Return_Ast($<ast>2,curproc_name,yylineno);
							return_ast->set_data_type(cp->get_return_type());
							$<ast>$ = return_ast;
						}
						|
						RETURN ';'
						{
							if ( cp->get_return_type() != void_data_type ){
								cout<<"cs316: Error : Incorrect return type\n" ;
								exit(EXIT_FAILURE) ;
							}

							Return_Ast* return_ast = new Return_Ast(NULL,curproc_name,yylineno);
							$<ast>$ = return_ast;	
						}
						;

while_statement			: WHILE '(' cond_expr ')' body		{$<ast>$ = new Iteration_Statement_Ast($<ast>3, $<ast>5,yylineno,0);;$<ast>$->check_ast();}
						;

do_while_statement		: DO body WHILE '(' cond_expr ')' ';'{$<ast>$ = new Iteration_Statement_Ast($<ast>5, $<ast>2,yylineno,1);;$<ast>$->check_ast();}
						;

assignment_statement	:	NAME ASSIGN expr_level1 ';'
						{
							Symbol_Table_Entry *ste ;
							if(current_procedure->variable_in_symbol_list_check(*($1))){
								ste = &current_procedure->get_symbol_table_entry(*($1));
							}
							else if(curr_formal_table ->variable_in_symbol_list_check(*($1))){
								ste = &curr_formal_table->get_symbol_table_entry(*($1));
							}
							else if (global_table->variable_in_symbol_list_check(*($1))) {
								ste = &global_table->get_symbol_table_entry(*($1));
							}
							else{
								cout<<"cs316: Error : Variable "<<yylineno<<' '<<*$1<<" not declared\n" ;
								exit(EXIT_FAILURE) ;
							}
							Name_Ast * name_ast = new Name_Ast(*($1), *ste ,yylineno) ;							
							Assignment_Ast * ass_ast = new Assignment_Ast(name_ast,$3,yylineno);
							ass_ast->check_ast();
							$$ = ass_ast;
							// cout<<"Ass done\n";
						}
						| 
						NAME ASSIGN call
						{							
							Symbol_Table_Entry *ste ;
							if(current_procedure->variable_in_symbol_list_check(*($1))){
								ste = &current_procedure->get_symbol_table_entry(*($1));
							}
							else if(curr_formal_table ->variable_in_symbol_list_check(*($1))){
								ste = &curr_formal_table->get_symbol_table_entry(*($1));
							}

							else if (global_table->variable_in_symbol_list_check(*($1))) {
								ste = &global_table->get_symbol_table_entry(*($1));
							}
							else{
								cout<<"cs316: Error : Variable "<<yylineno<<" "<<*$1<<" not declared\n" ;
								exit(EXIT_FAILURE) ;
							}
							Name_Ast * name_ast = new Name_Ast(*($1), *ste ,yylineno) ;							
							Assignment_Ast * ass_ast = new Assignment_Ast(name_ast,$3,yylineno);
							ass_ast->set_data_type($3->get_data_type());
							ass_ast->check_ast();
							$$ = ass_ast;
						}
						;

call 					:   NAME '(' args ')'';'
						{
							Call_Ast * call_ast = new Call_Ast(*($1), yylineno);
							call_ast->set_actual_param_list(*$3);	
							call_ast->check_actual_formal_param(program_object.get_procedure_prototype(*$1)->get_formal_param_list());
							call_ast->set_data_type(program_object.get_procedure_prototype(*$1)->get_return_type());
							$<c_ast>$ = call_ast;
						}
						;	

args 				:	
						{
							$<ast_list>$ = new list<Ast*>();
						}
						|	expr_level1
						{
							$<ast_list>$ = new list<Ast*>();
							$<ast_list>$->push_back($1);
						}
						|	expr_level1 ',' args
						{
							$<ast_list>3->push_front($1);
							$<ast_list>$ = $<ast_list>3;
						}
						;

print_statement			: PRINT NAME ';'
						{
							Symbol_Table_Entry *ste ;
							if(current_procedure->variable_in_symbol_list_check(*($2))){
								ste = &current_procedure->get_symbol_table_entry(*($2));
							}
							else if(curr_formal_table ->variable_in_symbol_list_check(*($2))){
								ste = &curr_formal_table->get_symbol_table_entry(*($2));
							}
							else if (global_table->variable_in_symbol_list_check(*($2))) {
								ste = &global_table->get_symbol_table_entry(*($2));
							}
							else{
								cout<<"cs316: Error : Variable "<<yylineno<<*$2<<" not declared\n" ;
								exit(EXIT_FAILURE) ;
							}
							Name_Ast * name_ast = new Name_Ast(*($2), *ste ,yylineno) ;	
							Print_Ast * print_ast = new Print_Ast(name_ast,yylineno) ;
							//cout<<"Printed\n";
							$<ast>$ = print_ast; 
						}


expr_level1		:		cond_expr '?' expr_level1 ':' expr_level1
						{
							$<ast>$ = new Conditional_Expression_Ast($<ast>1, $<ast>3, $<ast>5, yylineno);
							$<ast>$->check_ast() ;
						}
						| 	expr_level1 '+' expr_level1
						{
							Plus_Ast * plus_ast = new Plus_Ast($1,$3,yylineno);
							// plus_ast->set_data_type($1->get_data_type());
							plus_ast->check_ast();
							$$ = plus_ast;
						}
						|	expr_level1 '-' expr_level1
						{
							Minus_Ast * min_ast = new Minus_Ast($1,$3,yylineno);
							// min_ast->set_data_type($1->get_data_type());
							min_ast->check_ast();
							$$ = min_ast;
						}
						| 	expr_level2
						{
							$$ = $1;
						}
						;

expr_level2		: 	expr_level2 '*' expr_level2
						{
							Mult_Ast * mul_ast = new Mult_Ast($1,$3,yylineno);
							// mul_ast->set_data_type($1->get_data_type());	
							mul_ast->check_ast();
							$$ = mul_ast;
						}
						|	expr_level2 '/' expr_level2
						{
							Divide_Ast * div_ast = new Divide_Ast($1,$3,yylineno);
							// div_ast->set_data_type($1->get_data_type());
							div_ast->check_ast();
							$$ = div_ast;
						}
						| 	expr_level3
						{
							$$ = $1;
						}
						;

expr_level3		: 	'-' expr_level3 	%prec UMI
						{
							UMinus_Ast * minus_ast = new UMinus_Ast($2,NULL,yylineno);
							minus_ast->set_data_type($2->get_data_type());
							// TODO :: CHECK ; 
							// 
							minus_ast->check_ast();

							$$ = minus_ast;
						}
						| 	expr_level4 	%prec '*'
						{
							$$ = $1 ; 
						}
						;

expr_level4		: '(' expr_level1 ')'
						{
							$$ = $2 ; 
						}
						| NAME
						{	
							Symbol_Table_Entry *ste ;
							if(current_procedure->variable_in_symbol_list_check(*$1))
								ste = &current_procedure->get_symbol_table_entry(*$1);
							else if(curr_formal_table ->variable_in_symbol_list_check(*($1))){
								ste = &curr_formal_table->get_symbol_table_entry(*($1));
							}							
							else if (global_table->variable_in_symbol_list_check(*$1)) {
								ste = &global_table-> get_symbol_table_entry(*$1);
							}
							else{
								cout<<"cs316: Error : Variable "<<yylineno<<" "<<*$1<<" not declared\n" ;
								exit(EXIT_FAILURE) ;
							}
							Name_Ast * name_ast = new Name_Ast(*$1, *ste ,yylineno) ;							
							$$ = name_ast ; 
						}
						| constant
 						{
							$$ = $1 ;
						}
						;

constant		:	INTEGER_NUMBER
				{
					Number_Ast <int> *n_ast = new Number_Ast <int> ($1, int_data_type ,yylineno) ;							
					$$ = n_ast;		
				}
				| 	DOUBLE_NUMBER
				{
					Number_Ast <double> *n_ast = new Number_Ast <double> ($1, double_data_type ,yylineno) ;							
					$$ = n_ast;		
				}
				;

