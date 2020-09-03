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
ifstream file;  //���ļ��� ��ȡ"out.txt"�м��ļ�������
ofstream resultfile;//д���﷨�������"result.txt"���ļ���ָ��
enum KIND kind;
//ȫ�ֱ���: ÿһ��ID������
int LEV;
//ȫ�ֱ���: ����ȷ����ǰ�����������е�Ƕ�ײ��, ������CALָ��Ĳ��
vector<int> mem;
//�ڴ�ջ+�������㵥Ԫ: ��P�������, ������дP����Ľ���ָ��ʱ�Ż�ʹ��
int adrvalNum = 0;
//���ڴ洢��ʱ����ռ�еĳ�����������, ���ܻ�����������ֱ��д��ȫ�ֱ���
int nowlevel = 0;
//���ڴ洢��ʱ�����ľ��Բ��, ���ܻ�����������ֱ��д��ȫ�ֱ���
int leveldiff = 0;
//���ڴ洢���ù����뱻���ù���֮��Ĳ�β�, ���ܻ�������ֱ��д��ȫ�ֱ���, �ڱ��뵽callʱ�ı�
int retadr = 0;
//���ڴ洢�����ù��̷��ص��ù���ʱ������ַ������, ���ܻ�������ֱ��д��ȫ�ֱ���, �ڱ��뵽callʱ�ı�
bool fromproc = 0;
//�˲����������ڱ���Ǵ�proc->block->body�����, ���Ǵ�statement�н����
//����Ǵ�statement�н����, ������������endʱ����ҪOPRָ��
/********   P�������   *************/
/*
LIT		0 ��a ȡ����a��������ջջ��
OPR		0 ��a ִ�����㣬a��ʾִ��ĳ������
LOD	L ��a ȡ��������Ե�ַΪa�����ΪL���ŵ�����ջ��ջ��
STO		L ��a ������ջջ�������ݴ����������Ե�ַΪa����β�ΪL��
CAL		L ��a ���ù��̣�ת��ָ�����ڵ�ַΪa����β�ΪL��
INT		0 ��a ����ջջ��ָ������a
JMP		0 ��a������ת�Ƶ���ַΪa��ָ��
JPC		0 ��a ����ת��ָ�ת�Ƶ���ַΪa��ָ��
RED	L ��a �����ݲ������������Ե�ַΪa����β�ΪL��
WRT	0 ��0 ��ջ���������
*/
/*
CAL�����ù��̣���ʱA��Ϊ�����ù��̵Ĺ����壨������֮ǰ��ĳһ��ָ���Ŀ�����������ڵ�ַ��
LIT���������͵�����ջS��ջ������ʱA��Ϊ����ֵ��
LOD���������͵�����ջS��ջ������ʱA��Ϊ��������˵�����е����λ�á�
STO��������ջS��ջ����������ĳ��������Ԫ�У�A��Ϊ��������˵�����е����λ�á�
JMP��������ת�ƣ���ʱA��Ϊת���ַ��Ŀ����򣩡�
JPC������ת�ƣ�������ջS��ջ���Ĳ���ֵΪ�٣�0��ʱ����ת��A����ָĿ������ַ������˳��ִ�С�
OPR����ϵ����������:
			OPR 0 2	��ջ����ջ����ӣ�������ջԪ�أ����ֵ��ջ
			OPR 0 3	��ջ����ȥջ����������ջԪ�أ����ֵ��ջ
			OPR 0 4	��ջ������ջ����������ջԪ�أ����ֵ��ջ
			OPR 0 5	��ջ������ջ����������ջԪ�أ����ֵ��ջ.
			OPR 0 6	ջ��Ԫ�ص���ż�жϣ����ֵ��ջ��
			OPR 0 8	��ջ����ջ���Ƿ���ȣ�������ջԪ�أ����ֵ��ջ
			OPR 0 9	��ջ����ջ���Ƿ񲻵ȣ�������ջԪ�أ����ֵ��ջ
			OPR 0 10	��ջ���Ƿ�С��ջ����������ջԪ�أ����ֵ��ջ
			OPR 0 11	��ջ���Ƿ���ڵ���ջ����������ջԪ�أ����ֵ��ջ
			OPR 0 12	��ջ���Ƿ����ջ����������ջԪ�أ����ֵ��ջ
			OPR 0 13	��ջ���Ƿ�С�ڵ���ջ����������ջԪ�أ����ֵ��ջ
			OPR 0 14	ջ��ֵ�������Ļ
			OPR 0 15	��Ļ�������
			OPR 0 16	�������ж���һ����������ջ��
*/
/*******************************/
//����emit�����Ӧ��ö�����ͱ�������:
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
enum F_seg { LIT, OPR, LOD, STO, CAL, MLC, JMP, JPC, RED, WRT };//������MLC�����INT,��ΪINT��C++�������ͱ�����
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
//ÿһ��ָ��Ӧ����3��: F L A
emit_item emititem;//ֱ�Ӷ����ȫ�ֱ���
vector <emit_item> code;//��������
vector <emit_item> ::iterator EmitPtr=code.begin();
void EMIT(F_seg Fcode, int L, int A)
{
	emititem.F = Fcode;
	emititem.L = L;
	emititem.A = A;
	code.push_back(emititem);
}

void EMIT(F_seg Fcode, int L, int A,  vector<int> param)//param�����е�[0]�Ƿ��ص�ַ, ��[1]��ʼ���βε�ֵ
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
//�����ڶ����Ȩ��֮��
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
//��file�ļ��ж�һ��ǰ׺dst��������,��������
//����ֱ�Ӽ��һ��ǰ׺
//������˻���Ҫ����
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
//��file�ļ����ȶ�key���ٶ�value,������
//������˻���Ҫ����
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
//��һ���к�, ����
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
//��file�ļ��ж�һ��key,����value,����
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
			continue;//�˴����޸Ŀ�������޴�ĸ�����
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
//Ԥ��һ��KeyWord������KeyWord������
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


//��Ҫ��дexpΪһ����, ӵ��string���͵�place����, ��check( )����������������һЩ�����Ƿ���һ��exp
class EXP
{
public:
	string EPLACE;
	bool isCon;
	int levdif;
	int offset;
	int value;//��isConΪ1ʱ��Ч, Ϊ�ó�����ֵ
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
	if (it >= status.begin() && *it == E)//������һ����E
	{
		if (it > status.begin())
		{
			if (it - 1 >= status.begin() && *(it - 1) == UMINUS)//��������ڶ����Ǹ���
			{
				//�ȼ���������E�Ƿ��ڷ��ű��пɼ���Χ��
				int rightValue = 0;
				//�ȼ������2��E�Ƿ��ڷ��ű��пɼ���Χ��
				vector<int> ::iterator displayPtr = display.end() - 1;
				vector<EXP>::iterator REit = expvector.end() - 1;
				if ((*REit).isCon == 1)//����Ǹ�����, ֱ��������
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
						else//�Ǹ�const
						{
							EMIT(LIT, 0, tempER.adrval);
							rightValue = tempER.adrval;
						}
					}
					else//�Ǹ�var
					{
						EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
						rightValue = tempER.adrval;
					}
				}
				EMIT(OPR, 0, QUFAN);
				EXP e;
				e.setEplace(newtemp());//�˴�������Ҫ������ű�, ��ʱû���ü�����
				e.isCon = 1;
				e.value = -rightValue;
				//��������, ȷʵ��Ҫ������ű�, ����Ƿ�����
				name_item tempitem;
				//	//ȷ����var�Ժ�,ά�����ű�
				kind = var;//����������������ʽ�洢Ϊvar
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
				////��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
				////����Ϊ���ڻ��¼���������ڸò��е����λ��
				vector<name_item> ::iterator nearestProc13 = lookupProcedure(nametable);
				int Proc_have13 = (*nearestProc13).adrval;
				tempitem.adrval = - rightValue;
				tempitem.entrance = Proc_have13;
				(*nearestProc13).adrval++;//�ù�������ӵ�еı�����+1
				///***************************************************/
				/*һ��׼������, ������ű�, ����display��*/
				nametable.push_back(tempitem);
				display.back() = nametable.size() - 1;
				///*һ��׼������, ������ű�, ����display��*/
				EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//����д�뵱ǰ�������µı���
				expvector.pop_back();//ɾ��-E��E
				expvector.push_back(e);//���뱻�����E
				status.pop_back();//ɾ��-E��"E"
				status.pop_back();//ɾ��-E��"-"
				status.push_back(PLUS);
				status.push_back(E);//������"E"
				arrange(expvector, status);//������һ����
			}
			else if (it - 1 >= status.begin() && *(it - 1) == MULTI)//���һ����E�ҵ����ڶ�����*
			{
				if (status.size() > 2 && *(it - 2) == E)//���ҵ�����������E
				{
					int leftValue, rightValue = 0;
					//�ȼ������2��E�Ƿ��ڷ��ű��пɼ���Χ��
					vector<int> ::iterator displayPtr = display.end() - 1;
					vector<EXP>::iterator LEit = expvector.end() - 2;
					vector<EXP>::iterator REit = expvector.end() - 1;
					if ((*LEit).isCon == 1)//����Ǹ�����, ֱ��������
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
							else//�Ǹ�const
							{
								EMIT(LIT, 0, tempEL.adrval);
								leftValue = tempEL.adrval;
							}
						}
						else//�Ǹ�var
						{
							EMIT(LOD, display.size() - 1 - tempEL.level, tempEL.entrance + 3);
							leftValue = tempEL.adrval;
						}
					}

					if ((*REit).isCon == 1)//����Ǹ�����, ֱ��������
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
							else//�Ǹ�const
							{
								EMIT(LIT, 0, tempER.adrval);
								rightValue = tempER.adrval;
							}
						}
						else//�Ǹ�var
						{
							EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
							rightValue = tempER.adrval;
						}
					}
					EMIT(OPR, 0, CHENG);
					EXP e;
					e.setEplace(newtemp());//�˴�������Ҫ������ű�, ��ʱû���ü�����
					e.isCon = 1;
					e.value = leftValue * rightValue;
					name_item tempitem;
					//	//ȷ����var�Ժ�,ά�����ű�
					kind = var;//����������������ʽ�洢Ϊvar
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
					////��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
					////����Ϊ���ڻ��¼���������ڸò��е����λ��
					vector<name_item> ::iterator nearestProc13 = lookupProcedure(nametable);
					int Proc_have13 = (*nearestProc13).adrval;
					tempitem.adrval = leftValue * rightValue;
					tempitem.entrance = Proc_have13;
					(*nearestProc13).adrval++;//�ù�������ӵ�еı�����+1
					///***************************************************/
					/*һ��׼������, ������ű�, ����display��*/
					nametable.push_back(tempitem);
					display.back() = nametable.size() - 1;
					///*һ��׼������, ������ű�, ����display��*/
					EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//����д�뵱ǰ�������µı���
					expvector.pop_back();//ɾ��E1*E2��E2
					expvector.pop_back();//ɾ��E1*E2��E1
					expvector.push_back(e);//���뱻�����E
					status.pop_back();//ɾ��E1*E2��"E"
					status.pop_back();//ɾ��E1*E2��"*"
					status.pop_back();//ɾ��E1*E2��"E"
					status.push_back(E);//������"E"
					arrange(expvector, status);//������һ����
				}
			}
			else if (it - 1 >= status.begin() && *(it - 1) == PLUS)//���һ����E�ҵ����ڶ�����+
			{
				if (status.size() > 2 && *(it - 2) == E)//���ҵ�����������E
				{
					int leftValue, rightValue = 0;
					//�ȼ������2��E�Ƿ��ڷ��ű��пɼ���Χ��
					vector<int> ::iterator displayPtr = display.end() - 1;
					vector<EXP>::iterator LEit = expvector.end() - 2;
					vector<EXP>::iterator REit = expvector.end() - 1;
					if ((*LEit).isCon == 1)//����Ǹ�����, ֱ��������
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
							else//�Ǹ�const
							{
								EMIT(LIT, 0, tempEL.adrval);
								leftValue = tempEL.adrval;
							}
						}
						else//�Ǹ�var
						{
							EMIT(LOD, display.size() - 1 - tempEL.level, tempEL.entrance + 3);
							leftValue = tempEL.adrval;
						}
					}

					if ((*REit).isCon == 1)//����Ǹ�����, ֱ��������
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
							else//�Ǹ�const
							{
								EMIT(LIT, 0, tempER.adrval);
								rightValue = tempER.adrval;
							}
						}
						else//�Ǹ�var
						{
							EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
							rightValue = tempER.adrval;
						}
					}
					EMIT(OPR, 0, JIA);
					EXP e;
					e.setEplace(newtemp());//�˴�������Ҫ������ű�, ��ʱû���ü�����
					e.isCon = 1;
					e.value = leftValue + rightValue;
					name_item tempitem;
					//	//ȷ����var�Ժ�,ά�����ű�
					kind = var;//����������������ʽ�洢Ϊvar
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
					////��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
					////����Ϊ���ڻ��¼���������ڸò��е����λ��
					vector<name_item> ::iterator nearestProc10 = lookupProcedure(nametable);
					int Proc_have10 = (*nearestProc10).adrval;
					tempitem.adrval = leftValue + rightValue;
					tempitem.entrance = Proc_have10;
					(*nearestProc10).adrval++;//�ù�������ӵ�еı�����+1
					///***************************************************/
					/*һ��׼������, ������ű�, ����display��*/
					nametable.push_back(tempitem);
					display.back() = nametable.size() - 1;
					///*һ��׼������, ������ű�, ����display��*/
					EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//����д�뵱ǰ�������µı���
					expvector.pop_back();//ɾ��E1*E2��E2
					expvector.pop_back();//ɾ��E1*E2��E1
					expvector.push_back(e);//���뱻�����E
					status.pop_back();//ɾ��E1*E2��"E"
					status.pop_back();//ɾ��E1*E2��"+"
					status.pop_back();//ɾ��E1*E2��"E"
					status.push_back(E);//������"E"
					arrange(expvector, status);//������һ����
				}
				else if (it - 1 == status.begin() && *(it - 1) == PLUS)//������һ����E, �ҵ�һ����+, ����������, ��ֵ���黯����
				{
					EXP e;
					setEplace(expvector.back().EPLACE);
					status.pop_back();//ɾ��+E��"E"
					status.pop_back();//ɾ��+E��"+"
					if (status.empty())
						return e.EPLACE;
				}
			}
			else if (it - 1 >= status.begin() && *(it - 1) == DIVI)//���һ����E�ҵ����ڶ�����/
			{
				if (it - 2 >= status.begin() && *(it - 2) == E)//���ҵ�����������E
				{
					//�ȼ������2��E�Ƿ��ڷ��ű��пɼ���Χ��
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
						else//�Ǹ�const
						{
							EMIT(LIT, 0, tempEL.adrval);
						}
					}
					else//�Ǹ�var
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
						else//�Ǹ�const
						{
							EMIT(LIT, 0, tempER.adrval);
						}
					}
					else//�Ǹ�var
					{
						EMIT(LOD, display.size() - 1 - tempER.level, tempER.entrance + 3);
					}
					EMIT(OPR, 0, CHU);
					EXP e;
					e.setEplace(newtemp());//�˴�������Ҫ������ű�, ��ʱû���ü�����
					name_item tempitem;
					//	//ȷ����var�Ժ�,ά�����ű�
					kind = var;//����������������ʽ�洢Ϊvar
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
					////��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
					////����Ϊ���ڻ��¼���������ڸò��е����λ��
					vector<name_item> ::iterator nearestProc10 = lookupProcedure(nametable);
					int Proc_have10 = (*nearestProc10).adrval;
					tempitem.adrval = 0;
					tempitem.entrance = Proc_have10;
					(*nearestProc10).adrval++;//�ù�������ӵ�еı�����+1
					///***************************************************/
					///*һ��׼������, ������ű�, ����display��*/
					nametable.push_back(tempitem);
					display.back() = nametable.size() - 1;
					///*һ��׼������, ������ű�, ����display��*/
					EMIT(STO, display.size() - 1 - tempitem.level, tempitem.entrance + 3);//����д�뵱ǰ�������µı���
					expvector.pop_back();//ɾ��E1*E2��E2
					expvector.pop_back();//ɾ��E1*E2��E1
					expvector.push_back(e);//���뱻�����E
					status.pop_back();//ɾ��E1*E2��"E"
					status.pop_back();//ɾ��E1*E2��"/"
					status.pop_back();//ɾ��E1*E2��"E"
					status.push_back(E);//������"E"
					arrange(expvector, status);//������һ����
				}
			}
		}
	}
	else if (it >= status.begin() && *it == RightPAR)//������һ����)
	{
		if (it - 1 >= status.begin() && *(it - 1) == E)//��������ڶ�����E
		{
			if (it - 2 >= status.begin() && *(it - 2) == LeftPAR)//��������ڶ�����(
			{
				setEplace(expvector.back().EPLACE);
				status.pop_back();//ɾ��(E)��")"
				status.pop_back();//ɾ��(E)��"E"
				status.pop_back();//ɾ��(E)��"("
				status.push_back(E);//������E;
				arrange(expvector, status);//������һ��ļ��
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

	name_item nameitem;//��׼��һ��nameitem׼��ѹ��
	//�������nametable��, ͬʱά��btable;
	//Ӧ����ʱ�Ƿ�Ϊ��display�������, ������Ӧ��������һ��, ����ֱ��ѹ��
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

	//����const��ֵû�г�ʼ��

	flag1 = readWord(file, "ID");
	flag2 = readWord(file, "assign");

	string constVal_temp = readWord(file);
	if (constVal_temp == "-")
	{
		flag3 = readWord(file, "aop");
		constVal_temp = readWord(file);
		constVal_temp.insert(0, "-");
	}
	//ά�������ֵ
	char constVal[15] = { 0 };
	strcpy(constVal, constVal_temp.c_str());//��stringתchar*
	int constVAL = atoi(constVal);//��char*תint
	nameitem.adrval = constVAL;//��int�������һ���ֵ
	nameitem.num = nametable.size();
	/***************************************************/
	//��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
	//����Ϊ���ڻ��¼���������ڸò��е����λ��
	vector<name_item> ::iterator nearestProc0 = lookupProcedure(nametable);
	int Proc_have0 = (*nearestProc0).adrval;
	nameitem.entrance = Proc_have0;
	(*nearestProc0).adrval++;//�ù�������ӵ�еı�����+1
	/***************************************************/
	/*һ��׼������, ������ű�, ����display��*/
	nametable.push_back(nameitem);
	display.back() = nametable.size() - 1;
	/*һ��׼������, ������ű�, ����display��*/
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
	//ȷ����var�Ժ�,ά�����ű�
	name_item nameitem;
	kind = var;//����������������ʽ�洢Ϊvar
	string IDname = readWord(file);//�õ�������
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
	//��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
	//����Ϊ���ڻ��¼���������ڸò��е����λ��
	vector<name_item> ::iterator nearestProc6 = lookupProcedure(nametable);
	int Proc_have6 = (*nearestProc6).adrval;
	nameitem.adrval = 0;
	nameitem.entrance = Proc_have6;
	(*nearestProc6).adrval++;//�ù�������ӵ�еı�����+1
	/***************************************************/
	nametable.push_back(nameitem);
	display.back() = nametable.size() - 1;
	/*һ��׼������, ������ű�, ����display��*/
	flag = readWord(file, "ID");
	if (!flag)
	{
		resultfile << "error: fail to read \"ID\"" << endl;
	}
	option = readWord(file);
	while (option == "PAR0")
	{
		flag = readWord(file, "PAR0", ",");
		//ȷ����var�Ժ�,ά�����ű�
		name_item nameitem;
		kind = var;//����������������ʽ�洢Ϊvar
		string IDname = readWord(file);//�õ�������
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
		//��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
		//����Ϊ���ڻ��¼���������ڸò��е����λ��
		vector<name_item> ::iterator nearestProc2 = lookupProcedure(nametable);
		int Proc_have2 = (*nearestProc2).adrval;
		nameitem.adrval = 0;
		nameitem.entrance = Proc_have2;
		(*nearestProc2).adrval++;//�ù�������ӵ�еı�����+1
		/***************************************************/
		/*һ��׼������, ������ű�, ����display��*/
		nametable.push_back(nameitem);
		display.back() = nametable.size() - 1;
		/*һ��׼������, ������ű�, ����display��*/
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
	//����block, ������һ���µ�display��
	//var
	//const
	//procdure id ����ͬ��
	bool flag = 0;
	string option = readWord(file);
	if (option == "const")
	{
		kind = con;//����������id��������Ϊconst
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
	//�����������׷����������JMP���(��δ�������ַ��  ����Ϊ������Ƕ�׶���Ĺ���), ����Ż���
	for (EmitPtr = code.end() - 1; EmitPtr >= code.begin(); EmitPtr--)
	{
		if ((*EmitPtr).F == JMP && (*EmitPtr).A == 0)
		{
			(*EmitPtr).A = code.size();
			/****************************************************/
			break;
		}
	}

	//����begin��, �ٽ��ù������µǼǵ�var��const(������param)���ڴ�ջ
	vector<name_item> ::iterator iter = lookupProcedure(nametable);
	int N = (*iter).adrval;
	iter++;
	for (int i = 0; i < N; i++,iter++)
	{
		if ((*iter).kind == param)
			continue;
		else
			EMIT(LIT, 0, (*iter).adrval);//var��const��ֵ����ջ
	}

	//�Ե�ǰdisplay������һ���ڲ��¼������, ��ǰ��procedure��
	vector<int> ::iterator displayPtr = display.end()-2;//�����õ�: Ӧ�ڵ�ǰ���̵�ǰһ�����ҹ�����
	name_item nameitem=lookupProc(displayPtr, procedure);
	adrvalNum = nameitem.adrval;
	nowlevel = nameitem.level;
	//���¹���Ӧ���ڿ���callʱչ��
	////��̬��:
	//Fcode = LOD;
	//EMIT(Fcode, 0, 0);//��ǰ��ƫ����Ϊ[0]��ֵ��ջ��
	////���ص�ַ:
	//Fcode = LIT;
	//EMIT(Fcode, 0, retadr);//��Ϊ��ֵ������callʱһ����õ�, ���ĳ����û�м�ʹ������

	//Fcode = MLC;
	//EMIT(Fcode, 0, adrvalNum+5);//MLCָ���´�ʱ, Ӧά��SP��TOPָ��
	////adrvalNum�Ǹû��¼��Ҫ�õ��ı����ͳ�������Ŀ
	////5: ��̬��+���ص�ַ+��̬��+������Ŀ+��ǰ���¼���ڵľ��Բ����
	///*********�������¼*****************/
	////�����Ѿ�����һ��EMITָ��Ϊջ������adrvalNum+5���ռ�
	////���붯̬��
	////if (mem.empty())//������ոս���ʱ�ڴ�Ϊ��, ���ȶ��ڴ��ʼ��
	////{
	////	//��̬����ʼ��Ϊ0
	////	Fcode = LIT;//������ջָ��
	////	EMIT(Fcode, 0, 0);
	////	//���ص�ַ��ʼ��Ϊ0
	////	EMIT(Fcode, 0, 0);
	////	//��̬����ʼ��Ϊ0
	////	EMIT(Fcode, 0, 0);
	////}
	//	Fcode = LOD;
	//	EMIT(Fcode, 1, 0);//��̬��: ��[1]��ƫ����Ϊ[0]��ֵ: ����һ����Ļ���ַ
	//	Fcode = LIT;
	//	EMIT(Fcode, 0, retadr);//���ص�ַ: ��Ϊ��������callʱһ����õ�
	//	Fcode = LOD;
	//	//��̬��: 
	//	if (leveldiff == -1)
	//		//1. N�����N+1��:
	//	{
	//		EMIT(Fcode, 1, 0);//�������̵ľ�̬��=���ù��̵���ʼ��ַ
	//	}
	//	else if (leveldiff == 0)
	//		//2.N�����N��:
	//	{
	//		EMIT(Fcode, 1, 2);//�������̵ľ�̬��=���ù��̵ľ�̬��
	//	}
	//	else
	//		//3.N�����N-n��, leveldiff=n
	//	{
	//		////��Ҫ���ŵ�ǰ���¼(N��)�ľ�̬��������n��
	//		//int laststatic = mem[sp+2];
	//		//int lastsp = mem[sp];//�Ե�ǰ��̬��Ϊ���
	//		//for (int i = leveldiff; i > 0; i--)
	//		//{
	//		//	lastsp=mem[la]
	//		//	laststatic = mem[lastsp+2];
	//		//}
	//	}
	//	sp= top + 1;
	//	top = sp + adrvalNum+3;
	////��������ÿ�ν���procedure��Ҫ���ɵĴ���Ͳ���: adrvalNumֵ��ջ+��ǰ���̵ľ��Բ�����ջ+�������ͳ�����ջ
	////adrvalNumֵ��ջ
	//Fcode = LIT;
	//EMIT(Fcode, 0, adrvalNum);
	////����ǰ���̵ľ��Բ�����ջ
	//Fcode = LIT;
	//EMIT(Fcode, 0, nowlevel);
	////��adrvalNum�������ͳ���, �ǳ�����ֱ��дֵ, �Ǳ�����ʹ��LODָ���ӷ��ʱ�����ֵ����ջ��
	//vector<int> ::iterator displayPtr_t = display.end() - 1;//�����õ�: Ӧ�ڵ�ǰ���̵Ĳ�����ҹ�����
	//int tempadrvalNum = adrvalNum;//��Ҫ�ӷ��ű����ҳ�tempadrvalNum������������ջ��
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
	//		if (nameitem.link != -1)//���δ�ﵽ�������, ����link���Ҽ���
	//		{
	//			nameitem = nametable[nameitem.link];
	//			continue;
	//		}
	//		else//����һ�����
	//		{
	//			if (displayPtr != display.begin())
	//			{
	//				displayPtr--;
	//				nameitem = nametable[(*displayPtr)];
	//				continue;
	//			}
	//			else
	//			{
	//				nameitem = nametable[0];//��Ϊ�ҵ���������, �ڴ�δ����"Դ������δд��������"�ĵͼ�����ļ��
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
		vector<int> ::iterator displayPtr = display.end() - 1;//�����õ�: Ӧ�ڵ�ǰ���̵�ǰһ�����ұ�����
		name_item nameitem = lookupProc(displayPtr, var, idVAL);
		//�������δ����, ����������䵫������
		if (nameitem.level == 0)
		{
			string num = readWordNum(file);
			resultfile << "undefined var : " << idVAL << "[" << num << "]" << endl;
			flag = readWord(file, "ID");
			flag = readWord(file, "assign");
			tmp.check(file, status, EXPvector);
			return flag;
		}
		//��������Ѷ���, ��Ҫ��¼���Ĳ���ƫ����
		int levdif = display.size() - 1 - nameitem.level;
		int offset = nameitem.entrance+3;
		flag = readWord(file, "ID");
		flag = readWord(file, "assign");
		tmp.check(file, status, EXPvector);
		//��ʱ��EXPvector[0]�е�ֵ���������
		string RightName = EXPvector[0].EPLACE;
		displayPtr = display.end() - 1;
		name_item itemtmp = lookupProc(displayPtr, var, RightName);
		if (itemtmp.num == 0)//���û�ҵ�, ����ű�!
		{
			name_item tempitem;
			//	//ȷ����var�Ժ�,ά�����ű�
			kind = var;//����������������ʽ�洢Ϊvar
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
			////��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
			////����Ϊ���ڻ��¼���������ڸò��е����λ��
			vector<name_item> ::iterator nearestProc11 = lookupProcedure(nametable);
			int Proc_have11 = (*nearestProc11).adrval;
			tempitem.adrval = EXPvector[0].value;
			tempitem.entrance = Proc_have11;
			(*nearestProc11).adrval++;//�ù�������ӵ�еı�����+1
			///***************************************************/
			/*һ��׼������, ������ű�, ����display��*/
			nametable.push_back(tempitem);
			display.back() = nametable.size() - 1;
			///*һ��׼������, ������ű�, ����display��*/

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
		src = readWord(file);//src=��׺
		if (src == "if")
		{
			flag = readWord(file, "KeyWord", "if");
			flag = lexp(file);
			flag = readWord(file, "KeyWord", "then");//
			EMIT(JPC, 0, 0);
			flag = statement(file);
			EMIT(JMP, 0, code.size() + 1);
			int nownum = code.size() + 1;
			//����Ҫ����֮ǰ����JPC��A��
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
			//����Ҫ����֮ǰ����JPC��A��
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
			////ȷ����call��, ����ȡ��Ŀ�����ָ��ص�ַ
			retadr = code.size() + 1;
			vector<int> paramlist;
			paramlist.clear();
			paramlist.push_back(retadr);//paramlist[0]�Ƿ��ص�ַ,��[1]��ʼ���βε�ֵ
			vector<int> pairof;
			flag = readWord(file, "KeyWord");
			//�õ�������
			string procname = readWord(file);

			vector<int> ::iterator displayPtr = display.end() - 1;//�����õ�: Ӧ�ڵ�ǰ�������ڲ�������
			name_item nameitem = lookupProc(displayPtr, procedure, procname);
			int layerdiff = LEV - nameitem.level;//���
			//Fcode = CAL;
			//EMIT(Fcode, 0, nameitem.entrance);
			////CALָ���ִ��: TOPָ������, ���涯̬��  ���ص�ַ  ��̬��   ����SPָ���TOPָ��
			//���ڻ�̫����!

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
			string idname = readWord(file);//����id

			vector<int> ::iterator displayPtr = display.end() - 1;
			name_item nameitem= lookupProc(displayPtr, var, idname);
			if (nameitem.num == 0)
			{
				resultfile << "undefined var: " << idname << endl;
			}
			displayPtr = display.end() - 1;
			vector<name_item>::iterator iter = lookupPtr(displayPtr, var, nameitem.id);
			int laydif = LEV - (nameitem.level-1);//���
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
		//�˴�Ӧ��дlookup����
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
			else//��һ������
			{
				e.isCon = 1;
				e.value = nameitem.adrval;
			}
		}
		else//��һ������
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
	kind = procedure;//����������������������Ϊ����
	string procName = readWord(file);
	nameitem.adrval = 0;//һ�������������Ĳ���������ʼ��Ϊ0
	nameitem.id = procName;
	nameitem.level = display.size() - 1;
	nameitem.kind = kind;

	//��¼�����ֵ���ڵ�ַ
	int entrance = code.size();
	nameitem.entrance = entrance;
	//����P����
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
	//����һ��'('ʱ, Ӧ����һ���µ�display��
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
		nameitem.entrance = tempentrance++;//��¼�ñ��������������е�ƫ����
		/***************************************************/
		//��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
		//����Ϊ���ڻ��¼���������ڸò��е����λ��
		vector<name_item> ::iterator nearestProc4 = lookupProcedure(nametable);
		int Proc_have4 = (*nearestProc4).adrval;
		nameitem.adrval = Proc_have4 + 1;
		(*nearestProc4).adrval+=1;//�ù�������ӵ�еı�����+1
		/***************************************************/
		/*һ��׼������, ������ű�, ����display��*/
		nametable.push_back(nameitem);
		display.back() = nametable.size() - 1;
		/*һ��׼������, ������ű�, ����display��*/
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
			//��ʱҪʹ��int lookupProcedure()������øı����������������Ѿ��Ǽ��˼�������, ����+1����adrvalֵ��
			//����Ϊ���ڻ��¼���������ڸò��е����λ��
			vector<name_item> ::iterator nearestProc5 = lookupProcedure(nametable);
			int Proc_have5 = (*nearestProc5).adrval;
			nameitem.adrval = Proc_have5 + 1;
			(*nearestProc5).adrval++;//�ù�������ӵ�еı�����+1
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
			nameitem.entrance = tempentrance++;//��¼�ñ��������������е�ƫ����
			/*һ��׼������, ������ű�, ����display��*/
			nametable.push_back(nameitem);
			display.back() = nametable.size() - 1;
			/*һ��׼������, ������ű�, ����display��*/
			flag = readWord(file, "ID");
			option = readKeyWord(file);
		}
	}
	flag = readWord(file, "RPAR", ")");
	flag = readWord(file, "PAR1", ";");
	flag = block(file);
	//�ڽ���block(statement)ʱ����������������
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
		//��������id��ʼ��ʼ����һ��
		string programID = readWord(file);
		nameitem.id = programID;
		nameitem.kind = kind;
		nameitem.adrval = 0;//**********************
		//�������ֱ��id������
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
		nameitem.entrance = 0;//���������ڵ�ַΪ0
		nametable.push_back(nameitem);
		display.back() = nametable.size() - 1;
	}
	//���ɵ�һ��P����:
	Fcode = JMP;
	EMIT(Fcode, 0, 0);

	flag = readWord(file, "ID");
	flag = readWord(file, "PAR1", ";");
	newdisplay();
	LEV=0;
	flag = block(file);
	return flag;
}