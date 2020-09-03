#pragma once
#include "pch.h"
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#pragma warning (disable:4996)
using namespace std;
int linenum = 1;
int now;
ofstream out;
string sKeyWord[] =
{
	"program", "const","var",
	"procedure","begin","end",
	"if","then","else","odd",
	"while","do",
	"call","read","write",
};
string slop[] =
{
	"=","<>",
	"<","<=",
	">",">="
};
string saop[] =
{
	"+","-"
};
string smop[] =
{
	"*","/"
};
bool Eng(char temp)
{
	return ((temp >= 0x41 && temp <= 0x5A) ||
		(temp >= 0x61 && temp <= 0x7A));
}
bool Num(char temp)
{
	return ((temp >= 0x30) && (temp <= 0x39));
}
void Error(ifstream &file)
{
	now = file.tellg();
	file.seekg(0);
	int line = 1;
	int lastLine = 0;
	int nowline = 0;
	int nowrow = 0;
	for (int i = 0; i <= now; i++)
	{
		char t = file.get();
		if (t == '\n')
		{
			line++;
			i++;
			lastLine = file.tellg();
		}
	}
	nowrow = now - lastLine;
	nowline = line;
	out << "error:" << nowline << "," << nowrow <<"|"<<linenum<< endl;
	char temp = file.get();
	while (temp != ' ' && temp != '\n' && temp != ';' && temp != ',' &&
		(!Eng(temp)) && (!Num(temp)))
		temp = file.get();//读完这行
	file.seekg(-1, ios::cur);//往回一个
}
void word()
{
	set <string> KeyWord;
	for (int i = 0; i < 15; i++)
		KeyWord.insert(sKeyWord[i]);
	set <string> lop;
	ifstream file;
	file.open("TEXT_2.txt", ios::in);
	out.open("out.txt", ios::out);
	if (!file)
	{
		out << "源代码文件打开错误" << endl;
		exit(0);
	}
	while (!file.eof())
	{
		char temp;
		string word = "";
		temp = file.get();
		if (temp == -1)
			break;
		if (temp == ' ')//滤过空白
			continue;
		else if (temp == '\n')
		{
			linenum++;
			continue;
		}
		else if (Eng(temp))//KeyWord 或 ID
		{
			while (!file.eof() && (Eng(temp) || Num(temp)))
			{
				word.push_back(temp);
				temp = file.get();
			}
			if (KeyWord.find(word) != KeyWord.end())
			{
				out << "KeyWord:" << word <<"|"<<linenum<< endl;
				file.seekg(-1, ios::cur);
				continue;
			}
			else
			{
				out << "ID:" << word <<"|"<<linenum<< endl;
				file.seekg(-1, ios::cur);
				continue;
			}
		}
		else if (Num(temp))// INT
		{
			while (!file.eof() && Num(temp))
			{
				word.push_back(temp);
				temp = file.get();
			}
			if (Eng(temp))
			{
				Error(file);
				file.seekg(-1, ios::cur);
				continue;
			}
			{
				out << "INT:" << word << "|"<<linenum<<endl;
				file.seekg(-1, ios::cur);
				continue;
			}
		}
		else if (temp == '=')
		{
			out << "lop:" << "=" <<"|"<<linenum<< endl;
			if (file.peek() != ' ' && file.peek() != '+' && file.peek() != '-' &&
				(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
				Error(file);//转出错处理
			continue;
		}
		else if (temp == '<')
		{
			temp = file.get();
			if (temp == '=')
			{
				out << "lop:" << "<=" <<"|"<<linenum<< endl;
				if (file.peek() != ' ' && file.peek() != '+' && file.peek() != '-' &&
					(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
					Error(file);//转出错处理
				continue;
			}
			else if (temp == '>')
			{
				out << "lop:" << "<>" <<"|"<<linenum<< endl;
				if (file.peek() != ' ' && file.peek() != '+' && file.peek() != '-' &&
					(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
					Error(file);//转出错处理
				continue;
			}
			else
			{
				out << "lop: " << "<" <<"|"<<linenum<<endl;
				file.seekg(-1, ios::cur);
				if (file.peek() != ' ' && file.peek() != '+' && file.peek() != '-' &&
					(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
					Error(file);//转出错处理
				continue;
			}
		}
		else if (temp == '>')
		{
			temp = file.get();
			if (temp == '=')
			{
				out << "lop:" << ">=" <<"|"<<linenum<< endl;
				if (file.peek() != ' ' && file.peek() != '+' && file.peek() != '-' &&
					(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
					Error(file);//转出错处理
				continue;
			}
			else
			{
				out << "lop:" << ">" <<"|"<<linenum<< endl;
				file.seekg(-1, ios::cur);
				if (file.peek() != ' ' && file.peek() != '+' && file.peek() != '-' &&
					(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
					Error(file);//转出错处理
				continue;
			}
		}
		else if (temp == '+' || temp == '-')
		{
			out << "aop:" << temp <<"|"<<linenum<< endl;
			if (file.peek() != ' ' &&
				(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
				Error(file);//转出错处理
			continue;
		}
		else if (temp == '*' || temp == '/')
		{
			out << "mop:" << temp <<"|"<<linenum<< endl;
			if (file.peek() != ' ' &&
				(!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != '(')
				Error(file);//转出错处理
			continue;
		}
		else if (temp == ':')
		{
			temp = file.get();
			if (temp == '=')
			{
				out << "assign:" << ":=" <<"|"<<linenum<< endl;
				if ((!Eng(file.peek())) && (!Num(file.peek())) && file.peek() != ' ' && file.peek() != '+' && file.peek() != '-'  && file.peek() != '(')
				{
					Error(file);
				}
				continue;
			}
			else
			{
				file.seekg(-1, ios::cur);
				Error(file);//转出错处理
			}

		}
		else if (temp == '(')
		{
			out << "LPAR:" << '(' <<"|"<<linenum<< endl;
		}
		else if (temp == ')')
		{
			out << "RPAR:" << ')' <<"|"<<linenum<<endl;
		}
		else if (temp == ',')
		{
			out << "PAR0:" << temp <<"|"<<linenum<< endl;
		}
		else if (temp == ';')
		{
			out << "PAR1:" << temp <<"|"<<linenum<< endl;
		}
	}
	file.close();
}
