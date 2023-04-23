#include "ast.h"
extern int targetNum[1024];
extern int currentNum[1024];
extern int currentLayer;
extern int handle_next_display;

void display(ASTNode* T)
{
	handle_next_display = 1;
	switch (T->type)
	{
	case Root:
		print_root(T);
		break;
	case CompUnit:
		print_comp_unit(T);
		break;
	case ConstDecl:
		print_const_decl(T);
		break;
	case ConstDef:
		print_const_def(T);
		break;
	case ConstExpArray:
		print_const_exp_array(T);
		break;
	case ConstInitVal:
		print_const_init_val(T);
		break;
	case ConstExp:
		print_const_exp(T);
		break;
	case VarDecl:
		print_var_decl(T);
		break;
	case VarDef:
		print_var_def(T);
		break;
	case InitVal:
		print_init_val(T);
		break;
	case InitVals:
		print_init_vals(T);
		break;
	case FuncDef:
		print_func_def(T);
		break;
	case FuncFParam:
		print_func_f_param(T);
		break;
	case Block:
		print_block(T);
		break;
	case BlockItem:
		print_block_item(T);
		break;
	case Stmt:
		print_stmt(T);
		break;
   case Exp:
        print_exp(T);
        break;
    case AddExp:
        print_add_exp(T);
        break;
    case MulExp:
        print_mul_exp(T);
        break;
    case UnaryExp:
        print_unary_exp(T);
        break;
    case FuncRParams:
        print_func_r_params(T);
        break;
    case PrimaryExp:
        print_primary_exp(T);
        break;
    case LVal:
        print_lv_al(T);
        break;
	case Cond:
		print_cond(T);
		break;
	case LAndExp:
		print_l_and_exp(T);
		break;
	case EqExp:
		print_eq_exp(T);
		break;
    case RelExp:
        print_rel_exp(T);
        break;
    case ExpArray:
        print_exp_array(T);
        break;
	default:
		handle_next_display = 0;
		print_unknown(T);
		break;
	}
}