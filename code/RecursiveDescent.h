#pragma once
#ifndef __RECURSIVEDESCENT_H__
#define __RECURSIVEDESCENT_H__

#include "global.h"

//1.调用总程序处理函数
TreeNode* ParseRD();
//2.总程序处理分析函数
TreeNode* ProgramRD();
//3.程序头部分处理分析函数
TreeNode* ProgramHeadRD();
//4.程序声明部分处理分析函数
TreeNode* DeclarePartRD();
//5.类型声明处理分析程序
TreeNode* TypeDecRD();
//6.类型声明中的其他函数
TreeNode* TypeDeclarationRD();
//7.类型声明中的其他函数
TreeNode* TypeDeclistRD();
//8.类型声明中的其他函数
TreeNode* TYpeDecMoreRD();
//9.类型声明中新声明的类型名称处理分析程序
void typeIdRD(TreeNode* t);
//10.类型处理分析程序
void typeDefRD(TreeNode* t);
//11.基本类型处理分析程序
void BaseTypeRD(TreeNode* t);
//12.结构类型处理分析程序
void StructureTypeRD(TreeNode* t);
//13.数组类型的处理分析程序
void ArrayTypeRD(TreeNode* t);
//14.记录类型的处理分析程序
void RecTypeRD(TreeNode* t);
//15.记录类型中的域声明处理分析程序
TreeNode* FieldDecListRD();
//16.记录类型中的其他域声明处理分析程序
TreeNode* FieldDecMoreRD();
//17.记录类型域中标识符名处理分析程序
void IdListRD(TreeNode* t);
//18.记录类型域中其他标识符名处理分析程序
void IdMoreRD(TreeNode* t);
//19.变量声明处理分析程序
TreeNode* VarDecRD();
//20.变量声明处理函数
TreeNode* VarDeclarationRD();
//21.变量声明部分处理程序
TreeNode* VarDecListRD();
//22.变量声明部分处理程序
TreeNode* VarDecMoreRD();
//23.变量声明部分变量名部分处理程序
void VarIdListRD(TreeNode* t);
//24.变量声明部分变量名部分处理程序
void VarIdMoreRD(TreeNode* t);
//25.过程声明部分总的处理分析程序
TreeNode* ProcDecRD();
//26.过程声明部分具体的处理分析程序
TreeNode* ProcDeclarationRD();
//27.过程声明中的参数声明部分的处理分析程序
void ParamListRD(TreeNode* t);
//28.过程声明中的参数声明其他部分的处理分析程序
TreeNode* ParamDecListRD();
//29.过程声明中的参数声明其他部分的处理分析程序
TreeNode* ParamMoreRD();
//30.过程声明中的参数声明参数部分的处理分析程序
TreeNode* ParamRD();
//31.过程声明中的参数声明中参数名部分的处理分析程序
void FormListRD(TreeNode* t);
//32.过程声明中的参数声明中参数名部分的处理分析程序
void FidMoreRD(TreeNode* t);
//33.过程中声明部分的分析处理程序
TreeNode* ProcDecPartRD(void);
//34.过程体部分处理分析程序
TreeNode* procBodyRD(void);
//35.主程序体部分处理分析程序
TreeNode* ProgramBodyRD(void);
//36.语句序列部分处理分析程序 
TreeNode* StmListRD(void);
//37．更多语句部分处理分析程序
TreeNode* StmMoreRD(void);
//38．语句递归处理分析程序
TreeNode* StmRD(void);
//39．赋值语句和函数调用语句部分的处理分析程序
TreeNode* AssCallRD(void);
//40．赋值语句处理分析函数
TreeNode* AssignmentRestRD(void);
//41．条件语句处理分析程序
TreeNode* ConditionalStmRD(void);
//42．循环语句处理分析程序
TreeNode* LoopStmRD(void);
//43．输入语句的处理分析程序
TreeNode* InputStmRD(void);
//44．输出语句处理分析程序
TreeNode* OutputStmRD(void);
//45．过程返回语句处理分析程序
TreeNode* ReturnStmRD(void);
//46．过程调用语句处理分析程序
TreeNode* CallStmRestRD(void);
//47．实参部分处理分析程序
TreeNode* ActParamListRD(void);
//48．更多实参部分处理分析程序
TreeNode* ActParamMoreRD(void);
//49．表达式递归处理分析程序
TreeNode* ExpRD(void);
//50．简单表达式递归处理分析程序
TreeNode* Simple_expRD(void);
//51．项递归处理分析程序。
TreeNode* TermRD(void);
//52．因子递归处理分析程序
TreeNode* FactorRD(void);
//53．变量处理程序
TreeNode* VariableRD(void);
//54．变量其余部分处理程序
void VariMoreRD(TreeNode* t);
//55．域变量部分处理过程
TreeNode* FieldvarRD(void);
//56．域变量其它部分处理过程
void FieldvarMoreRD(TreeNode* t);
//57．终极符匹配处理程序
void Match(LexType expected);
#endif
