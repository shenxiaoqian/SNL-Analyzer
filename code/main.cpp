#include"scanner.h"
#include"LL1.h"
#include"semantic.h"
#include"windows.h"
#include"tchar.h"
#include"RecursiveDescent.h"
//#include "pch.h"
void printEnum(NodeKind a)
{
	switch (a)
	{
	case 0: cout << "ProK" << endl; break;
	case 1: cout << "PheadK" << endl; break;
	case 2: cout << "DecK" << endl; break;
	case 3: cout << "TypeK" << endl; break;
	case 4: cout << "VarK" << endl; break;
	case 5: cout << "ProcDecK" << endl; break;
	case 6: cout << "StmLK" << endl; break;
	case 7: cout << "StmtK" << endl; break;
	case 8: cout << "ExpK" << endl; break;
	}
}
void printTree(TreeNode* tree,int form, FILE *output)
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
				fprintf(output, "%s  ", "var param:");
			if (tree->attr.ProcAttr.paramt == valparamType)
				fprintf(output, "%s  ", "value param:");
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
			/*else
			{
				fprintf(output, "wrong!no var!\n");
				ErrorFlag = true;
			}*/
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
				if (tree->table[0] != NULL)
					fprintf(output, "%d   %d  ", tree->table[0]->attrIR.More.VarAttr.off, tree->table[0]->attrIR.More.VarAttr.level);
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
			fprintf(output, "%s ", "ExpK");
			switch (tree->kind.exp)
			{
			case OpK:
			{
				fprintf(output, "%s  ", "Op");
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
			case  ConstK|-842150452:
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
				case ArrayMembV | -842150452:
					fprintf(output, "ArrayMember  ");
					fprintf(output, "%s  ", tree->name[0]);
					break;
				default:
					fprintf(output, "变量错误");
					ErrorFlag = true;
				}

				fprintf(output, "%d  ", tree->attr.ExpAttr.val);
				break;
			case VariK :
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
				if (tree->table[0] != NULL)
					fprintf(output, "%d   %d  ", tree->table[0]->attrIR.More.VarAttr.off, tree->table[0]->attrIR.More.VarAttr.level);

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
			printTree(tree->child[i],form,output);
		tree = tree->sibling;
	}
	form = form - 4;
}

int Main()
{
	/*词法分析*/
	Scanner sc("./result/source.txt");
	sc.output = fopen("./result/scannerResult.txt", "w+");
	sc.all = fopen("./result/message.txt", "w+");
	sc.getTokenList();
	sc.printTokenList();
	if (ErrorFlag == false)
	{
	
		/*LL1语法分析*/
		ParseLL1 myParseLL1;
		myParseLL1.output = fopen("./result/LL1Result.txt", "w+");
		myParseLL1.all = fopen("./result/message.txt", "a+");
		TreeNode* tree = myParseLL1.parseLL1(sc);
		myParseLL1.printTree(tree);
		fclose(myParseLL1.output);	
		/*递归下降语法分析*/
		TreeNode* root = ParseRD();
		FILE* rdFile = fopen("./result/RDResult.txt", "w+");
		printTree(root,0,rdFile);
		fclose(rdFile);
		/*语义分析*/
		Semantic mySemantic;
		mySemantic.output = fopen("./result/semanticResult.txt", "w+");
		mySemantic.all = fopen("./result/message.txt", "a+");
		mySemantic.analyze(tree);
		mySemantic.PrintSymbTable();
		fclose(mySemantic.output);
		fclose(mySemantic.all);
		fclose(myParseLL1.all);
	}
	else
	{
		FILE* output;
		output= fopen("./result/LL1Result.txt", "w+");
		fprintf(output, "请先改正词法错误");
		output = fopen("./result/RDResult.txt", "w+");
		fprintf(output, "请先改正词法错误");
		output = fopen("./result/semanticResult.txt", "w+");
		fprintf(output, "请先改正词法错误");
		fclose(output);
	}
	fclose(sc.all);
	fclose(sc.output);
	TokenList.clear();
	ErrorFlag = false;
	cout << "succeed";
	return 0;
}