#include "RecursiveDescent.h"


int rdNext = 0;
int point = 0;
string message = "����Ϊ��";//������Ϣ

//1.�����ܳ�������

TreeNode* PrseRD()
{
	TreeNode* root = ProgramRD();
	//cout << message << endl;
	if (TokenList[rdNext]->word.Lex == ENDFILE)
	{
		return root;
	}
	else
	{
		message += TokenList[rdNext - 1]->Lineshow + ":except EOF at the end of file\n";
		return root;
	}

}

//2.�ܳ������������
TreeNode* ProgramRD()
{
	TreeNode* head, * part, * body;
	TreeNode* root = new TreeNode();
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		root->child[i] = nullptr;
	}
	point++;
	root->nodekind = ProK;

	//����ͷ
	head = ProgramHeadRD();
	if (head != nullptr)
	{
		root->child[0] = head;
	}
	else
	{
		message += TokenList[rdNext - 1]->Lineshow + ":program head wrong \n";
	}

	//��������
	while (TokenList[rdNext]->word.Lex != TYPE && TokenList[rdNext]->word.Lex != VAR
		&& TokenList[rdNext]->word.Lex != PROCEDURE && TokenList[rdNext]->word.Lex != BEGIN)
	{
		if (rdNext >= TokenList.size() - 1)
			return nullptr;
		rdNext++;
	}

	part = DeclarePartRD();
	if (part != nullptr)
	{
		root->child[1] = part;
	}
	else
	{
		message += TokenList[rdNext - 1]->Lineshow + ":program declare wrong \n";
	}

	//������
	while (TokenList[rdNext]->word.Lex != BEGIN)
	{
		if (rdNext >= TokenList.size() - 1)
			return nullptr;
		rdNext++;
	}
	body = ProgramBodyRD();
	if (body != nullptr)
	{
		root->child[2] = body;
	}
	else
	{
		message += TokenList[rdNext - 1]->Lineshow + ":program body wrong \n";
	}

	//ƥ��dot
	rdNext++;
	if (TokenList[rdNext]->word.Lex == DOT)
	{
		rdNext++;
		return root;
	}
	else
	{

		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect a dot as the end of file \n";
		return root;
	}
}

//3.����ͷ���ִ����������
TreeNode* ProgramHeadRD()
{
	Token* tem = TokenList[rdNext];
	if (tem->word.Lex == PROGRAM)
	{
		rdNext++;
		TreeNode* hroot = new TreeNode();
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			hroot->child[i] = nullptr;
		}
		point++;
		hroot->nodekind = PheadK;
		tem = TokenList[rdNext];
		if (tem->word.Lex == ID)
		{
			//����ʶ��������name����
			strcpy(hroot->name[0] , tem->word.Sem.c_str());
			hroot->idnum = 1;
			rdNext++;
		}
		else
		{
			message += +tem->Lineshow + "expect ID\n";
		}

		return hroot;
	}
	else
	{
		message += TokenList[rdNext - 1]->Lineshow + ":expect ID \n";
		return nullptr;
	}
}

//4.�����������ִ����������
TreeNode* DeclarePartRD()
{
	TreeNode* typeP = nullptr;
	//ȷ��type����֮ǰ����type
	while (TokenList[rdNext]->word.Lex != TYPE && TokenList[rdNext]->word.Lex != VAR &&
		TokenList[rdNext]->word.Lex != PROCEDURE && TokenList[rdNext]->word.Lex != BEGIN)
	{
		if (rdNext < TokenList.size() - 1)
			rdNext++;
	}
	if (TokenList[rdNext]->word.Lex == TYPE)
	{
		typeP = new TreeNode();
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			typeP->child[i] = nullptr;
		}
		point++;
		typeP->nodekind = TypeK;
		TreeNode* tp1 = TypeDecRD();
		typeP->child[0] = tp1;
	}

	TreeNode* varP = nullptr;
	//varDec֮ǰ��ֻ֤����var
	while (TokenList[rdNext]->word.Lex != VAR && TokenList[rdNext]->word.Lex != PROCEDURE &&
		TokenList[rdNext]->word.Lex != BEGIN)
	{
		if (rdNext < TokenList.size() - 1)
			rdNext++;
	}
	if (TokenList[rdNext]->word.Lex == VAR)
	{
		varP = new TreeNode();
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			varP->child[i] = nullptr;
		}
		point++;
		varP->nodekind = VarK;
		TreeNode* tp2 = VarDecRD();
		varP->child[0] = tp2;


	}

	//Procedure֮ǰ��ֻ֤������predict��
	TreeNode* s = nullptr;
	while (TokenList[rdNext]->word.Lex != PROCEDURE && TokenList[rdNext]->word.Lex != BEGIN)
	{
		if (rdNext < TokenList.size() - 1)
			rdNext++;
	}
	if (TokenList[rdNext]->word.Lex == PROCEDURE)
	{
		s = ProcDecRD();
	}


	if (typeP != nullptr)
	{
		typeP->sibling = varP;
		if (varP != nullptr)
		{
			varP->sibling = s;
		}
		return typeP;
	}
	else if (varP != nullptr)
	{
		varP->sibling = s;
		return varP;
	}
	else
	{
		return s;
	}
}

//5.�������������������
TreeNode* TypeDecRD()
{
	TreeNode* croot = new TreeNode();
	point++;
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		croot->child[i] = nullptr;
	}
	croot->nodekind = DecK;

	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == TYPE)
	{
		return TypeDeclarationRD();
	}
	else if (ctoken->word.Lex == VAR)
		return nullptr;
	else if (ctoken->word.Lex == PROCEDURE)
		return nullptr;
	else if (ctoken->word.Lex == BEGIN)
	{
		return nullptr;
	}
	else
	{
		rdNext++;
		return nullptr;
	}
}

//6.���������е���������
TreeNode* TypeDeclarationRD()
{
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == TYPE)
	{
		rdNext++;
		TreeNode* t = TypeDeclistRD();
		if (t == nullptr)
		{
			message += TokenList[rdNext - 1]->Lineshow + ":declaration wrong \n";
		}
		return t;
	}
	return nullptr;
}

//7.���������е���������
TreeNode* TypeDeclistRD()
{
	TreeNode* t = new TreeNode();
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		t->child[i] = nullptr;
	}
	point++;
	t->nodekind = DecK;
	if (TokenList[rdNext]->word.Lex != ID)
	{
		message += TokenList[rdNext]->Lineshow + ":expect ID\n";
	}
	else
	{
		typeIdRD(t);
	}

	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex != EQ)
	{
		rdNext--;
		message += TokenList[rdNext - 1]->Lineshow + ":expect \"=\"\n";
	}
	typeDefRD(t);

	ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex != SEMI)
	{
		rdNext--;
		message += TokenList[rdNext - 1]->Lineshow + ":expect \";\"\n";
	}
	TreeNode* p = TYpeDecMoreRD();
	if (p != nullptr)
	{
		t->sibling = p;
	}

	return t;
}

//8.���������е���������
TreeNode* TYpeDecMoreRD()
{
	TreeNode* tn = nullptr;
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == VAR || ctoken->word.Lex == PROCEDURE || ctoken->word.Lex == BEGIN)
	{
		return tn;
	}
	else
	{
		if (ctoken->word.Lex == ID)
		{
			return TypeDeclistRD();
		}
		else
		{
			message += TokenList[rdNext - 1]->Lineshow + ":expect \"ID\"\n";
			rdNext++;
			return TYpeDecMoreRD();
		}
	}
}

//9.�������������������������ƴ����������
void typeIdRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex == ID)
	{
		strcpy(t->name[t->idnum++] , ctoken->word.Sem.c_str());
	}
}

//10.���ʹ����������
void typeDefRD(TreeNode* t)
{
	if (t == nullptr)
	{
		return;
	}

	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == INTEGER || ctoken->word.Lex == CHAR1)
	{
		BaseTypeRD(t);
	}
	else if (ctoken->word.Lex == RECORD || ctoken->word.Lex == ARRAY)
	{
		StructureTypeRD(t);
	}
	else if (ctoken->word.Lex == ID)
	{
		t->kind.dec = IdK;
		rdNext++;
		strcpy(t->attr.type_name , ctoken->word.Sem.c_str());
	}
}

//11.�������ʹ����������
void BaseTypeRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex == INTEGER)
	{
		t->kind.dec = IntegerK;
	}
	else if (ctoken->word.Lex == CHAR1)
	{
		t->kind.dec = CharK;
	}
}

//12.�ṹ���ʹ����������
void StructureTypeRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == ARRAY)
	{
		ArrayTypeRD(t);
	}
	else if (ctoken->word.Lex == RECORD)
	{
		RecTypeRD(t);
	}
}
//13.�������͵Ĵ����������
void ArrayTypeRD(TreeNode* t)
{
	//array
	if (TokenList[rdNext]->word.Lex == ARRAY)
	{
		rdNext++;
		t->kind.dec = ArrayK;
	}
	else
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect array\n";
	}
	//[
	if (TokenList[rdNext++]->word.Lex != LMIDPAREN)
	{
		rdNext--;
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect [\n";
	}
	//number
	if (TokenList[rdNext]->word.Lex == INTC)
	{
		t->attr.ArrayAttr.low = stoi(TokenList[rdNext++]->word.Sem);
	}
	else
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect number\n";
	}
	//..
	if (TokenList[rdNext++]->word.Lex != UNDERANGE)
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect ..\n";
	}
	//number
	if (TokenList[rdNext]->word.Lex == INTC)
	{
		t->attr.ArrayAttr.up = stoi(TokenList[rdNext++]->word.Sem);
	}
	else
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect number\n";
	}
	//]
	if (TokenList[rdNext++]->word.Lex != RMIDPAREN)
	{
		rdNext--;
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect ]\n";
	}
	//of
	if (TokenList[rdNext++]->word.Lex != OF)
	{
		rdNext--;
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect of\n";
	}
	BaseTypeRD(t);
	t->attr.ArrayAttr.childtype = t->kind.dec;
	t->kind.dec = ArrayK;
}
//14.��¼���͵Ĵ����������
void RecTypeRD(TreeNode* t)
{
	//array
	if (TokenList[rdNext]->word.Lex == RECORD)
	{
		rdNext++;
		t->kind.dec = RecordK;
	}
	else
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect record\n";
	}
	TreeNode* fdl = FieldDecListRD();
	if (fdl != nullptr)
	{
		t->child[0] = fdl;
	}
	else
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":record type wrong\n";
	}

	if (TokenList[rdNext++]->word.Lex != END1)
	{
		rdNext--;
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect end\n";
	}
}

//15.��¼�����е������������������
TreeNode* FieldDecListRD()
{
	TreeNode* t = new TreeNode();
	point++;
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		t->child[i] = nullptr;
	}
	TreeNode* p = nullptr;
	t->nodekind = DecK;
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == INTEGER || ctoken->word.Lex == CHAR1)
	{
		BaseTypeRD(t);
		IdListRD(t);
		if (TokenList[rdNext++]->word.Lex != SEMI)
		{
			message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect ;\n";
			rdNext--;
		}
		p = FieldDecMoreRD();
	}
	else if (ctoken->word.Lex == ARRAY)
	{
		ArrayTypeRD(t);
		IdListRD(t);
		if (TokenList[rdNext++]->word.Lex != SEMI)
		{
			message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect ;\n";
			rdNext--;
		}
		p = FieldDecMoreRD();
	}
	if (p != nullptr)
	{
		t->sibling = p;
	}
	return t;
}

//16.��¼�����е����������������������
TreeNode* FieldDecMoreRD()
{
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == END1)
	{
		return nullptr;
	}
	else if (ctoken->word.Lex == INTEGER || ctoken->word.Lex == CHAR1)
	{
		return FieldDecListRD();
	}
	return nullptr;
}
//17.��¼�������б�ʶ���������������
void IdListRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex == ID)
	{
		strcpy(t->name[t->idnum++] , ctoken->word.Sem.c_str());
	}
	IdMoreRD(t);
}
//18.��¼��������������ʶ���������������
void IdMoreRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == SEMI)
	{
		return;
	}
	if (ctoken->word.Lex == COMMA)
	{
		rdNext++;
		IdListRD(t);
	}

}


//19.�������������������
TreeNode* VarDecRD()
{
	TreeNode* t = nullptr;
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == PROCEDURE || ctoken->word.Lex == BEGIN)
	{
		return t;
	}
	else if (ctoken->word.Lex == VAR)
	{
		t = VarDeclarationRD();
		return t;
	}
	else
	{
		rdNext++;
		return VarDeclarationRD();
	}
}
//20.��������������
TreeNode* VarDeclarationRD()
{
	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex == VAR)
	{
		return VarDecListRD();
	}
	else
	{
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect var\n";
		return nullptr;
	}
}
//21.�����������ִ������
TreeNode* VarDecListRD()
{
	TreeNode* t = new TreeNode();
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		t->child[i] = nullptr;
	}
	point++;
	t->nodekind = DecK;
	TreeNode* p = nullptr;
	typeDefRD(t);
	VarIdListRD(t);
	if (TokenList[rdNext++]->word.Lex != SEMI)
	{
		rdNext--;
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect ;\n";
	}
	p = VarDecMoreRD();
	if (p != nullptr)
	{
		t->sibling = p;
	}
	return t;
}
//22.�����������ִ������
TreeNode* VarDecMoreRD()
{
	Token* ctoken = TokenList[rdNext];
	TreeNode* t = nullptr;
	if (ctoken->word.Lex == PROCEDURE || ctoken->word.Lex == BEGIN)
	{
		return nullptr;
	}
	else if (ctoken->word.Lex == INTEGER || ctoken->word.Lex == CHAR1 ||
		ctoken->word.Lex == ARRAY || ctoken->word.Lex == RECORD ||
		ctoken->word.Lex == ID)
	{
		t = VarDecListRD();
	}
	return t;
}
//23.�����������ֱ��������ִ������
void VarIdListRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex == ID)
	{
		strcpy(t->name[t->idnum++] , ctoken->word.Sem.c_str());
	}
	else
	{
		rdNext--;
		message += to_string(TokenList[rdNext - 1]->Lineshow) + ":expect ID\n";
	}
	VarIdMoreRD(t);
}
//24.�����������ֱ��������ִ������
void VarIdMoreRD(TreeNode* t)
{
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == SEMI)
	{
		return;
	}
	if (ctoken->word.Lex == COMMA)
	{
		rdNext++;
		VarIdListRD(t);
	}
}
//25.�������������ܵĴ����������
TreeNode* ProcDecRD()
{
	TreeNode* t = nullptr;
	Token* ctoken = TokenList[rdNext];
	if (ctoken->word.Lex == BEGIN)
	{
		return t;
	}
	else if (ctoken->word.Lex == PROCEDURE)
	{
		t = ProcDeclarationRD();
		if (t != nullptr)
		{
			t->sibling = ProcDecRD();
		}
		return t;
	}
	else
	{
		rdNext++;
		return nullptr;
	}
}
//26.�����������־���Ĵ����������
TreeNode* ProcDeclarationRD()
{
	TreeNode* t = new TreeNode();
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		t->child[i] = nullptr;
	}
	point++;
	t->nodekind = ProcDecK;
	Token* ctoken = TokenList[rdNext++];
	if (ctoken->word.Lex == PROCEDURE)
	{
		//id
		if (TokenList[rdNext]->word.Lex == ID)
		{
			strcpy(t->name[t->idnum++] , TokenList[rdNext]->word.Sem.c_str());
			rdNext++;
		}
		else
		{
			message += to_string(TokenList[rdNext]->Lineshow) + ":expect ID\n";
		}
		//(
		if (TokenList[rdNext++]->word.Lex != LPAREN)
		{
			rdNext--;
			message += to_string(TokenList[rdNext]->Lineshow) + ":expect (\n";
		}
		//para
		ParamListRD(t);
		//)
		if (TokenList[rdNext++]->word.Lex != RPAREN)
		{
			rdNext--;
			message += to_string(TokenList[rdNext]->Lineshow) + ":expect )\n";
		}
		//;
		if (TokenList[rdNext++]->word.Lex != SEMI)
		{
			rdNext--;
			message += to_string(TokenList[rdNext]->Lineshow) + ":expect ;\n";
		}
		t->child[1] = ProcDecPartRD();
		t->child[2] = procBodyRD();
		t->child[2]->nodekind = StmLK;
	}
	return t;
}
//27.���������еĲ����������ֵĴ����������
void ParamListRD(TreeNode* t)
{
	if (TokenList[rdNext]->word.Lex == INTEGER || TokenList[rdNext]->word.Lex == CHAR1 ||
		TokenList[rdNext]->word.Lex == ARRAY || TokenList[rdNext]->word.Lex == RECORD ||
		TokenList[rdNext]->word.Lex == ID || TokenList[rdNext]->word.Lex == VAR)
	{
		t->child[0] = ParamDecListRD();
	}
}
//28.���������еĲ��������������ֵĴ����������
TreeNode* ParamDecListRD()
{
	TreeNode* t = ParamRD();
	if (t == nullptr)
	{
		return nullptr;
	}
	t->sibling = ParamMoreRD();
	return t;
}
//29.���������еĲ��������������ֵĴ����������
TreeNode* ParamMoreRD()
{
	//)
	if (TokenList[rdNext]->word.Lex == RPAREN)
	{
		rdNext++;
		return nullptr;
	}
	//;
	if (TokenList[rdNext]->word.Lex == SEMI)
	{
		rdNext++;
		return ParamDecListRD();
		
	}
	else
	{
		message += to_string(TokenList[rdNext]->Lineshow) + ":expect )\n";
		return nullptr;
	}
}
//30.���������еĲ��������������ֵĴ����������
TreeNode* ParamRD()
{
	TreeNode* t = new TreeNode();
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		t->child[i] = nullptr;
	}
	point++;
	t->nodekind = DecK;
	Token* ctoken = TokenList[rdNext];
	if (TokenList[rdNext]->word.Lex == INTEGER || TokenList[rdNext]->word.Lex == CHAR1 ||
		TokenList[rdNext]->word.Lex == ARRAY || TokenList[rdNext]->word.Lex == RECORD ||
		TokenList[rdNext]->word.Lex == ID)
	{
		//value

		typeDefRD(t);
		FormListRD(t);
		t->attr.ProcAttr.paramt = valparamType;
	}
	else if (ctoken->word.Lex == VAR)
	{
		//var

		rdNext++;
		typeDefRD(t);
		FormListRD(t);
		t->attr.ProcAttr.paramt = varparamType;
	}
	return t;
}
//31.���������еĲ��������в��������ֵĴ����������
void FormListRD(TreeNode* t)
{
	if (TokenList[rdNext]->word.Lex == ID)
	{
		strcpy(t->name[0] , TokenList[rdNext]->word.Sem.c_str());
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "ȱ��ID\n";
	}
	FidMoreRD(t);
}
//32.���������еĲ��������в��������ֵĴ����������
void FidMoreRD(TreeNode* t)
{
	if (TokenList[rdNext]->word.Lex == SEMI || TokenList[rdNext]->word.Lex == RPAREN)
	{
		return;
	}
	else if (TokenList[rdNext]->word.Lex == COMMA)
	{
		rdNext++;
		FormListRD(t);
	}
}
//33.�������������ֵķ����������
TreeNode* ProcDecPartRD(void)
{
	return DeclarePartRD();
}
//34.�����岿�ִ����������
TreeNode* procBodyRD(void)
{
	TreeNode* tem = ProgramBodyRD();
	if (tem == NULL)
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "���������\n";
	}
	return tem;
}
//35.�������岿�ִ����������
TreeNode* ProgramBodyRD(void)
{
	TreeNode* StmLK1 = new(TreeNode);//�µ�����־���ͽڵ�
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		StmLK1->child[i] = nullptr;
	}
	if (TokenList[rdNext]->word.Lex == BEGIN)
	{
		StmLK1->nodekind = StmLK;
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���begin";
	}
	StmLK1->child[0] = StmListRD();
	if (TokenList[rdNext]->word.Lex == END1)
	{
		return  StmLK1;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���end";
	}
}
//36.������в��ִ���������� 
TreeNode* StmListRD(void)
{
	TreeNode* StmNode_T = new(TreeNode);//�µ�����־���ͽڵ�
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		StmNode_T->child[i] = nullptr;
	}
	point++;
	TreeNode* StmNode_P = new(TreeNode);//�µ�����־���ͽڵ�
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		StmNode_P->child[i] = nullptr;
	}
	point++;
	StmNode_T = StmRD();
	StmNode_T->nodekind = StmtK;
	StmNode_P = StmMoreRD();
	if (StmNode_P != nullptr)
	{
		StmNode_T->sibling = StmNode_P;
	}

	return StmNode_T;


}
//37��������䲿�ִ����������
TreeNode* StmMoreRD(void)
{
	TreeNode* Tem = nullptr;

	if (TokenList[rdNext]->word.Lex == END1)
	{
		return Tem;
	}
	else if (TokenList[rdNext]->word.Lex == ENDWH)
	{
		rdNext++;
		return Tem;
	}
	else if (TokenList[rdNext]->word.Lex == SEMI)
	{
		rdNext++;
		Tem = StmListRD();
		return Tem;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "ȱ��; \n";
	}
}
//38�����ݹ鴦���������
TreeNode* StmRD(void)
{
	TreeNode* p = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		p->child[i] = nullptr;
	}
	point++;
	if (TokenList[rdNext]->word.Lex == IF)
	{

		p = ConditionalStmRD();
		p->kind.stmt = IfK;
		return p;
	}
	else if (TokenList[rdNext]->word.Lex == WHILE)
	{
		p = LoopStmRD();
		p->kind.stmt = WhileK;
		return p;
	}
	else if (TokenList[rdNext]->word.Lex == RETURN1)
	{
		p = ReturnStmRD();
		p->kind.stmt = ReturnK;
		return p;
	}
	else if (TokenList[rdNext]->word.Lex == READ)
	{
		p = InputStmRD();
		p->kind.stmt = ReadK;
		return p;
	}
	else if (TokenList[rdNext]->word.Lex == WRITE)
	{
		p = OutputStmRD();
		p->kind.stmt = WriteK;
		return p;
	}
	else if (TokenList[rdNext]->word.Lex == ID)
	{
		p = AssCallRD();//rdNextδ��һ
		p->kind.stmt = AssignK;
		p->nodekind = StmtK;
		return p;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += ":��ǰ����Ϊ���������ʣ������﷨����\n";

		return nullptr;
	}
}
//39����ֵ���ͺ���������䲿�ֵĴ����������
TreeNode* AssCallRD(void)
{
	TreeNode* p = ExpRD();//Ӧ�ô�id
	TreeNode* tem = nullptr;
	if (TokenList[rdNext]->word.Lex == ASSIGN)
	{
		tem = AssignmentRestRD();
		tem->child[0] = p;
		return tem;
	}
	else if (TokenList[rdNext]->word.Lex == LPAREN)
	{

		tem = CallStmRestRD();
		tem->child[1] = tem->child[0];
		tem->child[0] = p;
		return tem;
	}

	return nullptr;
}
//40����ֵ��䴦���������
TreeNode* AssignmentRestRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	if (tem != nullptr)
	{
		rdNext++;
		tem->child[0] = nullptr;
		tem->child[1] = ExpRD();
		return tem;
	}
}
//41��������䴦���������
TreeNode* ConditionalStmRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	if (TokenList[rdNext]->word.Lex == IF)
	{
		rdNext++;
		tem->child[0] = ExpRD();

	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���if\n";
	}

	if (TokenList[rdNext]->word.Lex == THEN)
	{

		rdNext++;
		tem->child[1] = StmRD();
		tem->child[1]->nodekind = StmtK;

	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���then\n";
	}
	if (TokenList[rdNext]->word.Lex == DOT)
	{
		rdNext++;
		rdNext++;
	}
	if (TokenList[rdNext]->word.Lex == ELSE)
	{
		rdNext++;
		tem->child[2] = StmRD();
		tem->sibling = StmMoreRD();

	}
	if (TokenList[rdNext]->word.Lex == DOT)
	{
		rdNext++;
		rdNext++;
	}
	if (TokenList[rdNext]->word.Lex != FI)
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���FI\n";
	}
	else
	{
		rdNext++;
	}
	return tem;
}
//42��ѭ����䴦���������
TreeNode* LoopStmRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	if (TokenList[rdNext]->word.Lex == WHILE)
	{
		rdNext++;
		tem->child[0] = ExpRD();
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���While\n";
	}
	if (TokenList[rdNext]->word.Lex == DO)
	{
		rdNext++;
		tem->child[1] = StmListRD();
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���DO\n";
	}
	return tem;
}
//43���������Ĵ����������
TreeNode* InputStmRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	tem->nodekind = StmtK;
	tem->kind.stmt = ReadK;
	if (TokenList[rdNext]->word.Lex == READ)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���Read\n";
	}
	if (TokenList[rdNext]->word.Lex == LPAREN)
	{

		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���(\n";
	}
	if (TokenList[rdNext]->word.Lex == ID)
	{
		strcpy(tem->name[0] , TokenList[rdNext]->word.Sem.c_str());
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���ID\n";
	}
	if (TokenList[rdNext]->word.Lex == RPAREN)
	{

		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���)\n";
	}
	return tem;
}
//44�������䴦���������
TreeNode* OutputStmRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	tem->nodekind = StmtK;
	tem->kind.stmt = WriteK;
	if (TokenList[rdNext]->word.Lex == WRITE)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���Write\n";
	}
	if (TokenList[rdNext]->word.Lex == LPAREN)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���(\n";
	}
	tem->child[0] = ExpRD();
	if (TokenList[rdNext]->word.Lex == RPAREN)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���)\n";
	}
	return tem;
}
//45�����̷�����䴦���������
TreeNode* ReturnStmRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	tem->nodekind = StmtK;
	tem->kind.stmt = ReturnK;
	if (TokenList[rdNext]->word.Lex == RETURN1)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���RETURN\n";
	}
	return tem;
}
//46�����̵�����䴦���������
TreeNode* CallStmRestRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	tem->nodekind = StmLK;
	tem->kind.stmt = CallK;
	if (TokenList[rdNext]->word.Lex == LPAREN)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���(\n";
	}
	tem->child[0] = ActParamListRD();
	tem->child[0]->nodekind = ExpK;
	if (TokenList[rdNext]->word.Lex == RPAREN)
	{
		rdNext++;
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���)\n";
	}
	return tem;
}
//47��ʵ�β��ִ����������
TreeNode* ActParamListRD(void)
{
	TreeNode* tem = nullptr;
	if (TokenList[rdNext]->word.Lex == RPAREN)
	{
		return nullptr;
	}
	else if (TokenList[rdNext]->word.Lex == ID || TokenList[rdNext]->word.Lex == INTC)
	{
		tem = ExpRD();
		if (tem != nullptr)
		{
			tem->sibling = ActParamMoreRD();
			return tem;
		}
	}
	return tem;
}
//48������ʵ�β��ִ����������
TreeNode* ActParamMoreRD(void)
{
	TreeNode* tem = nullptr;
	if (TokenList[rdNext]->word.Lex == RPAREN)
	{
		return nullptr;
	}
	else if (TokenList[rdNext]->word.Lex == COMMA)
	{
		rdNext++;
		return ActParamListRD();
	}
	return nullptr;
}
//49�����ʽ�ݹ鴦���������
TreeNode* ExpRD(void)
{

	TreeNode* tem = Simple_expRD();
	if (TokenList[rdNext]->word.Lex == ID || TokenList[rdNext]->word.Lex == LT || TokenList[rdNext]->word.Lex == INTC || TokenList[rdNext - 1]->word.Lex == ID)
	{

		point++;
		tem->nodekind = ExpK;
		if (TokenList[rdNext - 1]->word.Lex == ID && TokenList[rdNext]->word.Lex != LT)
		{
			//tem->name[0] = TokenList[rdNext-1]->word.Sem;
			tem->attr.ExpAttr.varkind = IdV;

		}
		else
		{

			tem->nodekind = ExpK;
			tem->kind.exp = OpK;

			if (TokenList[rdNext]->word.Lex == LT)
			{
				rdNext++;
				strcpy(tem->name[0] , "");
				tem->attr.ExpAttr.op = LT;
			}
			if (TokenList[rdNext]->word.Lex == ID)
			{
				strcpy(tem->name[1] , TokenList[rdNext]->word.Sem.c_str());
				rdNext++;
			}
			else if (TokenList[rdNext]->word.Lex == INTC)
			{
				TreeNode* ss = new TreeNode();
				for (int i = 0; i < MAXCHILDREN; i++)
				{
					ss->child[i] = nullptr;
				}
				TreeNode* ss2 = new TreeNode();
				for (int i = 0; i < MAXCHILDREN; i++)
				{
					ss2->child[i] = nullptr;
				}
				strcpy(ss->name[0] , TokenList[rdNext]->word.Sem.c_str());
				ss->kind.exp = ConstK;
				ss->nodekind = ExpK;
				strcpy(ss2->name[0] , TokenList[rdNext - 2]->word.Sem.c_str());
				ss->kind.exp = VariK;
				ss->attr.ExpAttr.varkind = IdV;
				tem->child[0] = ss2;
				ss2->nodekind = ExpK;
				ss2->sibling = ss;
				rdNext++;
			}
		}

	}
	if ((TokenList[rdNext]->word.Lex == PLUS || TokenList[rdNext]->word.Lex == MINUS || TokenList[rdNext]->word.Lex == TIMES || TokenList[rdNext]->word.Lex == OVER || TokenList[rdNext]->word.Lex == INTC) || (TokenList[rdNext]->word.Lex == LPAREN && TokenList[rdNext + 1]->word.Lex != ID && TokenList[rdNext + 2]->word.Lex != COMMA))
		tem->sibling = ExpRD();
	return tem;
}
//50���򵥱��ʽ�ݹ鴦���������
TreeNode* Simple_expRD(void)
{

	TreeNode* tem = TermRD();
	if (TokenList[rdNext]->word.Lex == PLUS || TokenList[rdNext]->word.Lex == MINUS)
	{
		TreeNode* op = new TreeNode();
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			op->child[i] = nullptr;
		}
		point++;
		op->nodekind = ExpK;
		if (TokenList[rdNext]->word.Lex == PLUS)
		{
			op->kind.exp = OpK;
			op->attr.ExpAttr.op = PLUS;
		}
		else {
			op->kind.exp = OpK;
			op->attr.ExpAttr.op = MINUS;
		}
		rdNext++;
		return op;
	}
	return tem;
}
//51����ݹ鴦���������
TreeNode* TermRD(void)
{

	TreeNode* tem1 = FactorRD();
	if (TokenList[rdNext]->word.Lex == TIMES || TokenList[rdNext]->word.Lex == OVER)
	{
		TreeNode* tem2 = new(TreeNode);
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			tem2->child[i] = nullptr;
		}
		point++;
		tem2->nodekind = ExpK;
		if (TokenList[rdNext]->word.Lex == TIMES)
		{
			tem2->kind.exp = OpK;
			tem2->attr.ExpAttr.op = TIMES;
		}
		else {
			tem2->kind.exp = OpK;
			tem2->attr.ExpAttr.op = OVER;
		}
		tem2->child[0] = tem1;
		rdNext++;
		tem2->child[1] = ExpRD();
		return tem1;
	}
	return tem1;
}
//52�����ӵݹ鴦���������
TreeNode* FactorRD(void)
{
	TreeNode* tem = nullptr;
	TreeNode* a = nullptr;
	if (TokenList[rdNext]->word.Lex == INTC)
	{
		TreeNode* p = new TreeNode();
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			p->child[i] = nullptr;
		}
		point++;
		p->nodekind = ExpK;
		return p;
	}
	else if (TokenList[rdNext]->word.Lex == ID)
	{
		a = VariableRD();
		return a;
	}
	else if (TokenList[rdNext]->word.Lex == LPAREN)
	{
		rdNext++;
		tem = ExpRD();
		if (TokenList[rdNext]->word.Lex == RPAREN)
		{
			rdNext++;
		}
		else
		{
			message += "" + to_string(TokenList[rdNext]->Lineshow);
			message += "��ȱ�ٹؼ���)\n";
		}
		return tem;
	}
	return tem;
}
//53�������������
TreeNode* VariableRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	tem->nodekind = ExpK;
	if (TokenList[rdNext]->word.Lex == ID)
	{
		strcpy(tem->name[0] , TokenList[rdNext]->word.Sem.c_str());
		rdNext++;
		VariMoreRD(tem);
		return tem;
	}
	else
	{
		return nullptr;
	}

}
//54���������ಿ�ִ������
void VariMoreRD(TreeNode* t)
{
	if (TokenList[rdNext]->word.Lex == LMIDPAREN)
	{
		rdNext++;
		t->child[0] = ExpRD();
		if (TokenList[rdNext]->word.Lex == RMIDPAREN)
		{
			rdNext++;
		}
		else
		{
			message += "" + to_string(TokenList[rdNext]->Lineshow);
			message += "��ȱ�ٹؼ���]\n";
		}
	}
	else if (TokenList[rdNext]->word.Lex == DOT)
	{
		if (rdNext > TokenList.size() - 1)
		{
			return;
		}
		rdNext++;
		t->child[0] = FieldvarRD();
	}
	else if (
		TokenList[rdNext]->word.Lex == RPAREN ||
		TokenList[rdNext]->word.Lex == COMMA ||
		TokenList[rdNext]->word.Lex == THEN ||
		TokenList[rdNext]->word.Lex == ELSE ||
		TokenList[rdNext]->word.Lex == FI ||
		TokenList[rdNext]->word.Lex == DO ||
		TokenList[rdNext]->word.Lex == ENDWH ||
		TokenList[rdNext]->word.Lex == END1)
	{
		rdNext++;
		return;
	}
	else if (TokenList[rdNext]->word.Lex == ASSIGN)
	{
		return;
	}
	else if (TokenList[rdNext]->word.Lex ==SEMI)
	{
		
		return;
	}
	else if (TokenList[rdNext]->word.Lex == TIMES ||
		TokenList[rdNext]->word.Lex == EQ ||
		TokenList[rdNext]->word.Lex == LT ||
		TokenList[rdNext]->word.Lex == PLUS ||
		TokenList[rdNext]->word.Lex == MINUS ||
		TokenList[rdNext]->word.Lex == OVER  )
	{
		t->attr.ExpAttr.op = TokenList[rdNext]->word.Lex;
		rdNext++;
		return;
	}
}
//55����������ִ������
TreeNode* FieldvarRD(void)
{
	TreeNode* tem = new(TreeNode);
	for (int i = 0; i < MAXCHILDREN; i++)
	{
		tem->child[i] = nullptr;
	}
	point++;
	tem->nodekind = ExpK;
	if (TokenList[rdNext]->word.Lex == ID)
	{
		strcpy(tem->name[0] , TokenList[rdNext]->word.Sem.c_str());
		rdNext++;
		FieldvarMoreRD(tem);
	}
	else
	{
		message += "" + to_string(TokenList[rdNext]->Lineshow);
		message += "��ȱ�ٹؼ���ID\n";
	}
	return tem;
}
//56��������������ִ������
void FieldvarMoreRD(TreeNode* t)
{
	if (TokenList[rdNext]->word.Lex == ASSIGN)
	{
		return;
	}
	else if (TokenList[rdNext]->word.Lex == LMIDPAREN)
	{
		t->sibling = ExpRD();
	}
}
//57���ռ���ƥ�䴦�����
TreeNode* ParseRD()
{
	return nullptr;
}

