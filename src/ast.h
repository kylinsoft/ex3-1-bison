#ifndef DEF_H
#define DEF_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdarg.h"
#include "parser.h" //由bison根据parser.y生成
#define MAXLENGTH   200
#define DX 3*sizeof(int)          /*活动记录控制信息需要的单元数，这个根据实际系统调整*/
//以下语法树结点类型、三地址结点类型等定义仅供参考，实验时一定要根据自己的理解来定义
extern int LEV;      //层号
#define BLOCK -2147483647

typedef enum node_type {
    CompUnit,
    ConstDecl,
    VarDecl,
    FuncDef,
    ConstDef,
    ConstInitVal,
    VarDef,
    InitVal,
    FuncFParam,
    ExpArray,
    Exp,
    Block,
    BlockItem,
    Stmt,
    LVal,
    PrimaryExp,
    UnaryExp,
    LOrExp,
    FuncRParams,
    MulExp,
    RelExp,
    EqExp,
    LAndExp,
    Cond,
    ConstExp,
    ConstExpArray,
    BlankStmt,   //空语句
    ExpStmt,     // 表达式语句
    AssignStmt,  // 赋值语句
    IfStmt,      // If语句
    IfElseStmt,  // If-Else语句
    WhileStmt,   //  while语句
    BreakStmt,   //  break语句
    ContinueStmt, // continue语句
    BlankReturnStmt, //不带返回值的return语句
    AddExp,
    ReturnStmt,   // 带返回值的return语句
    NonType,
    Float,
    Int,
    InitVals,
    Void,
    Plus,
    Minus,
    Root
} node_type;

// AST节点(最多三个子节点：lef,mid,right,当只有两节点时，置mid为null):
typedef struct ASTNode {
    node_type type;
    struct ASTNode *left;
    struct ASTNode *mid;
    struct ASTNode *right;
    int int_val;   
    float float_val;
    char *symbol;
    node_type d_type;
}ASTNode;

ASTNode *new_node(node_type type, ASTNode *left, ASTNode *mid, ASTNode *right, int int_val, float float_val, char *symbol, node_type d_type);

void display(ASTNode* T);
int getBranchNum(ASTNode* T);
void printVarType(node_type type);
void gapProcess();
void nextDisplay(ASTNode* T);
void nextDisplayReverse(ASTNode* T);

void print_root(ASTNode* T);
void print_comp_unit(ASTNode* T);
void print_const_decl(ASTNode* T);
void print_const_def(ASTNode* T);
void print_const_exp_array(ASTNode* T);
void print_const_init_val(ASTNode* T);
void print_const_exp(ASTNode* T);
void print_var_decl(ASTNode* T);
void print_var_def(ASTNode* T);
void print_init_val(ASTNode* T);
void print_init_vals(ASTNode* T);
void print_func_def(ASTNode* T);
void print_func_f_param(ASTNode* T);
void print_block(ASTNode* T);
void print_block_item(ASTNode* T);
void print_stmt(ASTNode* T);
void print_exp(ASTNode* T);
void print_add_exp(ASTNode* T);
void print_mul_exp(ASTNode* T);
void print_unary_exp(ASTNode* T);
void print_func_r_params(ASTNode* T);
void print_primary_exp(ASTNode* T);
void print_lv_al(ASTNode* T);
void print_cond(ASTNode* T);
void print_l_and_exp(ASTNode* T);
void print_eq_exp(ASTNode* T);
void print_rel_exp(ASTNode* T);
void print_exp_array(ASTNode* T);
void print_unknown(ASTNode* T);

#endif