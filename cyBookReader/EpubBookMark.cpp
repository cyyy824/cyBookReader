#include "StdAfx.h"
#include "EpubBookMark.h"
#include "tinyxml\\tinyxml.h"
#include "cyStrBase.h"

std::wstring chartowstring(const char* buf,int length)  
{  
    std::wstring result;  
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_UTF8, 0, buf, length, NULL, 0);  
    TCHAR* buffer = new TCHAR[len + 1];  
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_UTF8, 0, buf, length, buffer, len);  
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);  
    delete[] buffer;  
    return result;  
}



wchar_t* chartowchar(const char*  buf, int &length)
{
	ULONGLONG len=length;
	wchar_t* rbuf;
	if(len>3 && *(buf+0) == -1 && *(buf+1) == -2)
	{//UNICODE
		len = len/2;
		rbuf = (wchar_t*)buf;
	}
	else if(len>3 && *(buf+0) == -2 && *(buf+1) == -1)
	{//UNICODE big endian
		len = len/2;
		rbuf = (wchar_t*)buf;
		for(int i=0; i<len; i++)
		{
			//	unsigned char high, low;
			//	high = (*(pwBuffer+i) & 0xFF00) >>8;  
			//   low  = *(pwBuffer+i) & 0x00FF;  
			*(rbuf+i) = ( (*(rbuf+i) & 0x00FF) <<8) | ((*(rbuf+i) & 0xFF00) >>8);  
		}
	}
	else if(len>3 && *(buf+0) == -17 && *(buf+1) == -69 && *(buf+2) == -65)
	{//UTF-8
		len = MultiByteToWideChar (CP_UTF8, 0, buf, -1, NULL, 0);
		rbuf = new wchar_t[len];
		MultiByteToWideChar (CP_UTF8, 0, buf, -1, rbuf, len);// 开始转换
		delete []buf;
	}
	else if( strstr(buf,"encoding=\"utf-8\"") !=NULL)
	{//UTF-8
		len = MultiByteToWideChar (CP_UTF8, 0, buf, -1, NULL, 0);
		rbuf = new wchar_t[len];
		MultiByteToWideChar (CP_UTF8, 0, buf, -1, rbuf, len);// 开始转换
		delete []buf;
	}
	else
	{//ASCII
		len = MultiByteToWideChar (CP_ACP, 0, buf, -1, NULL, 0);
		rbuf = new wchar_t[len];
		MultiByteToWideChar (CP_ACP, 0, buf, -1, rbuf, len);// 开始转换
		delete []buf;
	}
	length=len;
	return rbuf;
}

std::wstring ParseHtml(const char* buf, int length)
{
	struct STRPOS
	{
		int pos;
		int len;
	};
	std::wstring ab=L"";
	std::vector<STRPOS> posList;
	int len;
	wchar_t* html = chartowchar(buf,length);

	wchar_t* pstr=html;
	wchar_t* pystr;
	wchar_t* pzstr;

	while(1)
	{

		pystr = wcschr(pstr,L'>');
		if( pystr==NULL)
			break;
		pzstr = wcschr(pystr,L'<');
		if( pzstr==NULL)
			break;

		pstr=pzstr;

		len = pzstr-pystr;
		if(len>1)
		{
			for(int pos=1;pos<len;pos++)
			{
				if( *(pystr+pos)!=L' ' && *(pystr+pos)!=L'\r' && *(pystr+pos)!=L'\n')
				{
					STRPOS pn;
					pn.pos = pystr-html+pos;
					pn.len = len-pos;
					posList.push_back(pn);
					break;
				}
			}
		}
	}
	
	for(int i=0;i<posList.size();i++)
	{
		wchar_t* ts = new wchar_t[posList[i].len+2];
		*(ts+posList[i].len+1) = L'\0';
		*(ts+posList[i].len) = L'\n';
		wcsncpy(ts,html+posList[i].pos,posList[i].len);
		ab.append(ts);
		delete []ts;
	}
	delete []html;

	TRACE(L"%d",posList.size());
	return ab;

}

CEpubBookMark::CEpubBookMark(void)
{
	m_preGetBlock = 0;
}


CEpubBookMark::~CEpubBookMark(void)
{
}

std::vector<std::wstring> CEpubBookMark::GetAllSectionName()
{
	std::vector<std::wstring> ab;
	for(std::vector<SectionStruct>::iterator it=m_sectionList.begin(); it!=m_sectionList.end();it++)
	{
		ab.push_back(it->name);
	}
	return ab;
}

int CEpubBookMark::GetSectionCount()
{
	return m_sectionList.size();
}
	//{return m_sectionList.size();}
bool CEpubBookMark::ParseFromFile(std::wstring fileName)
{
	HANDLE hFile, hFileToWrite;
	CString strFilePath = fileName.c_str();


	m_hz = OpenZip(strFilePath,0);
	if( !IsZipHandleU(m_hz)) 
	{
		CloseZip(m_hz);
		return false;
	}

	ParseContent();

	
	CloseZip(m_hz);

	return true;
}

bool CEpubBookMark::ParseFromString()
{
	return false;
}

std::wstring CEpubBookMark::GetSectionName(int sectionNum)
{
	if( sectionNum < m_sectionList.size() )
		return m_sectionList[sectionNum].name;
	return L"";
}

const wchar_t* CEpubBookMark::GetSectionContent(int sectionNum, int& length)
{
	if( sectionNum < m_sectionList.size() )
	{
		if( m_sectionList[sectionNum].cIndex > -1)
		{
			m_preGetBlock = sectionNum;
			length = m_textList[m_sectionList[sectionNum].cIndex].size();
			return m_textList[m_sectionList[sectionNum].cIndex].c_str();
		}
	}
	return NULL;
}

int CEpubBookMark::GetPreGetBlock()
{
	return m_preGetBlock;
}

bool CEpubBookMark::ParseToc(char* buf,int length)
{//解析目录，填充m_sectionList
	TiXmlDocument doc;
	doc.Parse(buf);
	TiXmlElement *root = doc.RootElement();
	TRACE(root->Value());

	TiXmlElement *pElt;
	for( pElt=root->FirstChildElement();pElt;pElt=pElt->NextSiblingElement())
	{
		if( strncmp(pElt->Value(),"navMap",6)==0)
		{
			TiXmlElement *pSecElt;
			for(pSecElt=pElt->FirstChildElement();pSecElt;pSecElt=pSecElt->NextSiblingElement())
			{
				TiXmlElement *pNode;
				SectionStruct node;
				for(pNode=pSecElt->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
				{
					if( strncmp( pNode->Value(), "navLabel",8)==0)
					{
						TiXmlElement *pText = pNode->FirstChildElement();

						TRACE(pText->Value());

						const char *pstr=pText->GetText();
						node.name = chartowstring(pstr,strlen(pstr));

					}
					else if( strncmp( pNode->Value(), "content",7)==0)
					{
						const char *pstr=pNode->Attribute("src");
						node.path = _T("OEBPS/")+chartowstring(pstr,strlen(pstr));
					}
				}
				node.contentType= EText;
				node.cIndex = -1;
				this->m_sectionList.push_back(node);
				
			}

			break;
		}
	}
	
	

	return true;
}

int CEpubBookMark::ParseSection(char* buf,int length)
{
	int index=0;
	//std::wstring content;
	m_textList.push_back(ParseHtml(buf,length));
	return m_textList.size()-1;
}

void CEpubBookMark::Clear()
{
	m_sectionList.clear();
	m_textList.clear();
}

bool CEpubBookMark::ParseContent()
{
	bool isRe=false;
	int len;
	int i;
	ZIPENTRY ze;
	Clear();

	//解析目录
	FindZipItem(m_hz,_T("OEBPS/toc.ncx"),true,&i,&ze);
	if(i<0) return false;
	char *buf = new char[ze.unc_size];
	ZRESULT er = UnzipItem(m_hz,ze.index,buf,ze.unc_size);
	if( er!=ZR_FLATE) 
	{
		len = ze.unc_size;
	//	wchar_t* buf1= chartowchar(buf,len);
		//解析buf
		isRe=ParseToc(buf,len);
	}
	delete []buf;
	if(!isRe)return isRe;

	//解析内容页
	for(std::vector<SectionStruct>::iterator it= m_sectionList.begin();it!=m_sectionList.end();it++)
	{
		if( it->contentType == EText)
		{
			FindZipItem(m_hz,it->path.c_str(),true,&i,&ze);
			if(i<0) continue;
			buf = new char[ze.unc_size];
			er = UnzipItem(m_hz,ze.index,buf,ze.unc_size);
			if( er!=ZR_FLATE) 
			{
				//解析buf
				it->cIndex=ParseSection(buf,ze.unc_size);

			}
		}
		else
		{

		}
	}
	return isRe;
}

