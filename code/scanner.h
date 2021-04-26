#ifndef __SCANNER_H__
#define __SCANNER_H__
#pragma once
#include"global.h"



class Scanner {
public:
	Scanner(const string& codeFile) {
		this->FilePath = codeFile;
	}
	bool IsSeparater(char ch);//·Ö¸ô·û

	bool IsOperator(char ch);//ÔËËã·û

	bool IsKeyWord(string ch);//±£Áô×Ö

	bool IsFilter(char ch);//¹ýÂË·û

	bool IsLetter(char ch);//×ÖÄ¸

	bool IsDigit(char ch);//Êý×Ö

	int is_zh_ch(char p)
	{
		if (~(p >> 8) == 0)
		{
			return 1; //ÖÐÎÄ×Ö·û
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

