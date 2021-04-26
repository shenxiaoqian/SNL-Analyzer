#ifndef __SCANNER_H__
#define __SCANNER_H__
#pragma once
#include"global.h"



class Scanner {
public:
	Scanner(const string& codeFile) {
		this->FilePath = codeFile;
	}
	bool IsSeparater(char ch);//�ָ���

	bool IsOperator(char ch);//�����

	bool IsKeyWord(string ch);//������

	bool IsFilter(char ch);//���˷�

	bool IsLetter(char ch);//��ĸ

	bool IsDigit(char ch);//����

	int is_zh_ch(char p)
	{
		if (~(p >> 8) == 0)
		{
			return 1; //�����ַ�
		}
		return 0;
	}

	LexType GetTokenType(string charList);
	string toString(int lextype);
	void getTokenList();
	Word reservedLookup(string s);
	void printTokenList();
	void test();
	FILE* output;
	FILE* all;

	string FilePath;
};
#endif

