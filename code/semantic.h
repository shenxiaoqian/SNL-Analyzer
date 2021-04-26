#pragma once
#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__
#include"scanner.h"
#include"global.h"
#define INITOFF 7/*初始化符号表中变量的偏移*/
#define SCOPESIZE 1000
class Semantic
{
public:
	/////////////////////////////////////////////////////////////////////////////////////////////变量/////
	int Level;
	int Off;/*每层变量的偏移*/
	int StoreNoff;/*主程序偏移*/
	int  savedOff;/*各层偏移*/

	FILE* output;
	FILE* all;
	SymbTable* scope[SCOPESIZE];
	TypeIR* intPtr;
	TypeIR* charPtr;
	TypeIR* boolPtr;
	/////////////////////////////////////////////////////////////////////////////////////////////函数/////
	Semantic();
	void initialize(void);
	int analyze(TreeNode* t);
	void  PrintOneLayer(int level);
	void   PrintSymbTable();
	SymbTable* NewTable(void);
	/*输入标识符和属性*/
	int Enter(char* id, AttributeIR* attribP, SymbTable** entry);
	/*寻找标识符表项地址*/
	int FindEntry(char* id, SymbTable** entry);
	/*判断类型是否相同*/
	int Compat(TypeIR* tp1, TypeIR* tp2);
	/*创建空类型的内部表示 */
	TypeIR* NewTy(TypeKind kind);
	/*创建空记录类型的域链 */
	fieldChain* NewBody(void);
	/*创建空形参链表*/
	ParamTable* NewParam(void);
	void ErrorPrompt(int line, const char* name, const char* message);
	/* 查找纪录的域名	*/
	bool  FindField(char* Id, fieldChain* head, fieldChain** Entry);
	/*类型分析*/
	TypeIR* TypeProcess(TreeNode* t, DecKind deckind);
	/*在符号表中寻找已定义的类型名字*/
	TypeIR* nameType(TreeNode* t);
	/*处理数组类型的内部表示*/
	TypeIR* arrayType(TreeNode* t);
	/*处理记录类型的内部表示*/
	TypeIR* recordType(TreeNode* t);
	/*处理类型声明的语义分析*/
	void TypeDecPart(TreeNode* t);
	/*变量声明的语义分析*/
	void VarDecPart(TreeNode* t);
	void varDecList(TreeNode* t);
	/*过程声明的语义分析*/
	void  procDecPart(TreeNode* t);
	/*函数头*/
	SymbTable* HeadProcess(TreeNode* t);
	ParamTable* ParaDecList(TreeNode* t);
	/*处理程序体*/
	void Body(TreeNode* t);
	/*根据kind项判断转向处理哪个语句*/
	void statement(TreeNode* t);
	/*查询属性*/
	AttributeIR FindAttr(SymbTable* entry);
	/*处理表达式*/
	TypeIR* Expr(TreeNode* t, AccessKind* Ekind);
	/*检查数组类型和下标*/
	TypeIR* arrayVar(TreeNode* t);
	/*检查域成员*/
	TypeIR* recordVar(TreeNode* t);
	/*检查赋值号两端变量*/
	void assignstatement(TreeNode* t);
	/*函数调用*/
	void callstatement(TreeNode* t);
	/*if逻辑表达式*/
	void ifstatment(TreeNode* t);
	/*while逻辑表达式*/
	void whilestatement(TreeNode* t);
	/*read中变量错误*/
	void readstatement(TreeNode* t);
	/*write中的表达式是否合法*/
	void writestatement(TreeNode* t);
	/*分析函数返回语句是否在主程序中出现*/
	void returnstatement(TreeNode* t);

};
#endif