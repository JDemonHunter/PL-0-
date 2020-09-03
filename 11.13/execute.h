#pragma once
#include "grammer.h"
#include "translate.h"
//��ͷ�ļ�����:
//������ȫ��ȷ��, ͨ��Ŀ�������ݶ�ջ, ʵ�ֳ���ִ��
int sp;//���¼ȫ��ָ��
int top;//���¼ȫ��ָ��
int nextad;//��һ��Ҫִ�е�Ŀ������ַ

void initialize()//����������ʱ���ڴ��ʼ��
{
	sp = 0;
	nextad = 0;
	mem.push_back(0);//�����̶�̬����ʼ��Ϊ0
	mem.push_back(0);//�����̷��ص�ַ��ʼ��Ϊ0
	mem.push_back(0);//�����̾�̬����ʼ��Ϊ0
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
		mem.push_back(mem[sp]);//������sp: ��̬��
		mem.push_back(code.parammiter[0]);//���淵�ص�ַ
		if (code.L == -1)
		{
			mem.push_back(mem[sp]);//�����N��N+1��, �����̵ľ�̬��=ǳ����̵Ļ��ʼ��ַ
		}
		else if (code.L == 0)
		{
			mem.push_back(mem[sp + 2]);//N1��N2��(ͬ�����), �����ù��̵ľ�̬��=���ù��̵ľ�̬��
		}
		else if (code.L > 0)
			//N���N-x��, �µľ�̬��=�ӵ�ǰ��ʼ�ؾ�̬����ǰ��x��������Ļ��¼�ľ�̬��
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