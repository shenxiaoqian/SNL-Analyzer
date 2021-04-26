#include"semantic.h"
//#include"global.h"

Semantic::Semantic()
{
	Level = -1;

	intPtr = NULL;
	charPtr = NULL;
	boolPtr = NULL;
}
void Semantic::initialize(void)
{
	intPtr = NewTy(intTy);
	charPtr = NewTy(charTy);
	boolPtr = NewTy(boolTy);
	for (int i = 0; i < SCOPESIZE; i++)
		scope[i] = NULL;
}
/*��ӡ��*/
void  Semantic::PrintOneLayer(int level)
{
	SymbTable* t = scope[level];
	//fprintf(output, "��%d�㣺\n", level);

	while (t != NULL)
	{
		fprintf(output, "%s:   ", t->idName);
		AttributeIR* Attrib = &(t->attrIR);
		if (Attrib->idtype != NULL)
			switch (Attrib->idtype->kind)
			{
			case  intTy:  fprintf(output, "intTy  ");   break;
			case  charTy:	fprintf(output, "charTy  ");  break;
			case  arrayTy: fprintf(output, "arrayTy  "); break;
			case  recordTy:fprintf(output, "recordTy  "); break;
			default: fprintf(output, "���ʹ���"); break;
			}
		switch (Attrib->kind)
		{
		case  typeKind:
			fprintf(output, "typekind  "); break;
		case  varKind:
			fprintf(output, "varkind  ");
			fprintf(output, "������ %d  ", Attrib->More.VarAttr.level);


			switch (Attrib->More.VarAttr.access)
			{
			case  dir:  fprintf(output, "dir  "); break;
			case  indir: fprintf(output, "indir  "); break;
			default:fprintf(output, "���ʹ���  ");  break;
			}
			break;
		case  procKind:
			fprintf(output, "funckind   ");
			fprintf(output, "������%d  ", Attrib->More.ProcAttr.level);
			break;
		default:fprintf(output, "���ʹ��� ");
		}
		fprintf(output, "\n");
		t = t->next;
	}
}

void   Semantic::PrintSymbTable()
{
	int  level = 0;
	while (scope[level] != NULL)
	{
		PrintOneLayer(level);
		level++;
	}
}

SymbTable* Semantic::NewTable(void)
{
	SymbTable* table = (SymbTable*)malloc(sizeof(SymbTable));
	if (table == NULL)
	{
		ErrorFlag = true;
	}
	table->next = NULL;
	table->attrIR.kind = typeKind;
	table->attrIR.idtype = NULL;
	table->next = NULL;
	table->attrIR.More.VarAttr.isParam = false;
	return table;
}
/*�����ʶ��������*/
int  Semantic::Enter(char* id, AttributeIR* attribP, SymbTable** entry)
{
	int present = false;
	int result = 1;
	SymbTable* curentry = scope[Level];
	SymbTable* prentry = scope[Level];

	if (scope[Level] == NULL)
	{
		curentry = NewTable();
		scope[Level] = curentry;
	}
	else
	{
		while (curentry != NULL)
		{
			prentry = curentry;
			result = strcmp(id, curentry->idName);
			if (result == 0)
			{
				fprintf(output, "��������ظ�����");
				ErrorFlag = true;
				present = true;
				break;
			}
			else
				curentry = (prentry->next);
		}

		if (present == false)
		{
			curentry = NewTable();
			prentry->next = curentry;
		}
	}
	strcpy(curentry->idName, id);

	curentry->attrIR.idtype = attribP->idtype;
	curentry->attrIR.kind = attribP->kind;
	switch (attribP->kind)
	{
	case  typeKind: break;
	case  varKind:
		curentry->attrIR.More.VarAttr.level = attribP->More.VarAttr.level;
		curentry->attrIR.More.VarAttr.off = attribP->More.VarAttr.off;
		curentry->attrIR.More.VarAttr.access = attribP->More.VarAttr.access;
		break;
	case  procKind:
		curentry->attrIR.More.ProcAttr.level = attribP->More.ProcAttr.level;
		curentry->attrIR.More.ProcAttr.param = attribP->More.ProcAttr.param;
		break;
	default:break;
	}
	(*entry) = curentry;

	return present;
}
/*Ѱ�ұ�ʶ�������ַ*/
int Semantic::FindEntry(char* id, SymbTable** entry)
{
	int present = false;
	int result = 1;
	int lev = Level;

	SymbTable* findentry = scope[lev];

	while ((lev != -1) && (present != true))
	{
		//Ѱ�ұ�ʶ��
		while ((findentry != NULL) && (present != true))
		{
			result = strcmp(id, findentry->idName);
			if (result == 0)
				present = true;
			else
				findentry = findentry->next;
		}
		if (present != true)
		{
			lev = lev - 1;
			findentry = scope[lev];

		}
	}
	if (present != true)
	{
		(*entry) = NULL;
	}
	else
		(*entry) = findentry;

	return present;
}
/*�ж������Ƿ���ͬ*/
int   Semantic::Compat(TypeIR* tp1, TypeIR* tp2)
{
	int  present;
	if (tp1 != tp2)
		present = false;
	else
		present = true;
	return present;
}
/*���������͵��ڲ���ʾ */
TypeIR* Semantic::NewTy(TypeKind  kind)
{
	TypeIR* table = (TypeIR*)malloc(sizeof(TypeIR));
	if (table == NULL)
	{
		fprintf(output, "�ڴ����ʧ��");
	}
	else
		switch (kind)
		{
		case intTy:
		case charTy:
		case boolTy:
			table->kind = kind;
			table->size = 1;
			break;
		case arrayTy:
			table->kind = arrayTy;
			table->More.ArrayAttr.indexTy = NULL;
			table->More.ArrayAttr.elemTy = NULL;
			break;
		case recordTy:
			table->kind = recordTy;
			table->More.body = NULL;
			break;
		}
	return table;
}
/*�����ռ�¼���͵����� */
fieldChain* Semantic::NewBody(void)
{
	fieldChain* Ptr = (fieldChain*)malloc(sizeof(fieldChain));

	if (Ptr == NULL)
	{
		fprintf(output, "�ڴ����ʧ��");
	}
	else
	{
		Ptr->Next = NULL;
		Ptr->UnitType = NULL;
	}
	return Ptr;
}
/*�������β�����*/
ParamTable* Semantic::NewParam(void)
{
	ParamTable* Ptr = (ParamTable*)malloc(sizeof(ParamTable));

	if (Ptr == NULL)
	{
		fprintf(output, "�ڴ����ʧ��");
	}
	else
	{
		Ptr->entry = NULL;
		Ptr->next = NULL;
	}
	return Ptr;
}
void Semantic::ErrorPrompt(int line, const char* name, const char* message)
{
	fprintf(all, "��%d�г����������%s %s", line, name, message);
	ErrorFlag = true;
}
/* ���Ҽ�¼������	*/
bool  Semantic::FindField(char* Id, fieldChain* head, fieldChain** Entry)
{
	bool  present = false;
	/*��¼��ǰ�ڵ�*/
	fieldChain* currentItem = head;
	while ((currentItem != NULL) && (present == false))
	{
		if (strcmp(currentItem->id, Id) == 0)
		{
			present = true;
			if (Entry != NULL)
				(*Entry) = currentItem;
		}
		else  currentItem = currentItem->Next;
	}
	return(present);
}
/*���ͷ���*/
TypeIR* Semantic::TypeProcess(TreeNode* t, DecKind deckind)
{
	TypeIR* Ptr = NULL;
	switch (deckind)
	{
	case IdK:
		Ptr = nameType(t); break;
	case ArrayK:
		Ptr = arrayType(t); break;
	case RecordK:
		Ptr = recordType(t); break;
	case IntegerK:
		Ptr = intPtr; break;
	case CharK:
		Ptr = charPtr; break;
	}
	return Ptr;
}
/*�ڷ��ű���Ѱ���Ѷ������������*/
TypeIR* Semantic::nameType(TreeNode* t)
{
	TypeIR* Ptr = NULL;
	SymbTable* entry = NULL;
	int present;
	present = FindEntry(t->attr.type_name, &entry);

	if (present == true)
	{
		/*���ñ�ʶ���Ƿ�Ϊ���ͱ�ʶ��*/
		if (entry->attrIR.kind != typeKind)
			ErrorPrompt(t->lineno, t->attr.type_name, "������ǰʹ��\n");
		else
			Ptr = entry->attrIR.idtype;
	}
	else/*û���ҵ��ñ�ʶ��*/
	{
		ErrorPrompt(t->lineno, t->attr.type_name, "������δ����\n");
	}
	return Ptr;
}
/*�����������͵��ڲ���ʾ*/
TypeIR* Semantic::arrayType(TreeNode* t)
{
	TypeIR* Ptr0 = NULL;
	TypeIR* Ptr1 = NULL;
	TypeIR* Ptr = NULL;

	/*��������Ͻ��Ƿ�С���½�*/
	if ((t->attr.ArrayAttr.low) > (t->attr.ArrayAttr.up))
	{
		ErrorPrompt(t->lineno, "", "��������Խ�����\n");
		ErrorFlag = true;
	}
	else
	{
		Ptr0 = TypeProcess(t, IntegerK);
		/*�������ͷ��������������±�����*/
		Ptr1 = TypeProcess(t, t->attr.ArrayAttr.childtype);
		/*�������ͷ�������������Ԫ������*/
		Ptr = NewTy(arrayTy);
		/*ָ��һ�´�����������Ϣ��*/
		Ptr->size = ((t->attr.ArrayAttr.up) - (t->attr.ArrayAttr.low) + 1) * (Ptr1->size);
		/*���㱾���ͳ���*/

/*��д������Ϣ*/
		Ptr->More.ArrayAttr.indexTy = Ptr0;
		Ptr->More.ArrayAttr.elemTy = Ptr1;
		Ptr->More.ArrayAttr.low = t->attr.ArrayAttr.low;
		Ptr->More.ArrayAttr.up = t->attr.ArrayAttr.up;
	}
	return Ptr;
}
/*�����¼���͵��ڲ���ʾ*/
TypeIR* Semantic::recordType(TreeNode* t)
{
	TypeIR* Ptr = NewTy(recordTy);
	t = t->child[0];
	fieldChain* Ptr2 = NULL;
	fieldChain* Ptr1 = NULL;
	fieldChain* body = NULL;
	while (t != NULL)
	{
		for (int i = 0; i < t->idnum; i++)
		{
			Ptr2 = NewBody();
			if (body == NULL)
				body = Ptr1 = Ptr2;
			strcpy(Ptr2->id, t->name[i]);
			Ptr2->UnitType = TypeProcess(t, t->kind.dec);

			Ptr2->Next = NULL;
			if (Ptr2 != Ptr1)
			{
				Ptr1->Next = Ptr2;
				Ptr1 = Ptr2;
			}
		}
		t = t->sibling;
	}
	Ptr->More.body = body;

	return Ptr;
}
/*���������������������*/
void Semantic::TypeDecPart(TreeNode* t)
{
	bool present = false;
	AttributeIR  attrIr;
	SymbTable* entry = NULL;
	attrIr.kind = typeKind;
	while (t != NULL)
	{
		present = Enter(t->name[0], &attrIr, &entry);
		if (present == true)
		{
			ErrorPrompt(t->lineno, t->name[0], "�ظ�����\n");
			entry = NULL;
		}
		else
			entry->attrIR.idtype = TypeProcess(t, t->kind.dec);
		t = t->sibling;
	}
}
/*�����������������*/
void Semantic::VarDecPart(TreeNode* t)
{
	varDecList(t);
}

void  Semantic::varDecList(TreeNode* t)
{
	AttributeIR  attrIr;
	int present = false;

	SymbTable* entry = NULL;

	while (t != NULL)
	{
		attrIr.kind = varKind;
		for (int i = 0; i < (t->idnum); i++)
		{
			attrIr.idtype = TypeProcess(t, t->kind.dec);
			if (t->attr.ProcAttr.paramt == varparamType)
			{
				attrIr.More.VarAttr.access = indir;
				attrIr.More.VarAttr.level = Level;
			}

			else
			{
				attrIr.More.VarAttr.access = dir;
				attrIr.More.VarAttr.level = Level;
			}
			present = Enter(t->name[i], &attrIr, &entry);
			if (present != false)
			{
				ErrorPrompt(t->lineno, t->name[i], "�ظ�����\n");
			}
			else
				t->table[i] = entry;

		}
		if (t != NULL)
			t = t->sibling;
	}
}
/*�����������������*/
void  Semantic::procDecPart(TreeNode* t)
{
	TreeNode* p = t;
	SymbTable* entry = HeadProcess(t);

	t = t->child[1];
	while (t != NULL)
	{
		switch (t->nodekind)
		{
		case  TypeK:     TypeDecPart(t->child[0]);  break;
		case  VarK:     VarDecPart(t->child[0]);   break;
		case  ProcDecK:  break;
		default:
			ErrorPrompt(t->lineno, "", "�﷨���ڵ����ʹ���");
			break;
		}
		if (t->nodekind == ProcDecK)
			break;
		else
			t = t->sibling;
	}

	while (t != NULL)
	{
		procDecPart(t);
		t = t->sibling;
	}
	t = p;
	Body(t->child[2]);

	if (Level != -1)
		Level = Level - 1;
}
/*����ͷ*/
SymbTable* Semantic::HeadProcess(TreeNode* t)
{
	AttributeIR attrIr;
	int present = false;
	SymbTable* entry = NULL;

	/*������*/
	attrIr.kind = procKind;
	attrIr.idtype = NULL;
	attrIr.More.ProcAttr.level = Level + 1;

	if (t != NULL)
	{
		present = Enter(t->name[0], &attrIr, &entry);
		t->table[0] = entry;
	}
	entry->attrIR.More.ProcAttr.param = ParaDecList(t);

	return entry;
}
ParamTable* Semantic::ParaDecList(TreeNode* t)
{
	TreeNode* p = NULL;
	ParamTable* Ptr1 = NULL;
	ParamTable* Ptr2 = NULL;
	ParamTable* head = NULL;

	if (t != NULL)
	{
		if (t->child[0] != NULL)
			p = t->child[0];

		Level = Level + 1;
		scope[Level] = NULL;
		Off = INITOFF;


		VarDecPart(p);

		SymbTable* Ptr0 = scope[Level];

		while (Ptr0 != NULL)
		{
			Ptr2 = NewParam();
			if (head == NULL)
				head = Ptr1 = Ptr2;
			Ptr2->entry = Ptr0;
			Ptr2->next = NULL;

			if (Ptr2 != Ptr1)
			{
				Ptr1->next = Ptr2;
				Ptr1 = Ptr2;
			}
			Ptr0 = Ptr0->next;
		}
	}
	return head;
}

void Semantic::Body(TreeNode* t)
{
	if (t->nodekind == StmLK)
	{
		TreeNode* p = t->child[0];
		while (p != NULL)
		{
			statement(p);
			p = p->sibling;
		}
	}
}

void Semantic::statement(TreeNode* t)
{
	switch (t->kind.stmt)
	{
	case IfK:			ifstatment(t); break;
	case WhileK:		whilestatement(t); break;
	case AssignK:		assignstatement(t); break;
	case ReadK:		    readstatement(t); break;
	case WriteK:		writestatement(t); break;
	case CallK:		    callstatement(t); break;
	case ReturnK:		returnstatement(t); break;
	default:
		ErrorPrompt(t->lineno, "", "��������\n");
		break;
	}
}
AttributeIR Semantic::FindAttr(SymbTable* entry)
{
	AttributeIR attrIr = entry->attrIR;
	return attrIr;
}
TypeIR* Semantic::Expr(TreeNode* t, AccessKind* Ekind)
{
	int present = false;
	SymbTable* entry = NULL;

	TypeIR* Eptr0 = NULL;
	TypeIR* Eptr1 = NULL;
	TypeIR* Eptr = NULL;
	if (t != NULL)
		switch (t->kind.exp)
		{
		case ConstK:
			Eptr = TypeProcess(t, IntegerK);
			Eptr->kind = intTy;
			if (Ekind != NULL)
				(*Ekind) = dir;
			break;
		case VariK:
			if (t->child[0] == NULL)
			{
				present = FindEntry(t->name[0], &entry);
				t->table[0] = entry;

				if (present != false)
				{
					if (FindAttr(entry).kind != varKind)
					{
						ErrorPrompt(t->lineno, t->name[0], "���Ǳ���\n");
						Eptr = NULL;
					}
					else
					{
						Eptr = entry->attrIR.idtype;
						if (Ekind != NULL)
							(*Ekind) = indir;

					}
				}
				else
				{
					ErrorPrompt(t->lineno, t->name[0], "����δ����\n");
				}

			}
			else
			{
				if (t->attr.ExpAttr.varkind == ArrayMembV)
					Eptr = arrayVar(t);
				else
					if (t->attr.ExpAttr.varkind == FieldMembV)
						Eptr = recordVar(t);
			}
			break;
		case OpK:
			Eptr0 = Expr(t->child[0], NULL);
			if (Eptr0 == NULL)
				return NULL;
			Eptr1 = Expr(t->child[1], NULL);
			if (Eptr1 == NULL)
				return NULL;

			present = Compat(Eptr0, Eptr1);
			if (present != false)
			{
				switch (t->attr.ExpAttr.op)
				{
				case LT:
				case EQ:
					Eptr = boolPtr;
					break;  /*�������ʽ*/
				case PLUS:
				case MINUS:
				case TIMES:
				case OVER:
					Eptr = intPtr;
					break;  /*�������ʽ*/
				}
				if (Ekind != NULL)
					(*Ekind) = dir;
			}
			else
				ErrorPrompt(t->lineno, "", "������������Ͳ�һ��\n");
			break;
		}
	return Eptr;
}
/*����������±����*/
TypeIR* Semantic::arrayVar(TreeNode* t)
{
	int present = false;
	SymbTable* entry = NULL;
	TypeIR* Eptr0 = NULL;
	TypeIR* Eptr1 = NULL;
	TypeIR* Eptr = NULL;
	present = FindEntry(t->name[0], &entry);
	t->table[0] = entry;
	if (present != false)
	{
		if (FindAttr(entry).kind != varKind)
		{
			ErrorPrompt(t->lineno, t->name[0], "���Ǳ���\n");
			Eptr = NULL;
		}
		else
			if (FindAttr(entry).idtype != NULL)
				if (FindAttr(entry).idtype->kind != arrayTy)
				{
					ErrorPrompt(t->lineno, t->name[0], "�����������ͱ���\n");
					Eptr = NULL;
				}
				else
				{
					Eptr0 = entry->attrIR.idtype->More.ArrayAttr.indexTy;
					if (t->child[0]->attr.ExpAttr.val > entry->attrIR.idtype->More.ArrayAttr.up)
						ErrorPrompt(t->lineno, t->name[0], "����ʹ��Խ��\n");
					if (t->child[0]->attr.ExpAttr.val < entry->attrIR.idtype->More.ArrayAttr.low)
						ErrorPrompt(t->lineno, t->name[0], "����ʹ��Խ��\n");
					if (Eptr0 == NULL)
						return NULL;
					Eptr1 = Expr(t->child[0], NULL);
					if (Eptr1 == NULL)
						return NULL;
					present = Compat(Eptr0, Eptr1);
					if (present != true)
					{
						ErrorPrompt(t->lineno, "", "�����Ա���Ͳ�һ��\n");
						Eptr = NULL;
					}
					else
						Eptr = entry->attrIR.idtype->More.ArrayAttr.elemTy;
				}
	}
	else
		ErrorPrompt(t->lineno, t->name[0], "��ʶ��δ����\n");
	return Eptr;
}

TypeIR* Semantic::recordVar(TreeNode* t)
{
	int present = false;
	int result = true;
	SymbTable* entry = NULL;
	TypeIR* Eptr0 = NULL;
	TypeIR* Eptr1 = NULL;
	TypeIR* Eptr = NULL;
	fieldchain* currentP = NULL;
	present = FindEntry(t->name[0], &entry);
	t->table[0] = entry;
	if (present != false)
	{
		if (FindAttr(entry).kind != varKind)
		{
			ErrorPrompt(t->lineno, t->name[0], "���Ǳ���\n");
			Eptr = NULL;
		}
		else
			if (FindAttr(entry).idtype->kind != recordTy)
			{
				ErrorPrompt(t->lineno, t->name[0], "���Ǽ�¼����\n");
				Eptr = NULL;
			}
			else
			{
				Eptr0 = entry->attrIR.idtype;
				currentP = Eptr0->More.body;
				while ((currentP != NULL) && (result != false))
				{
					result = strcmp(t->child[0]->name[0], currentP->id);
					if (result == false)
						Eptr = currentP->UnitType;
					else
						currentP = currentP->Next;
				}
				if (currentP == NULL)
					if (result != false)
					{
						ErrorPrompt(t->child[0]->lineno, t->child[0]->name[0], "����������\n");
						Eptr = NULL;
					}
					else
						if (t->child[0]->child[0] != NULL)
							Eptr = arrayVar(t->child[0]);
			}
	}
	else
		ErrorPrompt(t->lineno, t->name[0], "����δ����\n");
	return Eptr;
}

void Semantic::assignstatement(TreeNode* t)
{
	SymbTable* entry = NULL;

	int present = false;
	TypeIR* ptr = NULL;
	TypeIR* Eptr = NULL;

	TreeNode* child1 = NULL;
	TreeNode* child2 = NULL;

	child1 = t->child[0];
	child2 = t->child[1];

	if (child1->child[0] == NULL)
	{
		present = FindEntry(child1->name[0], &entry);

		if (present != false)
		{
			if (FindAttr(entry).kind != varKind)
			{
				ErrorPrompt(child1->lineno, child1->name[0], "����δ����\n");
				Eptr = NULL;
			}
			else
			{
				Eptr = entry->attrIR.idtype;
				child1->table[0] = entry;
			}
		}
		else
		{
			ErrorPrompt(child1->lineno, child1->name[0], "����δ����\n");
			ErrorFlag = true;
		}
	}
	else
	{
		if (child1->attr.ExpAttr.varkind == ArrayMembV)
			Eptr = arrayVar(child1);
		else
			if (child1->attr.ExpAttr.varkind == FieldMembV)
				Eptr = recordVar(child1);
	}
	if (Eptr != NULL)
	{
		if ((t->nodekind == StmtK) && (t->kind.stmt == AssignK))
		{
			ptr = Expr(child2, NULL);
			if (!Compat(ptr, Eptr))
			{
				ErrorPrompt(t->lineno, "", "��ֵ���������Ͳ�һ��\n");
			}
		}
	}
}

void Semantic::callstatement(TreeNode* t)
{
	AccessKind  Ekind;
	int present = false;
	SymbTable* entry = NULL;
	TreeNode* p = NULL;

	present = FindEntry(t->child[0]->name[0], &entry);
	t->child[0]->table[0] = entry;

	if (present == false)
	{
		ErrorPrompt(t->lineno, t->child[0]->name[0], "����δ����\n");
	}
	else
		if (FindAttr(entry).kind != procKind)
			ErrorPrompt(t->lineno, t->name[0], "���Ǻ�����\n");
	/*��ʵ��ƥ��*/
		else
		{
			p = t->child[1];
			ParamTable* paramP = FindAttr(entry).More.ProcAttr.param;
			while ((p != NULL) && (paramP != NULL))
			{
				SymbTable* paraEntry = paramP->entry;
				TypeIR* Etp = Expr(p, &Ekind);
				if ((FindAttr(paraEntry).More.VarAttr.access == indir) && (Ekind == dir))
					ErrorPrompt(p->lineno, "", "�������ƥ��\n");
				else
					if ((FindAttr(paraEntry).idtype) != Etp)
						ErrorPrompt(p->lineno, "", "�������Ͳ�ƥ��\n");
				p = p->sibling;
				paramP = paramP->next;
			}
			if ((p != NULL) || (paramP != NULL))
				ErrorPrompt(t->child[1]->lineno, "", "����������ƥ��\n");
		}
}

void Semantic::ifstatment(TreeNode* t)
{
	AccessKind* Ekind = NULL;
	TypeIR* Etp = Expr(t->child[0], Ekind);
	if (Etp != NULL)
	{
		if (Etp->kind != boolTy)
			ErrorPrompt(t->lineno, "", "�߼����ʽ����\n");
		else
		{
			TreeNode* p = t->child[1];
			while (p != NULL)
			{
				statement(p);
				p = p->sibling;
			}
			t = t->child[2];
			while (t != NULL)
			{
				statement(t);
				t = t->sibling;
			}
		}
	}
}

void Semantic::whilestatement(TreeNode* t)
{
	TypeIR* Etp = Expr(t->child[0], NULL);
	if (Etp != NULL)
		if (Etp->kind != boolTy)
			ErrorPrompt(t->lineno, "", "�߼����ʽ����\n");
		else
		{
			t = t->child[1];
			while (t != NULL)
			{
				statement(t);
				t = t->sibling;
			}
		}
}

void Semantic::readstatement(TreeNode* t)
{
	SymbTable* entry = NULL;
	int present = false;

	present = FindEntry(t->name[0], &entry);
	t->table[0] = entry;

	if (present == false)
		ErrorPrompt(t->lineno, t->name[0], "����δ����\n");
	else
		if (entry->attrIR.kind != varKind)
			ErrorPrompt(t->lineno, t->name[0], "���Ǳ�����ʶ��\n ");
}

void Semantic::writestatement(TreeNode* t)
{
	TypeIR* Etp = Expr(t->child[0], NULL);
	if (Etp != NULL)
		/*������ʽ����Ϊbool���ͣ�����*/
		if (Etp->kind == boolTy)
			ErrorPrompt(t->lineno, "", "���ʽ���ʹ���");
}

void Semantic::returnstatement(TreeNode* t)
{
	if (Level == 0)
		ErrorPrompt(t->lineno, "", "���ش���");
}

int  Semantic::analyze(TreeNode* t)
{
	Level = -1;
	intPtr = NULL;
	charPtr = NULL;
	boolPtr = NULL;
	ErrorFlag = false;

	SymbTable* entry = NULL;
	TreeNode* p = NULL;
	TreeNode* pp = t;

	Level = Level + 1;
	scope[Level] = NULL;
	Off = INITOFF;
	initialize();
	p = t->child[1];
	while (p != NULL)
	{
		switch (p->nodekind)
		{
		case  TypeK:     TypeDecPart(p->child[0]);  break;
		case  VarK:     VarDecPart(p->child[0]);   break;
		case  ProcDecK:  procDecPart(p);        break;
		default:
			ErrorPrompt(p->lineno, "", "�﷨�����޸ýڵ�����");
			break;
		}
		p = p->sibling;
	}

	/*������*/
	t = t->child[2];
	if (t->nodekind == StmLK)
		Body(t);
	if (Level != -1)
		Level = Level - 1;
	return(StoreNoff);
}