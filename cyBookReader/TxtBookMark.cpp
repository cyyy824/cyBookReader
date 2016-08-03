#include "StdAfx.h"
#include "TxtBookMark.h"
#include "cyStrBase.h"
#include "string.h"
#include <sstream>
#include<stack>



CTxtBookMark::CTxtBookMark(void)
{
	m_txtbuffer=NULL;
	m_txtlen=0;
	m_preGetBlock=0;
	m_isSplit = false;
}


CTxtBookMark::~CTxtBookMark(void)
{
	if( m_txtbuffer!=NULL) delete []m_txtbuffer;
}
std::vector<std::wstring> CTxtBookMark::GetAllSectionName()
{
	std::vector<std::wstring> reList;
	for(std::vector<SectionStruct>::iterator it=m_sectionList.begin();it!=m_sectionList.end();++it)
	{
		reList.push_back(it->name);
	}
	return reList;
}

int IsTextUTF8(char* str,ULONGLONG length)
{
	int i;
	DWORD nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr;
	BOOL bAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8
	if( length>500) length=500;
	for(i=0;i<length;i++)
	{
		chr= *(str+i);
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii= FALSE;
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				else if(chr>=0xC0)
					nBytes=2;
				else
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	//if( nBytes > 0 ) //违返规则
	//{
	//	return FALSE;
	//}
	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return FALSE;
	}
	return TRUE;
}

bool CTxtBookMark::ParseFromFile(std::wstring fileName)
{
	char *pcBuffer;
	ULONGLONG len;
	//file read
	CFile s;

	if(m_txtbuffer!=NULL)delete []m_txtbuffer;
	



	bool is = s.Open(fileName.c_str(), CFile::modeRead);
	if(!is) return false;
	len = s.GetLength();
	pcBuffer = new char[len+1];
	s.Read(pcBuffer,len);
	s.Close();
	//end read

	if( IsTextUTF8(pcBuffer,len) )
	{
		len = MultiByteToWideChar (CP_UTF8, 0, pcBuffer, -1, NULL, 0);
		m_txtbuffer = new wchar_t[len];
		MultiByteToWideChar (CP_UTF8, 0, pcBuffer, -1, m_txtbuffer, len);// 开始转换
		delete []pcBuffer;
	}
	else if(len>3 && *(pcBuffer+0) == -1 && *(pcBuffer+1) == -2)
	{//UNICODE
		len = len/2;
		m_txtbuffer = (wchar_t*)pcBuffer;
	}
	else if(len>3 && *(pcBuffer+0) == -2 && *(pcBuffer+1) == -1)
	{//UNICODE big endian
		len = len/2;
		m_txtbuffer = (wchar_t*)pcBuffer;
		for(int i=0; i<len; i++)
		{
			//	unsigned char high, low;
			//	high = (*(pwBuffer+i) & 0xFF00) >>8;  
			//   low  = *(pwBuffer+i) & 0x00FF;  
			*(m_txtbuffer+i) = ( (*(m_txtbuffer+i) & 0x00FF) <<8) | ((*(m_txtbuffer+i) & 0xFF00) >>8);  
		}
	}
	else if(len>3 && *(pcBuffer+0) == -17 && *(pcBuffer+1) == -69 && *(pcBuffer+2) == -65)
	{//UTF-8
		len = MultiByteToWideChar (CP_UTF8, 0, pcBuffer, len, NULL, 0);
		m_txtbuffer = new wchar_t[len];
		MultiByteToWideChar (CP_UTF8, 0, pcBuffer, -1, m_txtbuffer, len);// 开始转换
		delete []pcBuffer;
	}
	else
	{//ASCII
		len = MultiByteToWideChar (CP_ACP, 0, pcBuffer, len, NULL, 0);
		m_txtbuffer = new wchar_t[len];
		MultiByteToWideChar (CP_ACP, 0, pcBuffer, -1, m_txtbuffer, len);// 开始转换
		delete []pcBuffer;
	}
	m_txtlen=len;
	return ParseFromString();
}
bool CTxtBookMark::ParseFromString()
{
	int startpos=0;
	int endpos=0;
	wchar_t* wbuffer = m_txtbuffer;
	m_sectionList.clear();
	if( m_txtbuffer==NULL) return false;
	const wchar_t* pbuffer=m_txtbuffer;
	int length = m_txtlen;

	if (m_isSplit)
	{
		do {
			int pos = 0;
			int posz = 0;
			int posj = 0;
			int poszk = 0;
			int posyk = 0;
			int posxzk = 0;
			int posxyk = 0;

			int csn;
			bool isFind = false;
			//每行提取

			const wchar_t* pnext = wcschr(pbuffer, L'\n');
			if (pnext == NULL) break;
			int gl = pnext - pbuffer;
			if (gl > 50) gl = 50;

			//章节检测
			const wchar_t* pt1;
			pt1 = wcsnchr(pbuffer, gl, L'第');
			if (pt1 == NULL) pos = -1;
			pos = pt1 - pbuffer;

			pt1 = wcsnchr(pbuffer, gl, L'章');
			if (pt1 == NULL) posz = -1;
			posz = pt1 - pbuffer;

			pt1 = wcsnchr(pbuffer, gl, L'节');
			if (pt1 == NULL) posj = -1;
			posj = pt1 - pbuffer;

			pt1 = wcsnchr(pbuffer, gl, L'（');
			if (pt1 == NULL) poszk = -1;
			poszk = pt1 - pbuffer;

			pt1 = wcsnchr(pbuffer, gl, L'）');
			if (pt1 == NULL) posyk = -1;
			posyk = pt1 - pbuffer;

			pt1 = wcsnchr(pbuffer, gl, L'(');
			if (pt1 == NULL) posxzk = -1;
			posxzk = pt1 - pbuffer;

			pt1 = wcsnchr(pbuffer, gl, L')');
			if (pt1 == NULL) posxyk = -1;
			posxyk = pt1 - pbuffer;

			if (pos >= 0)
			{
				if (posz > 0 && posz > pos)//第？章
				{
					csn = ParseNum(pbuffer + pos + 1, posz - pos - 1);
					if (csn >= 0)
					{
						isFind = true;
					}

				}
				//if( !isFind && posj>pos)//第？节
				//{
				//	csn = ParseNum(pbuffer+pos+1,posj-pos-1);
				//	if(csn>=0)
				//	{
				//		isFind = true;
				//	}
				//}
			}
			if (!isFind && poszk >= 0) //（?）
			{
				if (posyk > poszk)
				{

					csn = ParseNum(pbuffer + poszk + 1, posyk - poszk - 1);
					if (csn >= 0)
					{
						pos = poszk;
						posz = posyk;
						isFind = true;
					}
				}
			}
			if (!isFind && posxzk >= 0) //(?)
			{
				if (posxyk > posxzk)
				{

					csn = ParseNum(pbuffer + posxzk + 1, posxyk - posxzk - 1);
					if (csn >= 0)
					{
						pos = posxzk;
						posz = posxyk;
						isFind = true;
					}
				}
			}

			if (isFind)
			{//(123)
				wchar_t* pts = new wchar_t[gl];//[posz-pos+2];
				wchar_t pts1[50];
				wcsncpy(pts1, pbuffer, gl);//posz-pos+1);
				pts1[gl - 1] = L'\0';
				wcsncpy(pts, pbuffer, gl);//posz-pos+1);
				*(pts + gl - 1) = L'\0';
				std::wstring tempStr = pts;

				SectionStruct node;
				node.startpos = pbuffer - m_txtbuffer;
				//		node.length= pnext-pbuffer;
				tempStr = WTrim(tempStr);

				node.name = tempStr;
				node.sectionnum = csn;
				this->m_sectionList.push_back(node);
			}


			pbuffer = pnext + 1;
		} while (pbuffer - m_txtbuffer < length);
	}
	
	ProcSectionList();

	return true;
}

void CTxtBookMark::ProcSectionList()
{
	int i=0;
	int j=0;
	int plen=-1;
	int length = m_txtlen;
	int vcount = m_sectionList.size();
	if( vcount==0)
	{
		SectionStruct node;
		node.startpos=0;
		node.length=length;
		node.sectionnum = 0;
		node.name=L"正文";
		this->m_sectionList.push_back(node);
	//		node.length= pnext-pbuffer;
		return;
	}
	if( vcount==1)
	{
		m_sectionList[0].length=length;
		m_sectionList[0].sectionnum = 0;
	}

	if( m_sectionList[0].startpos > 10)
	{
		SectionStruct node;
		node.startpos=0;
		node.length=m_sectionList[0].startpos;
		node.sectionnum = 0;
		node.name=L"正文";
		this->m_sectionList.insert(m_sectionList.begin(),node);
		vcount++;
	}


	for(i=0,j=1;j<vcount;j++,i++)
	{
		//加长度
		int len = m_sectionList[j].startpos-m_sectionList[i].startpos;
		m_sectionList[i].length=len;

	}
	m_sectionList[i].length = length-m_sectionList[i].startpos;
}


std::wstring CTxtBookMark::GetSectionName(int sectionNum)
{
	if( sectionNum<this->m_sectionList.size())
	{
		return this->m_sectionList.at(sectionNum).name;
	}
}
const wchar_t* CTxtBookMark::GetSectionContent(int sectionNum, int& length)
{
	int start;
	int end;
	if(sectionNum>=this->m_sectionList.size())
		return NULL;

	m_preGetBlock = sectionNum;
	start = m_sectionList[sectionNum].startpos;
	length = m_sectionList[sectionNum].length;
	return m_txtbuffer+start;
}

