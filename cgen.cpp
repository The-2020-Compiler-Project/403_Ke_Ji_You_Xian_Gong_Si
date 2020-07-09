/****************************************************/
/*目标代码生成*/
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"
#include "util.h"

static char buffer[1000];
#define ofpFO 0
#define retFO -1
#define initFO -2

static int globalOffset = 0; //
static int frameoffset = initFO; //
static int numOfParams = 0; //当前函数参数数量
static int isInFunc = FALSE; //显示当前节点的标志，用于计算localOffset
static int mainFuncLoc = 0; //存储主函数的位置

static void cGen(TreeNode* tree); //内部递归代码生成

static int getBlockOffset(TreeNode* list) ;
// {
//     int offset = 0;
//     //等代码完成
//     return offset;
// }



/* 遍历表达式节点并生成代码 */
static void genExp(TreeNode* tree, int lhs)
{
    int loc;
    int varOffset, baseReg;
    int numOfArgs;
    TreeNode* p1, * p2;
    switch (tree->kind.exp) {

    case OpK:
        if (TraceCode) emitComment("-> Op");
        p1 = tree->child[0];
        p2 = tree->child[1];
        cGen(p1);//store指令
        emitRM("ST", ac, frameoffset--, fp, "op: push left");
        cGen(p2);//load指令
        emitRM("LD", ac1, ++frameoffset, fp, "op: load left");

        switch (tree->attr.op) {

        case PLUS:
            emitRO("ADD", ac, ac1, ac, "op +");
            break;
        case MINUS:
            emitRO("SUB", ac, ac1, ac, "op -");
            break;
        case MULTIPLY:
            emitRO("MUL", ac, ac1, ac, "op *");
            break;
        case DIVIDE:
            emitRO("DIV", ac, ac1, ac, "op /");
            break;
        case LESSTHAN:
            emitRO("SUB", ac, ac1, ac, "op <");
            emitRM("JLT", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        case LESSOREQUAL:
            emitRO("SUB", ac, ac1, ac, "op <=");
            emitRM("JLE", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        case GREATERTHAN:
            emitRO("SUB", ac, ac1, ac, "op >");
            emitRM("JGT", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        case GREATEROREQUAL:
            emitRO("SUB", ac, ac1, ac, "op >=");
            emitRM("JGE", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        case EEQUAL:
            emitRO("SUB", ac, ac1, ac, "op ==");
            emitRM("JEQ", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        case UNEQUAL:
            emitRO("SUB", ac, ac1, ac, "op !=");
            emitRM("JNE", ac, 2, pc, "br if true");
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jmp");
            emitRM("LDC", ac, 1, ac, "true case");
            break;
        default:
            emitComment("ERROR: 操作数未知 ");
            break;
        }

        if (TraceCode)  emitComment("<- Op");

        break;

    case ConstK:
        if (TraceCode) emitComment("-> Const");
        /* gen code to load integer constant using LDC */
        emitRM("LDC", ac, tree->attr.val, 0, "load const");
        if (TraceCode)  emitComment("<- Const");
        break; /* ConstK */

    case IdK:
    case ArrIdK:
        char* IdName;
        if (tree->kind.exp == IdK)
        {
            IdName = tree->attr.name;
        }
        else
        {
            IdName = tree->attr.arr.name;
        }
        if (TraceCode) {
            sprintf(buffer, "-> Id (%s)", IdName);
            emitComment(buffer);
        }

        loc = st_lookup_top(IdName);
        if (loc >= 0)
            varOffset = initFO - loc;
        else
        {
            varOffset = -(st_lookup(IdName));
        }


        /* 可变偏移量 */
        emitRM("LDC", ac, varOffset, 0, "id: load varOffset");

        if (tree->kind.exp == ArrIdK) {
            /* 节点类型为数组id*/

            if (loc >= 0 && loc < numOfParams) {

                /* 推送地址 */
                emitRO("ADD", ac, fp, ac, "id: load the memory address of base address of array to ac");
                emitRO("LD", ac, 0, ac, "id: load the base address of array to ac");
            }
            else {
                /* 全局&局部变量 */

                /* 地址 */
                if (loc >= 0)
                {
                    /* 在当前函数找 */
                    emitRO("ADD", ac, fp, ac, "id: calculate the address");
                }
                else
                {
                    /* 在全局找 */
                    emitRO("ADD", ac, gp, ac, "id: calculate the address");

                }
            }

            /* 生成将当前偏移量压栈的代码 */
            emitRM("ST", ac, frameoffset--, fp, "id: push base address");

            /* 生成索引表达式代码*/
            p1 = tree->child[1];
            if (p1 != NULL)
            {
                cGen(p1);
                /* 获取可变偏移量的目标代码 */
                emitRM("LD", ac1, ++frameoffset, fp, "id: pop base address");
                emitRO("SUB", ac, ac1, ac, "id: calculate element address with index");
            }
            else
            {
                emitRM("LD", ac1, ++frameoffset, fp, "id: pop base address");
            }

        }
        else {
            /* 非数组id节点 */

            /* 地址代码 */
            if (loc >= 0)
            {
                /* 当前函数 */
                emitRO("ADD", ac, fp, ac, "id: calculate the address found in current frame");
            }
            else
            {
                /* 全局 */
                emitRO("ADD", ac, gp, ac, "id: calculate the address found in global scope");

            }
        }

        if (lhs) {
            emitRM("LDA", ac, 0, ac, "load id address");
        }
        else {
            emitRM("LD", ac, 0, ac, "load id value");
        }

        if (TraceCode)  emitComment("<- Id");

        break; /* IdK, ArrIdK */

    case CallK:
        if (TraceCode) emitComment("-> Call");

        /* 初始化 */
        numOfArgs = 0;

        p1 = tree->child[1];

        /* 对每个参数进行一次循环 */
        while (p1 != NULL) {                                    //*****************************************
            /* 函数表达式生成代码 */         //*****************************************
            if (p1->type == IntegerArray)                       //*****************************************
                genExp(p1, TRUE);                               //*****************************************
            else                                                //*****************************************
                genExp(p1, FALSE);

            emitRM("ST", ac, frameoffset + initFO - (numOfArgs++), fp,
                "call: push argument");

            p1 = p1->sibling;
        }

        if (strcmp(tree->attr.name, "input") == 0) {
            /* 输入函数生成代码 */
            emitRO("IN", ac, 0, 0, "read integer value");
        }
        else if (strcmp(tree->attr.name, "output") == 0) {
            /* 输出函数生成代码 */
            /* 生成要写入的值的代码 */
            emitRM("LD", ac, frameoffset + initFO, fp, "load arg to ac");
            /* 输出这个值 */
            emitRO("OUT", ac, 0, 0, "write ac");
        }
        else {
            /* 储存当前mp的生成代码 */
            emitRM("ST", fp, frameoffset + ofpFO, fp, "call: store current mp");
            /* 压栈新函数 */
            emitRM("LDA", fp, frameoffset, fp, "call: push new frame");
            /* 保存返回 */
            emitRM("LDA", ac, 1, pc, "call: save return in ac");

            /* 跳转到函数入口 */
            loc = -(st_lookup(tree->attr.name));
            emitRM("LD", pc, loc, gp, "call: relative jump to function entry");

            /* 弹栈当前函数 */
            emitRM("LD", fp, ofpFO, fp, "call: pop current frame");
        }

        if (TraceCode)  emitComment("<- Call");

        break; /* CallK */

    default:
        break;
    }
}

/* 遍历语句节点并生成代码 */
static void genStmt(TreeNode* tree) {
    TreeNode* p1, * p2, * p3;
    int savedLoc1, savedLoc2, currentLoc;
    int Loc;
    int offset;
    switch (tree->kind.stmt) {

    case AssignK:
        if (TraceCode) emitComment("-> assign");

        p1 = tree->child[0];
        p2 = tree->child[1];


        genExp(p1, TRUE);
        // 压栈lhs的生成代码
        emitRM("ST", ac, frameoffset--, fp, "assign: push left (address)");


        cGen(p2);

        emitRM("LD", ac1, ++frameoffset, fp, "assign: load left (address)");

        emitRM("ST", ac, 0, ac1, "assign: store value");

        if (TraceCode) emitComment("<- assign");
        break; // assign_k

    case CompK:
        if (TraceCode)emitComment("->compound");

        p1 = tree->child[0];
        p2 = tree->child[1];

        /* 更新一下函数声明的偏移量 */
        offset = getBlockOffset(p1);
        frameoffset -= offset;

        /* 压栈 */
		if (tree->isInFuncCom)
		{
			sc_push(tree->attr.scope);
		}


        /*  本体的代码生成 */
        cGen(p2);

        /* 弹栈 */
		if (tree->isInFuncCom)
		{
			sc_pop();
		}

        frameoffset -= offset;//恢复当前偏移量

        if (TraceCode)emitComment("<-compound");

        break;

    case IfK:
        if (TraceCode)emitComment("->if");

        p1 = tree->child[0];
        p2 = tree->child[1];
        p3 = tree->child[2];

        /* 测试表达式生成代码 */
        cGen(p1);

        savedLoc1 = emitSkip(1);
        emitComment("if:jump to else belongs here");
        //部分递归
        cGen(p2);

        savedLoc2 = emitSkip(1);
        emitComment("if:jump to end belongs here");

        currentLoc = emitSkip(0);
        emitBackup(savedLoc1);
        emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
        emitRestore();

        //其他部分递归
        cGen(p3);
        currentLoc = emitSkip(0);
        emitBackup(savedLoc2);
        emitRM_Abs("LDA", pc, currentLoc, "jmp to end");
        emitRestore();
        if (TraceCode)  emitComment("<- if");

        break;

    case IterK:
        if (TraceCode) emitComment("-> iter.");

        p1 = tree->child[0];
        p2 = tree->child[1];

        savedLoc1 = emitSkip(0);
        emitComment("while: jump after body comes back here");

        /* 测试表达式生成代码 */
        cGen(p1);

        savedLoc2 = emitSkip(1);
        emitComment("while: jump to end belongs here");

        /* 本体代码生成 */
        cGen(p2);
        emitRM_Abs("LDA", pc, savedLoc1, "while: jmp back to test");

        currentLoc = emitSkip(0);
        emitBackup(savedLoc2);
        emitRM_Abs("JEQ", ac, currentLoc, "while: jmp to end");
        emitRestore();

        if (TraceCode)  emitComment("<- iter.");

        break;

    case RetK:
        if (TraceCode) emitComment("-> return");

        p1 = tree->child[0];

        /* 为表达式生成代码 */
        cGen(p1);
        emitRM("LD", pc, retFO, fp, "return: to caller");

        if (TraceCode) emitComment("<- return");

        break;
    default:
        break;
    }
}

/* 声明节点生成代码 */
static void genDecl(TreeNode* tree)
{
    TreeNode* p1, * p2;
    int loadFuncLoc, jmpLoc, funcBodyLoc, nextDeclLoc;
    int loc;
    int size;

    switch (tree->kind.decl) {
    case FuncK:
        if (TraceCode) {
            sprintf(buffer, "-> Function (%s)", tree->attr.name);
            emitComment(buffer);
        }

        p1 = tree->child[1];
        p2 = tree->child[2];

        isInFunc = TRUE;

        /* 函数入口代码生成 */
        loc = -(st_lookup(tree->attr.name));
        loadFuncLoc = emitSkip(1);
        emitRM("ST", ac1, loc, gp, "func: store the location of func. entry");
        /* -1全局偏移量 */
        --globalOffset;

        /* 无条件跳转至下一个声明 */
        jmpLoc = emitSkip(1);
        emitComment("func: unconditional jump to next declaration belongs here");

        /* 函数被调用时跳过代码生成 */
        funcBodyLoc = emitSkip(0);
        emitComment("func: function body starts here");

        emitBackup(loadFuncLoc);
        emitRM("LDC", ac1, funcBodyLoc, 0, "func: load function location");
        emitRestore();

        /* 存储返回地址 */
        emitRM("ST", ac, retFO, fp, "func: store return address");

        /* 计算参数数量和当前偏移量 */
        frameoffset = initFO;
        numOfParams = 0;
        cGen(p1);

        /* 函数本体生成代码*/
        if (strcmp(tree->attr.name, "main") == 0)
            mainFuncLoc = funcBodyLoc;

        cGen(p2);

        /* 带返回地址 */
        emitRM("LD", pc, retFO, fp, "func: load pc with return address");


        nextDeclLoc = emitSkip(0);
        emitBackup(jmpLoc);
        emitRM_Abs("LDA", pc, nextDeclLoc, "func: unconditional jump to next declaration");
        emitRestore();

        isInFunc = FALSE;

        if (TraceCode) {
            sprintf(buffer, "-> Function (%s)", tree->attr.name);
			sprintf(buffer, " frameoffset (%d) ", frameoffset);
            emitComment(buffer);
        }

        break;

    case VarK:
    case ArrVarK:
        if (TraceCode) emitComment("-> var. decl.");

        if (tree->kind.decl == ArrVarK)
            size = tree->attr.arr.size;
        else
            size = 1;

        if (isInFunc == TRUE)
            frameoffset -= size;
        else
            globalOffset -= size;

        if (TraceCode) emitComment("<- var. decl.");
        break;

    default:
        break;
    }
}


static void genParam(TreeNode* tree)
{
    switch (tree->kind.stmt) {

    case ArrParamK:
        if (TraceCode) emitComment("-> param");
        emitComment(tree->attr.arr.name);

        --frameoffset;
        ++numOfParams;

        if (TraceCode) emitComment("<- param");
        break;
    case NonArrParamK:
        if (TraceCode) emitComment("-> param");
        emitComment(tree->attr.name);

        --frameoffset;
        ++numOfParams;

        if (TraceCode) emitComment("<- param");

        break;

    default:
        break;
    }
}


static void cGen(TreeNode* tree)  //通过递归遍历树生成代码
{
    if (tree != NULL)
    {
        switch (tree->nodeKind) {
        case StmtK:
            genStmt(tree);
            break;
        case ExpK:
            genExp(tree, FALSE);
            break;
        case DeclK:
            genDecl(tree);
            break;
        case ParamK:
            genParam(tree);
            break;
        default:
            break;
        }
        cGen(tree->sibling);
    }
}

static int getBlockOffset(TreeNode* list) {
    int offset = 0;
    if (list == NULL) {
        //offset=0;
    }
    else if (list->nodeKind == DeclK) {
        /*local variable declaration*/
        TreeNode* node = list;
        while (node != NULL) {
            switch (node->kind.decl) {
            case VarK:
                ++offset;
                break;
            case ArrVarK:
                offset += node->attr.arr.size;
                break;
            default:
                break;
            }
            node = node->sibling;
        }
    }
    else if (list->nodeKind == ParamK) {
        /* parameter declaration */
        TreeNode* node = list;
        while (node != NULL) {
            ++offset;
            node = node->sibling;
        }
    }
    return offset;
}

void genMainCall() {
    emitRM("LDC", ac, globalOffset, 0, "init: load globalOffset");
    emitRO("ADD", fp, fp, ac, "init: initialize mp with globalOffset");

    if (TraceCode) emitComment("-> Call");

    /* generate code to store current mp */
    emitRM("ST", fp, ofpFO, fp, "call: store current mp");
    /* generate code to push new frame */
    emitRM("LDA", fp, 0, fp, "call: push new frame");
    /* generate code to save return in ac */
    emitRM("LDA", ac, 1, pc, "call: save return in ac");

    /* generate code for unconditional jump to function entry */
    emitRM("LDC", pc, mainFuncLoc, 0, "call: unconditional jump to main() entry");

    /* generate code to pop current frame */
    emitRM("LD", fp, ofpFO, fp, "call: pop current frame");

    if (TraceCode) emitComment("<- Call");
}

/**********************************************/
/* 欢迎使用代码生成功能 */
/**********************************************/
/* 过程codeGen将代码生成目标代码
    通过遍历语法树，第二个参数是代码文件的文件名，
    用于在代码文件中将文件名打印为注释
 */
void codeGen(TreeNode* syntaxTree, char* codefile)
{
    char* s = (char*)malloc(strlen(codefile) + 7);
    strcpy(s, "File: ");
    strcat(s, codefile);
    emitComment("TINY Compilation to TM Code");
    emitComment(s);
    /* generate standard prelude */
    emitComment("Standard prelude:");
    emitRM("LD", gp, 0, ac, "load gp with maxaddress");
    emitRM("LDA", fp, -MAX_GLOBAL_SIZE, gp, "copy gp to mp");
    emitRM("ST", ac, 0, ac, "clear location 0");
    emitComment("End of standard prelude.");
    /* push global scope */
    sc_push(globalScope);
    /* generate code for TINY program */
    cGen(syntaxTree);
    /* pop global scope */
    sc_pop();
    /* call main() function */
    genMainCall();
    /* finish */
    emitComment("End of execution.");
    emitRO("HALT", 0, 0, 0, "");
}
