#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include "translate.h"
#pragma warning (disable:4996)
using namespace std;
set<string>keyWord;
set<string>ID;
set<string>INT;
set<string>PAR0;
set<string>PAR1;
set<string>assign;
set<string>LPAR;
set<string>RPAR;
set<string>aop;
set<string>mop;
set<string>lop;
set<string>error;
ifstream file;  //读文件流 读取"out.txt"中间文件的内容
ofstream resultfile;//写入语法分析结果"result.txt"的文件流指针
enum KIND kind;
//全局变量: 每一个ID的种属
int LEV;
//全局变量: 用于确定当前在整个程序中的嵌套层次, 仅用于CAL指令的层差
vector<int> mem;
//内存栈+数据运算单元: 由P代码控制, 在最后编写P代码的解释指令时才会使用
int adrvalNum = 0;
//用于存储当时层名占有的常量变量个数, 可能会多次引用所以直接写成全局变量
int nowlevel = 0;
//用于存储当时层名的绝对层次, 可能会多次引用所以直接写成全局变量
int leveldiff = 0;
//用于存储调用过程与被调用过程之间的层次差, 可能会多次引用直接写成全局变量, 在编译到call时改变
int retadr = 0;
//用于存储被调用过程返回调用过程时的三地址代码编号, 可能会多次引用直接写成全局变量, 在编译到call时改变
bool fromproc = 0;
//此布尔变量用于标记是从proc->block->body进入的, 还是从statement中进入的
//如果是从statement中进入的, 则在最终遇到end时不需要OPR指令
/********   P代码设计   *************/
/*
LIT		0 ，a 取常量a放入数据栈栈顶
OPR		0 ，a 执行运算，a表示执行某种运算
LOD	L ，a 取变量（相对地址为a，层差为L）放到数据栈的栈顶
STO		L ，a 将数据栈栈顶的内容存入变量（相对地址为a，层次差为L）
CAL		L ，a 调用过程（转子指令）（入口地址为a，层次差为L）
INT		0 ，a 数据栈栈顶指针增加a
JMP		0 ，a无条件转移到地址为a的指令
JPC		0 ，a 条件转移指令，转移到地址为a的指令
RED	L ，a 读数据并存入变量（相对地址为a，层次差为L）
WRT	0 ，0 将栈顶内容输出
*/
/*
CAL：调用过程，这时A段为被调用过程的过程体（过程体之前的某一条指令）在目标程序区的入口地址。
LIT：将常量送到运行栈S的栈顶，这时A段为常量值。
LOD：将变量送到运行栈S的栈顶，这时A段为变量所在说明层中的相对位置。
STO：将运行栈S的栈顶内容送入某个变量单元中，A段为变量所在说明层中的相对位置。
JMP：无条件转移，这时A段为转向地址（目标程序）。
JPC：条件转移，当运行栈S的栈顶的布尔值为假（0）时，则转向A段所指目标程序地址；否则顺序执行。
OPR：关系或算术运算:
			OPR 0 2	次栈顶与栈顶相加，退两个栈元素，结果值进栈
			OPR 0 3	次栈顶减去栈顶，退两个栈元素，结果值进栈
			OPR 0 4	次栈顶乘以栈顶，退两个栈元素，结果值进栈
			OPR 0 5	次栈顶除以栈顶，退两个栈元素，结果值进栈.
			OPR 0 6	栈顶元素的奇偶判断，结果值在栈顶
			OPR 0 8	次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
			OPR 0 9	次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
			OPR 0 10	次栈顶是否小于栈顶，退两个栈元素，结果值进栈
			OPR 0 11	次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
			OPR 0 12	次栈顶是否大于栈顶，退两个栈元素，结果值进栈
			OPR 0 13	次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
			OPR 0 14	栈顶值输出至屏幕
			OPR 0 15	屏幕输出换行
			OPR 0 16	从命令行读入一个输入置于栈顶
*/
/*******************************/
//用于emit代码对应的枚举类型变量如下:
int FANHUI = 0;
int QUFAN=1;
int JIA=2;
int JIAN=3;
int CHENG=4;
int CHU=5;
int JIOU = 6;
int XIANGDENG=  8;
int BUDENG = 9;
int XIAOYU = 10;
int DAYUDENGYU = 11;
int DAYU = 12;
int XIAOYUDENGYU = 13;
int SHUCHU = 14;
int HUAHANG = 15;
int DURU = 16;
enum F_seg { LIT, OPR, LOD, STO, CAL, MLC, JMP, JPC, RED, WRT };//其中用MLC替代了INT,因为INT是C++数据类型保留字
typedef struct emit_item
{
	enum F_seg F;
	int L;
	int A;
	vector <int>parammiter;
	vector<name_item>::iterator iter;
	int flag = 0;
	vector <int> pairof;
};
F_seg Fcode;
//每一条指令应包含3项: F L A
emit_item emititem;//直接定义成全局变量
vector <emit_item> code;//代码数组
vector <emit_item> ::iterator EmitPtr=code.begin();
void EMIT(F_seg Fcode, int L, int A)
{
	emititem.F = Fcode;
	emititem.L = L;
	emititem.A = A;
	code.push_back(emititem);
}

void EMIT(F_seg Fcode, int L, int A,  vector<int> param)//param数组中的[0]是返回地址, 从[1]开始是形参的值
{
	emititem.F = Fcode;
	emititem.L = L;
	emititem.A = A;
	vector<int>::iterator it = param.begin();
	for (; it != param.end(); it++)
	{
		emititem.parammiter.push_back(*it);
	}
	code.push_back(emititem);
}
void EMIT(F_seg Fcode, int L, int A, int flag, int retad , vector<int> pairof)
//仅用于读入的权宜之计
{
	emititem.F = Fcode;
	emititem.L = L;
	emititem.A = A;
	emititem.flag = 100;
	emititem.pairof = pairof;
	emititem.parammiter.push_back(retad);
	code.push_back(emititem);
}
bool proc(ifstream &file);
bool body(ifstream &file);
bool lexp(ifstream &file);
bool func_lop(ifstream &file);
bool statement(ifstream &file);

string errormsg="";
enum symble {  E, LeftPAR, RightPAR, PLUS, MULTI, DIVI, UMINUS };
ofstream midlefile;
bool readWord(ifstream &file, string dst)
//在file文件中读一个前缀dst，不回溯,读完这行
//用于直接检查一个前缀
//如果错了还是要回溯
{
LAB1:
	int now = file.tellg();
	string src = "";
	char temp = 0;
	while (!file.eof() && temp != '|')
	{
		temp = file.get();
		if (temp == -1)
		{
			file.close();
			exit(0);
		}

		if (temp == '\n')
		{
			src.clear();
			continue;
		}
		if (src == "error")
		{
			resultfile << "word error";
			while (temp != '|')
			{
				resultfile << temp;
				temp = file.get();
			}
			resultfile << endl;
			while (temp != '\n')
				temp = file.get();
			return false;
		}
		if (src == dst)
		{
			while (temp != '|')
				temp = file.get();
			return true;
		}
		src.push_back(temp);
	}
	while (temp != '\n')
	{
		if (temp == '|')
			temp = file.get();
		errormsg.push_back(temp);
		temp = file.get();
	}
	resultfile << "error: expect\"" << dst << "\"" << "[" << errormsg << "]" << endl;
	errormsg = "";
	file.seekg(now, ios::beg);
	return false;
}

bool readWord(ifstream &file, string key, string value)
//在file文件中先读key，再读value,不回溯
//如果错了还是要回溯
{
LAB2:
	int now = file.tellg();
	string src = "";
	char temp = 0;
	while (!file.eof() && temp != '|')
	{
		temp = file.get();
		if (temp == -1)
		{
			file.close();
			exit(0);
		}

		if (temp == '\n')
		{
			src.clear();
			continue;
		}
		if (temp == ':')
			continue;
		src.push_back(temp);
		if (src == "error")
		{
			if (src == "error")
			{
				resultfile << "word error";
				while (temp != '|')
				{
					resultfile << temp;
					temp = file.get();
				}
				resultfile << endl;
				while (temp != '\n')
					temp = file.get();
				return false;
			}
		}
		if (src == key)
		{
			src = "";
			temp = file.get();
			if (temp == ':')
			{
				temp = file.get();
				do
				{
					src.push_back(temp);
					temp = file.get();
				} while (temp != '|');
				if (src == value)
					return true;
			}
		}
	}
	while (temp != '\n')
	{
		if (temp == '|')
			temp = file.get();
		errormsg.push_back(temp);
		temp = file.get();
	}

	resultfile << "error: expect\"" << key << ":\"" << "\"" << value << "\""
		<< "[" << errormsg << "]" << endl;
	errormsg = "";
	file.seekg(now, ios::beg);
	return false;
}
string readWordNum(ifstream &file)
//读一个行号, 回溯
{
	int now = file.tellg();
	string num = "";
	char temp = 0;
	while (!file.eof() && temp != '|')
	{
		temp = file.get();
		if (temp == '|')
			break;
	}
	temp = file.get();
	while (!file.eof() && temp != '\n')
	{
		num.push_back(temp);
		temp = file.get();
	}
	file.seekg(now, ios::beg);
	return num;
}

string readWord(ifstream &file)
//在file文件中读一个key,返回value,回溯
{
	unsigned int now = file.tellg();
	string src = "";
	char temp = 0;
	while (!file.eof() && temp != '|')
	{
		temp = file.get();
		if (temp == -1)
		{
			file.close();
			exit(0);
		}

		if (temp == '\n')
		{
			src.clear();
			continue;
		}
		if (temp == ':')
		{
			src.clear();
			continue;//此处的修改可能引起巨大的副作用
		}
		if (temp == '|')
			continue;
		src.push_back(temp);
		if (src == "error")
		{
			resultfile << "word error";
			while (temp != '|')
			{
				resultfile << temp;
				temp = file.get();
			}
			resultfile << endl;
			while (temp != '\n')
				temp = file.get();
			return src;
		}
		if (src == "KeyWord" || src == "ID")
		{
			src = "";
			temp = file.get();
			if (temp == ':')
			{
				temp = file.get();
				do
				{
					src.push_back(temp);
					temp = file.get();
				} while (temp != '|');
				file.seekg(now, ios::beg);
				break;
			}
		}
		else if (src == "PAR0")
		{
			break;
		}
		else if (src == "PAR1")
		{
			file.seekg(now, ios::beg);
			return src;
		}
	}
	file.seekg(now, ios::beg);
	return src;
}


string readKeyWord(ifstream &file)
//预读一个KeyWord，返回KeyWord，回溯
{
LAB4:
	unsigned int now = file.tellg();
	string src = "";
	char temp = 0;
	while (!file.eof() && temp != '|')
	{
		temp = file.get();
		if (temp == -1)
		{
			file.clear();
			return "WRONGEND";
		}
		if (temp == '\n')
		{
			src.clear();
			continue;
		}
		if (temp == ':')
		{
			file.seekg(now, ios::beg);
			return src;
		}

		src.push_back(temp);
		if (src == "error")
		{
			resultfile << "word error";
			while (temp != '|')
			{
				resultfile << temp;
				temp = file.get();
			}
			resultfile << endl;
			while (temp != '\n')
			{
				temp = file.get();
				if (temp == -1)
				{
					file.close();
					exit(0);
				}
			}
			goto LAB4;
		}
	}
	file.seekg(now, ios::beg);
	return src;
}


//需要改写exp为一个类, 拥有string类型的place属性, 和check( )方法来检查接下来的一些代码是否是一个exp
class EXP
{
public:
	string EPLACE;
	bool isCon;
	int levdif;
	int offset;
	int value;//当isCon为1时有效, 为该常量的值
	void setEplace(string Eplace);
	void check(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector);
	string arrange(vector<EXP> &expvector, vector<enum symble> &status);
};

bool term(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector);

void EXP::check(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector)
{
	string src = "";
	bool flag = 0;
	src = readKeyWord(file);
	if (src == "aop")
	{
		string PLUSorUMINUS = readWord(file);
		if (PLUSorUMINUS == "+")
			status.push_back(PLUS);
		else
			status.push_back(UMINUS);
		flag = readWord(file, "aop");
	}
	flag = term(file, status, EXPvector);
	src = readKeyWord(file);
	while (src == "aop")
	{
		string PLUSorUMINUS = readWord(file);
		{
			if (PLUSorUMINUS == "+")
				status.push_back(PLUS);
			else
				status.push_back(UMINUS);
		}
		flag = readWord(file, "aop");
		flag = term(file,status,EXPvector);
		src = readKeyWord(file);
	} while (src == "aop");

}
void EXP::setEplace(string Eplace)
{
	EPLACE = Eplace;
}

string EXP::arrange(vector<EXP> &expvector, vector<enum symble> &status)
{
	vector<enum symble>::iterator it = status.end() - 1;
	vector<EXP>::iterator Eit = expvector.end() - 1;
	if (it >= status.begin() && *it == E)//如果最后一个是E
	{
		if (it > status.begin())
		{
			if (it - 1 >= status.begin() && *(it - 1) == UMINUS)//如果倒数第二项是负号
			{
				//先检查最后的这个E是否在符号表中可见范围内
				int rightValue = 0;
				//先检查最后的2个E是否在符号表中可见范围内
				vector<int> ::iterator displayPtr = display.end() - 1;
				vector<EXP>::iterator REit = expvector.end() - 1;
				if ((*REit).isCon == 1)//如果是个常数, 直接拿来用
				{
					EMIT(LIT, 0, (*REit).value);
					rightValue = (*REit).value;
				}
				else
				{
					displayPtr = display.end() - 1;
					name_item tempER = lookupProc(displayPtr, var, (*REit).EPLACE);
					if (tempER.level == 0)
					{
						displayPtr = display.end() - 1;
						tempER = lookupProc(displayPtr, con, (*REit).EPLACE);
						if (tempER.level == 0)
						{
							resultfile << "unexpect :" << (*REit).EPLACE << endl;
							return "error";
						}
						else//是个const
						{
							EMIT(LIT, 0, tempER.adrval);
							rightValue = tempER.adrval;
						}
					}
					else//是个var
					{
						EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
						rightValue = tempER.adrval;
					}
				}
				EMIT(OPR, 0, QUFAN);
				EXP e;
				e.setEplace(newtemp());//此处可能需要加入符号表, 暂时没来得及考虑
				e.isCon = 1;
				e.value = -rightValue;
				//想了三天, 确实需要加入符号表, 真的是烦死了
				name_item tempitem;
				//	//确认是var以后,维护符号表
				kind = var;//将接下来的数据形式存储为var
				tempitem.id = e.EPLACE;
				tempitem.level = display.size() - 1;
				if (display.back() != -1)
				{
					tempitem.link = lastlink(nametable, tempitem.level);
				}
				else
				{
					nametable.pop_back();
					tempitem.link = -1;
				}
				tempitem.kind = kind;

				tempitem.num = nametable.size();
				///***************************************************/
				////这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
				////这是为了在活动记录中填入它在该层中的相对位置
				vector<name_item> ::iterator nearestProc13 = lookupProcedure(nametable);
				int Proc_have13 = (*nearestProc13).adrval;
				tempitem.adrval = - rightValue;
				tempitem.entrance = Proc_have13;
				(*nearestProc13).adrval++;//该过程名所拥有的变量名+1
				///***************************************************/
				/*一切准备就绪, 填入符号表, 更新display表*/
				nametable.push_back(tempitem);
				display.back() = nametable.size() - 1;
				///*一切准备就绪, 填入符号表, 更新display表*/
				EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//将其写入当前过程名下的变量
				expvector.pop_back();//删掉-E的E
				expvector.push_back(e);//加入被划归的E
				status.pop_back();//删掉-E的"E"
				status.pop_back();//删掉-E的"-"
				status.push_back(PLUS);
				status.push_back(E);//加入新"E"
				arrange(expvector, status);//进入下一层检查
			}
			else if (it - 1 >= status.begin() && *(it - 1) == MULTI)//最后一个是E且倒数第二个是*
			{
				if (status.size() > 2 && *(it - 2) == E)//并且倒数第三个是E
				{
					int leftValue, rightValue = 0;
					//先检查最后的2个E是否在符号表中可见范围内
					vector<int> ::iterator displayPtr = display.end() - 1;
					vector<EXP>::iterator LEit = expvector.end() - 2;
					vector<EXP>::iterator REit = expvector.end() - 1;
					if ((*LEit).isCon == 1)//如果是个常数, 直接拿来用
					{
						EMIT(LIT, 0, (*LEit).value);
						leftValue = (*LEit).value;
					}
					else
					{
						name_item tempEL = lookupProc(displayPtr, var, (*LEit).EPLACE);
						if (tempEL.level == 0)
						{
							displayPtr = display.end() - 1;
							tempEL = lookupProc(displayPtr, con, (*LEit).EPLACE);
							if (tempEL.level == 0)
							{
								resultfile << "unexpect :" << (*LEit).EPLACE << endl;
								return "error";
							}
							else//是个const
							{
								EMIT(LIT, 0, tempEL.adrval);
								leftValue = tempEL.adrval;
							}
						}
						else//是个var
						{
							EMIT(LOD, display.size() - 1 - tempEL.level, tempEL.entrance + 3);
							leftValue = tempEL.adrval;
						}
					}

					if ((*REit).isCon == 1)//如果是个常数, 直接拿来用
					{
						EMIT(LIT, 0, (*REit).value);
						rightValue = (*REit).value;
					}
					else
					{
						displayPtr = display.end() - 1;
						name_item tempER = lookupProc(displayPtr, var, (*REit).EPLACE);
						if (tempER.level == 0)
						{
							displayPtr = display.end() - 1;
							tempER = lookupProc(displayPtr, con, (*REit).EPLACE);
							if (tempER.level == 0)
							{
								resultfile << "unexpect :" << (*REit).EPLACE << endl;
								return "error";
							}
							else//是个const
							{
								EMIT(LIT, 0, tempER.adrval);
								rightValue = tempER.adrval;
							}
						}
						else//是个var
						{
							EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
							rightValue = tempER.adrval;
						}
					}
					EMIT(OPR, 0, CHENG);
					EXP e;
					e.setEplace(newtemp());//此处可能需要加入符号表, 暂时没来得及考虑
					e.isCon = 1;
					e.value = leftValue * rightValue;
					name_item tempitem;
					//	//确认是var以后,维护符号表
					kind = var;//将接下来的数据形式存储为var
					tempitem.id = e.EPLACE;
					tempitem.level = display.size() - 1;
					if (display.back() != -1)
					{
						tempitem.link = lastlink(nametable, tempitem.level);
					}
					else
					{
						nametable.pop_back();
						tempitem.link = -1;
					}
					tempitem.kind = kind;

					tempitem.num = nametable.size();
					///***************************************************/
					////这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
					////这是为了在活动记录中填入它在该层中的相对位置
					vector<name_item> ::iterator nearestProc13 = lookupProcedure(nametable);
					int Proc_have13 = (*nearestProc13).adrval;
					tempitem.adrval = leftValue * rightValue;
					tempitem.entrance = Proc_have13;
					(*nearestProc13).adrval++;//该过程名所拥有的变量名+1
					///***************************************************/
					/*一切准备就绪, 填入符号表, 更新display表*/
					nametable.push_back(tempitem);
					display.back() = nametable.size() - 1;
					///*一切准备就绪, 填入符号表, 更新display表*/
					EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//将其写入当前过程名下的变量
					expvector.pop_back();//删掉E1*E2的E2
					expvector.pop_back();//删掉E1*E2的E1
					expvector.push_back(e);//加入被划归的E
					status.pop_back();//删掉E1*E2的"E"
					status.pop_back();//删掉E1*E2的"*"
					status.pop_back();//删掉E1*E2的"E"
					status.push_back(E);//加入新"E"
					arrange(expvector, status);//进入下一层检查
				}
			}
			else if (it - 1 >= status.begin() && *(it - 1) == PLUS)//最后一个是E且倒数第二个是+
			{
				if (status.size() > 2 && *(it - 2) == E)//并且倒数第三个是E
				{
					int leftValue, rightValue = 0;
					//先检查最后的2个E是否在符号表中可见范围内
					vector<int> ::iterator displayPtr = display.end() - 1;
					vector<EXP>::iterator LEit = expvector.end() - 2;
					vector<EXP>::iterator REit = expvector.end() - 1;
					if ((*LEit).isCon == 1)//如果是个常数, 直接拿来用
					{
						EMIT(LIT, 0, (*LEit).value);
						leftValue = (*LEit).value;
					}
					else
					{
						name_item tempEL = lookupProc(displayPtr, var, (*LEit).EPLACE);
						if (tempEL.level == 0)
						{
							displayPtr = display.end() - 1;
							tempEL = lookupProc(displayPtr, con, (*LEit).EPLACE);
							if (tempEL.level == 0)
							{
								resultfile << "unexpect :" << (*LEit).EPLACE << endl;
								return "error";
							}
							else//是个const
							{
								EMIT(LIT, 0, tempEL.adrval);
								leftValue = tempEL.adrval;
							}
						}
						else//是个var
						{
							EMIT(LOD, display.size() - 1 - tempEL.level, tempEL.entrance + 3);
							leftValue = tempEL.adrval;
						}
					}

					if ((*REit).isCon == 1)//如果是个常数, 直接拿来用
					{
						EMIT(LIT, 0, (*REit).value);
						rightValue = (*REit).value;
					}
					else
					{
						displayPtr = display.end() - 1;
						name_item tempER = lookupProc(displayPtr, var, (*REit).EPLACE);
						if (tempER.level == 0)
						{
							displayPtr = display.end() - 1;
							tempER = lookupProc(displayPtr, con, (*REit).EPLACE);
							if (tempER.level == 0)
							{
								resultfile << "unexpect :" << (*REit).EPLACE << endl;
								return "error";
							}
							else//是个const
							{
								EMIT(LIT, 0, tempER.adrval);
								rightValue = tempER.adrval;
							}
						}
						else//是个var
						{
							EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
							rightValue = tempER.adrval;
						}
					}
					EMIT(OPR, 0, JIA);
					EXP e;
					e.setEplace(newtemp());//此处可能需要加入符号表, 暂时没来得及考虑
					e.isCon = 1;
					e.value = leftValue + rightValue;
					name_item tempitem;
					//	//确认是var以后,维护符号表
					kind = var;//将接下来的数据形式存储为var
					tempitem.id = e.EPLACE;
					tempitem.level = display.size() - 1;
					if (display.back() != -1)
					{
						tempitem.link = lastlink(nametable, tempitem.level);
					}
					else
					{
						nametable.pop_back();
						tempitem.link = -1;
					}
					tempitem.kind = kind;

					tempitem.num = nametable.size();
					///***************************************************/
					////这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
					////这是为了在活动记录中填入它在该层中的相对位置
					vector<name_item> ::iterator nearestProc10 = lookupProcedure(nametable);
					int Proc_have10 = (*nearestProc10).adrval;
					tempitem.adrval = leftValue + rightValue;
					tempitem.entrance = Proc_have10;
					(*nearestProc10).adrval++;//该过程名所拥有的变量名+1
					///***************************************************/
					/*一切准备就绪, 填入符号表, 更新display表*/
					nametable.push_back(tempitem);
					display.back() = nametable.size() - 1;
					///*一切准备就绪, 填入符号表, 更新display表*/
					EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//将其写入当前过程名下的变量
					expvector.pop_back();//删掉E1*E2的E2
					expvector.pop_back();//删掉E1*E2的E1
					expvector.push_back(e);//加入被划归的E
					status.pop_back();//删掉E1*E2的"E"
					status.pop_back();//删掉E1*E2的"+"
					status.pop_back();//删掉E1*E2的"E"
					status.push_back(E);//加入新"E"
					arrange(expvector, status);//进入下一层检查
				}
				else if (it - 1 == status.begin() && *(it - 1) == PLUS)//如果最后一个是E, 且第一个是+, 就这俩符号, 则赋值语句归化结束
				{
					EXP e;
					setEplace(expvector.back().EPLACE);
					status.pop_back();//删除+E的"E"
					status.pop_back();//删除+E的"+"
					if (status.empty())
						return e.EPLACE;
				}
			}
			else if (it - 1 >= status.begin() && *(it - 1) == DIVI)//最后一个是E且倒数第二个是/
			{
				if (it - 2 >= status.begin() && *(it - 2) == E)//并且倒数第三个是E
				{
					//先检查最后的2个E是否在符号表中可见范围内
					vector<int> ::iterator displayPtr = display.end() - 1;
					vector<EXP>::iterator LEit = expvector.end() - 2;
					vector<EXP>::iterator REit = expvector.end() - 1;
					name_item tempEL = lookupProc(displayPtr, var, (*LEit).EPLACE);
					if (tempEL.level == 0)
					{
						displayPtr = display.end() - 1;
						tempEL = lookupProc(displayPtr, con, (*LEit).EPLACE);
						if (tempEL.level == 0)
						{
							resultfile << "unexpect :" << (*LEit).EPLACE << endl;
							return "error";
						}
						else//是个const
						{
							EMIT(LIT, 0, tempEL.adrval);
						}
					}
					else//是个var
					{
						EMIT(LOD, display.size() - 1 - tempEL.level, tempEL.entrance + 3);
					}
					displayPtr = display.end() - 1;
					name_item tempER = lookupProc(displayPtr, var, (*REit).EPLACE);
					if (tempER.level == 0)
					{
						displayPtr = display.end() - 1;
						tempER = lookupProc(displayPtr, con, (*REit).EPLACE);
						if (tempER.level == 0)
						{
							resultfile << "unexpect :" << (*REit).EPLACE << endl;
							return "error";
						}
						else//是个const
						{
							EMIT(LIT, 0, tempER.adrval);
						}
					}
					else//是个var
					{
						EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
					}
					EMIT(OPR, 0, CHU);
					EXP e;
					e.setEplace(newtemp());//此处可能需要加入符号表, 暂时没来得及考虑
					name_item tempitem;
					//	//确认是var以后,维护符号表
					kind = var;//将接下来的数据形式存储为var
					tempitem.id = e.EPLACE;
					tempitem.level = display.size() - 1;
					if (display.back() != -1)
					{
						tempitem.link = lastlink(nametable, tempitem.level);
					}
					else
					{
						nametable.pop_back();
						tempitem.link = -1;
					}
					tempitem.kind = kind;

					tempitem.num = nametable.size();
					///***************************************************/
					////这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
					////这是为了在活动记录中填入它在该层中的相对位置
					vector<name_item> ::iterator nearestProc10 = lookupProcedure(nametable);
					int Proc_have10 = (*nearestProc10).adrval;
					tempitem.adrval = 0;
					tempitem.entrance = Proc_have10;
					(*nearestProc10).adrval++;//该过程名所拥有的变量名+1
					///***************************************************/
					///*一切准备就绪, 填入符号表, 更新display表*/
					nametable.push_back(tempitem);
					display.back() = nametable.size() - 1;
					///*一切准备就绪, 填入符号表, 更新display表*/
					EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//将其写入当前过程名下的变量
					expvector.pop_back();//删掉E1*E2的E2
					expvector.pop_back();//删掉E1*E2的E1
					expvector.push_back(e);//加入被划归的E
					status.pop_back();//删掉E1*E2的"E"
					status.pop_back();//删掉E1*E2的"/"
					status.pop_back();//删掉E1*E2的"E"
					status.push_back(E);//加入新"E"
					arrange(expvector, status);//进入下一层检查
				}
			}
		}
	}
	else if (it >= status.begin() && *it == RightPAR)//如果最后一项是)
	{
		if (it - 1 >= status.begin() && *(it - 1) == E)//如果倒数第二项是E
		{
			if (it - 2 >= status.begin() && *(it - 2) == LeftPAR)//如果倒数第二项是(
			{
				setEplace(expvector.back().EPLACE);
				status.pop_back();//删掉(E)的")"
				status.pop_back();//删掉(E)的"E"
				status.pop_back();//删掉(E)的"("
				status.push_back(E);//加入新E;
				arrange(expvector, status);//进入下一层的检查
			}
		}
	}
	return EPLACE;
}

bool Const(ifstream &file)
{
	bool flag1 = 0;
	bool flag2 = 0;
	bool flag3 = 0;

	string constID = readWord(file);

	name_item nameitem;//新准备一个nameitem准备压入
	//将其加入nametable中, 同时维护btable;
	//应检查此时是否为该display表的新项, 若是则应立即弹出一项, 否则直接压入
	nameitem.level = display.size() - 1;
	if (display.back() == -1)
	{
		nametable.pop_back();
		nameitem.link = -1;
	}
	else
	{
		nameitem.link = lastlink(nametable, nameitem.level);
	}
	nameitem.id = constID;
	nameitem.kind = con;

	//还差const的值没有初始化

	flag1 = readWord(file, "ID");
	flag2 = readWord(file, "assign");

	string constVal_temp = readWord(file);
	if (constVal_temp == "-")
	{
		flag3 = readWord(file, "aop");
		constVal_temp = readWord(file);
		constVal_temp.insert(0, "-");
	}
	//维护该项的值
	char constVal[15] = { 0 };
	strcpy(constVal, constVal_temp.c_str());//将string转char*
	int constVAL = atoi(constVal);//将char*转int
	nameitem.adrval = constVAL;//将int赋给最后一项的值
	nameitem.num = nametable.size();
	/***************************************************/
	//这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
	//这是为了在活动记录中填入它在该层中的相对位置
	vector<name_item> ::iterator nearestProc0 = lookupProcedure(nametable);
	int Proc_have0 = (*nearestProc0).adrval;
	nameitem.entrance = Proc_have0;
	(*nearestProc0).adrval++;//该过程名所拥有的变量名+1
	/***************************************************/
	/*一切准备就绪, 填入符号表, 更新display表*/
	nametable.push_back(nameitem);
	display.back() = nametable.size() - 1;
	/*一切准备就绪, 填入符号表, 更新display表*/
	flag3 = readWord(file, "INT");
	if (!flag3)
	{
		resultfile << "error:fail to read INT" << endl;
	}
	return flag1&&flag2&&flag3;
}

bool condecl(ifstream &file)
{
	bool flag = 0;

	string option = "";
	flag = readWord(file, "KeyWord", "const");
	flag = Const(file);
	option = readWord(file);
	while (option == "PAR0")
	{
		flag = readWord(file, "PAR0", ",");
		flag = Const(file);
		option = readWord(file);
	}
	if (option == "PAR1")
	{
		flag = readWord(file, "PAR1", ";");
	}
	return flag;
}

bool vardecl(ifstream &file)
{
	bool flag = 0;

	string option;
	flag = readWord(file, "KeyWord", "var");
	if (!flag)
	{
		resultfile << "error: fail to read \"var\"" << endl;
	}
	//确认是var以后,维护符号表
	name_item nameitem;
	kind = var;//将接下来的数据形式存储为var
	string IDname = readWord(file);//得到变量名
	nameitem.id = IDname;
	nameitem.level = display.size() - 1;
	if (display.back() != -1)
	{
		nameitem.link = lastlink(nametable, nameitem.level);
	}
	else
	{
		nametable.pop_back();
		nameitem.link = -1;
	}
	nameitem.kind = kind;

	nameitem.num = nametable.size();
	/***************************************************/
	//这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
	//这是为了在活动记录中填入它在该层中的相对位置
	vector<name_item> ::iterator nearestProc6 = lookupProcedure(nametable);
	int Proc_have6 = (*nearestProc6).adrval;
	nameitem.adrval = 0;
	nameitem.entrance = Proc_have6;
	(*nearestProc6).adrval++;//该过程名所拥有的变量名+1
	/***************************************************/
	nametable.push_back(nameitem);
	display.back() = nametable.size() - 1;
	/*一切准备就绪, 填入符号表, 更新display表*/
	flag = readWord(file, "ID");
	if (!flag)
	{
		resultfile << "error: fail to read \"ID\"" << endl;
	}
	option = readWord(file);
	while (option == "PAR0")
	{
		flag = readWord(file, "PAR0", ",");
		//确认是var以后,维护符号表
		name_item nameitem;
		kind = var;//将接下来的数据形式存储为var
		string IDname = readWord(file);//得到变量名
		nameitem.id = IDname;
		nameitem.level = display.size() - 1;
		if (display.back() != -1)
		{
			nameitem.link = lastlink(nametable, nameitem.level);
		}
		else
		{
			nametable.pop_back();
			nameitem.link = -1;
		}
		nameitem.kind = kind;
		nameitem.num = nametable.size();
		/***************************************************/
		//这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
		//这是为了在活动记录中填入它在该层中的相对位置
		vector<name_item> ::iterator nearestProc2 = lookupProcedure(nametable);
		int Proc_have2 = (*nearestProc2).adrval;
		nameitem.adrval = 0;
		nameitem.entrance = Proc_have2;
		(*nearestProc2).adrval++;//该过程名所拥有的变量名+1
		/***************************************************/
		/*一切准备就绪, 填入符号表, 更新display表*/
		nametable.push_back(nameitem);
		display.back() = nametable.size() - 1;
		/*一切准备就绪, 填入符号表, 更新display表*/
		flag = readWord(file, "ID");
		option = readWord(file);
	}
	if (option == "PAR1")
	{
		flag = readWord(file, "PAR1", ";");
	}
	return flag && flag &&flag &&flag &&flag;
}

bool block(ifstream &file)
{
	//进入block, 不开辟一个新的display层
	//var
	//const
	//procdure id 均属同层
	bool flag = 0;
	string option = readWord(file);
	if (option == "const")
	{
		kind = con;//将接下来的id种属归类为const
		flag = condecl(file);
		option = readWord(file);
		if (option == "var")
		{
			flag = vardecl(file);
			option = readWord(file);
			if (option == "procedure")
				flag = proc(file);
		}
		else if (option == "procedure")
			flag = proc(file);
	}
	else if (option == "var")
	{
		flag = vardecl(file);
		option = readWord(file);
		if (option == "procedure")
			flag = proc(file);
	}
	else if (option == "procedure")
	{
		flag = proc(file);
	}
	fromproc = 0;
	flag = body(file);
	return flag;
}
bool body(ifstream &file)
{
	bool flag = 0;
	string src = "";
	flag = readWord(file, "KeyWord", "begin");
	//首先向代码段首方向找最近的JMP语句(且未被填入地址的  这是为了跳过嵌套定义的过程), 将编号回填
	for (EmitPtr = code.end() - 1; EmitPtr >= code.begin(); EmitPtr--)
	{
		if ((*EmitPtr).F == JMP && (*EmitPtr).A == 0)
		{
			(*EmitPtr).A = code.size();
			/****************************************************/
			break;
		}
	}

	//读过begin后, 再将该过程名下登记的var和const(不包括param)入内存栈
	vector<name_item> ::iterator iter = lookupProcedure(nametable);
	int N = (*iter).adrval;
	iter++;
	for (int i = 0; i < N; i++,iter++)
	{
		if ((*iter).kind == param)
			continue;
		else
			EMIT(LIT, 0, (*iter).adrval);//var和const的值均入栈
	}

	//以当前display表的最后一个在册记录做索引, 向前找procedure名
	vector<int> ::iterator displayPtr = display.end()-2;//分析得到: 应在当前过程的前一层中找过程名
	name_item nameitem=lookupProc(displayPtr, procedure);
	adrvalNum = nameitem.adrval;
	nowlevel = nameitem.level;
	//以下工作应该在看到call时展开
	////动态链:
	//Fcode = LOD;
	//EMIT(Fcode, 0, 0);//当前层偏移量为[0]的值送栈顶
	////返回地址:
	//Fcode = LIT;
	//EMIT(Fcode, 0, retadr);//认为该值在遇到call时一定会得到, 如果某过程没有即使生成了

	//Fcode = MLC;
	//EMIT(Fcode, 0, adrvalNum+5);//MLC指令下达时, 应维护SP和TOP指针
	////adrvalNum是该活动记录需要用到的变量和常量的数目
	////5: 动态链+返回地址+静态链+参数数目+当前活动记录所在的绝对层次数
	///*********建立活动记录*****************/
	////首先已经由上一条EMIT指令为栈开辟了adrvalNum+5个空间
	////存入动态链
	////if (mem.empty())//当程序刚刚进入时内存为空, 首先对内存初始化
	////{
	////	//动态链初始化为0
	////	Fcode = LIT;//常数入栈指令
	////	EMIT(Fcode, 0, 0);
	////	//返回地址初始化为0
	////	EMIT(Fcode, 0, 0);
	////	//静态链初始化为0
	////	EMIT(Fcode, 0, 0);
	////}
	//	Fcode = LOD;
	//	EMIT(Fcode, 1, 0);//动态链: 上[1]层偏移量为[0]的值: 即上一个活动的基地址
	//	Fcode = LIT;
	//	EMIT(Fcode, 0, retadr);//返回地址: 认为它在遇到call时一定会得到
	//	Fcode = LOD;
	//	//静态链: 
	//	if (leveldiff == -1)
	//		//1. N层调用N+1层:
	//	{
	//		EMIT(Fcode, 1, 0);//被调过程的静态链=调用过程的起始地址
	//	}
	//	else if (leveldiff == 0)
	//		//2.N层调用N层:
	//	{
	//		EMIT(Fcode, 1, 2);//被调过程的静态链=调用过程的静态链
	//	}
	//	else
	//		//3.N层调用N-n层, leveldiff=n
	//	{
	//		////先要沿着当前活动记录(N层)的静态链向上找n次
	//		//int laststatic = mem[sp+2];
	//		//int lastsp = mem[sp];//以当前静态链为起点
	//		//for (int i = leveldiff; i > 0; i--)
	//		//{
	//		//	lastsp=mem[la]
	//		//	laststatic = mem[lastsp+2];
	//		//}
	//	}
	//	sp= top + 1;
	//	top = sp + adrvalNum+3;
	////接下来是每次进入procedure都要生成的代码和操作: adrvalNum值入栈+当前过程的绝对层数入栈+将变量和常量入栈
	////adrvalNum值入栈
	//Fcode = LIT;
	//EMIT(Fcode, 0, adrvalNum);
	////将当前过程的绝对层数入栈
	//Fcode = LIT;
	//EMIT(Fcode, 0, nowlevel);
	////找adrvalNum个变量和常量, 是常量的直接写值, 是变量的使用LOD指令间接访问变量的值并存栈顶
	//vector<int> ::iterator displayPtr_t = display.end() - 1;//分析得到: 应在当前过程的层次中找过程名
	//int tempadrvalNum = adrvalNum;//需要从符号表中找出tempadrvalNum个变量并存入栈中
	//for (name_item nameitem = nametable[(*displayPtr_t)]; nameitem.level != 0; )
	//{
	//	if (nameitem.kind == con)
	//	{
	//		Fcode = LIT;
	//		EMIT(Fcode, 0, nameitem.adrval);
	//		tempadrvalNum--;
	//		if (tempadrvalNum == 0)
	//			break;
	//		else
	//			continue;
	//	}
	//	else if (nameitem.kind == var)
	//	{

	//	}
	//	else
	//	{
	//		if (nameitem.link != -1)//如果未达到本层起点, 沿着link链找即可
	//		{
	//			nameitem = nametable[nameitem.link];
	//			continue;
	//		}
	//		else//向上一层查找
	//		{
	//			if (displayPtr != display.begin())
	//			{
	//				displayPtr--;
	//				nameitem = nametable[(*displayPtr)];
	//				continue;
	//			}
	//			else
	//			{
	//				nameitem = nametable[0];//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
	//				break;
	//			}
	//		}
	//	}
	//}
	/****************************************/
	flag = statement(file);
	src = readKeyWord(file);
	while (src == "PAR1")
	{
		flag=readWord(file, "PAR1", ";");
		flag = statement(file);
		src = readKeyWord(file);
		if (src == "WRONGEND")
		{
			resultfile << "unexpected end of code" << endl;
			exit(0);
		}
	}
	flag = readWord(file, "KeyWord", "end");
	LEV--;
	display.pop_back();
	if(!fromproc)
		EMIT(OPR, 0, FANHUI);
	return flag;
}
bool statement(ifstream &file)
{
	EXP tmp;
	vector < enum symble > status;
	vector <EXP> EXPvector;
	bool flag = 0;
	string src = "";
	src = readKeyWord(file);
	if (src == "ID")
	{
		string idVAL = readWord(file);
		vector<int> ::iterator displayPtr = display.end() - 1;//分析得到: 应在当前过程的前一层中找变量名
		name_item nameitem = lookupProc(displayPtr, var, idVAL);
		//如果变量未定义, 读过这条语句但不分析
		if (nameitem.level == 0)
		{
			string num = readWordNum(file);
			resultfile << "undefined var : " << idVAL << "[" << num << "]" << endl;
			flag = readWord(file, "ID");
			flag = readWord(file, "assign");
			tmp.check(file, status, EXPvector);
			return flag;
		}
		//如果变量已定义, 则要记录它的层差和偏移量
		int levdif = display.size() - 1 - nameitem.level;
		int offset = nameitem.entrance+3;
		flag = readWord(file, "ID");
		flag = readWord(file, "assign");
		tmp.check(file, status, EXPvector);
		//此时将EXPvector[0]中的值赋给左变量
		string RightName = EXPvector[0].EPLACE;
		displayPtr = display.end() - 1;
		name_item itemtmp = lookupProc(displayPtr, var, RightName);
		if (itemtmp.num == 0)//如果没找到, 入符号表!
		{
			name_item tempitem;
			//	//确认是var以后,维护符号表
			kind = var;//将接下来的数据形式存储为var
			tempitem.id = RightName;
			tempitem.level = display.size() - 1;
			if (display.back() != -1)
			{
				tempitem.link = lastlink(nametable, tempitem.level);
			}
			else
			{
				nametable.pop_back();
				tempitem.link = -1;
			}
			tempitem.kind = kind;

			tempitem.num = nametable.size();
			///***************************************************/
			////这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
			////这是为了在活动记录中填入它在该层中的相对位置
			vector<name_item> ::iterator nearestProc11 = lookupProcedure(nametable);
			int Proc_have11 = (*nearestProc11).adrval;
			tempitem.adrval = EXPvector[0].value;
			tempitem.entrance = Proc_have11;
			(*nearestProc11).adrval++;//该过程名所拥有的变量名+1
			///***************************************************/
			/*一切准备就绪, 填入符号表, 更新display表*/
			nametable.push_back(tempitem);
			display.back() = nametable.size() - 1;
			///*一切准备就绪, 填入符号表, 更新display表*/

			/******************************************************************************/
			itemtmp = tempitem;
			EMIT(LIT, 0, tempitem.adrval);
			EMIT(STO, display.size() - 1 - itemtmp.level, itemtmp.entrance + 3);
		}
		EMIT(LOD, display.size() - 1 - itemtmp.level, itemtmp.entrance+3);
		EMIT(STO, levdif, offset);
		midlefile << endl;
	}
	else if (src == "KeyWord")
	{
		src = readWord(file);//src=后缀
		if (src == "if")
		{
			flag = readWord(file, "KeyWord", "if");
			flag = lexp(file);
			flag = readWord(file, "KeyWord", "then");//
			EMIT(JPC, 0, 0);
			flag = statement(file);
			EMIT(JMP, 0, code.size() + 1);
			int nownum = code.size() + 1;
			//现在要回填之前那条JPC的A域
			vector<emit_item>::iterator emiter = code.end() - 1;
			for (; emiter != code.begin(); emiter--)
			{
				if ((*emiter).F == JPC && (*emiter).A == 0)
				{
					(*emiter).A = nownum;
					break;
				}
			}
			if (emiter == code.begin())
				resultfile << "expect keyword: if" << endl;
			src = readWord(file);
			if (src == "else")
			{ 
				nownum = code.size() + 1;
				for (; emiter != code.begin(); emiter--)
				{
					if ((*emiter).F == JPC)
					{
						(*emiter).A = nownum;
						break;
					}
				}
				if (emiter == code.begin())
					resultfile << "expect keyword: if" << endl;
				flag = readWord(file, "KeyWord");
				flag = statement(file);

				emiter = code.end() - 1;
				nownum = code.size() + 1;
				for (; emiter != code.begin(); emiter--)
				{
					if ((*emiter).F == JMP)
					{
						(*emiter).A = nownum;
						break;
					}
				}
				if (emiter == code.begin())
					resultfile << "expect keyword: if" << endl;
			}

			return flag;
		}
		else if (src == "while")
		{
			flag = readWord(file, "KeyWord");
			int loopnum = code.size();
			flag = lexp(file);
			EMIT(JPC, 0, 0);
			flag = readWord(file, "KeyWord", "do");
			flag = statement(file);

			EMIT(JMP, 0, loopnum);
			int nownum = code.size() + 1;
			//现在要回填之前那条JPC的A域
			vector<emit_item>::iterator emiter = code.end() - 1;
			for (; emiter != code.begin(); emiter--)
			{
				if ((*emiter).F == JPC && (*emiter).A == 0)
				{
					(*emiter).A = nownum;
					break;
				}
			}
			if (emiter == code.begin())
				resultfile << "expect keyword: if" << endl;
			return flag;
		}
		else if (src == "call")
		{
			int flagp=0;
			////确认是call后, 立刻取得目标代码指令返回地址
			retadr = code.size() + 1;
			vector<int> paramlist;
			paramlist.clear();
			paramlist.push_back(retadr);//paramlist[0]是返回地址,从[1]开始是形参的值
			vector<int> pairof;
			flag = readWord(file, "KeyWord");
			//得到过程名
			string procname = readWord(file);

			vector<int> ::iterator displayPtr = display.end() - 1;//分析得到: 应在当前过程所在层中找名
			name_item nameitem = lookupProc(displayPtr, procedure, procname);
			int layerdiff = LEV - nameitem.level;//层差
			//Fcode = CAL;
			//EMIT(Fcode, 0, nameitem.entrance);
			////CAL指令的执行: TOP指针以上, 保存动态链  返回地址  静态链   更新SP指针和TOP指针
			//现在还太早了!

			flag = readWord(file, "ID");
			flag = readWord(file, "LPAR");
			tmp.check(file, status, EXPvector);//**
			if(EXPvector[0].isCon)
				paramlist.push_back(EXPvector[0].value);//
			else
			{
				flagp = 100;
				pairof.push_back(EXPvector[0].levdif);
				pairof.push_back(EXPvector[0].offset);
			}
			EXPvector.clear();
			status.clear();
			src = readKeyWord(file);
			if (src == "ID" || src == "aop" || src == "int" || src == "LPAR")
			{
				src = "PAR0";
			}
			while (src == "PAR0")
			{
				flag = readWord(file, "PAR0", ",");
				tmp.check(file, status, EXPvector);//**
				if (EXPvector[0].isCon)
					paramlist.push_back(EXPvector[0].value);//
				else
				{
					flag = 100;
					pairof.push_back(EXPvector[0].levdif);
					pairof.push_back(EXPvector[0].offset);
				}
				src = readKeyWord(file);
				if (src == "ID" || src == "aop" || src == "int" || src == "LPAR")
				{
					src = "PAR0";
				}
			}
			flag = readWord(file, "RPAR");
			if (flagp != 100)
				EMIT(CAL, layerdiff, nameitem.entrance, paramlist);
			else
				EMIT(CAL, layerdiff, nameitem.entrance, 100, retadr, pairof);
			return flag;
		}
		else if (src == "begin")
		{
			fromproc = 1;
			EMIT(JMP, 0, 0);
			flag=body(file);
			fromproc = 0;
		}
		else if (src == "read")
		{
			flag = readWord(file,"KeyWord");
			flag = readWord(file, "LPAR");
			string idname = readWord(file);//读到id

			vector<int> ::iterator displayPtr = display.end() - 1;
			name_item nameitem= lookupProc(displayPtr, var, idname);
			if (nameitem.num == 0)
			{
				resultfile << "undefined var: " << idname << endl;
			}
			displayPtr = display.end() - 1;
			vector<name_item>::iterator iter = lookupPtr(displayPtr, var, nameitem.id);
			int laydif = LEV - (nameitem.level-1);//层差
			EMIT(OPR, 0, DURU);
			EMIT(STO, laydif, nameitem.entrance+3);
			flag = readWord(file, "ID");
			src = readKeyWord(file);
			while (src == "PAR0")
			{
				flag = readWord(file, "PAR0", ",");
				flag = readWord(file, "ID");
				src = readKeyWord(file);
			}
			flag = readWord(file, "RPAR");
		}
		else if (src == "write")
		{
			flag = readWord(file, "KeyWord");
			flag = readWord(file, "LPAR");
			tmp.check(file, status, EXPvector);//**
			if (!EXPvector[0].isCon)
				EMIT(LOD, EXPvector[0].levdif, EXPvector[0].offset);
			else
				EMIT(LIT, 0, EXPvector[0].value);
			EMIT(OPR, 0, SHUCHU);
			src = readKeyWord(file);
			while (src == "PAR0")
			{
				flag = readWord(file, "PAR0", ",");
				tmp.check(file, status, EXPvector);//**
				EMIT(OPR, 0, SHUCHU);
				src = readKeyWord(file);
			}
			flag=readWord(file,"RPAR");
		}
		else if (src == "end")
		{
			string errormsg = "";
			char temp = ' ';
			int now = file.tellg();
			temp = file.get();
			while (temp!= '\n')
			{
				errormsg.push_back(temp);
				temp = file.get();
			}
			resultfile << "error: expect statement, maybe no \"end\" or have extra \";\"  " <<"["<< errormsg<<"]" << endl;
			file.seekg(now, ios::beg);
			LEV--;
		}
	}
	return flag;
}

bool lexp(ifstream &file)
{
	EXP e;
	vector < enum symble > status;
	vector <EXP> EXPvector;
	bool flag = 0;
	string src = "";
	src = readWord(file);
	if (src == "odd")
	{
		EXP e;
		flag = readWord(file, "KeyWord", "odd");
		e.check(file, status, EXPvector);//** 
		if (EXPvector[0].isCon)
			EMIT(LIT, 0, EXPvector[0].value);
		else
			EMIT(LOD, EXPvector[0].levdif, EXPvector[0].offset);
		EMIT(OPR, 0, JIOU);
		return flag;
	}
	e.check(file, status, EXPvector);
	if (EXPvector[0].isCon)
		EMIT(LIT, 0, EXPvector[0].value);
	else
		EMIT(LOD, EXPvector[0].levdif, EXPvector[0].offset);
	string lopsyb = readWord(file);
	flag = func_lop(file);
	e.check(file, status, EXPvector);
	if (EXPvector[1].isCon)
		EMIT(LIT, 0, EXPvector[1].value);
	else
		EMIT(LOD, EXPvector[1].levdif, EXPvector[1].offset);
	if (lopsyb == "<")
		EMIT(OPR, 0, XIAOYU);
	else if (lopsyb == "<=")
		EMIT(OPR, 0, XIAOYUDENGYU);
	else if (lopsyb == ">")
		EMIT(OPR, 0, DAYU);
	else if (lopsyb == ">=")
		EMIT(OPR, 0, DAYUDENGYU);
	else if (lopsyb == "<>")
		EMIT(OPR, 0, BUDENG);
	else if (lopsyb == "=")
		EMIT(OPR, 0, XIANGDENG);
	return flag;
}

bool func_lop(ifstream &file)
{
	bool flag = 0;
	flag = readWord(file, "lop");
	return flag;
}
bool func_mop(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector)
{
	string mopVAL = readWord(file);
	if (mopVAL == "*")
	{
		status.push_back(MULTI);
	}
	else if (mopVAL == "/")
	{
		status.push_back(DIVI);
	}
	bool flag = 0;
	flag = readWord(file, "mop");
	return flag;
}
bool func_aop(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector)
{
	string mopVAL = readWord(file);
	if (mopVAL == "+")
	{
		status.push_back(PLUS);
	}
	else if (mopVAL == "-")
	{
		status.push_back(UMINUS);
	}
	bool flag = 0;
	flag = readWord(file, "mop");
	return flag;
}


bool factor(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector)
{
	EXP e;
	bool flag = 0;
	EXP tmp;
	string src = "";
	src = readKeyWord (file);
	if (src == "ID")
	{
		string idname = readWord(file);
		//此处应编写lookup函数
		vector<int> ::iterator displayPtr = display.end() - 1;
		name_item nameitem = lookupProc(displayPtr, var, idname);
		if (nameitem.level == 0)
		{
			displayPtr = display.end() - 1;
			nameitem = lookupProc(displayPtr, con, idname);
			if (nameitem.level == 0)
			{
				string num = readWordNum(file);
				resultfile << "undefined  : " << idname << "[" << num << "]" << endl;
				flag = readWord(file, "ID");
				flag = readWord(file, "assign");
				flag = readWord(file, "ID");
				return flag;
			}
			else//是一个常量
			{
				e.isCon = 1;
				e.value = nameitem.adrval;
			}
		}
		else//是一个变量
		{
			e.isCon = 0;
			e.levdif = display.size() - 1 - nameitem.level;
			e.offset = nameitem.entrance+3;
			e.value = nameitem.adrval;
		}
		e.setEplace(idname);
		status.push_back(E);
		EXPvector.push_back(e);
		string EPLACE = e.arrange(EXPvector, status);
		flag = readWord(file, "ID");
	}

	else if (src == "INT")
	{
		string number = readWord(file);
		char constVal[15] = { 0 };
		strcpy(constVal, number.c_str());//stringchar*
		int numberVAL = atoi(constVal);//char*int
		e.setEplace(newtemp());
		e.isCon = 1;
		e.value = numberVAL;
		EXPvector.push_back(e);
		status.push_back(E);
		string EPLACE = e.arrange(EXPvector, status);
		flag = readWord(file, "INT");
	}
	else if (src == "LPAR")
	{
		status.push_back(LeftPAR);
		readWord(file,"LPAR");
		tmp.check(file, status, EXPvector);
		string RPARtest = readKeyWord(file);
		if (RPARtest == "RPAR")
		{
			status.push_back(RightPAR);
			tmp.arrange(EXPvector,status);
		}
		flag = readWord(file, "RPAR");
	}
	return flag;
}
bool proc(ifstream &file)
{
	LEV++;
	name_item nameitem;
	bool flag = 0;
	string option;
	flag = readWord(file, "KeyWord", "procedure");
	kind = procedure;//将接下来的名字种属定义为过程
	string procName = readWord(file);
	nameitem.adrval = 0;//一个过程中声明的参数个数初始化为0
	nameitem.id = procName;
	nameitem.level = display.size() - 1;
	nameitem.kind = kind;

	//记录该名字的入口地址
	int entrance = code.size();
	nameitem.entrance = entrance;
	//生成P代码
	Fcode = JMP;
	EMIT(Fcode, 0, 0);

	if (display.back() != -1)
	{
		nameitem.link = lastlink(nametable, nameitem.level);
	}
	else
	{
		nametable.pop_back();
		nameitem.link = -1;
	}
	nameitem.num = nametable.size();
	nametable.push_back(nameitem);
	display.back()=nametable.size()-1;
	flag = readWord(file, "ID");
	flag = readWord(file, "LPAR", "(");
	//读到一个'('时, 应开辟一个新的display层
	newdisplay();
	option = readKeyWord(file);
	if (option == "ID")
	{
		int tempentrance = 0;
		kind = param;
		string IDName = readWord(file);
		name_item nameitem;
		nameitem.id = IDName;
		nameitem.kind = kind;
		nameitem.level = display.size() - 1;
		if (display.back() != -1)
		{
			nameitem.link = lastlink(nametable, nameitem.level);
		}
		else
		{
			nametable.pop_back();
			nameitem.link = -1;
		}
		nameitem.num = nametable.size();
		nameitem.entrance = tempentrance++;//记录该变量在所属过程中的偏移量
		/***************************************************/
		//这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
		//这是为了在活动记录中填入它在该层中的相对位置
		vector<name_item> ::iterator nearestProc4 = lookupProcedure(nametable);
		int Proc_have4 = (*nearestProc4).adrval;
		nameitem.adrval = Proc_have4 + 1;
		(*nearestProc4).adrval+=1;//该过程名所拥有的变量名+1
		/***************************************************/
		/*一切准备就绪, 填入符号表, 更新display表*/
		nametable.push_back(nameitem);
		display.back() = nametable.size() - 1;
		/*一切准备就绪, 填入符号表, 更新display表*/
		flag = readWord(file, "ID");
		option = readWord(file);
		while (option == "PAR0")
		{
			flag = readWord(file, "PAR0", ",");
			kind = param;
			string IDName = readWord(file);
			name_item nameitem;
			nameitem.id = IDName;
			nameitem.kind = kind;
			/***************************************************/
			//这时要使用int lookupProcedure()函数查得改变量所属过程名下已经登记了几个变量, 将其+1填入adrval值中
			//这是为了在活动记录中填入它在该层中的相对位置
			vector<name_item> ::iterator nearestProc5 = lookupProcedure(nametable);
			int Proc_have5 = (*nearestProc5).adrval;
			nameitem.adrval = Proc_have5 + 1;
			(*nearestProc5).adrval++;//该过程名所拥有的变量名+1
			/***************************************************/
			nameitem.level = display.size() - 1;
			if (display.back() != -1)
			{
				nameitem.link = lastlink(nametable, nameitem.level);
			}
			else
			{
				nametable.pop_back();
				nameitem.link = -1;
			}
			nameitem.num = nametable.size();
			nameitem.entrance = tempentrance++;//记录该变量在所属过程中的偏移量
			/*一切准备就绪, 填入符号表, 更新display表*/
			nametable.push_back(nameitem);
			display.back() = nametable.size() - 1;
			/*一切准备就绪, 填入符号表, 更新display表*/
			flag = readWord(file, "ID");
			option = readKeyWord(file);
		}
	}
	flag = readWord(file, "RPAR", ")");
	flag = readWord(file, "PAR1", ";");
	flag = block(file);
	//在进入block(statement)时必须检查变量定义与否
	option = readWord(file);
	while (option == "PAR1")
	{
		readWord(file, "PAR1");
		proc(file);
		option = readWord(file);
	}
	return flag;
}

bool term(ifstream &file, vector < enum symble > &status, vector <EXP> &EXPvector)
{
	bool flag = 0;
	string src = "";
	flag = factor(file, status, EXPvector);
	src = readKeyWord(file);
	while (src == "mop")
	{
		flag = func_mop(file, status, EXPvector);
		flag = factor(file, status, EXPvector);
		src = readKeyWord(file);
	}
	return flag;
}

bool prog(ifstream &file)
{
	midlefile.open("midFile.txt", ios::out);
	bool flag = 0;
	newdisplay();
	flag = readWord(file, "KeyWord", "program");
	{
		kind = procedure;
		name_item nameitem;
		//读到程序id后开始初始化第一项
		string programID = readWord(file);
		nameitem.id = programID;
		nameitem.kind = kind;
		nameitem.adrval = 0;//**********************
		//更新名字表的id和类型
		nameitem.level = display.size() - 1;
		if (display.back() != -1)
		{
			nameitem.link = lastlink(nametable, nameitem.level);
		}
		else
		{
			nametable.pop_back();
			nameitem.link = -1;
		}
		nameitem.num = nametable.size();
		nameitem.entrance = 0;//主程序的入口地址为0
		nametable.push_back(nameitem);
		display.back() = nametable.size() - 1;
	}
	//生成第一条P代码:
	Fcode = JMP;
	EMIT(Fcode, 0, 0);

	flag = readWord(file, "ID");
	flag = readWord(file, "PAR1", ";");
	newdisplay();
	LEV=0;
	flag = block(file);
	return flag;
}