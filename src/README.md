// AST节点(最多三个子节点：lef,mid,right,当只有两节点时，置mid为null,只有一个子节点时，赋值给right)
// type是节点类型,比如Root,CompUnit,Stmt等。
// int_val用于具体的Stmt种类，如IfStmt, IfElseStmt,WhileStmt等，
//        还用于区分算术运算符：PLUS,MINUS,MUL,DIV,MOD运算
//        区分比较运算:EQ, NE, LT, GT, LE, GE等
// d_type用于记录类型信息:int,float,void等，在不需时，其值为NonType。
// 当d_type是int型时，int_val在需要时记录int值
// 当d_type是float型时，float_val在需要时记录float的值
// symbol记录id的名字
// 凡不需要的字段，直接设为0, NULL , NonType等即可。

typedef struct ASTNode {
    node_type type;       //节点类型，多为产生式左部符号
    struct ASTNode *left; //left指针
    struct ASTNode *mid;  //mid指针
    struct ASTNode *right; //right指针
    int int_val;           //d_type为Int时，记录需要int值;
                           //语句类别:IfStmt,WhileStmt,BreakStmt等；
                           //算术操作符类别: PLUS,MINUS,MUL,DIV,MOD等；
                           //比较运算符类别: EQ,NE,LT,GT,LE,GE等
                           //逻辑运算符类别: AND, OR, 
    float float_val;       //
    char *symbol;          //记录符号名(id)
    node_type d_type;      //记录类型(int,float,void)
}ASTNode;


AST节点类型：
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
    WithElseStmt,
    BlankStmt,
    ExpStmt,
    AssignStmt,
    IfStmt,
    IfElseStmt,
    WhileStmt,
    BreakStmt,
    ContinueStmt,
    BlankReturnStmt,
    AddExp,
    ReturnStmt,
    NonType,
    Float,
    Int,
    InitVals,
    Void,
    Plus,
    Minus,
    Root
} node_type;


// 创建AST子树，由于所有节点共用一个函数，不是每个字段对每类节点都有意义
// 对不需要的字段分别为Null, 0, NonType等即可.
// 1个Child,用right; 两个用left,right； 三个用left,mid,right.
ASTNode *new_node(node_type type, 
                 ASTNode *left, 
                 ASTNode *mid, 
                 ASTNode *right, 
                 int int_val, 
                 float float_val, 
                 char *symbol, 
                 node_type d_type);
