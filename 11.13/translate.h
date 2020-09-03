#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <fstream>
#include <stdlib.h>
#include "grammer.h"
using namespace std;
#pragma warning(disable:4996)
int newlable = 0;
//临时变量表编号,该编号只增不减, 每次用完了+1
//但后来并未使用形如"T0:=a+b"形式的四元式作为中间代码
//使用P代码直接生成目标代码, 于是此变量及其引用函数作废, 仅在编程中途调试中测试使用
enum KIND
{
	con, var, procedure, param
};
//每个名字的种类:常量名, 变量名, 过程名, 形参

//*********************************************************************
typedef struct name_item
{
	string id;
	enum KIND kind;
	unsigned int level;
	int adrval;//若kind==con, adrval=val ; 若kind==var, adrval=adr; 若kind==procedure, adrval=该过程声明的变量个数
	int link;//同一程序体中,link为上一name在nametable中的位置; 每一个程序体第一个name的link值为0
	//为了精确的找到同名不同层的ID在nametable中的位置, 应再加入一个变量:num, 记录该名字在nametable中的位置
	int num;
	int entrance;//若kind==procedure, 该变量为过程入口的目标代码序号
						//若kind==var或con, 该变量为该变量在所属过程中的变量号, 即从偏移量
};
//名字表项
//vector<name_item> nametable;
//名字表模板, 包括一个名字表nametb 和 tx指针,该指针指向名字表下一个位置, 应初始化为0
//*********************************************************************


////*********************************************************************
//typedef struct block_item
//{
//	int lastpar;//指向本程序体中最后一个形式参数的名字在nametable中的位置
//	int last;//指向本程序体中最后一个名字在nametable中的位置
//};
//程序体表项, 每一项包括2个指针
//vector<block_item> btable;
//程序体表模板, 包括一个程序体表 和 bx指针, 该指针指向程序体表下一个位置, 应初始化为0
//*********************************************************************


//*********************************************************************
//stack<int> displaytable;
//display表模板, 包括一个向下增长的栈, level通过displaytable.size()取得, 在此只做注释中的形式定义
//*********************************************************************

string newtemp()
{
	string t_l = "T";
	char t_rtemp[6];
	itoa(newlable, t_rtemp, 5);
	string t_r = t_rtemp;
	newlable++;
	return t_l + t_r;
}
//新的形参名称生成器,每次调用将返回递增的变量名,T0 T1,T2..., 返回类型为sring, 后期删除该函数

vector<int> display;//合并display表与btable表, 抛弃btable表中的"最后一个形参"
									//直接用display栈顶元素表示当前显示层可用的最后一个名字在名字表中的位置
									//用display栈中元素数目表示当前层次数
vector<name_item> nametable;


//每当进入一层新的display调用此函数:
void newdisplay()
{
	//对三项符号表开始初始化
//建立display新项
	display.push_back(-1);//将程序开头初始化为-1
	//建立nametable新项
	name_item nameitem;
	nameitem.level = display.size()-1;//主程序名称为第0层, 此后从第1层开始
	nameitem.link = -1;//新层的第一个link初始化为-1
	nameitem.adrval = 0;//将值 或 地址 或参数个数 初始化为0, 仅仅是为了保险, 副作用尚不明确
	nametable.push_back(nameitem);

}
int lastlink(vector<name_item> nametable, int layer)
//该函数用于寻找当前层次中上一个名字的link值
//用于在建立符号表时填入link值
{
	vector<name_item> :: iterator it=nametable.end()-1;
	while (it != nametable.begin())
	{
		if ((*it).level == layer)//如果符号表中某名字的层次为当前层次, 则将其地址返回
			return (*it).num;
		else
			it--;
	}
	return -1;
}
vector<name_item> ::iterator  lookupProcedure(vector<name_item> &nametable)
//该函数仅用于定义var时, 从下往上查询最近的一个procedure, 返回该名字的指针
{
	vector<name_item> ::iterator it;
	if (display.size() == 1)
		return nametable.begin();
	vector<int> ::iterator displayPtr = display.end() - 2;
	if (*(display.end() - 2) == -1)
		it = nametable.end() - 1;
	else
		it = nametable.begin()+(*displayPtr);
	while (it != nametable.begin())
	{
		if ((*it).kind == procedure)
			return it;
		else
		{
			if ((*it).link != -1)//如果未达到本层起点, 沿着link链找即可
			{
				it = nametable.begin()+(*it).link;
				continue;
			}
			else//向上一层查找
			{
				if (displayPtr != display.begin())
				{
					displayPtr--;
					it = nametable.begin()+(*displayPtr);
					continue;
				}
				else
				{
					//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
					return nametable.begin();
				}
			}
		}
		return nametable.begin();
	}
	return nametable.begin();
}

name_item lookupProc(vector<int> ::iterator &displayPtr , enum KIND kind)//仅按照kind查找过程, 返回一个name_item类型变量
{
	name_item nameitem= nametable[(*displayPtr)];
	for (nameitem = nametable[(*displayPtr)]; nameitem.level != 0; )
	{
		if (nameitem.kind == kind)
		{
			return nameitem;
		}
		else
		{
			if (nameitem.link != -1)//如果未达到本层起点, 沿着link链找即可
			{
				nameitem = nametable[nameitem.link];
				continue;
			}
			else//向上一层查找
			{
				if (displayPtr != display.begin())
				{
					displayPtr--;
					nameitem = nametable[(*displayPtr)];
					continue;
				}
				else
				{
					nameitem = nametable[0];//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
					return nameitem;
				}
			}
		}
	}
	return nameitem;
}

name_item lookupProc(vector<int> ::iterator &displayPtr, enum KIND kind, string name)//按照kind和过程名查找过程, 返回一个name_item类型变量
{
	name_item nameitem;
	if (kind == var)
	{
		for (nameitem = nametable[(*displayPtr)]; nameitem.level != 0; )
		{
			if ((nameitem.kind == kind || nameitem.kind == param ) && nameitem.id == name)
			{
				return nameitem;
			}
			else
			{
				if (nameitem.link != -1)//如果未达到本层起点, 沿着link链找即可
				{
					nameitem = nametable[nameitem.link];
					continue;
				}
				else//向上一层查找
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable[(*displayPtr)];
						continue;
					}
					else
					{
						nameitem = nametable[0];//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
						return nameitem;
					}
				}
			}
		}

	}
	else
	{
		for (nameitem = nametable[(*displayPtr)]; nameitem.level != 0; )
		{
			if (nameitem.kind == kind && nameitem.id == name)
			{
				return nameitem;
			}
			else
			{
				if (nameitem.link != -1)//如果未达到本层起点, 沿着link链找即可
				{
					nameitem = nametable[nameitem.link];
					continue;
				}
				else//向上一层查找
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable[(*displayPtr)];
						continue;
					}
					else
					{
						nameitem = nametable[0];//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
						return nameitem;
					}
				}
			}
		}
	}
	return nametable[0];
}

vector<name_item>::iterator lookupPtr(vector<int> ::iterator &displayPtr, enum KIND kind, string name)
//按照kind和过程名查找过程, 返回一个name_item指针
{
	vector<name_item>::iterator nameitem;
	if (kind == var)
	{
		for (nameitem = nametable.begin()+(*displayPtr); nameitem != nametable.begin(); )
		{
			if (((*nameitem).kind == kind || ((*nameitem).kind == param)) && (*nameitem).id == name)
			{
				return nameitem;
			}
			else
			{
				if ((*nameitem).link != -1)//如果未达到本层起点, 沿着link链找即可
				{
					nameitem = nametable.begin()+(*nameitem).link;
					continue;
				}
				else//向上一层查找
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable.begin()+(*displayPtr);
						continue;
					}
					else
					{
						nameitem = nametable.begin();//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
						return nameitem;
					}
				}
			}
		}

	}
	else
	{
		for (nameitem = nametable.begin()+(*displayPtr); (*nameitem).level != 0; )
		{
			if ((*nameitem).kind == kind && (*nameitem).id == name)
			{
				return nameitem;
			}
			else
			{
				if ((*nameitem).link != -1)//如果未达到本层起点, 沿着link链找即可
				{
					nameitem = nametable.begin()+(*nameitem).link;
					continue;
				}
				else//向上一层查找
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable.begin()+(*displayPtr);
						continue;
					}
					else
					{
						nameitem = nametable.begin();//认为找到了主过程, 在此未设置"源代码中未写主过程名"的低级错误的检查
						return nameitem;
					}
				}
			}
		}
	}
	return nametable.begin();
}