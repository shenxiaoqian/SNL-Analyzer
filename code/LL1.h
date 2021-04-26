#ifndef __LL1_H__
#define __LL1_H__
#pragma once
#include"scanner.h"
#include"global.h"
class ParseLL1
{
public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////变量///
	Token currentToken;
	int  LL1Table[TABLESIZE][TABLESIZE];
	int lineshow;
	TreeNode* currentP;
	DecKind* temp;
	TreeNode* tempNode;
	int  LPARENnum;
	FILE* output;
	FILE* all;
	//符号栈栈顶指针
	StackNode* StackNodeTop;
	//符号栈栈空
	int StackNodeEmpty;
	//语法树栈栈顶指针
	StackTree* StackTreeTop;
	//语法树栈栈空
	int StackTreeEmpty;
	//操作符栈栈顶指针
	StackOp* StackOpTop;
	//操作符栈栈空
	int StackOpEmpty;
	//操作数栈栈顶指针
	StackOp* StackNumTop;
	//操作数栈栈空
	int StackNumEmpty;


	/*整个表达式是否处理结束标识*/
	int  getExpResult;
	/*专用于数组成员变量，当数组成员表达式处理结束时，
	需要将其与数组变量节点相连，数组成员表达式的根结点 作为数组成员变量节点的儿子节点*/
	int  getExpResult2;
	//读取Token序列时的标记号
	int TokenListNum;
	//打印语法树时用来控制格式
	static int form;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////函数///
	//自定义构造函数
	//ParseLL1();
	void CreateLL1Table();
	//读取词法分析后的Token序列
	void GetToken(Token* p, Scanner sc);
	//predict集
	void predict(int num);
	//设置运算符优先级
	int  Priosity(LexType  op);
	//LL1语法分析主函数
	TreeNode* parseLL1(Scanner sc);
	//打印语法树
	void printTree(TreeNode* tree);
	TreeNode* newNode(NodeKind nodekind);
	TreeNode* newStmtNode(StmtKind kind);
	TreeNode* newExpNode(ExpKind kind);
	void Push(int, int);
	void Pop();
	void PushTree(TreeNode** t);
	TreeNode** PopTree();
	void  PushOp(TreeNode* t);
	TreeNode* PopOp();
	void PushNum(TreeNode* t);
	TreeNode* PopNum();
	string toString(int lextype);

	/*104个程序*/
	void process1();
	void process2();
	void process3();
	void process4();
	void process5();
	void process6();
	void process7();
	void process8();
	void process9();
	void process10();
	void process11();
	void process12();
	void process13();
	void process14();
	void process15();
	void process16();
	void process17();
	void process18();
	void process19();
	void process20();
	void process21();
	void process22();
	void process23();
	void process24();
	void process25();
	void process26();
	void process27();
	void process28();
	void process29();
	void process30();
	void process31();
	void process32();
	void process33();
	void process34();
	void process35();
	void process36();
	void process37();
	void process38();
	void process39();
	void process40();
	void process41();
	void process42();
	void process43();
	void process44();
	void process45();
	void process46();
	void process47();
	void process48();
	void process49();
	void process50();
	void process51();
	void process52();
	void process53();
	void process54();
	void process55();
	void process56();
	void process57();
	void process58();
	void process59();
	void process60();
	void process61();
	void process62();
	void process63();
	void process64();
	void process65();
	void process66();
	void process67();
	void process68();
	void process69();
	void process70();
	void process71();
	void process72();
	void process73();
	void process74();
	void process75();
	void process76();
	void process77();
	void process78();
	void process79();
	void process80();
	void process81();
	void process82();
	void process83();
	void process84();
	void process85();
	void process86();
	void process87();
	void process88();
	void process89();
	void process90();
	void process91();
	void process92();
	void process93();
	void process94();
	void process95();
	void process96();
	void process97();
	void process98();
	void process99();
	void process100();
	void process101();
	void process102();
	void process103();
	void process104();
};
#endif