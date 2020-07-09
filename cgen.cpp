
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