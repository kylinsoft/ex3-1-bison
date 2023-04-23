#include "ast.h"
#include "parser.h"  //由bison根据parser.y生成
#define DEBUG 1
#define INTEGER_MINVALUE -2147483647
#define ARRAYCALL 123456789
int LEV=0;
int main_flag = 0;
int call_flag = 0;
int main_call = 0;
int current_offset = 0;
char break_label[30];
char continue_label[30];
char case_temp[30];
char case_label[30];
char array_name[30];
char struct_name[33];
int struct_width = 0;
int struct_flag = 0;
int array_index = 0;
int struct_var_flag = 0;
int rtn, flag = 0;
int rtn2, op;
int return_flag = 0;
struct ASTNode* left;
struct ASTNode* right;
char tokens[200][200]; //用来存储TAC语句信息
const char spState[8][20] = { "FUNCTION", "GOTO", "CALL", "PARAM", "LABEL", "ARG", "RETURN","BLOCK" };

int varlen = 0; //记录当前变量长度
int lineNo = 0; //记录行号
ASTNode *new_node(node_type type, ASTNode *left, ASTNode *mid, ASTNode *right, int int_val, float float_val, char *symbol, node_type d_type) {
    ASTNode *n = (ASTNode *)malloc(sizeof(ASTNode));
    n->type = type;
    n->left = left;
    n->mid = mid;
    n->right = right;
    n->int_val = int_val;
    n->float_val = float_val;
    n->symbol = symbol;
    n->d_type = d_type;
    return n;
}

int targetNum[1024];
int currentNum[1024];
int currentLayer = 0;
int handle_next_display = 1;

int getBranchNum(ASTNode* T)
{
	int count = 0;
	if(T->left)
	{
		count++;
	}
	if(T->right)
	{
		count++;
	}
	if(T->mid)
	{
		count++;
	}
	return count;
}

void printVarType(node_type type)
{
	switch(type)
	{
		case Int:
			printf("int");
			break;
		case Float:
			printf("float");
			break;
		case Void:
			printf("void");
			break;
		default:
			printf("unknown");
			break;
	}
}

void gapProcess()
{
	for(int i=1;i<=currentLayer;i++)
	{
		if(i < currentLayer)
		{
			if(currentNum[i] <= targetNum[i])
			{
				printf("|  ");
			}
			else
			{
				printf("   ");
			}
		}
		else
		{
			if(currentNum[i] < targetNum[i])
			{
				printf("|--");
			}
			else
			{
				printf("`--");
			}
		}
	}
}

void gapManage(ASTNode* T, int reverse)
{
	currentNum[currentLayer]++;
	currentLayer++;
	targetNum[currentLayer] = getBranchNum(T);
	currentNum[currentLayer] = 1;
	if(reverse)
	{
		nextDisplayReverse(T);
	}
	else
	{
		nextDisplay(T);
	}
	currentLayer--;
}

void sameGapManage(ASTNode* T, int reverse)
{
	currentNum[currentLayer]++;
	currentLayer++;
	targetNum[currentLayer] = getBranchNum(T) - 1;
	currentNum[currentLayer] = 1;
	if(T->right)
	{
		display(T->right);
	}
	currentLayer--;
	display(T->left);
}

void nextDisplay(ASTNode* T)
{
	if(T->left)
	{
		display(T->left);
	}
	if(T->mid)
	{
		display(T->mid);
	}
	if(T->right)
	{
		display(T->right);
	}
}

void nextDisplayReverse(ASTNode* T)
{
	if(T->right)
	{
		display(T->right);
	}
	if(T->mid)
	{
		display(T->mid);
	}
	if(T->left)
	{
		display(T->left);
	}
}

void print_root(ASTNode* T)
{
	gapProcess();
	printf("CompUnit\n");
	gapManage(T, 1);
}

void print_comp_unit(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	handle_next_display = 0;
	switch(T->right->type)
	{
	case ConstDecl:
		print_const_decl(T->right);
		break;
	case VarDecl:
		print_var_decl(T->right);
		break;
	case FuncDef:
		print_func_def(T->right);
		break;
	}
	if(T->left)
	{
		currentLayer++;
		targetNum[currentLayer] = getBranchNum(T) - 1;
		currentNum[currentLayer] = 1;
		currentLayer--;
		display(T->left);
	}
}

void print_const_decl(ASTNode* T)
{
	gapProcess();
	printf("ConstDecl ");
	printVarType(T->d_type);
	printf("\n");
	gapManage(T, 1);
}

void print_const_def(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	gapProcess();
	printf("ConstDef %s\n", T->symbol);
	if(T->left)
	{
		sameGapManage(T, 1);
	}
	else
	{
		gapManage(T, 1);
	}
}

void print_const_exp_array(ASTNode* T)
{
	gapProcess();
	printf("ConstExpArray\n");
	gapManage(T, 1);
}

void print_const_init_val(ASTNode* T)
{
	if(T->right && T->right->type == ConstExp)
	{
		handle_next_display = 0;
		print_const_exp(T->right);
		return;
	}
	gapProcess();
	printf("ConstInitVal {}\n");
	gapManage(T, 1);
}

void print_const_exp(ASTNode* T)
{
	if(T->int_val == 0)
	{
		handle_next_display = 0;
		print_mul_exp(T->right);
		return;
	}
	gapProcess();
	printf("ConstExp ");
	if(T->d_type == PLUS)
	{
		printf("+\n");
	}
	else
	{
		printf("-\n");
	}
	gapManage(T, 1);
}

void print_var_decl(ASTNode* T)
{
	gapProcess();
	printf("VarDecl ");
	printVarType(T->d_type);
	printf("\n");
	gapManage(T, 1);
}

void print_var_def(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	gapProcess();
	printf("VarDef %s\n", T->symbol);
	if(T->left)
	{
		sameGapManage(T, 1);
	}
	else
	{
		gapManage(T, 1);
	}
}

void print_init_val(ASTNode* T)
{
	if(T->int_val == Exp)
	{
		handle_next_display = 0;
		print_exp(T->right);
	}
	else
	{
		if(T->right)
		{
			handle_next_display = 0;
			print_init_vals(T->right);
		}
		else
		{
			gapProcess();
			printf("Null Init Vals\n");
			gapManage(T, 1);
		}
	}
}

void print_init_vals(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	gapProcess();
	printf("InitVals\n");
	if(T->left)
	{
		sameGapManage(T, 1);
	}
	else
	{
		gapManage(T, 1);
	}
}

void print_func_def(ASTNode* T)
{
	gapProcess();
	printf("FuncDef ");
	printVarType(T->d_type);
	printf(" %s\n", T->symbol);
	gapManage(T, 0);
}

void print_func_f_param(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	gapProcess();
	printf("FuncFParams ");
	printVarType(T->d_type);
	printf(" %s\n", T->symbol);
	if(T->left)
	{
		sameGapManage(T, 1);
	}
	else
	{
		gapManage(T, 1);
	}
}

void print_block(ASTNode* T)
{
	gapProcess();
	printf("Block\n");
	gapManage(T, 1);
}

void print_block_item(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	handle_next_display = 0;
	switch(T->right->type)
	{
	case ConstDecl:
		print_const_decl(T->right);
		break;
	case VarDecl:
		print_var_decl(T->right);
		break;
	case Stmt:
		print_stmt(T->right);
		break;
	}
	if(T->left)
	{
		currentLayer++;
		targetNum[currentLayer] = getBranchNum(T) - 1;
		currentNum[currentLayer] = 1;
		currentLayer--;
		display(T->left);
	}
}

void print_stmt(ASTNode* T)
{
	switch (T->int_val)
	{
	case BlankStmt:
		gapProcess();
		printf("Blank Statement\n");
		gapManage(T, 0);
		break;
	case ExpStmt:
		handle_next_display = 0;
		print_exp(T->right);
		break;
	case AssignStmt:
		gapProcess();
		printf("Assign Statement\n");
		gapManage(T, 0);
		break;
	case Block:
		handle_next_display = 0;
		print_block(T->right);
		break;
	case IfStmt:
		gapProcess();
		printf("If Statement\n");
		gapManage(T, 0);
		break;
	case IfElseStmt:
		gapProcess();
		printf("If-Else Statement\n");
		gapManage(T, 0);
		break;
	case WhileStmt:
		gapProcess();
		printf("While Statement\n");
		gapManage(T, 0);
		break;
	case BreakStmt:
		gapProcess();
		printf("Break Statement\n");
		gapManage(T, 0);
		break;
	case ContinueStmt:
		gapProcess();
		printf("Continue Statement\n");
		gapManage(T, 0);
		break;
	case BlankReturnStmt:
		gapProcess();
		printf("Blank Return Statement\n");
		gapManage(T, 0);
		break;
	case ReturnStmt:
		gapProcess();
		printf("Return Statement\n");
		gapManage(T, 0);
		break;
	default:
		gapProcess();
		printf("Unknown Statement\n");
		gapManage(T, 0);
		break;
	}
}

void print_exp(ASTNode* T)
{
	handle_next_display = 0;
	print_add_exp(T->right);
}

void print_add_exp(ASTNode* T)
{
	if(T->int_val == MUL)
	{
		handle_next_display = 0;
		print_mul_exp(T->right);
		return;
	}
	gapProcess();
	printf("AddExp");
	if(T->int_val == PLUS)
	{
		printf(" +\n");
	}
	else
	{
		printf(" -\n");
	}
	gapManage(T, 1);
}

void print_mul_exp(ASTNode* T)
{
	if(T->int_val == UnaryExp)
	{
		handle_next_display = 0;
		print_unary_exp(T->right);
		return;
	}
	gapProcess();
	printf("MulExp");
	if(T->int_val == MUL)
	{
		printf(" *\n");
	}
	else if(T->int_val == DIV)
	{
		printf(" /\n");
	}
	else if(T->int_val == MOD)
	{
		printf(" %%\n");
	}
	gapManage(T, 1);
}

void print_unary_exp(ASTNode* T)
{
	if(T->int_val == PrimaryExp)
	{
		handle_next_display = 0;
		print_primary_exp(T->right);
		return;
	}
	gapProcess();
	printf("UnaryExp ");
	switch (T->int_val)
	{
	case FuncRParams:
		printf("%s()\n", T->symbol);
		break;
	case Plus:
		printf("+\n");
		break;
	case Minus:
		printf("-\n");
		break;
	case NOT:
		printf("NOT\n");
		break;
	default:
		break;
	}
	gapManage(T, 1);
}

void print_func_r_params(ASTNode* T)
{
	if(T->left)
	{
		currentNum[currentLayer]--;
	}
	gapProcess();
	printf("FuncRParams\n");
	if(T->left)
	{
		sameGapManage(T, 1);
	}
	else
	{
		gapManage(T, 1);
	}
}

void print_primary_exp(ASTNode* T)
{
	if(T->d_type == NonType)
	{
		handle_next_display = 0;
		if(T->int_val == Exp)
		{
			print_exp(T->right);
		}
		else
		{
			print_lv_al(T->right);
		}
		return;
	}
	gapProcess();
	printf("PrimaryExp ");
	if(T->d_type == Int)
	{
		printf("IntLiteral %d\n", T->int_val);
	}
	else
	{
		printf("FloatLiteral %f\n", T->float_val);
	}
	gapManage(T, 1);
}

void print_lv_al(ASTNode* T)
{
	gapProcess();
	//printVarType(T->d_type);
	printf("LVal %s\n", T->symbol);
	gapManage(T, 1);
}

void print_cond(ASTNode* T)
{
	if(T->int_val == 0 && T->right->type == Cond)
	{
		handle_next_display = 0;
		print_cond(T->right);
		return;
	}
	gapProcess();
	printf("Cond");
	if(T->int_val == OR)
	{
		printf(" OR\n");
	}
	else
	{
		printf("\n");
	}
	gapManage(T, 1);
}

void print_l_and_exp(ASTNode* T)
{
	if(T->int_val == 0)
	{
		handle_next_display = 0;
		print_eq_exp(T->right);
		return;
	}
	gapProcess();
	printf("LAndExp AND\n");
	gapManage(T, 1);
}

void print_eq_exp(ASTNode* T)
{
	if(T->int_val == 0)
	{
		handle_next_display = 0;
		print_rel_exp(T->right);
		return;
	}
	gapProcess();
	printf("EqExp");
	if(T->int_val == EQ)
	{
		printf(" ==\n");
	}
	else if(T->int_val == NE)
	{
		printf(" !=\n");
	}
	else
	{
		printf("\n");
	}
	gapManage(T, 1);
}

void print_rel_exp(ASTNode* T)
{
	if(T->int_val == 0)
	{
		handle_next_display = 0;
		print_add_exp(T->right);
		return;
	}
	gapProcess();
	printf("RelExp ");
	if(T->int_val == LT)
	{
		printf("<\n");
	}
	else if(T->int_val == GT)
	{
		printf(">\n");
	}
	else if(T->int_val == LE)
	{
		printf("<=\n");
	}
	else
	{
		printf(">=\n");
	}
	gapManage(T, 1);
}

void print_exp_array(ASTNode* T)
{
	gapProcess();
	printf("ExpArray []\n");
	gapManage(T, 1);
}

void print_unknown(ASTNode* T)
{
	currentNum[currentLayer]++;
	gapProcess();
	printf("Unknown\n");
}
