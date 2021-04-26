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
/*打印层*/
void  Semantic::PrintOneLayer(int level)
{
	SymbTable* t = scope[level];
	//fprintf(output, "第%d层：\n", level);

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
			default: fprintf(output, "类型错误"); break;
			}
		switch (Attrib->kind)
		{
		case  typeKind:
			fprintf(output, "typekind  "); break;
		case  varKind:
			fprintf(output, "varkind  ");
			fprintf(output, "层数： %d  ", Attrib->More.VarAttr.level);


			switch (Attrib->More.VarAttr.access)
			{
			case  dir:  fprintf(output, "dir  "); break;
			case  indir: fprintf(output, "indir  "); break;
			default:fprintf(output, "类型错误  ");  break;
			}
			break;
		case  procKind:
			fprintf(output, "funckind   ");
			fprintf(output, "层数：%d  ", Attrib->More.ProcAttr.level);
			break;
		default:fprintf(output, "类型错误 ");
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
/*输入标识符和属性*/
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
				fprintf(output, "语义错误：重复定义");
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
/*寻找标识符表项地址*/
int Semantic::FindEntry(char* id, SymbTable** entry)
{
	int present = false;
	int result = 1;
	int lev = Level;

	SymbTable* findentry = scope[lev];

	while ((lev != -1) && (present != true))
	{
		//寻找标识符
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
/*判断类型是否相同*/
int   Semantic::Compat(TypeIR* tp1, TypeIR* tp2)
{
	int  present;
	if (tp1 != tp2)
		present = false;
	else
		present = true;
	return present;
}
/*创建空类型的内部表示 */
TypeIR* Semantic::NewTy(TypeKind  kind)
{
	TypeIR* table = (TypeIR*)malloc(sizeof(TypeIR));
	if (table == NULL)
	{
		fprintf(output, "内存分配失败");
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
/*创建空记录类型的域链 */
fieldChain* Semantic::NewBody(void)
{
	fieldChain* Ptr = (fieldChain*)malloc(sizeof(fieldChain));

	if (Ptr == NULL)
	{
		fprintf(output, "内存分配失败");
	}
	else
	{
		Ptr->Next = NULL;
		Ptr->UnitType = NULL;
	}
	return Ptr;
}
/*创建空形参链表*/
ParamTable* Semantic::NewParam(void)
{
	ParamTable* Ptr = (ParamTable*)malloc(sizeof(ParamTable));

	if (Ptr == NULL)
	{
		fprintf(output, "内存分配失败");
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
	fprintf(all, "第%d行出现语义错误：%s %s", line, name, message);
	ErrorFlag = true;
}
/* 查找纪录的域名	*/
bool  Semantic::FindField(char* Id, fieldChain* head, fieldChain** Entry)
{
	bool  present = false;
	/*记录当前节点*/
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
/*类型分析*/
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
/*在符号表中寻找已定义的类型名字*/
TypeIR* Semantic::nameType(TreeNode* t)
{
	TypeIR* Ptr = NULL;
	SymbTable* entry = NULL;
	int present;
	present = FindEntry(t->attr.type_name, &entry);

	if (present == true)
	{
		/*检查该标识符是否为类型标识符*/
		if (entry->attrIR.kind != typeKind)
			ErrorPrompt(t->lineno, t->attr.type_name, "在声明前使用\n");
		else
			Ptr = entry->attrIR.idtype;
	}
	else/*没有找到该标识符*/
	{
		ErrorPrompt(t->lineno, t->attr.type_name, "类型名未声明\n");
	}
	return Ptr;
}
/*处理数组类型的内部表示*/
TypeIR* Semantic::arrayType(TreeNode* t)
{
	TypeIR* Ptr0 = NULL;
	TypeIR* Ptr1 = NULL;
	TypeIR* Ptr = NULL;

	/*检查数组上界是否小于下界*/
	if ((t->attr.ArrayAttr.low) > (t->attr.ArrayAttr.up))
	{
		ErrorPrompt(t->lineno, "", "数组声明越界错误\n");
		ErrorFlag = true;
	}
	else
	{
		Ptr0 = TypeProcess(t, IntegerK);
		/*调用类型分析函数，处理下标类型*/
		Ptr1 = TypeProcess(t, t->attr.ArrayAttr.childtype);
		/*调用类型分析函数，处理元素类型*/
		Ptr = NewTy(arrayTy);
		/*指向一新创建的类型信息表*/
		Ptr->size = ((t->attr.ArrayAttr.up) - (t->attr.ArrayAttr.low) + 1) * (Ptr1->size);
		/*计算本类型长度*/

/*填写其他信息*/
		Ptr->More.ArrayAttr.indexTy = Ptr0;
		Ptr->More.ArrayAttr.elemTy = Ptr1;
		Ptr->More.ArrayAttr.low = t->attr.ArrayAttr.low;
		Ptr->More.ArrayAttr.up = t->attr.ArrayAttr.up;
	}
	return Ptr;
}
/*处理记录类型的内部表示*/
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
/*处理类型声明的语义分析*/
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
			ErrorPrompt(t->lineno, t->name[0], "重复定义\n");
			entry = NULL;
		}
		else
			entry->attrIR.idtype = TypeProcess(t, t->kind.dec);
		t = t->sibling;
	}
}
/*变量声明的语义分析*/
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
				ErrorPrompt(t->lineno, t->name[i], "重复定义\n");
			}
			else
				t->table[i] = entry;

		}
		if (t != NULL)
			t = t->sibling;
	}
}
/*过程声明的语义分析*/
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
			ErrorPrompt(t->lineno, "", "语法树节点类型错误");
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
/*函数头*/
SymbTable* Semantic::HeadProcess(TreeNode* t)
{
	AttributeIR attrIr;
	int present = false;
	SymbTable* entry = NULL;

	/*填属性*/
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
		ErrorPrompt(t->lineno, "", "声明错误\n");
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
						ErrorPrompt(t->lineno, t->name[0], "不是变量\n");
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
					ErrorPrompt(t->lineno, t->name[0], "变量未定义\n");
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
					break;  /*条件表达式*/
				case PLUS:
				case MINUS:
				case TIMES:
				case OVER:
					Eptr = intPtr;
					break;  /*算数表达式*/
				}
				if (Ekind != NULL)
					(*Ekind) = dir;
			}
			else
				ErrorPrompt(t->lineno, "", "运算符两侧类型不一致\n");
			break;
		}
	return Eptr;
}
/*数组变量的下标分析*/
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
			ErrorPrompt(t->lineno, t->name[0], "不是变量\n");
			Eptr = NULL;
		}
		else
			if (FindAttr(entry).idtype != NULL)
				if (FindAttr(entry).idtype->kind != arrayTy)
				{
					ErrorPrompt(t->lineno, t->name[0], "不是数组类型变量\n");
					Eptr = NULL;
				}
				else
				{
					Eptr0 = entry->attrIR.idtype->More.ArrayAttr.indexTy;
					if (t->child[0]->attr.ExpAttr.val > entry->attrIR.idtype->More.ArrayAttr.up)
						ErrorPrompt(t->lineno, t->name[0], "数组使用越界\n");
					if (t->child[0]->attr.ExpAttr.val < entry->attrIR.idtype->More.ArrayAttr.low)
						ErrorPrompt(t->lineno, t->name[0], "数组使用越界\n");
					if (Eptr0 == NULL)
						return NULL;
					Eptr1 = Expr(t->child[0], NULL);
					if (Eptr1 == NULL)
						return NULL;
					present = Compat(Eptr0, Eptr1);
					if (present != true)
					{
						ErrorPrompt(t->lineno, "", "数组成员类型不一致\n");
						Eptr = NULL;
					}
					else
						Eptr = entry->attrIR.idtype->More.ArrayAttr.elemTy;
				}
	}
	else
		ErrorPrompt(t->lineno, t->name[0], "标识符未声明\n");
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
			ErrorPrompt(t->lineno, t->name[0], "不是变量\n");
			Eptr = NULL;
		}
		else
			if (FindAttr(entry).idtype->kind != recordTy)
			{
				ErrorPrompt(t->lineno, t->name[0], "不是记录类型\n");
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
						ErrorPrompt(t->child[0]->lineno, t->child[0]->name[0], "不是域类型\n");
						Eptr = NULL;
					}
					else
						if (t->child[0]->child[0] != NULL)
							Eptr = arrayVar(t->child[0]);
			}
	}
	else
		ErrorPrompt(t->lineno, t->name[0], "变量未定义\n");
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
				ErrorPrompt(child1->lineno, child1->name[0], "变量未定义\n");
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
			ErrorPrompt(child1->lineno, child1->name[0], "变量未定义\n");
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
				ErrorPrompt(t->lineno, "", "赋值号两侧类型不一致\n");
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
		ErrorPrompt(t->lineno, t->child[0]->name[0], "函数未声明\n");
	}
	else
		if (FindAttr(entry).kind != procKind)
			ErrorPrompt(t->lineno, t->name[0], "不是函数名\n");
	/*形实参匹配*/
		else
		{
			p = t->child[1];
			ParamTable* paramP = FindAttr(entry).More.ProcAttr.param;
			while ((p != NULL) && (paramP != NULL))
			{
				SymbTable* paraEntry = paramP->entry;
				TypeIR* Etp = Expr(p, &Ekind);
				if ((FindAttr(paraEntry).More.VarAttr.access == indir) && (Ekind == dir))
					ErrorPrompt(p->lineno, "", "参数类别不匹配\n");
				else
					if ((FindAttr(paraEntry).idtype) != Etp)
						ErrorPrompt(p->lineno, "", "参数类型不匹配\n");
				p = p->sibling;
				paramP = paramP->next;
			}
			if ((p != NULL) || (paramP != NULL))
				ErrorPrompt(t->child[1]->lineno, "", "参数个数不匹配\n");
		}
}

void Semantic::ifstatment(TreeNode* t)
{
	AccessKind* Ekind = NULL;
	TypeIR* Etp = Expr(t->child[0], Ekind);
	if (Etp != NULL)
	{
		if (Etp->kind != boolTy)
			ErrorPrompt(t->lineno, "", "逻辑表达式错误\n");
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
			ErrorPrompt(t->lineno, "", "逻辑表达式错误\n");
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
		ErrorPrompt(t->lineno, t->name[0], "变量未声明\n");
	else
		if (entry->attrIR.kind != varKind)
			ErrorPrompt(t->lineno, t->name[0], "不是变量标识符\n ");
}

void Semantic::writestatement(TreeNode* t)
{
	TypeIR* Etp = Expr(t->child[0], NULL);
	if (Etp != NULL)
		/*如果表达式类型为bool类型，报错*/
		if (Etp->kind == boolTy)
			ErrorPrompt(t->lineno, "", "表达式类型错误");
}

void Semantic::returnstatement(TreeNode* t)
{
	if (Level == 0)
		ErrorPrompt(t->lineno, "", "返回错误");
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
			ErrorPrompt(p->lineno, "", "语法树中无该节点类型");
			break;
		}
		p = p->sibling;
	}

	/*程序体*/
	t = t->child[2];
	if (t->nodekind == StmLK)
		Body(t);
	if (Level != -1)
		Level = Level - 1;
	return(StoreNoff);
}