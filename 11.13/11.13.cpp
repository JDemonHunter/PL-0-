#include "pch.h"
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include "grammer.h"
#include "word.h"
#include "translate.h"
#include "execute.h"
#pragma warning (disable:4996)
using namespace std;
char FcodeTxt[][10] = { "LIT", "OPR", "LOD", "STO", "CAL", "MLC", "JMP", "JPC", "RED", "WRT" };
int main()
{

	word();
	//词法分析
	//词法分析的结果将保存为中间文件"out.txt"

	//开始语法分析
	file.open("out.txt", ios::in);
	resultfile.open("result.txt", ios::out);
	//freopen("result.txt", "w", stdout);

	//"result.txt"文件将存储语法分析的出错信息结果, 改为在函数内部用ofstream实现

	char temp = ' ';
	if (!file)
	{
		cout << "中间文件打开错误" << endl;
		exit(0);
	}
	prog(file);
	file.clear();
	file.close();

	fstream file2;
	file2.open("result.txt");
	temp = file2.get();
	if (temp==-1)
	{
		file2.clear();
		ofstream targetCode;
		targetCode.open("code.txt", ios::out);
		vector<emit_item> ::iterator itc;
		for (itc = code.begin(); itc != code.end(); itc++)
		{
			targetCode << FcodeTxt[(*itc).F] << "  " << (*itc).L << "  " << (*itc).A << endl;
		}
		initialize();

		MAIN_EXECUTE(code);
	}
	file2.clear();
	file2.close();
	return 0;
}

