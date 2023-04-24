# Sysy2022语法分析器

Sysy2020语言是在`2022`年全国大学生计算机系统能力大赛编译系统设计赛定义的源程序语言，
本实验实现该语言的语法检查和分析。在检查语法错误的情形下，报告语法错误，通过语法检查后构建AST，
并输出该AST。

## 目录结构：
```
├── README.md    本说明文档
├── ast   ＃存放输出的AST文件，prog.sy对应的AST为prog.ast.txt 
├── src
│   ├── Makefile   # Makefile
│   ├── README.md  # AST说明文档
│   ├── ast.c      # ast构建及输出的相关函数
│   ├── ast.h      # ast相关头文件
│   ├── cgen.sh    # 由flex/bison生成lexer.c,parser.c,parser.h
│   ├── display.c  # ast输出主函数display(ASTNode* T)
│   ├── lexer.l    # 词法规则文件
│   └── parser.y   # 语法规则文件
└── test_cases     # 测试用例（没有语法错误和有语法错误的用例各一组）
    ├── 012_func_defn.sy    # 语法分析测试用例
    ├── 038_continue.sy     # 语法分析测试用例
    ├── 067_sort_test7.sy   # 语法分析测试用例
    ├── 087_gcd.sy          # 语法分析测试用例
    ├── case_1.sy   #  语法检查测试用例   
    ├── case_2.sy   #  语法检查测试用例 
    ├── case_3.sy   #  语法检查测试用例 
    ├── case_4.sy   #  语法检查测试用例 
    ├── case_5.sy   #  语法检查测试用例 
    ├── test1.sy    #  语法检查测试用例 
    └── test2.sy    #  语法分析测试用例
```
## 你的任务：　

完善parser.y的语法规则和语义计算规则，实现语法检查和语法分析。
只要正确写出文法的产生式即可通过语法检查，正确写出语义计算(创建AST节点)规则即可完成语法分析。

具体要完成的语法规则，是SysY语言(2022版)定义中的stmt语法部分：
语句 
```
Stmt → LVal '=' Exp ';' | [Exp] ';' | Block
| 'if' '(' Cond ')' Stmt [ 'else' Stmt ]
| 'while' '(' Cond ')' Stmt
| 'break' ';' | 'continue' ';'
| 'return' [Exp] ';'
```
将上述产生式转换成bison的语法规则(Grammar Rule),并关联语义动作，创建抽象语法树。

将产生式转换BISON的语法规则时，类似下列含有任选项[Exp](Exp可以出现一次或不出现)的产生式：

Stmt → [Exp] ';'

应将其改为两个产生式：

Stmt → ';'  //Exp不出现

Stmt → Exp ';' //Exp可以出现一次

注意，转换成bison语法规则时，终结符';'用相应的Token代号(SEMICOLON)取代。

### 字段说明
在创建AST时，需要调用new_node()函数，该函数及其用到的数据结构和类型，均定义在ast.h中.

```cpp
typedef struct ASTNode {
    node_type type;       //节点类型，多为产生式左部符号
    struct ASTNode *left; //left指针
    struct ASTNode *mid;  //mid指针
    struct ASTNode *right; //right指针
    int int_val;           //d_type为Int时，记录必要的int值;
                           //语句类别:IfStmt,WhileStmt,BreakStmt等；
                           //算术操作符类别: PLUS,MINUS,MUL,DIV,MOD等；
                           //比较运算符类别: EQ,NE,LT,GT,LE,GE等
                           //逻辑运算符类别: AND, OR, 
    float float_val;       // d_type为float时，记录必要的float值 
    char *symbol;          //记录符号名(id)
    node_type d_type;      //记录类型(int,float,void)，不需要时设为NonType
}ASTNode;
```

// 创建AST子树，由于所有节点共用一个函数，不是每个字段对每类节点都有意义
// 对不需要的字段分别为Null, 0, NonType等即可.
// 1个Child,用right; 两个用left,right； 三个用left,mid,right.
```cpp
ASTNode *new_node(node_type type, 
                 ASTNode *left, 
                 ASTNode *mid, 
                 ASTNode *right, 
                 int int_val, 
                 float float_val, 
                 char *symbol, 
                 node_type d_type);

```

node_type的定义：
···cpp
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
```

### 编译提示：
cd src
make clean
./cgen.sh 
make 
测试提示：
./parser ../testcases/case1.sy
./parser ../testcases/087_gcd.sy
...
