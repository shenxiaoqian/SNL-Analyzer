#pragma once
#ifndef __RECURSIVEDESCENT_H__
#define __RECURSIVEDESCENT_H__

#include "global.h"

//1.�����ܳ�������
TreeNode* ParseRD();
//2.�ܳ������������
TreeNode* ProgramRD();
//3.����ͷ���ִ����������
TreeNode* ProgramHeadRD();
//4.�����������ִ����������
TreeNode* DeclarePartRD();
//5.�������������������
TreeNode* TypeDecRD();
//6.���������е���������
TreeNode* TypeDeclarationRD();
//7.���������е���������
TreeNode* TypeDeclistRD();
//8.���������е���������
TreeNode* TYpeDecMoreRD();
//9.�������������������������ƴ����������
void typeIdRD(TreeNode* t);
//10.���ʹ����������
void typeDefRD(TreeNode* t);
//11.�������ʹ����������
void BaseTypeRD(TreeNode* t);
//12.�ṹ���ʹ����������
void StructureTypeRD(TreeNode* t);
//13.�������͵Ĵ����������
void ArrayTypeRD(TreeNode* t);
//14.��¼���͵Ĵ����������
void RecTypeRD(TreeNode* t);
//15.��¼�����е������������������
TreeNode* FieldDecListRD();
//16.��¼�����е����������������������
TreeNode* FieldDecMoreRD();
//17.��¼�������б�ʶ���������������
void IdListRD(TreeNode* t);
//18.��¼��������������ʶ���������������
void IdMoreRD(TreeNode* t);
//19.�������������������
TreeNode* VarDecRD();
//20.��������������
TreeNode* VarDeclarationRD();
//21.�����������ִ������
TreeNode* VarDecListRD();
//22.�����������ִ������
TreeNode* VarDecMoreRD();
//23.�����������ֱ��������ִ������
void VarIdListRD(TreeNode* t);
//24.�����������ֱ��������ִ������
void VarIdMoreRD(TreeNode* t);
//25.�������������ܵĴ����������
TreeNode* ProcDecRD();
//26.�����������־���Ĵ����������
TreeNode* ProcDeclarationRD();
//27.���������еĲ����������ֵĴ����������
void ParamListRD(TreeNode* t);
//28.���������еĲ��������������ֵĴ����������
TreeNode* ParamDecListRD();
//29.���������еĲ��������������ֵĴ����������
TreeNode* ParamMoreRD();
//30.���������еĲ��������������ֵĴ����������
TreeNode* ParamRD();
//31.���������еĲ��������в��������ֵĴ����������
void FormListRD(TreeNode* t);
//32.���������еĲ��������в��������ֵĴ����������
void FidMoreRD(TreeNode* t);
//33.�������������ֵķ����������
TreeNode* ProcDecPartRD(void);
//34.�����岿�ִ����������
TreeNode* procBodyRD(void);
//35.�������岿�ִ����������
TreeNode* ProgramBodyRD(void);
//36.������в��ִ���������� 
TreeNode* StmListRD(void);
//37��������䲿�ִ����������
TreeNode* StmMoreRD(void);
//38�����ݹ鴦���������
TreeNode* StmRD(void);
//39����ֵ���ͺ���������䲿�ֵĴ����������
TreeNode* AssCallRD(void);
//40����ֵ��䴦���������
TreeNode* AssignmentRestRD(void);
//41��������䴦���������
TreeNode* ConditionalStmRD(void);
//42��ѭ����䴦���������
TreeNode* LoopStmRD(void);
//43���������Ĵ����������
TreeNode* InputStmRD(void);
//44�������䴦���������
TreeNode* OutputStmRD(void);
//45�����̷�����䴦���������
TreeNode* ReturnStmRD(void);
//46�����̵�����䴦���������
TreeNode* CallStmRestRD(void);
//47��ʵ�β��ִ����������
TreeNode* ActParamListRD(void);
//48������ʵ�β��ִ����������
TreeNode* ActParamMoreRD(void);
//49�����ʽ�ݹ鴦���������
TreeNode* ExpRD(void);
//50���򵥱��ʽ�ݹ鴦���������
TreeNode* Simple_expRD(void);
//51����ݹ鴦���������
TreeNode* TermRD(void);
//52�����ӵݹ鴦���������
TreeNode* FactorRD(void);
//53�������������
TreeNode* VariableRD(void);
//54���������ಿ�ִ������
void VariMoreRD(TreeNode* t);
//55����������ִ������
TreeNode* FieldvarRD(void);
//56��������������ִ������
void FieldvarMoreRD(TreeNode* t);
//57���ռ���ƥ�䴦�����
void Match(LexType expected);
#endif
