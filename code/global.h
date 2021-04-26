#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#pragma once
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include<string>
#include<fstream> 
#include<vector>

#define ReservedNUM 21
# define MAXCHILDREN 3
#define  TABLESIZE  104

using namespace std;

int Main();
extern bool ErrorFlag;
typedef enum
{
	ENDFILE, ERROR,
	/*保留字*/
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END1, READ, WRITE, ARRAY, OF,
	RECORD, RETURN1,
	INTEGER, CHAR1,
	/*多字符单词符号*/
	ID, INTC, CHARC,
	/*特殊符号*/
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
}LexType;

static struct Word {
	string Sem;   //语义信息
	LexType Lex; //词法信息
	Word(string sem, LexType lex) {
		Sem = sem;
		Lex = lex;
	}
	Word() {}
}reservedWords[ReservedNUM] = { {"program",PROGRAM},{"type",TYPE},{"var",VAR},
	{"procedure",PROCEDURE},{"begin",BEGIN},{"end",END1},{"array",ARRAY},
	{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
	{"while",WHILE},{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},
	{"return",RETURN1},{"integer",INTEGER},{"char",CHAR1} };//保留字

//Token结构
struct Token {
	int Lineshow;  //记录行数
	struct Word word;
	Token(int lineshow, struct Word w) {
		Lineshow = lineshow;
		word = w;
	}
	Token() {}
};
extern vector<Token*> TokenList;//TokenList的结构
///////////////////////////////////////////////////////////////////////////////////递归下降语法分析//////////////////////////////////////
/*语法树节点类型*/
typedef enum
{
	ProK, PheadK, DecK, TypeK, VarK, ProcDecK, StmLK, StmtK, ExpK
}NodeKind;

/*语法树节点声明类型*/
typedef enum
{
	ArrayK, CharK, IntegerK, RecordK, IdK
}DecKind;

/*语法树节点语句类型*/
typedef enum
{
	IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK
} StmtKind;

/*语法树节点表达式类型*/
typedef enum
{
	OpK, ConstK, VariK
} ExpKind;

/*记录变量的类别 分别表示变量是标志符变量、数组成员变量、域成员变量*/
typedef enum
{
	IdV, ArrayMembV, FieldMembV
} VarKind;

/*记录语法树节点的检查类型*/
typedef enum
{
	Void, Integer, Boolean
} ExpType;

/*记录过程的参数类型*/
typedef enum
{
	valparamType, varparamType
} ParamType;
/*形参表的结构定义*/
typedef struct  paramTable
{
	struct symbtable* entry;/*指向该形参所在符号表中的地址入口*/
	struct paramTable* next;
}ParamTable;
struct symbtable;
struct TreeNode
{
	struct TreeNode* child[MAXCHILDREN];		
	struct TreeNode* sibling;					
	int lineno;								
	NodeKind nodekind;						   
	union
	{
		DecKind  dec;
		StmtKind stmt;
		ExpKind  exp;
	} kind;                       /* 记录语法树节点的具体类型     */

	int idnum;                    /* 记录一个节点中的标识符个数 */

	char name[10][10];            /* 标识符的名称  */

	struct symbtable* table[10]; /* 与标志符对应的符号表地址，在语义分析阶段填入*/



	/*记录语法树节点其他属性*/
	struct
	{
		/*记录数组类型的属性*/
		struct
		{
			int low;            
			int up;               
			DecKind   childtype;  /* 数组的子类型 */
		}ArrayAttr;               /* 数组属性     */

		/*记录过程的属性*/
		struct 
		{
			ParamType  paramt;     /* 过程的参数类型*/
		}ProcAttr;

		/*记录表达式的属性*/
		struct 
		{
			LexType op;
			int val;		      /*记录语法树节点的数值,当语法树节点为“数字 因子”对应的语法树节点时有效,为整数类型*/
			VarKind  varkind;     /* 记录变量的类别    */
			ExpType type;         /*记录语法树节点的检查类型*/
		}ExpAttr;
		char type_name[10];             /*记录类型名，当节点为声明类型，且类型是由类型标志符表示时有效 */
	}attr;
	TreeNode() 
	{
		for (int i = 0; i < MAXCHILDREN; i++)
			child[i] = NULL;
		sibling = NULL;
		lineno = 0;
		idnum = 0;
		for (int i = 0; i < 10; i++)
		{
			strcpy(name[i], "\0");
			table[i] = NULL;
		}
		attr.ArrayAttr.low = 0;
		attr.ArrayAttr.up = 0;
		attr.ExpAttr.val = 0;
		strcpy(attr.type_name, "\0");
	}
};

///////////////////////////////////////////////////////////////////////////////////LL1语法分析//////////////////////////////////////

/*非终极符*/
typedef enum
{
	Program, ProgramHead, ProgramName, DeclarePart,
	TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
	TypeId, TypeDef, BaseType, StructureType,
	ArrayType, Low, Top, RecType,
	FieldDecList, FieldDecMore, IdList, IdMore,
	VarDec, VarDeclaration, VarDecList, VarDecMore,
	VarIdList, VarIdMore, ProcDec, ProcDeclaration,
	ProcDecMore, ProcName, ParamList, ParamDecList,
	ParamMore, Param, FormList, FidMore,
	ProcDecPart, ProcBody, ProgramBody, StmList,
	StmMore, Stm, AssCall, AssignmentRest,
	ConditionalStm, StmL, LoopStm, InputStm,
	InVar, OutputStm, ReturnStm, CallStmRest,
	ActParamList, ActParamMore, RelExp, OtherRelE,
	Exp, OtherTerm, Term, OtherFactor,
	Factor, Variable, VariMore, FieldVar,
	FieldVarMore, CmpOp, AddOp, MultOp

}  NontmlType;

/*符号栈 存放终极符和非终极符 */
struct StackNode
{  
	int flag;    /* 1表示非终极符；2表示终极符    */
	union {
		NontmlType Ntmlvar;  /*非终极符部分*/
		LexType tmlvar;       /*终极符部分*/
	}var;

	/*指向下一个节点的指针*/
	struct StackNode* nextNode;

};
/*语法树栈*/
struct StackTree
{
	TreeNode** pointer;
	struct StackTree* nextNode;
};
/*操作符操作数栈*/
struct StackOp
{
	TreeNode* pointer;
	StackOp* nextNode;
};

///////////////////////////////////////////////////////////////////////////////////语义分析//////////////////////////////////////
struct typeIR;

/*标识符的类型*/
typedef  enum { typeKind, varKind, procKind }IdKind;

typedef  enum { dir, indir }AccessKind;

typedef  enum { intTy, charTy, arrayTy, recordTy, boolTy }TypeKind;

/*标识符的属性结构定义*/
typedef struct
{
	struct typeIR* idtype;		/*指向标识符的类型内部表示*/
	IdKind    kind;					/*标识符的类型*/
	union
	{
		struct
		{
			AccessKind   access;   /*判断是变参还是值参*/
			int          level;
			int          off;
			bool         isParam;  /*判断是参数还是普通变量*/
		}VarAttr;/*变量标识符的属性*/
		struct
		{
			int         level;     /*该过程的层数*/
			ParamTable* param;   /*参数表*/
		}ProcAttr;/*过程名标识符的属性*/
	}More;/*标识符的不同类型有不同的属性*/
}AttributeIR;


/*符号表的结构定义*/
typedef struct  symbtable
{
	char  idName[10];
	AttributeIR  attrIR;
	struct symbtable* next;

}SymbTable;

typedef struct fieldchain
{
	char   id[10];              /*变量名*/
	struct typeIR* UnitType; /*域中成员的类型*/
	struct fieldchain* Next;
}fieldChain;

typedef   struct  typeIR
{
	int				size;   /*类型所占空间大小*/
	TypeKind		kind;
	union
	{
		struct
		{
			struct typeIR* indexTy;
			struct typeIR* elemTy;
			int    low;     
			int    up;      
		}ArrayAttr;
		fieldChain* body;  /*记录类型中的域链*/
	} More;
}TypeIR;




#endif
