/****************************************************/
/*Ŀ���������*/
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
static int numOfParams = 0; //��ǰ������������
static int isInFunc = FALSE; //��ʾ��ǰ�ڵ�ı�־�����ڼ���localOffset
static int mainFuncLoc = 0; //�洢��������λ��

static void cGen(TreeNode* tree); //�ڲ��ݹ��������

static int getBlockOffset(TreeNode* list) {
    int offset = 0;
    //�ȴ������
    return offset;
}



/* ��������ʽ�ڵ㲢���ɴ��� */
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
        cGen(p1);//storeָ��
        emitRM("ST", ac, frameoffset--, fp, "op: push left");
        cGen(p2);//loadָ��
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
            emitComment("ERROR: ������δ֪ ");
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


        /* �ɱ�ƫ���� */
        emitRM("LDC", ac, varOffset, 0, "id: load varOffset");

        if (tree->kind.exp == ArrIdK) {
            /* �ڵ�����Ϊ����id*/

            if (loc >= 0 && loc < numOfParams) {

                /* ���͵�ַ */
                emitRO("ADD", ac, fp, ac, "id: load the memory address of base address of array to ac");
                emitRO("LD", ac, 0, ac, "id: load the base address of array to ac");
            }
            else {
                /* ȫ��&�ֲ����� */

                /* ��ַ */
                if (loc >= 0)
                {
                    /* �ڵ�ǰ������ */
                    emitRO("ADD", ac, fp, ac, "id: calculate the address");
                }
                else
                {
                    /* ��ȫ���� */
                    emitRO("ADD", ac, gp, ac, "id: calculate the address");

                }
            }

            /* ���ɽ���ǰƫ����ѹջ�Ĵ��� */
            emitRM("ST", ac, frameoffset--, fp, "id: push base address");

            /* ������������ʽ����*/
            p1 = tree->child[1];
            if (p1 != NULL)
            {
                cGen(p1);
                /* ��ȡ�ɱ�ƫ������Ŀ����� */
                emitRM("LD", ac1, ++frameoffset, fp, "id: pop base address");
                emitRO("SUB", ac, ac1, ac, "id: calculate element address with index");
            }
            else
            {
                emitRM("LD", ac1, ++frameoffset, fp, "id: pop base address");
            }

        }
        else {
            /* ������id�ڵ� */

            /* ��ַ���� */
            if (loc >= 0)
            {
                /* ��ǰ���� */
                emitRO("ADD", ac, fp, ac, "id: calculate the address found in current frame");
            }
            else
            {
                /* ȫ�� */
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

        /* ��ʼ�� */
        numOfArgs = 0;

        p1 = tree->child[1];

        /* ��ÿ����������һ��ѭ�� */
        while (p1 != NULL) {                                    //*****************************************
            /* ��������ʽ���ɴ��� */         //*****************************************
            if (p1->type == IntegerArray)                       //*****************************************
                genExp(p1, TRUE);                               //*****************************************
            else                                                //*****************************************
                genExp(p1, FALSE);

            emitRM("ST", ac, frameoffset + initFO - (numOfArgs++), fp,
                "call: push argument");

            p1 = p1->sibling;
        }

        if (strcmp(tree->attr.name, "input") == 0) {
            /* ���뺯�����ɴ��� */
            emitRO("IN", ac, 0, 0, "read integer value");
        }
        else if (strcmp(tree->attr.name, "output") == 0) {
            /* ����������ɴ��� */
            /* ����Ҫд���ֵ�Ĵ��� */
            emitRM("LD", ac, frameoffset + initFO, fp, "load arg to ac");
            /* ������ֵ */
            emitRO("OUT", ac, 0, 0, "write ac");
        }
        else {
            /* ���浱ǰmp�����ɴ��� */
            emitRM("ST", fp, frameoffset + ofpFO, fp, "call: store current mp");
            /* ѹջ�º��� */
            emitRM("LDA", fp, frameoffset, fp, "call: push new frame");
            /* ���淵�� */
            emitRM("LDA", ac, 1, pc, "call: save return in ac");

            /* ��ת��������� */
            loc = -(st_lookup(tree->attr.name));
            emitRM("LD", pc, loc, gp, "call: relative jump to function entry");

            /* ��ջ��ǰ���� */
            emitRM("LD", fp, ofpFO, fp, "call: pop current frame");
        }

        if (TraceCode)  emitComment("<- Call");

        break; /* CallK */

    default:
        break;
    }
}

/* �������ڵ㲢���ɴ��� */
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
        // ѹջlhs�����ɴ���
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

        /* ����һ�º���������ƫ���� */
        offset = getBlockOffset(p1);
        frameoffset -= offset;

        /* ѹջ */
		if (tree->isInFuncCom)
		{
			sc_push(tree->attr.scope);
		}


        /*  ����Ĵ������� */
        cGen(p2);

        /* ��ջ */
		if (tree->isInFuncCom)
		{
			sc_pop();
		}

        frameoffset -= offset;//�ָ���ǰƫ����

        if (TraceCode)emitComment("<-compound");

        break;

    case IfK:
        if (TraceCode)emitComment("->if");

        p1 = tree->child[0];
        p2 = tree->child[1];
        p3 = tree->child[2];

        /* ���Ա���ʽ���ɴ��� */
        cGen(p1);

        savedLoc1 = emitSkip(1);
        emitComment("if:jump to else belongs here");
        //���ֵݹ�
        cGen(p2);

        savedLoc2 = emitSkip(1);
        emitComment("if:jump to end belongs here");

        currentLoc = emitSkip(0);
        emitBackup(savedLoc1);
        emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
        emitRestore();

        //�������ֵݹ�
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

        /* ���Ա���ʽ���ɴ��� */
        cGen(p1);

        savedLoc2 = emitSkip(1);
        emitComment("while: jump to end belongs here");

        /* ����������� */
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

        /* Ϊ����ʽ���ɴ��� */
        cGen(p1);
        emitRM("LD", pc, retFO, fp, "return: to caller");

        if (TraceCode) emitComment("<- return");

        break;
    default:
        break;
    }
}

/* �����ڵ����ɴ��� */
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

        /* ������ڴ������� */
        loc = -(st_lookup(tree->attr.name));
        loadFuncLoc = emitSkip(1);
        emitRM("ST", ac1, loc, gp, "func: store the location of func. entry");
        /* -1ȫ��ƫ���� */
        --globalOffset;

        /* ��������ת����һ������ */
        jmpLoc = emitSkip(1);
        emitComment("func: unconditional jump to next declaration belongs here");

        /* ����������ʱ������������ */
        funcBodyLoc = emitSkip(0);
        emitComment("func: function body starts here");

        emitBackup(loadFuncLoc);
        emitRM("LDC", ac1, funcBodyLoc, 0, "func: load function location");
        emitRestore();

        /* �洢���ص�ַ */
        emitRM("ST", ac, retFO, fp, "func: store return address");

        /* ������������͵�ǰƫ���� */
        frameoffset = initFO;
        numOfParams = 0;
        cGen(p1);

        /* �����������ɴ���*/
        if (strcmp(tree->attr.name, "main") == 0)
            mainFuncLoc = funcBodyLoc;

        cGen(p2);

        /* �����ص�ַ */
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


static void cGen(TreeNode* tree)  //ͨ���ݹ���������ɴ���
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