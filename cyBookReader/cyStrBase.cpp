
#include "cyStrBase.h"
#include "StdAfx.h"
#include <sstream>
#include<stack>
using namespace std;

int ParseNum(const wchar_t* pstr,int length)
{
	int re=0;
	if( length<=0) return -1;
	wchar_t cnum[] = {L'零',L'一',L'二',L'三',L'四',L'五',L'六',L'七',L'八',L'九',L'十',L'百',L'千'};
	int cnumlen = sizeof(cnum)/sizeof(cnum[0]);
	if( length>6)return -1;

	if( *pstr >=L'0' && *pstr<=L'9')
	{
		 re = _wtoi(pstr);
		 return re;
	}
	
	int tn=0;
	int wn = 0;
	int ss=0;
	stack<int> stk;
	while( ss<length)
	{
		wchar_t pchar = *(pstr+ss);
		int i;
		for(i=0;i<cnumlen;i++)
		{
			if( pchar == cnum[i] )
			{
				if(i>0&&i<10)
				{
					stk.push(i);
				}
				else if(i>=10)
				{
					int pn=1;
					switch(i)
					{
					case 10:
						pn=10;
						break;
					case 11:
						pn=100;
						break;
					case 12:
						pn=1000;
						break;
					}
					if( stk.empty() )
					{
						stk.push(pn);
					}
					else
					{
						int pn1 = stk.top();
						stk.pop();
						stk.push(pn1*pn);
					}
				}
				break;
			}
		}
		if(i>=cnumlen)
		{
			return -1;
		}
		
		ss+=1;
		
	}
	int rev=0;
	while( !stk.empty())
	{
		rev+=stk.top();
		stk.pop();
	}

	return rev;
}


//解析数字
int ParseNum(wstring str)
{
	TRACE(str.c_str());
	int re=0;
	if( str[0]>=L'0' && str[0]<=L'9')
	{
		std::wstringstream ss;
		ss<<str;
		ss>>re;
		return re;
	}
	wstring cnum[] = {L"零",L"一",L"二",L"三",L"四",L"五",L"六",L"七",L"八",L"九",L"十",L"百",L"千"};
	int tn=0;
	int wn = 0;
	int ss=0;
	stack<int> stk;
	while( ss<str.length())
	{
		wstring sr = str.substr(ss,1);
		int i;
		for(i=0;i<13;i++)
		{
			if( sr == cnum[i] )
			{
				if( i>0 && i<10)
				{
					stk.push(i);
				}
				else if( i>=10 && i<13)
				{
					int pn=1;
					switch(i)
					{
					case 10:
						pn=10;
						break;
					case 11:
						pn=100;
						break;
					case 12:
						pn=1000;
						break;
					}
					if( stk.empty() )
					{
						stk.push(pn);
					}
					else
					{
						int pn1 = stk.top();
						stk.pop();
						stk.push(pn1*pn);
					}
				}

				break;
			}
		}
		if(i>=13)
		{
			TRACE("END1");
			return -1;
		}
		
		ss+=1;
	}
	int rev=0;
	while( !stk.empty())
	{
		rev+=stk.top();
		stk.pop();
	}
	TRACE("END2");
	return rev;
}

//去空格
wstring WTrim(wstring str)
{
	wchar_t sg = L' ';
	wchar_t sg1 = L'　';
	int i;
	for(i=0; i<str.length(); i++)
	{
		if( str[i]!=sg && str[i]!=sg1)
		{
			break;
		}
	}
	if(i>0)
	{
		str.erase(0,i);
	}

	for(i=str.length()-1;i>=0;i--)
	{
		if( str[i]!=sg && str[i]!=sg1)
		{
			break;
		}
	}
	if(i<str.length()-1)
	{
		str.erase(i);
	}
	return str;
}

string Trim(string str)
{
	string sg = " ";
	std::string re1 = str.erase(str.find_last_not_of(sg) + 1);
	std::string re2 = re1.erase(0, re1.find_first_not_of(sg));
	sg = "　";
	re1 = str.erase(str.find_last_not_of(sg) + 1);
	return re1.erase(0, re1.find_first_not_of(sg));
}

const wchar_t* wcsnchr(const wchar_t* pstr,int length, wchar_t ch)
{
	for(int i=0;i<length;i++)
	{
		if( *(pstr+i)==ch)
			return pstr+i;
	}
	return NULL;
}