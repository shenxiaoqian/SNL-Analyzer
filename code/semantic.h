#pragma once
#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__
#include"scanner.h"
#include"global.h"
#define INITOFF 7/*��ʼ�����ű��б�����ƫ��*/
#define SCOPESIZE 1000
class Semantic
{
public:
	/////////////////////////////////////////////////////////////////////////////////////////////����/////
	int Level;
	int Off;/*ÿ�������ƫ��*/
	int StoreNoff;/*������ƫ��*/
	int  savedOff;/*����ƫ��*/

	FILE* output;
	FILE* all;
	SymbTable* scope[SCOPESIZE];
	TypeIR* intPtr;
	TypeIR* charPtr;
	TypeIR* boolPtr;
	/////////////////////////////////////////////////////////////////////////////////////////////����/////
	Semantic();
	void initialize(void);
	int analyze(TreeNode* t);
	void  PrintOneLayer(int level);
	void   PrintSymbTable();
	SymbTable* NewTable(void);
	/*�����ʶ��������*/
	int Enter(char* id, AttributeIR* attribP, SymbTable** entry);
	/*Ѱ�ұ�ʶ�������ַ*/
	int FindEntry(char* id, SymbTable** entry);
	/*�ж������Ƿ���ͬ*/
	int Compat(TypeIR* tp1, TypeIR* tp2);
	/*���������͵��ڲ���ʾ */
	TypeIR* NewTy(TypeKind kind);
	/*�����ռ�¼���͵����� */
	fieldChain* NewBody(void);
	/*�������β�����*/
	ParamTable* NewParam(void);
	void ErrorPrompt(int line, const char* name, const char* message);
	/* ���Ҽ�¼������	*/
	bool  FindField(char* Id, fieldChain* head, fieldChain** Entry);
	/*���ͷ���*/
	TypeIR* TypeProcess(TreeNode* t, DecKind deckind);
	/*�ڷ��ű���Ѱ���Ѷ������������*/
	TypeIR* nameType(TreeNode* t);
	/*�����������͵��ڲ���ʾ*/
	TypeIR* arrayType(TreeNode* t);
	/*�����¼���͵��ڲ���ʾ*/
	TypeIR* recordType(TreeNode* t);
	/*���������������������*/
	void TypeDecPart(TreeNode* t);
	/*�����������������*/
	void VarDecPart(TreeNode* t);
	void varDecList(TreeNode* t);
	/*�����������������*/
	void  procDecPart(TreeNode* t);
	/*����ͷ*/
	SymbTable* HeadProcess(TreeNode* t);
	ParamTable* ParaDecList(TreeNode* t);
	/*���������*/
	void Body(TreeNode* t);
	/*����kind���ж�ת�����ĸ����*/
	void statement(TreeNode* t);
	/*��ѯ����*/
	AttributeIR FindAttr(SymbTable* entry);
	/*������ʽ*/
	TypeIR* Expr(TreeNode* t, AccessKind* Ekind);
	/*����������ͺ��±�*/
	TypeIR* arrayVar(TreeNode* t);
	/*������Ա*/
	TypeIR* recordVar(TreeNode* t);
	/*��鸳ֵ�����˱���*/
	void assignstatement(TreeNode* t);
	/*��������*/
	void callstatement(TreeNode* t);
	/*if�߼����ʽ*/
	void ifstatment(TreeNode* t);
	/*while�߼����ʽ*/
	void whilestatement(TreeNode* t);
	/*read�б�������*/
	void readstatement(TreeNode* t);
	/*write�еı��ʽ�Ƿ�Ϸ�*/
	void writestatement(TreeNode* t);
	/*����������������Ƿ����������г���*/
	void returnstatement(TreeNode* t);

};
#endif