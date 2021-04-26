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
	/*������*/
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END1, READ, WRITE, ARRAY, OF,
	RECORD, RETURN1,
	INTEGER, CHAR1,
	/*���ַ����ʷ���*/
	ID, INTC, CHARC,
	/*�������*/
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
}LexType;

static struct Word {
	string Sem;   //������Ϣ
	LexType Lex; //�ʷ���Ϣ
	Word(string sem, LexType lex) {
		Sem = sem;
		Lex = lex;
	}
	Word() {}
}reservedWords[ReservedNUM] = { {"program",PROGRAM},{"type",TYPE},{"var",VAR},
	{"procedure",PROCEDURE},{"begin",BEGIN},{"end",END1},{"array",ARRAY},
	{"of",OF},{"record",RECORD},{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},
	{"while",WHILE},{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},
	{"return",RETURN1},{"integer",INTEGER},{"char",CHAR1} };//������

//Token�ṹ
struct Token {
	int Lineshow;  //��¼����
	struct Word word;
	Token(int lineshow, struct Word w) {
		Lineshow = lineshow;
		word = w;
	}
	Token() {}
};
extern vector<Token*> TokenList;//TokenList�Ľṹ
///////////////////////////////////////////////////////////////////////////////////�ݹ��½��﷨����//////////////////////////////////////
/*�﷨���ڵ�����*/
typedef enum
{
	ProK, PheadK, DecK, TypeK, VarK, ProcDecK, StmLK, StmtK, ExpK
}NodeKind;

/*�﷨���ڵ���������*/
typedef enum
{
	ArrayK, CharK, IntegerK, RecordK, IdK
}DecKind;

/*�﷨���ڵ��������*/
typedef enum
{
	IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK
} StmtKind;

/*�﷨���ڵ���ʽ����*/
typedef enum
{
	OpK, ConstK, VariK
} ExpKind;

/*��¼��������� �ֱ��ʾ�����Ǳ�־�������������Ա���������Ա����*/
typedef enum
{
	IdV, ArrayMembV, FieldMembV
} VarKind;

/*��¼�﷨���ڵ�ļ������*/
typedef enum
{
	Void, Integer, Boolean
} ExpType;

/*��¼���̵Ĳ�������*/
typedef enum
{
	valparamType, varparamType
} ParamType;
/*�βα�Ľṹ����*/
typedef struct  paramTable
{
	struct symbtable* entry;/*ָ����β����ڷ��ű��еĵ�ַ���*/
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
	} kind;                       /* ��¼�﷨���ڵ�ľ�������     */

	int idnum;                    /* ��¼һ���ڵ��еı�ʶ������ */

	char name[10][10];            /* ��ʶ��������  */

	struct symbtable* table[10]; /* ���־����Ӧ�ķ��ű��ַ������������׶�����*/



	/*��¼�﷨���ڵ���������*/
	struct
	{
		/*��¼�������͵�����*/
		struct
		{
			int low;            
			int up;               
			DecKind   childtype;  /* ����������� */
		}ArrayAttr;               /* ��������     */

		/*��¼���̵�����*/
		struct 
		{
			ParamType  paramt;     /* ���̵Ĳ�������*/
		}ProcAttr;

		/*��¼���ʽ������*/
		struct 
		{
			LexType op;
			int val;		      /*��¼�﷨���ڵ����ֵ,���﷨���ڵ�Ϊ������ ���ӡ���Ӧ���﷨���ڵ�ʱ��Ч,Ϊ��������*/
			VarKind  varkind;     /* ��¼���������    */
			ExpType type;         /*��¼�﷨���ڵ�ļ������*/
		}ExpAttr;
		char type_name[10];             /*��¼�����������ڵ�Ϊ�������ͣ��������������ͱ�־����ʾʱ��Ч */
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

///////////////////////////////////////////////////////////////////////////////////LL1�﷨����//////////////////////////////////////

/*���ռ���*/
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

/*����ջ ����ռ����ͷ��ռ��� */
struct StackNode
{  
	int flag;    /* 1��ʾ���ռ�����2��ʾ�ռ���    */
	union {
		NontmlType Ntmlvar;  /*���ռ�������*/
		LexType tmlvar;       /*�ռ�������*/
	}var;

	/*ָ����һ���ڵ��ָ��*/
	struct StackNode* nextNode;

};
/*�﷨��ջ*/
struct StackTree
{
	TreeNode** pointer;
	struct StackTree* nextNode;
};
/*������������ջ*/
struct StackOp
{
	TreeNode* pointer;
	StackOp* nextNode;
};

///////////////////////////////////////////////////////////////////////////////////�������//////////////////////////////////////
struct typeIR;

/*��ʶ��������*/
typedef  enum { typeKind, varKind, procKind }IdKind;

typedef  enum { dir, indir }AccessKind;

typedef  enum { intTy, charTy, arrayTy, recordTy, boolTy }TypeKind;

/*��ʶ�������Խṹ����*/
typedef struct
{
	struct typeIR* idtype;		/*ָ���ʶ���������ڲ���ʾ*/
	IdKind    kind;					/*��ʶ��������*/
	union
	{
		struct
		{
			AccessKind   access;   /*�ж��Ǳ�λ���ֵ��*/
			int          level;
			int          off;
			bool         isParam;  /*�ж��ǲ���������ͨ����*/
		}VarAttr;/*������ʶ��������*/
		struct
		{
			int         level;     /*�ù��̵Ĳ���*/
			ParamTable* param;   /*������*/
		}ProcAttr;/*��������ʶ��������*/
	}More;/*��ʶ���Ĳ�ͬ�����в�ͬ������*/
}AttributeIR;


/*���ű�Ľṹ����*/
typedef struct  symbtable
{
	char  idName[10];
	AttributeIR  attrIR;
	struct symbtable* next;

}SymbTable;

typedef struct fieldchain
{
	char   id[10];              /*������*/
	struct typeIR* UnitType; /*���г�Ա������*/
	struct fieldchain* Next;
}fieldChain;

typedef   struct  typeIR
{
	int				size;   /*������ռ�ռ��С*/
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
		fieldChain* body;  /*��¼�����е�����*/
	} More;
}TypeIR;




#endif
