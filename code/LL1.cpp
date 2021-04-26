#include"LL1.h"
//#include "pch.h"

//主函数
TreeNode* ParseLL1::parseLL1(Scanner sc)
{
	currentP = NULL;
	temp = NULL;
	tempNode = NULL;
	LPARENnum = 0;
	getExpResult = true;
	getExpResult2 = false;
	getExpResult = true;
	getExpResult2 = false;
	StackNodeTop = NULL;
	StackNodeEmpty = true;
	StackTreeTop = NULL;
	StackTreeEmpty = true;
	StackOpTop = NULL;
	StackOpEmpty = true;
	StackNumTop = NULL;
	StackNumEmpty = true;
	ErrorFlag = false;
	TokenListNum = 0;
	NontmlType  stacktopN;

	LexType     stacktopT;

	TreeNode* rootPointer = NULL;
	int predictNum = 0; 
	lineshow = 0;
	ErrorFlag = false;
	CreateLL1Table();
	rootPointer = newNode(ProK);
	PushTree(&(rootPointer->child[2]));
	PushTree(&(rootPointer->child[1]));
	PushTree(&(rootPointer->child[0]));

	Push(1, Program);
	GetToken(&currentToken, sc);

	lineshow = currentToken.Lineshow;
	while (!(StackNodeEmpty) && (currentToken.word.Lex != ENDFILE))
	{
		if (StackNodeTop->flag == 2)
		{
			stacktopT = (*StackNodeTop).var.tmlvar;

			if (stacktopT == currentToken.word.Lex)
			{
				Pop();
				GetToken(&currentToken, sc);
				lineshow = currentToken.Lineshow;
			}
			else
			{
				if ((stacktopT == SEMI ||  stacktopT == OVER || stacktopT == RPAREN || stacktopT == DOT || stacktopT == COLON ||  stacktopT == COMMA ||  stacktopT == RMIDPAREN || stacktopT == UNDERANGE)&& currentToken.word.Lex != SEMI)
				{
					fprintf(all, "第%d行出现语法错误：", lineshow - 1);
					fprintf(all, "%s前缺少%s\n", currentToken.word.Sem.c_str(), toString(stacktopT).c_str());
					Pop();
				}
				else if (stacktopT == IF || stacktopT == THEN || stacktopT == ELSE)
				{
					fprintf(all, "第%d行出现语法错误：", lineshow, "错误的token是: ");
					fprintf(all, "语义信息：%s", currentToken.word.Sem.c_str());
					fprintf(all, " 应为：%s", toString(stacktopT).c_str());
					fprintf(all, "\n");
					Pop();
				}
				else if (stacktopT== END1|| stacktopT == RETURN1)
				{
					fprintf(all, "第%d行出现语法错误：", lineshow, "错误的token是: ");
					fprintf(all, "语义信息：%s", currentToken.word.Sem.c_str());
					fprintf(all, " 应删去");
					fprintf(all, "\n");
					GetToken(&currentToken, sc);
					lineshow = currentToken.Lineshow;
					//Pop();
				}
				else
				{
					fprintf(all, "第%d行出现语法错误：", lineshow, "错误的token是: ");
					fprintf(all, "语义信息：%s", currentToken.word.Sem.c_str());
					fprintf(all, " 应删去");
					fprintf(all, "\n");
					GetToken(&currentToken, sc);
					lineshow = currentToken.Lineshow;
					//PopTree();
				}
			}
		}
		else {
			stacktopN = (*StackNodeTop).var.Ntmlvar;

			predictNum = LL1Table[stacktopN][currentToken.word.Lex];
			char str = '\0';
			Pop();
			predict(predictNum);

		}
	}
	if ((!ErrorFlag) && (currentToken.word.Lex != ENDFILE))
	{
		fprintf(all, "第%d行出现语法错误：", lineshow, "\n");
		fprintf(all, "\n");
	}
	return  rootPointer;
}
/////////////////////////////////////////////////////////////压栈弹栈处理//////////////////////////////////////////////////

//符号栈压栈
void ParseLL1::Push(int i, int j)
{
	StackNode* p;
	p = (StackNode*)malloc(sizeof(StackNode));
	if (i == 1)
		(*p).var.Ntmlvar = (NontmlType)j;
	else if (i == 2)
		(*p).var.tmlvar = (LexType)j;

	p->flag = i;
	p->nextNode = StackNodeTop;
	StackNodeTop = p;
	StackNodeEmpty = false;
}
//符号栈弹栈
void  ParseLL1::Pop()
{
	StackNode* p;
	p = StackNodeTop;
	StackNodeTop = (*StackNodeTop).nextNode;
	free(p);
	if (StackNodeTop == NULL)
		StackNodeEmpty = true;
}
/*语法树栈压栈*/
void  ParseLL1::PushTree(TreeNode** t)
{
	StackTree* p = NULL;
	p = (StackTree*)malloc(sizeof(StackTree));
	p->pointer = t;
	p->nextNode = StackTreeTop;
	StackTreeTop = p;
	StackTreeEmpty = false;
}
/*语法树栈弹栈*/
TreeNode** ParseLL1::PopTree()
{
	StackTree* p = NULL;
	TreeNode** backpointer;
	p = StackTreeTop;
	backpointer = p->pointer;
	StackTreeTop = StackTreeTop->nextNode;
	free(p);
	if (StackTreeTop == NULL)
		StackTreeEmpty = true;

	return backpointer;
}
/*操作符栈压栈*/
void  ParseLL1::PushOp(TreeNode* t)
{
	StackOp* p = NULL;
	p = (StackOp*)malloc(sizeof(StackOp));
	p->pointer = t;
	p->nextNode = StackOpTop;
	StackOpTop = p;
	StackOpEmpty = false;
}
/*操作符栈弹栈*/
TreeNode* ParseLL1::PopOp()
{
	StackOp* p = NULL;
	TreeNode* backpointer;
	p = StackOpTop;
	backpointer = p->pointer;
	StackOpTop = StackOpTop->nextNode;
	free(p);
	if (StackOpTop == NULL)
		StackOpEmpty = true;

	return backpointer;
}
/*操作数压栈实现过程*/
void  ParseLL1::PushNum(TreeNode* t)
{
	StackOp* p = NULL;
	p = (StackOp*)malloc(sizeof(StackOp));
	p->pointer = t;
	p->nextNode = StackNumTop;
	StackNumTop = p;
	StackNumEmpty = false;
}
/*操作数栈弹栈*/
TreeNode* ParseLL1::PopNum()
{
	StackOp* p = NULL;
	TreeNode* backpointer;
	p = StackNumTop;
	backpointer = p->pointer;
	StackNumTop = StackNumTop->nextNode;
	free(p);
	if (StackNumTop == NULL)
		StackNumEmpty = true;

	return backpointer;
}
//////////////////////////////////////////////////////////////建语法树函数///////////////////////////////////////////////
TreeNode* ParseLL1::newNode(NodeKind nodekind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	if (t == NULL)
	{
		ErrorFlag = true;
	}
	else {
		for (int i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = nodekind;
		t->lineno = lineshow;
		t->idnum = 0;
		for (int i = 0; i < 10; i++)
		{
			strcpy_s(t->name[i], "\0");
			t->table[i] = NULL;
		}
	}
	return t;
}
/* 创建语句类型语法树节点函数*/
TreeNode* ParseLL1::newStmtNode(StmtKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	if (t == NULL)
	{
		ErrorFlag = true;
	}
	else {
		for (int i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineshow;
		t->idnum = 0;
		for (int i = 0; i < 10; i++)
		{
			strcpy_s(t->name[i], "\0");
			t->table[i] = NULL;
		}
	}
	return t;
}
/* 创建表达式类型语法树节点函数*/
TreeNode* ParseLL1::newExpNode(ExpKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	if (t == NULL)
	{
		ErrorFlag = true;
	}
	else {
		for (int i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineshow;
		t->attr.ExpAttr.varkind = IdV;
		t->attr.ExpAttr.type = Void;

		for (int i = 0; i < 10; i++)
		{
			strcpy_s(t->name[i], "\0");
			t->table[i] = NULL;
		}
	}
	return t;
}

string ParseLL1::toString(int lextype) {
	switch (lextype) {
	case 0:return "ENDFILE";
	case 1:return "ERRORR";
		/*保留字*/
	case 2:return "PROGRAM";
	case 3:return "PROCEDURE";
	case 4:return "TYPE";
	case 5:return "VAR";
	case 6:return "IF";
	case 7:return "THEN";
	case 8:return "ELSE";
	case 9:return "FI";
	case 10:return "WHILE";
	case 11:return "DO";
	case 12:return "ENDWH";
	case 13:return "BEGIN";
	case 14:return "END";
	case 15:return "READ";
	case 16:return "WRITE";
	case 17:return "ARRAY";
	case 18:return "OF";
	case 19:return "RECORD";
	case 20:return "RETURN";
	case 21:return "INTEGER";
	case 22:return "CHAR";
		/*多字符单词符号*/
	case 23:return "ID";
	case 24:return "INTC";
	case 25:return "CHARC";

		/*特殊符号*/
	case 26:return "ASSIGN";
	case 27:return "EQ";
	case 28:return "LT";
	case 29:return "PLUS";
	case 30:return "MINUS";
	case 31:return "TIMES";
	case 32:return "OVER";
	case 33:return "LPAREN";
	case 34:return "RPAREN";
	case 35:return "DOT";
	case 36:return "COLON";
	case 37:return "SEMI";
	case 38:return "COMMA";
	case 39:return "LMIDPAREN";
	case 40:return "RMIDPAREN";
	case 41:return "UNDERANGE";
	}
}
/////////////////////////////////////////////////////////////////////////////////////改59/
/*LL1分析表*/
void ParseLL1::CreateLL1Table()
{
	int i, j;

	/*初始化LL1表元素*/
	for (i = 0; i < TABLESIZE; i++)
		for (j = 0; j < TABLESIZE; j++)
			LL1Table[i][j] = 0;

	LL1Table[Program][PROGRAM] = 1;
	LL1Table[ProgramHead][PROGRAM] = 2;
	LL1Table[ProgramName][ID] = 3;
	LL1Table[DeclarePart][TYPE] = 4;
	LL1Table[DeclarePart][VAR] = 4;
	LL1Table[DeclarePart][PROCEDURE] = 4;
	LL1Table[DeclarePart][BEGIN] = 4;
	LL1Table[TypeDec][VAR] = 5;
	LL1Table[TypeDec][PROCEDURE] = 5;
	LL1Table[TypeDec][BEGIN] = 5;
	LL1Table[TypeDec][TYPE] = 6;
	LL1Table[TypeDeclaration][TYPE] = 7;
	LL1Table[TypeDecList][ID] = 8;
	LL1Table[TypeDecMore][VAR] = 9;
	LL1Table[TypeDecMore][PROCEDURE] = 9;
	LL1Table[TypeDecMore][BEGIN] = 9;
	LL1Table[TypeDecMore][ID] = 10;
	LL1Table[TypeId][ID] = 11;
	LL1Table[TypeDef][INTEGER] = 12;
	LL1Table[TypeDef][CHAR1] = 12;
	LL1Table[TypeDef][ARRAY] = 13;
	LL1Table[TypeDef][RECORD] = 13;
	LL1Table[TypeDef][ID] = 14;
	LL1Table[BaseType][INTEGER] = 15;
	LL1Table[BaseType][CHAR1] = 16;
	LL1Table[StructureType][ARRAY] = 17;
	LL1Table[StructureType][RECORD] = 18;
	LL1Table[ArrayType][ARRAY] = 19;
	LL1Table[Low][INTC] = 20;
	LL1Table[Top][INTC] = 21;
	LL1Table[RecType][RECORD] = 22;
	LL1Table[FieldDecList][INTEGER] = 23;
	LL1Table[FieldDecList][CHAR1] = 23;
	LL1Table[FieldDecList][ARRAY] = 24;
	LL1Table[FieldDecMore][END1] = 25;
	LL1Table[FieldDecMore][INTEGER] = 26;
	LL1Table[FieldDecMore][CHAR1] = 26;
	LL1Table[FieldDecMore][ARRAY] = 26;
	LL1Table[IdList][ID] = 27;
	LL1Table[IdMore][SEMI] = 28;
	LL1Table[IdMore][COMMA] = 29;
	LL1Table[VarDec][PROCEDURE] = 30;
	LL1Table[VarDec][BEGIN] = 30;
	LL1Table[VarDec][VAR] = 31;
	LL1Table[VarDeclaration][VAR] = 32;
	LL1Table[VarDecList][INTEGER] = 33;
	LL1Table[VarDecList][CHAR1] = 33;
	LL1Table[VarDecList][ARRAY] = 33;
	LL1Table[VarDecList][RECORD] = 33;
	LL1Table[VarDecList][ID] = 33;
	LL1Table[VarDecMore][PROCEDURE] = 34;
	LL1Table[VarDecMore][BEGIN] = 34;
	LL1Table[VarDecMore][INTEGER] = 35;
	LL1Table[VarDecMore][CHAR1] = 35;
	LL1Table[VarDecMore][ARRAY] = 35;
	LL1Table[VarDecMore][RECORD] = 35;
	LL1Table[VarDecMore][ID] = 35;
	LL1Table[VarIdList][ID] = 36;
	LL1Table[VarIdMore][SEMI] = 37;
	LL1Table[VarIdMore][COMMA] = 38;
	LL1Table[ProcDec][BEGIN] = 39;
	LL1Table[ProcDec][PROCEDURE] = 40;
	LL1Table[ProcDeclaration][PROCEDURE] = 41;
	LL1Table[ProcDecMore][BEGIN] = 42;
	LL1Table[ProcDecMore][PROCEDURE] = 43;
	LL1Table[ProcName][ID] = 44;
	LL1Table[ParamList][RPAREN] = 45;
	LL1Table[ParamList][INTEGER] = 46;
	LL1Table[ParamList][CHAR1] = 46;
	LL1Table[ParamList][ARRAY] = 46;
	LL1Table[ParamList][RECORD] = 46;
	LL1Table[ParamList][ID] = 46;
	LL1Table[ParamList][VAR] = 46;
	LL1Table[ParamDecList][INTEGER] = 47;
	LL1Table[ParamDecList][CHAR1] = 47;
	LL1Table[ParamDecList][ARRAY] = 47;
	LL1Table[ParamDecList][RECORD] = 47;
	LL1Table[ParamDecList][ID] = 47;
	LL1Table[ParamDecList][VAR] = 47;
	LL1Table[ParamMore][RPAREN] = 48;
	LL1Table[ParamMore][SEMI] = 49;
	LL1Table[Param][INTEGER] = 50;
	LL1Table[Param][CHAR1] = 50;
	LL1Table[Param][ARRAY] = 50;
	LL1Table[Param][RECORD] = 50;
	LL1Table[Param][ID] = 50;
	LL1Table[Param][VAR] = 51;
	LL1Table[FormList][ID] = 52;
	LL1Table[FidMore][SEMI] = 53;
	LL1Table[FidMore][RPAREN] = 53;
	LL1Table[FidMore][COMMA] = 54;
	LL1Table[ProcDecPart][TYPE] = 55;
	LL1Table[ProcDecPart][VAR] = 55;
	LL1Table[ProcDecPart][PROCEDURE] = 55;
	LL1Table[ProcDecPart][BEGIN] = 55;
	LL1Table[ProcBody][BEGIN] = 56;
	LL1Table[ProgramBody][BEGIN] = 57;
	LL1Table[StmList][ID] = 58;
	LL1Table[StmList][IF] = 58;
	LL1Table[StmList][WHILE] = 58;
	LL1Table[StmList][RETURN1] = 58;
	LL1Table[StmList][READ] = 58;
	LL1Table[StmList][WRITE] = 58;
	LL1Table[StmList][END1] = 59;
	LL1Table[StmMore][END1] = 59;
	LL1Table[StmMore][ENDWH] = 59;
	LL1Table[StmMore][ELSE] = 59;
	LL1Table[StmMore][FI] = 59;
	LL1Table[StmMore][SEMI] = 60;
	LL1Table[Stm][IF] = 61;
	LL1Table[Stm][WHILE] = 62;
	LL1Table[Stm][READ] = 63;
	LL1Table[Stm][WRITE] = 64;
	LL1Table[Stm][RETURN1] = 65;
	LL1Table[Stm][ID] = 66;
	LL1Table[AssCall][ASSIGN] = 67;
	LL1Table[AssCall][LMIDPAREN] = 67;
	LL1Table[AssCall][DOT] = 67;
	LL1Table[AssCall][LPAREN] = 68;
	LL1Table[AssignmentRest][ASSIGN] = 69;
	LL1Table[AssignmentRest][LMIDPAREN] = 69;
	LL1Table[AssignmentRest][DOT] = 69;
	LL1Table[ConditionalStm][IF] = 70;
	LL1Table[LoopStm][WHILE] = 71;
	LL1Table[InputStm][READ] = 72;
	LL1Table[InVar][ID] = 73;
	LL1Table[OutputStm][WRITE] = 74;
	LL1Table[ReturnStm][RETURN1] = 75;
	LL1Table[CallStmRest][LPAREN] = 76;
	LL1Table[ActParamList][RPAREN] = 77;
	LL1Table[ActParamList][ID] = 78;
	LL1Table[ActParamList][INTC] = 78;
	LL1Table[ActParamList][LPAREN] = 78;
	LL1Table[ActParamMore][RPAREN] = 79;
	LL1Table[ActParamMore][COMMA] = 80;
	LL1Table[RelExp][LPAREN] = 81;
	LL1Table[RelExp][INTC] = 81;
	LL1Table[RelExp][ID] = 81;
	LL1Table[OtherRelE][LT] = 82;
	LL1Table[OtherRelE][EQ] = 82;
	LL1Table[Exp][LPAREN] = 83;
	LL1Table[Exp][INTC] = 83;
	LL1Table[Exp][ID] = 83;
	LL1Table[OtherTerm][LT] = 84;
	LL1Table[OtherTerm][EQ] = 84;
	LL1Table[OtherTerm][THEN] = 84;
	LL1Table[OtherTerm][DO] = 84;
	LL1Table[OtherTerm][RPAREN] = 84;
	LL1Table[OtherTerm][END1] = 84;
	LL1Table[OtherTerm][SEMI] = 84;
	LL1Table[OtherTerm][COMMA] = 84;
	LL1Table[OtherTerm][ENDWH] = 84;
	LL1Table[OtherTerm][ELSE] = 84;
	LL1Table[OtherTerm][FI] = 84;
	LL1Table[OtherTerm][RMIDPAREN] = 84;
	LL1Table[OtherTerm][PLUS] = 85;
	LL1Table[OtherTerm][MINUS] = 85;
	LL1Table[Term][LPAREN] = 86;
	LL1Table[Term][INTC] = 86;
	LL1Table[Term][ID] = 86;
	LL1Table[OtherFactor][PLUS] = 87;
	LL1Table[OtherFactor][MINUS] = 87;
	LL1Table[OtherFactor][LT] = 87;
	LL1Table[OtherFactor][EQ] = 87;
	LL1Table[OtherFactor][THEN] = 87;
	LL1Table[OtherFactor][ELSE] = 87;
	LL1Table[OtherFactor][FI] = 87;
	LL1Table[OtherFactor][DO] = 87;
	LL1Table[OtherFactor][ENDWH] = 87;
	LL1Table[OtherFactor][RPAREN] = 87;
	LL1Table[OtherFactor][END1] = 87;
	LL1Table[OtherFactor][SEMI] = 87;
	LL1Table[OtherFactor][COMMA] = 87;
	LL1Table[OtherFactor][RMIDPAREN] = 87;
	LL1Table[OtherFactor][TIMES] = 88;
	LL1Table[OtherFactor][OVER] = 88;
	LL1Table[Factor][LPAREN] = 89;
	LL1Table[Factor][INTC] = 90;
	LL1Table[Factor][ID] = 91;
	LL1Table[Variable][ID] = 92;
	LL1Table[VariMore][ASSIGN] = 93;
	LL1Table[VariMore][TIMES] = 93;
	LL1Table[VariMore][OVER] = 93;
	LL1Table[VariMore][PLUS] = 93;
	LL1Table[VariMore][MINUS] = 93;
	LL1Table[VariMore][LT] = 93;
	LL1Table[VariMore][EQ] = 93;
	LL1Table[VariMore][THEN] = 93;
	LL1Table[VariMore][ELSE] = 93;
	LL1Table[VariMore][FI] = 93;
	LL1Table[VariMore][DO] = 93;
	LL1Table[VariMore][ENDWH] = 93;
	LL1Table[VariMore][RPAREN] = 93;
	LL1Table[VariMore][END1] = 93;
	LL1Table[VariMore][SEMI] = 93;
	LL1Table[VariMore][COMMA] = 93;
	LL1Table[VariMore][RMIDPAREN] = 93;
	LL1Table[VariMore][LMIDPAREN] = 94;
	LL1Table[VariMore][DOT] = 95;
	LL1Table[FieldVar][ID] = 96;
	LL1Table[FieldVarMore][ASSIGN] = 97;
	LL1Table[FieldVarMore][TIMES] = 97;
	LL1Table[FieldVarMore][OVER] = 97;
	LL1Table[FieldVarMore][PLUS] = 97;
	LL1Table[FieldVarMore][MINUS] = 97;
	LL1Table[FieldVarMore][LT] = 97;
	LL1Table[FieldVarMore][EQ] = 97;
	LL1Table[FieldVarMore][THEN] = 97;
	LL1Table[FieldVarMore][ELSE] = 97;
	LL1Table[FieldVarMore][FI] = 97;
	LL1Table[FieldVarMore][DO] = 97;
	LL1Table[FieldVarMore][ENDWH] = 97;
	LL1Table[FieldVarMore][RPAREN] = 97;
	LL1Table[FieldVarMore][END1] = 97;
	LL1Table[FieldVarMore][SEMI] = 97;
	LL1Table[FieldVarMore][COMMA] = 97;
	LL1Table[FieldVarMore][LMIDPAREN] = 98;
	LL1Table[CmpOp][LT] = 99;
	LL1Table[CmpOp][EQ] = 100;
	LL1Table[AddOp][PLUS] = 101;
	LL1Table[AddOp][MINUS] = 102;
	LL1Table[MultOp][TIMES] = 103;
	LL1Table[MultOp][OVER] = 104;
}

int  ParseLL1::Priosity(LexType  op)
{
	int  pri = 0;
	switch (op)
	{
	case END1:
		pri = -1; break;
	case LPAREN:
		pri = 0;
	case LT:
	case EQ:
		pri = 1; break;
	case PLUS:
	case MINUS:
		pri = 2; break;
	case TIMES:
	case OVER:
		pri = 3; break;
	default:
		fprintf(all, "运算符错误");
		fprintf(all, "%d\n", op);
		pri = -1;
	}
	return  pri;
}

void ParseLL1::GetToken(Token* p, Scanner sc)
{
	//跳过词法错误 只读入正确的词法
	if (TokenList.at(TokenListNum)->word.Lex == ERROR)
		TokenListNum++;
	p->Lineshow = TokenList.at(TokenListNum)->Lineshow;
	p->word.Lex = TokenList.at(TokenListNum)->word.Lex;
	p->word.Sem = TokenList.at(TokenListNum)->word.Sem;
	TokenListNum++;
}
int ParseLL1::form = 0;
void ParseLL1::printTree(TreeNode* tree)
{
	int i;
	form = form + 4;
	while (tree != NULL)
	{
		for (i = 0; i < form; i++)
			fprintf(output, " ");
		switch (tree->nodekind)
		{
		case ProK:
			fprintf(output, "%s  ", "ProK"); break;
		case PheadK:
		{
			fprintf(output, "%s  ", "PheadK");
			fprintf(output, "%s  ", tree->name[0]);
		}
		break;
		case DecK:
		{
			fprintf(output, "%s  ", "DecK");
			if (tree->attr.ProcAttr.paramt == varparamType)
				fprintf(output, "%s  ", "varparam:");
			if (tree->attr.ProcAttr.paramt == valparamType)
				fprintf(output, "%s  ", "valparam:");
			switch (tree->kind.dec)
			{
			case  ArrayK:
			{
				fprintf(output, "%s  ", "ArrayK");
				fprintf(output, "%d  ", tree->attr.ArrayAttr.low);
				fprintf(output, "%d  ", tree->attr.ArrayAttr.up);
				if (tree->attr.ArrayAttr.childtype == CharK)
					fprintf(output, "%s  ", "Chark");
				else if (tree->attr.ArrayAttr.childtype == IntegerK)
					fprintf(output, "%s  ", "IntegerK");
			}; break;
			case  CharK:
				fprintf(output, "%s  ", "CharK"); break;
			case  IntegerK:
				fprintf(output, "%s  ", "IntegerK"); break;
			case  RecordK:
				fprintf(output, "%s  ", "RecordK"); break;
			case  IdK:
				fprintf(output, "%s  ", "IdK");
				fprintf(output, "%s  ", tree->attr.type_name);
				break;
			default:
				fprintf(output, "声明节点错误!");
				ErrorFlag = true;
			};
			if (tree->idnum != 0)
				for (int i = 0; i <= (tree->idnum); i++)
				{
					fprintf(output, "%s  ", tree->name[i]);
				}
		} break;
		case TypeK:
			fprintf(output, "%s  ", "TypeK");
			break;
		case VarK:
			fprintf(output, "%s  ", "VarK");
			break;

		case ProcDecK:
			fprintf(output, "%s  ", "ProcDecK");
			fprintf(output, "%s  ", tree->name[0]);
			break;

		case StmLK:
			fprintf(output, "%s  ", "StmLk"); break;

		case StmtK:
		{
			fprintf(output, "%s  ", "StmtK");
			switch (tree->kind.stmt)
			{
			case IfK:
				fprintf(output, "%s  ", "If"); break;
			case WhileK:
				fprintf(output, "%s  ", "While"); break;

			case AssignK:
				fprintf(output, "%s  ", "Assign");
				break;

			case ReadK:
				fprintf(output, "%s  ", "Read");
				fprintf(output, "%s  ", tree->name[0]);
				break;

			case WriteK:
				fprintf(output, "%s  ", "Write"); break;

			case CallK:
				fprintf(output, "%s  ", "Call");
				fprintf(output, "%s  ", tree->name[0]);
				break;

			case ReturnK:
				fprintf(output, "%s  ", "Return"); break;

			default:
				fprintf(output, "语句节点错误!");
				ErrorFlag = true;
			}
		}; break;
		case ExpK:
		{
			fprintf(output, "%s  ", "ExpK");
			switch (tree->kind.exp)
			{
			case OpK:
			{ fprintf(output, "%s  ", "Op");
			switch (tree->attr.ExpAttr.op)
			{
			case EQ:   fprintf(output, "%s  ", "="); break;
			case LT:   fprintf(output, "%s  ", "<"); break;
			case PLUS: fprintf(output, "%s  ", "+"); break;
			case MINUS:fprintf(output, "%s  ", "-"); break;
			case TIMES:fprintf(output, "%s  ", "*"); break;
			case OVER: fprintf(output, "%s  ", "/"); break;
			default:
				fprintf(output, "操作符错误");
				ErrorFlag = true;
			}

			if (tree->attr.ExpAttr.varkind == ArrayMembV)
			{
				fprintf(output, "ArrayMember  ");
				fprintf(output, "%s  ", tree->name[0]);
			}
			}; break;
			case ConstK:
				fprintf(output, "%s  ", "Const");
				switch (tree->attr.ExpAttr.varkind)
				{
				case IdV:
					fprintf(output, "Id  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				case FieldMembV:
					fprintf(output, "FieldMember  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				case ArrayMembV:
					fprintf(output, "ArrayMember  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				default:
					fprintf(output, "变量错误");
					ErrorFlag = true;
				}

				fprintf(output, "%d  ", tree->attr.ExpAttr.val);
				break;
			case VariK:
				fprintf(output, "%s  ", "Vari");
				switch (tree->attr.ExpAttr.varkind)
				{
				case IdV:
					fprintf(output, "Id  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				case FieldMembV:
					fprintf(output, "FieldMember  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				case ArrayMembV:
					fprintf(output, "ArrayMember  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				default:
					fprintf(output, "变量错误");
					ErrorFlag = true;
				}
				break;
			default:
				fprintf(output, "表达式节点错误!");
				ErrorFlag = true;
			}
		}; break;
		default:
			fprintf(output, "语法树节点节点错误!");
			ErrorFlag = true;
		}

		fprintf(output, "\n");
		for (i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	form = form - 4;

}

//右部非终极符从右至左依次入符号栈
void ParseLL1::process1()
{
	Push(2, DOT);
	Push(1, ProgramBody);
	Push(1, DeclarePart);
	Push(1, ProgramHead);
}
/*当栈顶为 ProgramHead ，当前 Token 的词法信息为 PROGRAM 时， 选择这个产生式，处理程序头：将产生式右部压入符号栈；
生成程序 头节点，弹语法树栈，并对弹出元素进行赋值，使得语法树根节点的 儿子节点指向程序头节点*/
void ParseLL1::process2()
{
	Push(1, ProgramName);
	Push(2, PROGRAM);

	TreeNode** t = PopTree();
	currentP = newNode(PheadK);
	(*t) = currentP;

}
/*处理程序名：ID 压入符号栈；并将当前标识符的语义信息（标识符名） 写入程序头节点，标识符个数加 1。*/
void ParseLL1::process3()
{
	Push(2, ID);

	strcpy_s(currentP->name[0], currentToken.word.Sem.c_str());
	currentP->idnum++;
}
/*处理程序声明：产生式右部非终极符从右至左依次进符号栈，语法树部分没有动作*/
void ParseLL1::process4()
{
	Push(1, ProcDec);
	Push(1, VarDec);
	Push(1, TypeDec);
}
/*空函数*/
void ParseLL1::process5()
{
}
/*右部入符号栈*/
void ParseLL1::process6()
{
	Push(1, TypeDeclaration);

}
/*<TypeDeclaration> ::= TYPE TypeDecList*/
void ParseLL1::process7()
{
	Push(1, TypeDecList);
	Push(2, TYPE);
	currentP = newNode(TypeK);
	TreeNode** t = PopTree();

	(*t) = currentP;
	PushTree(&((*currentP).sibling));  /* 压currentP的兄弟指针地址入语法树栈 变量声明标识节点*/
	PushTree(&((*currentP).child[0])); /*压currentP的儿子指针地址入语法树栈 具体的声明节点*/
}
/*进入具体的类型声明。语法树处理部分，生成一个声明类型节点，不 在此添加任何其他信息；
弹语法树栈，得到指针的地址，令指针指向 此声明类型节点，若是第一个声明节点，则是 Type 类型的子节点指向 当前节点，否则，作为上一个类型声明的兄弟节点出现。并将此节点 的兄弟节点压入语法树栈，以便处理下一个类型声明。*/
void ParseLL1::process8()
{
	Push(1, TypeDecMore);
	Push(2, SEMI);
	Push(1, TypeDef);
	Push(2, EQ);
	Push(1, TypeId);

	TreeNode** t = PopTree();
	currentP = newNode(DecK);
	(*t) = currentP;
	PushTree(&((*currentP).sibling));
}
/*没有其它的类型声明，这时语法树栈顶存放的是最后一个类型声明节 点的兄弟节点，弹出，完成类型部分的语法树节点生成。*/
void ParseLL1::process9()
{
	PopTree();
}
/*右部入符号栈*/
void ParseLL1::process10()
{
	Push(1, TypeDecList);

}
/*处理类型声明节点的类型标识符，把标识符名存入节点中，标识符个数加 1*/
void ParseLL1::process11()
{
	Push(2, ID);

	strcpy_s((*currentP).name[0], currentToken.word.Sem.c_str());
	currentP->idnum++;
}
/*处理声明节点的标识符的类型部分。
基本类型可以是整型和字符型， 这里用变量 temp 记录节点上填写标识符类型信息的部分的地址，
在下面的产生式处理对 temp 里的内容进行赋值，就完成了类型部分的填写*/
void ParseLL1::process12()
{
	Push(1, BaseType);

	temp = &(currentP->kind.dec);

}
/*右部入符号栈*/
void ParseLL1::process13()
{
	Push(1, StructureType);
}
/*声明的类型标识符的类型是用已声明过的类型标识符给出的，在当前节点存储此标识符的名字，节点上标识符的个数加 1*/
void ParseLL1::process14()
{
	Push(2, ID);
	(*currentP).kind.dec = IdK;
	strcpy_s(currentP->attr.type_name, currentToken.word.Sem.c_str());
}
/*声明的类型是整型，对temp 这个地址的内容赋值，将整型信息存入声明节点*/
void ParseLL1::process15()
{
	Push(2, INTEGER);
	(*temp) = IntegerK;
}
/*声明的类型是字符型，对 temp 这个地址的内容赋值，将字符型信息存入声明节点*/
void ParseLL1::process16()
{
	Push(2, CHAR1);
	(*temp) = CharK;
}
/*右部入符号栈*/
void ParseLL1::process17()
{
	Push(1, ArrayType);
}
/*右部入符号栈*/
void ParseLL1::process18()
{
	Push(1, RecType);
}
/*声明的类型信息赋值为数组类型 ArrayK，并用 temp 记录填写数组的基类型部分的地址，
以后对 temp 地址的内容赋值，就完成了对数组基 类型信息的填写*/
void ParseLL1::process19()
{
	Push(1, BaseType);
	Push(2, OF);
	Push(2, RMIDPAREN);
	Push(1, Top);
	Push(2, UNDERANGE);
	Push(1, Low);
	Push(2, LMIDPAREN);
	Push(2, ARRAY);
	(*currentP).kind.dec = ArrayK;
	temp = &(currentP->attr.ArrayAttr.childtype);

}
/*右部终极符入符号栈，并将整数的值写入数组类型声明节点的下界*/
void ParseLL1::process20()
{
	Push(2, INTC);
	(*currentP).attr.ArrayAttr.low = atoi(currentToken.word.Sem.c_str());
}
/*右部终极符入符号栈，并将整数的值写入数组类型声明节点的上界*/
void ParseLL1::process21()
{
	Push(2, INTC);
	(*currentP).attr.ArrayAttr.up = atoi(currentToken.word.Sem.c_str());
}
void ParseLL1::process22()
{
	Push(2, END1);
	Push(1, FieldDecList);
	Push(2, RECORD);
	(*currentP).kind.dec = RecordK;
	tempNode = currentP;
	PushTree(&((*currentP).child[0]));
}
void ParseLL1::process23()
{
	Push(1, FieldDecMore);
	Push(2, SEMI);
	Push(1, IdList);
	Push(1, BaseType);

	TreeNode** t = PopTree();
	currentP = newNode(DecK);
	temp = (&(currentP->kind.dec));
	(*t) = currentP;
	PushTree(&((*currentP).sibling));

}
void ParseLL1::process24()
{
	Push(1, FieldDecMore);
	Push(2, SEMI);
	Push(1, IdList);
	Push(1, ArrayType);

	TreeNode** t = PopTree();
	currentP = newNode(DecK);
	(*t) = currentP;
	PushTree(&((*currentP).sibling));

}
/*没有记录类型的下一个域了，恢复当前纪录类型节点的指针*/
void ParseLL1::process25()
{

	PopTree();
	currentP = tempNode;
}
void ParseLL1::process26()
{
	Push(1, FieldDecList);

}
/*并将当前标识符的名字，存入节点中，标识符个数加 1*/
void ParseLL1::process27()
{
	Push(1, IdMore);
	Push(2, ID);

	/*纪录一个域中各个变量的语义信息*/
	strcpy_s(currentP->name[currentP->idnum], currentToken.word.Sem.c_str());
	currentP->idnum++;
}
void ParseLL1::process28()
{
}
void ParseLL1::process29()
{
	Push(1, IdList);
	Push(2, COMMA);

}
void ParseLL1::process30()
{
}
void ParseLL1::process31()
{
	Push(1, VarDeclaration);

}
/*处理变量声明*/
void ParseLL1::process32()
{
	Push(1, VarDecList);
	Push(2, VAR);

	currentP = newNode(VarK);   /*生成一个标志变量声明的节点*/
	TreeNode** t = PopTree();
	(*t) = currentP;
	/*当前变量声明节点的兄弟节点应该指向函数声明节点或语句序列 节点，而子节点则应指向具体的声明节点，故将当前节点的兄弟节点 和第一个儿子节点压入语法树栈，以待以后处理*/
	PushTree(&((*currentP).sibling));  /*压入指向函数声明的指针*/
	PushTree(&((*currentP).child[0])); /*压入指向第一个变量声明节点的指针*/
}
/*进入具体的变量声明*/
void ParseLL1::process33()
{
	Push(1, VarDecMore);
	Push(2, SEMI);
	Push(1, VarIdList);
	Push(1, TypeDef);

	TreeNode** t = PopTree();
	currentP = newNode(DecK);
	(*t) = currentP;
	PushTree(&((*currentP).sibling));

}
void ParseLL1::process34()
{
	PopTree();
}
void ParseLL1::process35()
{
	Push(1, VarDecList);

}
void ParseLL1::process36()
{
	Push(1, VarIdMore);
	Push(2, ID);

	strcpy_s(currentP->name[currentP->idnum], currentToken.word.Sem.c_str());
	currentP->idnum++;

}
void ParseLL1::process37()
{
}
void ParseLL1::process38()
{
	Push(1, VarIdList);
	Push(2, COMMA);

}
void ParseLL1::process39()
{
}
void ParseLL1::process40()
{
	Push(1, ProcDeclaration);

}
/////////////////////////////////////////////////////////////////////////////////////inconsistent/
void ParseLL1::process41()
{
	Push(1, ProcDecMore);
	Push(1, ProcBody);
	Push(1, ProcDecPart);
	Push(2, SEMI);
	//Push(1, BaseType);                                                                                           
	Push(2, RPAREN);
	Push(1, ParamList);
	Push(2, LPAREN);
	Push(1, ProcName);
	Push(2, PROCEDURE);

	currentP = newNode(ProcDecK);
	TreeNode** t = PopTree();
	(*t) = currentP;
	PushTree(&(currentP->sibling));
	PushTree(&(currentP->child[2]));
	PushTree(&(currentP->child[1]));
	PushTree(&(currentP->child[0]));
}
void ParseLL1::process42()
{
}
void ParseLL1::process43()
{
	Push(1, ProcDeclaration);
}
void ParseLL1::process44()
{
	Push(2, ID);

	strcpy_s(currentP->name[0], currentToken.word.Sem.c_str());
	currentP->idnum++;
}
void ParseLL1::process45()
{
	PopTree();
}
void ParseLL1::process46()
{
	Push(1, ParamDecList);
}
void ParseLL1::process47()
{
	Push(1, ParamMore);
	Push(1, Param);
}
void ParseLL1::process48()
{
	PopTree();
}
void ParseLL1::process49()
{
	Push(1, ParamDecList);
	Push(2, SEMI);

}
void ParseLL1::process50()
{
	Push(1, FormList);
	Push(1, TypeDef);
	currentP = newNode(DecK);
	TreeNode** t = PopTree();

	/*函数的参数类型是值类型*/
	currentP->attr.ProcAttr.paramt = valparamType;
	(*t) = currentP;
	PushTree(&(currentP->sibling));


}
void ParseLL1::process51()
{
	Push(1, FormList);
	Push(1, TypeDef);
	Push(2, VAR);
	currentP = newNode(DecK);
	TreeNode** t = PopTree();

	/*函数的参数类型是变量类型*/
	currentP->attr.ProcAttr.paramt = varparamType;
	(*t) = currentP;
	PushTree(&(currentP->sibling));

}
void ParseLL1::process52()
{
	Push(1, FidMore);
	Push(2, ID);

	strcpy_s(currentP->name[currentP->idnum], currentToken.word.Sem.c_str());
	currentP->idnum++;

}
void ParseLL1::process53()
{
}
void ParseLL1::process54()
{
	Push(1, FormList);
	Push(2, COMMA);
}
void ParseLL1::process55()
{
	Push(1, DeclarePart);
}
void ParseLL1::process56()
{
	Push(1, ProgramBody);
}
/////////////////////////////////////////////////////////////////////////////////////inconsistent/
void ParseLL1::process57()
{
	Push(2, END1);
	Push(1, StmList);
	Push(2, BEGIN);
	PopTree();

	/*建立语句序列标识节点*/
	TreeNode** t = PopTree();
	currentP = newNode(StmLK);
	(*t) = currentP;
	PushTree(&(currentP->child[0]));

}
void ParseLL1::process58()
{
	Push(1, StmMore);
	Push(1, Stm);
}
void ParseLL1::process59()
{
	PopTree();
}
void ParseLL1::process60()
{
	Push(1, StmList);
	Push(2, SEMI);

}
void ParseLL1::process61()
{
	Push(1, ConditionalStm);
	currentP = newStmtNode(IfK);
	TreeNode** t = PopTree();
	(*t) = currentP;
	PushTree(&currentP->sibling);
}
void ParseLL1::process62()
{
	Push(1, LoopStm);
	currentP = newStmtNode(WhileK);
	TreeNode** t = PopTree();
	(*t) = currentP;
	PushTree(&currentP->sibling);

}
void ParseLL1::process63()
{
	Push(1, InputStm);
	TreeNode** t = PopTree();
	currentP = newStmtNode(ReadK);
	(*t) = currentP;
	PushTree(&currentP->sibling);
}
void ParseLL1::process64()
{
	Push(1, OutputStm);
	TreeNode** t = PopTree();
	currentP = newStmtNode(WriteK);
	(*t) = currentP;
	PushTree(&currentP->sibling);

}
void ParseLL1::process65()
{
	Push(1, ReturnStm);
	TreeNode** t = PopTree();
	currentP = newStmtNode(ReturnK);
	(*t) = currentP;
	PushTree(&currentP->sibling);

}
void ParseLL1::process66()
{
	Push(1, AssCall);
	Push(2, ID);
	currentP = newStmtNode(AssignK);
	TreeNode* t = newExpNode(VariK);
	strcpy_s(t->name[0], currentToken.word.Sem.c_str());
	t->idnum++;
	currentP->child[0] = t;
	TreeNode** t1 = PopTree();
	(*t1) = currentP;
	PushTree(&currentP->sibling);

}
void ParseLL1::process67()
{
	Push(1, AssignmentRest);

	currentP->kind.stmt = AssignK;
}
void ParseLL1::process68()
{
	Push(1, CallStmRest);
	currentP->child[0]->attr.ExpAttr.varkind = IdV;

	currentP->kind.stmt = CallK;
}
void ParseLL1::process69()
{
	Push(1, Exp);
	Push(2, ASSIGN);
	Push(1, VariMore);
	PushTree(&(currentP->child[1]));
	currentP = currentP->child[0];

	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = END1;
	PushOp(t);
}
void ParseLL1::process70()
{
	Push(2, FI);
	Push(1, StmList);
	Push(2, ELSE);
	Push(1, StmList);
	Push(2, THEN);
	Push(1, RelExp);
	Push(2, IF);

	PushTree(&(currentP->child[2]));
	PushTree(&(currentP->child[1]));
	PushTree(&(currentP->child[0]));

}
void ParseLL1::process71()
{
	Push(2, ENDWH);
	Push(1, StmList);
	Push(2, DO);
	Push(1, RelExp);
	Push(2, WHILE);

	PushTree(&(currentP->child[1]));
	PushTree(&(currentP->child[0]));

}
void ParseLL1::process72()
{
	Push(2, RPAREN);
	Push(1, InVar);
	Push(2, LPAREN);
	Push(2, READ);
}
void ParseLL1::process73()
{
	Push(2, ID);

	strcpy_s(currentP->name[0], currentToken.word.Sem.c_str());
	currentP->idnum++;
}
void ParseLL1::process74()
{
	Push(2, RPAREN);
	Push(1, Exp);
	Push(2, LPAREN);
	Push(2, WRITE);

	PushTree(&(currentP->child[0]));
	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = END1;
	PushOp(t);
}
void ParseLL1::process75()
{
	Push(2, RETURN1);
}
void ParseLL1::process76()
{
	Push(2, RPAREN);
	Push(1, ActParamList);
	Push(2, LPAREN);

	PushTree(&(currentP->child[1]));

}
void ParseLL1::process77()
{
	PopTree();
}
void ParseLL1::process78()
{
	Push(1, ActParamMore);
	Push(1, Exp);

	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = END1;
	PushOp(t);  
}
void ParseLL1::process79()
{
}
void ParseLL1::process80()
{
	Push(1, ActParamList);
	Push(2, COMMA);

	PushTree(&(currentP->sibling));
}
void ParseLL1::process81()
{
	Push(1, OtherRelE);
	Push(1, Exp);

	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = END1;
	PushOp(t);  

	getExpResult = false;
}
void ParseLL1::process82()
{
	Push(1, Exp);
	Push(1, CmpOp);

	TreeNode* currentP = newExpNode(OpK);
	currentP->attr.ExpAttr.op = currentToken.word.Lex;

	LexType  sTop = StackOpTop->pointer->attr.ExpAttr.op;
	while (Priosity(sTop) >= Priosity(currentToken.word.Lex))
	{
		TreeNode* t = PopOp();
		TreeNode* Rnum = PopNum();
		TreeNode* Lnum = PopNum();
		t->child[1] = Rnum;
		t->child[0] = Lnum;
		PushNum(t);

		sTop = StackOpTop->pointer->attr.ExpAttr.op;
	}

	PushOp(currentP);
	getExpResult = true;
}
void ParseLL1::process83()
{
	Push(1, OtherTerm);
	Push(1, Term);
}
void ParseLL1::process84()
{
	if ((currentToken.word.Lex == RPAREN) && (LPARENnum != 0))
	{
		while (StackOpTop->pointer->attr.ExpAttr.op != LPAREN)
		{
			TreeNode* t = PopOp();
			TreeNode* Rnum = PopNum();
			TreeNode* Lnum = PopNum();

			t->child[1] = Rnum;
			t->child[0] = Lnum;
			PushNum(t);
		}
		PopOp(); //弹出左括号
		LPARENnum--;
	}
	else
	{
		if ((getExpResult) || (getExpResult2))
		{
			while (StackOpTop->pointer->attr.ExpAttr.op != END1)
			{
				TreeNode* t = PopOp();
				TreeNode* Rnum = PopNum();
				TreeNode* Lnum = PopNum();

				t->child[1] = Rnum;
				t->child[0] = Lnum;
				PushNum(t);
			}
			PopOp();
			currentP = PopNum();

			TreeNode** t = PopTree();
			(*t) = currentP;

			if (getExpResult2 == true)
				getExpResult2 = false;
		}
	}
}
void ParseLL1::process85()
{
	Push(1, Exp);
	Push(1, AddOp);

	TreeNode* currentP = newExpNode(OpK);
	currentP->attr.ExpAttr.op = currentToken.word.Lex;
	LexType  sTop = StackOpTop->pointer->attr.ExpAttr.op;
	while (Priosity(sTop) >= Priosity(currentToken.word.Lex))
	{
		TreeNode* t = PopOp();
		TreeNode* Rnum = PopNum();
		TreeNode* Lnum = PopNum();
		t->child[1] = Rnum;
		t->child[0] = Lnum;
		PushNum(t);
		sTop = StackOpTop->pointer->attr.ExpAttr.op;
	}
	PushOp(currentP);
}
void ParseLL1::process86()
{
	Push(1, OtherFactor);
	Push(1, Factor);
}
void ParseLL1::process87()
{
}
void ParseLL1::process88()
{
	Push(1, Term);
	Push(1, MultOp);

	TreeNode* currentP = newExpNode(OpK);
	currentP->attr.ExpAttr.op = currentToken.word.Lex;

	LexType  sTop = StackOpTop->pointer->attr.ExpAttr.op;
	while (Priosity(sTop) >= Priosity(currentToken.word.Lex))
	{
		TreeNode* t = PopOp();
		TreeNode* Rnum = PopNum();
		TreeNode* Lnum = PopNum();
		t->child[1] = Rnum;
		t->child[0] = Lnum;
		PushNum(t);

		sTop = StackOpTop->pointer->attr.ExpAttr.op;
	}
	PushOp(currentP);
}
void ParseLL1::process89()
{
	Push(2, RPAREN);
	Push(1, Exp);
	Push(2, LPAREN);

	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = currentToken.word.Lex; /*把左括号也压入栈中*/
	PushOp(t);
	LPARENnum++;
}
void ParseLL1::process90()
{
	Push(2, INTC);

	TreeNode* t = newExpNode(ConstK);
	t->attr.ExpAttr.val = atoi(currentToken.word.Sem.c_str());
	/*常数节点入操作数栈*/
	PushNum(t);

}
void ParseLL1::process91()
{
	Push(1, Variable);
}
void ParseLL1::process92()
{
	Push(1, VariMore);
	Push(2, ID);

	currentP = newExpNode(VariK);
	strcpy_s(currentP->name[0], currentToken.word.Sem.c_str());
	currentP->idnum++;
	/*变量节点入操作数栈*/
	PushNum(currentP);

}
void ParseLL1::process93()
{
	currentP->attr.ExpAttr.varkind = IdV;
}
void ParseLL1::process94()
{
	Push(2, RMIDPAREN);
	Push(1, Exp);
	Push(2, LMIDPAREN);
	currentP->attr.ExpAttr.varkind = ArrayMembV;
	PushTree(&currentP->child[0]);

	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = END1;
	PushOp(t);

	getExpResult2 = true;


}
void ParseLL1::process95()
{
	Push(1, FieldVar);
	Push(2, DOT);
	currentP->attr.ExpAttr.varkind = FieldMembV;
	PushTree(&currentP->child[0]);
}
void ParseLL1::process96()
{
	Push(1, FieldVarMore);
	Push(2, ID);

	currentP = newExpNode(VariK);
	strcpy_s(currentP->name[0], currentToken.word.Sem.c_str());
	currentP->idnum++;

	TreeNode** t = PopTree();
	(*t) = currentP;


}
void ParseLL1::process97()
{
	currentP->attr.ExpAttr.varkind = IdV;
}
void ParseLL1::process98()
{
	Push(2, RMIDPAREN);
	Push(1, Exp);
	Push(2, LMIDPAREN);
	currentP->attr.ExpAttr.varkind = ArrayMembV;
	PushTree(&currentP->child[0]);

	TreeNode* t = newExpNode(OpK);
	t->attr.ExpAttr.op = END1;
	PushOp(t);

	getExpResult2 = true;
}
void ParseLL1::process99()
{
	Push(2, LT);
}
void ParseLL1::process100()
{
	Push(2, EQ);
}
void ParseLL1::process101()
{
	Push(2, PLUS);
}
void ParseLL1::process102()
{
	Push(2, MINUS);
}
void ParseLL1::process103()
{
	Push(2, TIMES);
}
void ParseLL1::process104()
{
	Push(2, OVER);
}
void ParseLL1::predict(int num)
{
	switch (num)
	{
	case 1:     ParseLL1::process1();		break;
	case 2:     ParseLL1::process2();	break;
	case 3:     ParseLL1::process3();	break;
	case 4:     ParseLL1::process4();	break;
	case 5:	   ParseLL1::process5();	    break;
	case 6:	   ParseLL1::process6();	    break;
	case 7:	   ParseLL1::process7();	    break;
	case 8:	   ParseLL1::process8();		break;
	case 9:	   ParseLL1::process9();		break;
	case 10:	  ParseLL1::process10();	break;
	case 11:	      ParseLL1::process11();	    break;
	case 12:	  ParseLL1::process12();	break;
	case 13:	  ParseLL1::process13();	break;
	case 14:	  ParseLL1::process14();	break;
	case 15:	  ParseLL1::process15();	break;
	case 16:	  ParseLL1::process16();	break;
	case 17:	  ParseLL1::process17();	break;
	case 18:	  ParseLL1::process18();	break;
	case 19:	  ParseLL1::process19();	break;
	case 20:	  ParseLL1::process20();	break;
	case 21:	  ParseLL1::process21();	break;
	case 22:	  ParseLL1::process22();	break;
	case 23:	  ParseLL1::process23();	break;
	case 24:	  ParseLL1::process24();	break;
	case 25:	  ParseLL1::process25();	break;
	case 26:	  ParseLL1::process26();	break;
	case 27:	  ParseLL1::process27();	break;
	case 28:	  ParseLL1::process28();	break;
	case 29:	  ParseLL1::process29();	break;
	case 30:	  ParseLL1::process30();	break;
	case 31:	  ParseLL1::process31();	break;
	case 32:	  ParseLL1::process32();	break;
	case 33:	  ParseLL1::process33();	break;
	case 34:	  ParseLL1::process34();	break;
	case 35:	  ParseLL1::process35();	break;
	case 36:	  ParseLL1::process36();	break;
	case 37:	  ParseLL1::process37();	break;
	case 38:	  ParseLL1::process38();	break;
	case 39:	  ParseLL1::process39();	break;
	case 40:	  ParseLL1::process40();	break;
	case 41:	  ParseLL1::process41();	break;
	case 42:	  ParseLL1::process42();	break;
	case 43:	  ParseLL1::process43();	break;
	case 44:	  ParseLL1::process44();	break;
	case 45:	  ParseLL1::process45();	break;
	case 46:     ParseLL1::process46();	break;
	case 47:	  ParseLL1::process47();	break;
	case 48:	  ParseLL1::process48();	break;
	case 49:	  ParseLL1::process49();	break;
	case 50:	  ParseLL1::process50();	break;
	case 51:	  ParseLL1::process51();	break;
	case 52:	  ParseLL1::process52();	break;
	case 53:	  ParseLL1::process53();	break;
	case 54:	  ParseLL1::process54();	break;
	case 55:	  ParseLL1::process55();	break;
	case 56:	  ParseLL1::process56();	break;
	case 57:	  ParseLL1::process57();   break;
	case 58:	  ParseLL1::process58();	break;
	case 59:	  ParseLL1::process59();	break;
	case 60:	  ParseLL1::process60();	break;
	case 61:	  ParseLL1::process61();	break;
	case 62:	  ParseLL1::process62();	break;
	case 63:	  ParseLL1::process63();	break;
	case 64:	  ParseLL1::process64();	break;
	case 65:	  ParseLL1::process65();	break;
	case 66:	  ParseLL1::process66();	break;
	case 67:	  ParseLL1::process67();	break;
	case 68:	  ParseLL1::process68();	break;
	case 69:     ParseLL1::process69();	break;
	case 70:     ParseLL1::process70();	break;
	case 71:	  ParseLL1::process71();	break;
	case 72:	  ParseLL1::process72();	break;
	case 73:	  ParseLL1::process73();	break;
	case 74:	  ParseLL1::process74();	break;
	case 75:	  ParseLL1::process75();	break;
	case 76:	  ParseLL1::process76();	break;
	case 77:	  ParseLL1::process77();	break;
	case 78:     ParseLL1::process78();	break;
	case 79:     ParseLL1::process79();	break;
	case 80:	  ParseLL1::process80();	break;
	case 81:	  ParseLL1::process81();	break;
	case 82:	  ParseLL1::process82();	break;
	case 83:	  ParseLL1::process83();	break;
	case 84:	  ParseLL1::process84();	break;
	case 85:	  ParseLL1::process85();	break;
	case 86:	  ParseLL1::process86();	break;
	case 87:	  ParseLL1::process87();	break;
	case 88:	  ParseLL1::process88();	break;
	case 89:	  ParseLL1::process89();	break;
	case 90:	  ParseLL1::process90();	break;
	case 91:	  ParseLL1::process91();	break;
	case 92:	  ParseLL1::process92();	break;
	case 93:	  ParseLL1::process93();	break;
	case 94:	  ParseLL1::process94();	break;
	case 95:	  ParseLL1::process95();	break;
	case 96:	  ParseLL1::process96();	break;
	case 97:     ParseLL1::process97();	break;
	case 98:     ParseLL1::process98();	break;
	case 99:     ParseLL1::process99();	break;
	case 100:   ParseLL1::process100();	break;
	case 101:   ParseLL1::process101();	break;
	case 102:   ParseLL1::process102();	break;
	case 103:   ParseLL1::process103();	break;
	case 104:   ParseLL1::process104();	break;
	case 0:
	default: {
		/*fprintf(output, "检测到语法错误test5：在第%d行: %s", lineshow, "错误的token是: ");
		ErrorFlag = true;
		fprintf(output, "词法信息：%s", toString(currentToken.word.Lex).c_str());
		fprintf(output, "；");
		fprintf(output, "语义信息：%s", currentToken.word.Sem.c_str());
		fprintf(output, "\n");
		TokenListNum++;*/
	}
	}
}