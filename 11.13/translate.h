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
//��ʱ��������,�ñ��ֻ������, ÿ��������+1
//��������δʹ������"T0:=a+b"��ʽ����Ԫʽ��Ϊ�м����
//ʹ��P����ֱ������Ŀ�����, ���Ǵ˱����������ú�������, ���ڱ����;�����в���ʹ��
enum KIND
{
	con, var, procedure, param
};
//ÿ�����ֵ�����:������, ������, ������, �β�

//*********************************************************************
typedef struct name_item
{
	string id;
	enum KIND kind;
	unsigned int level;
	int adrval;//��kind==con, adrval=val ; ��kind==var, adrval=adr; ��kind==procedure, adrval=�ù��������ı�������
	int link;//ͬһ��������,linkΪ��һname��nametable�е�λ��; ÿһ���������һ��name��linkֵΪ0
	//Ϊ�˾�ȷ���ҵ�ͬ����ͬ���ID��nametable�е�λ��, Ӧ�ټ���һ������:num, ��¼��������nametable�е�λ��
	int num;
	int entrance;//��kind==procedure, �ñ���Ϊ������ڵ�Ŀ��������
						//��kind==var��con, �ñ���Ϊ�ñ��������������еı�����, ����ƫ����
};
//���ֱ���
//vector<name_item> nametable;
//���ֱ�ģ��, ����һ�����ֱ�nametb �� txָ��,��ָ��ָ�����ֱ���һ��λ��, Ӧ��ʼ��Ϊ0
//*********************************************************************


////*********************************************************************
//typedef struct block_item
//{
//	int lastpar;//ָ�򱾳����������һ����ʽ������������nametable�е�λ��
//	int last;//ָ�򱾳����������һ��������nametable�е�λ��
//};
//���������, ÿһ�����2��ָ��
//vector<block_item> btable;
//�������ģ��, ����һ��������� �� bxָ��, ��ָ��ָ����������һ��λ��, Ӧ��ʼ��Ϊ0
//*********************************************************************


//*********************************************************************
//stack<int> displaytable;
//display��ģ��, ����һ������������ջ, levelͨ��displaytable.size()ȡ��, �ڴ�ֻ��ע���е���ʽ����
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
//�µ��β�����������,ÿ�ε��ý����ص����ı�����,T0 T1,T2..., ��������Ϊsring, ����ɾ���ú���

vector<int> display;//�ϲ�display����btable��, ����btable���е�"���һ���β�"
									//ֱ����displayջ��Ԫ�ر�ʾ��ǰ��ʾ����õ����һ�����������ֱ��е�λ��
									//��displayջ��Ԫ����Ŀ��ʾ��ǰ�����
vector<name_item> nametable;


//ÿ������һ���µ�display���ô˺���:
void newdisplay()
{
	//��������ű�ʼ��ʼ��
//����display����
	display.push_back(-1);//������ͷ��ʼ��Ϊ-1
	//����nametable����
	name_item nameitem;
	nameitem.level = display.size()-1;//����������Ϊ��0��, �˺�ӵ�1�㿪ʼ
	nameitem.link = -1;//�²�ĵ�һ��link��ʼ��Ϊ-1
	nameitem.adrval = 0;//��ֵ �� ��ַ ��������� ��ʼ��Ϊ0, ������Ϊ�˱���, �������в���ȷ
	nametable.push_back(nameitem);

}
int lastlink(vector<name_item> nametable, int layer)
//�ú�������Ѱ�ҵ�ǰ�������һ�����ֵ�linkֵ
//�����ڽ������ű�ʱ����linkֵ
{
	vector<name_item> :: iterator it=nametable.end()-1;
	while (it != nametable.begin())
	{
		if ((*it).level == layer)//������ű���ĳ���ֵĲ��Ϊ��ǰ���, �����ַ����
			return (*it).num;
		else
			it--;
	}
	return -1;
}
vector<name_item> ::iterator  lookupProcedure(vector<name_item> &nametable)
//�ú��������ڶ���varʱ, �������ϲ�ѯ�����һ��procedure, ���ظ����ֵ�ָ��
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
			if ((*it).link != -1)//���δ�ﵽ�������, ����link���Ҽ���
			{
				it = nametable.begin()+(*it).link;
				continue;
			}
			else//����һ�����
			{
				if (displayPtr != display.begin())
				{
					displayPtr--;
					it = nametable.begin()+(*displayPtr);
					continue;
				}
				else
				{
					//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
					return nametable.begin();
				}
			}
		}
		return nametable.begin();
	}
	return nametable.begin();
}

name_item lookupProc(vector<int> ::iterator &displayPtr , enum KIND kind)//������kind���ҹ���, ����һ��name_item���ͱ���
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
			if (nameitem.link != -1)//���δ�ﵽ�������, ����link���Ҽ���
			{
				nameitem = nametable[nameitem.link];
				continue;
			}
			else//����һ�����
			{
				if (displayPtr != display.begin())
				{
					displayPtr--;
					nameitem = nametable[(*displayPtr)];
					continue;
				}
				else
				{
					nameitem = nametable[0];//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
					return nameitem;
				}
			}
		}
	}
	return nameitem;
}

name_item lookupProc(vector<int> ::iterator &displayPtr, enum KIND kind, string name)//����kind�͹��������ҹ���, ����һ��name_item���ͱ���
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
				if (nameitem.link != -1)//���δ�ﵽ�������, ����link���Ҽ���
				{
					nameitem = nametable[nameitem.link];
					continue;
				}
				else//����һ�����
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable[(*displayPtr)];
						continue;
					}
					else
					{
						nameitem = nametable[0];//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
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
				if (nameitem.link != -1)//���δ�ﵽ�������, ����link���Ҽ���
				{
					nameitem = nametable[nameitem.link];
					continue;
				}
				else//����һ�����
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable[(*displayPtr)];
						continue;
					}
					else
					{
						nameitem = nametable[0];//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
						return nameitem;
					}
				}
			}
		}
	}
	return nametable[0];
}

vector<name_item>::iterator lookupPtr(vector<int> ::iterator &displayPtr, enum KIND kind, string name)
//����kind�͹��������ҹ���, ����һ��name_itemָ��
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
				if ((*nameitem).link != -1)//���δ�ﵽ�������, ����link���Ҽ���
				{
					nameitem = nametable.begin()+(*nameitem).link;
					continue;
				}
				else//����һ�����
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable.begin()+(*displayPtr);
						continue;
					}
					else
					{
						nameitem = nametable.begin();//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
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
				if ((*nameitem).link != -1)//���δ�ﵽ�������, ����link���Ҽ���
				{
					nameitem = nametable.begin()+(*nameitem).link;
					continue;
				}
				else//����һ�����
				{
					if (displayPtr != display.begin())
					{
						displayPtr--;
						nameitem = nametable.begin()+(*displayPtr);
						continue;
					}
					else
					{
						nameitem = nametable.begin();//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
						return nameitem;
					}
				}
			}
		}
	}
	return nametable.begin();
}