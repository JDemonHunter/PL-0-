#pragma once
#include "grammer.h"
#include "translate.h"
//此头文件作用:
//编译完全正确后, 通过目标代码操纵堆栈, 实现程序执行
int sp;//活动记录全局指针
int top;//活动记录全局指针
int nextad;//下一条要执行的目标代码地址

void initialize()//进入主过程时对内存初始化
{
	sp = 0;
	nextad = 0;
	mem.push_back(0);//主过程动态链初始化为0
	mem.push_back(0);//主过程返回地址初始化为0
	mem.push_back(0);//主过程静态链初始化为0
	top = 2;
}

void exe(emit_item code)
{
	if (code.F == LIT)
	{
		mem.push_back(code.A);
		top++;
		nextad++;
		return;
	}
	else if (code.F == JMP)
	{
		nextad = code.A;
		return;
	}
	else if (code.F == JPC)
	{
		if (mem[top] == 0)
			nextad = code.A;
		else
			nextad++;
		return;
	}
	else if (code.F == OPR)
	{
		int temp = 0;
		if (code.A == JIA)
		{
			temp = mem[top - 1] + mem[top];
		}
		else if (code.A == JIAN)
		{
			temp = mem[top - 1] - mem[top];
		}
		else if (code.A == CHU)
		{
			temp = mem[top - 1] / mem[top];
		}
		else if (code.A == CHENG)
		{
			temp = mem[top - 1] * mem[top];
		}
		else if (code.A == SHUCHU)
		{
			cout << mem[top] << endl;
			nextad++;
			return;
		}
		else if (code.A == FANHUI)
		{
			if (sp == 0)
				exit(0);
			nextad = mem[sp + 1];
			while (top != sp)
			{
				mem.pop_back();
				top--;
			}
			sp = mem[sp];
			return;
		}
		else if (code.A == DURU)
		{
			int temp = 0;
			cin >> temp;
			mem.push_back(temp);
			nextad++;
			top++;
			return;
		}
		else if (code.A == DAYU)
		{
			if (mem[top - 1] > mem[top])
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(1);
			}
			else
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(0);
			}
			top--;
			nextad++;
			return;
		}
		else if (code.A == XIAOYU)
		{
			if (mem[top - 1] < mem[top])
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(1);
			}
			else
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(0);
			}
			top--;
			nextad++;
			return;
		}
		else if (code.A == XIAOYUDENGYU)
		{
			if (mem[top - 1] <= mem[top])
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(1);
			}
			else
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(0);
			}
			top--;
			nextad++;
			return;
		}
		else if (code.A == DAYUDENGYU)
		{
			if (mem[top - 1] >= mem[top])
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(1);
			}
			else
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(0);
			}
			top--;
			nextad++;
			return;
		}
		else if (code.A == XIANGDENG)
		{
			if (mem[top - 1] == mem[top])
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(1);
			}
			else
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(0);
			}
			top--;
			nextad++;
			return;
		}
		else if (code.A == BUDENG)
		{
			if (mem[top - 1] != mem[top])
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(1);
			}
			else
			{
				mem.pop_back();
				mem.pop_back();
				mem.push_back(0);
			}
			top--;
			nextad++;
			return;
		}
		mem.pop_back();
		mem.pop_back();
		mem.push_back(temp);
		top--;
		nextad++;
		return;
	}
	else if (code.F == LOD)
	{
		int tempsp = sp;
		for (int i = 0; i < code.L; i++)
		{
			tempsp = mem[tempsp];
		}
		mem.push_back(mem[tempsp + code.A]);
		top++;
		nextad++;
		return;
	}
	else if (code.F == STO)
	{
		int tempsp = sp;
		for (int i = 0; i < code.L; i++)
		{
			tempsp = mem[tempsp];
		}
		if ((tempsp + code.A) >= mem.size())
		{
			mem.push_back(mem.back());
			top++;
		}
		else
			mem[tempsp + code.A] = mem.back();
		nextad++;
		return;
	}
	else if (code.F == CAL)
	{
		int tempsp = sp;
		int nextsp = top + 1;
		mem.push_back(mem[sp]);//保存老sp: 动态链
		mem.push_back(code.parammiter[0]);//保存返回地址
		if (code.L == -1)
		{
			mem.push_back(mem[sp]);//如果是N调N+1层, 深层过程的静态链=浅层过程的活动起始地址
		}
		else if (code.L == 0)
		{
			mem.push_back(mem[sp + 2]);//N1调N2层(同层调用), 被调用过程的静态链=调用过程的静态链
		}
		else if (code.L > 0)
			//N层调N-x层, 新的静态链=从当前开始沿静态链向前走x步所到达的活动记录的静态链
		{
			for (int i = 0; i < code.L; i++)
			{
				tempsp = mem[tempsp + 2];
			}
			mem.push_back(mem[tempsp] + 2);
		}
		if (code.flag != 100)
		{
			for (vector <int> ::iterator it = code.parammiter.begin() + 1; it != code.parammiter.end(); it++)
			{
				mem.push_back((*it));
			}
		}
		else
		{
			int i = 0;
			for (vector <int> ::iterator it = code.pairof.begin(); it != code.pairof.end(); it++)
			{
				int tempsp = sp;
				for (i=(*it); i > 0 ; i--)
				{
					tempsp = mem[tempsp];
				}
				mem.push_back(mem[tempsp + (*(it+1))]);
				top++;
				it++;
			}
		}
		top = mem.size() - 1;
		sp = nextsp;
		nextad = code.A;
		return;
	}
}

void MAIN_EXECUTE(vector <emit_item> codeVec)
{
	while (1)
	{
		exe(codeVec[nextad]);
	}
}