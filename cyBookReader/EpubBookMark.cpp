#include "StdAfx.h"
#include "EpubBookMark.h"
#include "tinyxml\\tinyxml.h"
#include "cyStrBase.h"

CString chartowstring(const char* buf,int length)  
{  
    CString result;  
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_UTF8, 0, buf, length, NULL, 0);  
    TCHAR* buffer = new TCHAR[len + 1];  
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_UTF8, 0, buf, length, buffer, len);  
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result=buffer;  
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
	//	delete []buf;
	}
	else if( strstr(buf,"encoding=\"utf-8\"") !=NULL)
	{//UTF-8
		len = MultiByteToWideChar (CP_UTF8, 0, buf, -1, NULL, 0);
		rbuf = new wchar_t[len];
		MultiByteToWideChar (CP_UTF8, 0, buf, -1, rbuf, len);// 开始转换
	//	delete []buf;
	}
	else if (strstr(buf, "charset=utf-8") != NULL)
	{//UTF-8
		len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
		rbuf = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, rbuf, len);// 开始转换
															//	delete []buf;
	}
	else
	{//ASCII
		len = MultiByteToWideChar (CP_ACP, 0, buf, -1, NULL, 0);
		rbuf = new wchar_t[len];
		MultiByteToWideChar (CP_ACP, 0, buf, -1, rbuf, len);// 开始转换
	//	delete []buf;
	}
	length=len;
	return rbuf;
}

CString ParseHtml(const char* buf, int length)
{
	TCHAR nc;
	TCHAR strp[] = _T("<p>");
	TCHAR strpk[] = _T("<p ");
	CString ret = _T("");
	TCHAR* html = chartowchar(buf, length);
	const TCHAR* pstr = html;
	const TCHAR* pdstr;
	int i = 0;
	int iscon = 1;
	while (iscon)
	{
		pstr = wcschr(pstr, _T('<'));
		if (pstr == 0) break;
		pdstr = wcschr(pstr, _T('>'));
		if (pdstr == 0) break;
		switch (*(pstr + 1))
		{
		case _T('p'):
		case _T('P'):
			nc = *(pstr + 2);
			if (nc == ' ' || nc == '>')
			{
				TCHAR dhs[] = { _T('<'),_T('/'),*(pstr + 1),_T('>'),0 };
				const TCHAR *vstr = wcsstr(pdstr, dhs);
				if (vstr == 0)
				{
					pstr = pdstr;
					break;
				}
				int len = vstr - pdstr;
				ret.Append(pdstr+1, len-1);
				ret.AppendChar(_T('\n'));
				pstr = vstr + 1;
			}
			else
			{
				pstr = pdstr;
			}
			
			break;
		case 'h':
		case 'H':
			nc = *(pstr + 2);
			if (nc >= '0' && nc <= '9')
			{
				TCHAR dhs[] = { '<','/',*(pstr + 1),*(pstr + 2),'>',0 };
				const TCHAR *vstr = wcsstr(pdstr, dhs);
				if (vstr == 0)
				{
					pstr = pdstr;
					break;
				}
				int len = vstr - pdstr;
				ret.Append(pdstr+1, len-1);
				ret.AppendChar(_T('\n'));
				pstr = vstr + 1;
			}
			else
			{
				pstr = pdstr;
			}
			break;
		default:
			pstr = pdstr;
		}
		

	}
	delete[]html;
	return ret;
}


CEpubBookMark::CEpubBookMark(void)
{
	m_preGetBlock = 0;
}


CEpubBookMark::~CEpubBookMark(void)
{
}

std::vector<CString> CEpubBookMark::GetAllSectionName()
{
	std::vector<CString> ab;
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

int CEpubBookMark::FindzfFile(CString filename, ZipFileInfo &zf)
{
	for (std::vector<ZipFileInfo>::iterator it = m_zfList.begin(); it != m_zfList.end(); it++)
	{
		int ret = it->name.Find(filename);
		if (ret >= 0)
		{
			zf.index = it->index;
			zf.name = it->name;
			zf.size = it->size;
			return 1;
		}
	}
	return 0;
}

bool CEpubBookMark::InitFile(CString fileName)
{
	int i = 0;
	ZIPENTRY ze;
	ZipFileInfo zf;
	m_zfList.clear();
	m_hz = OpenZip(fileName, 0);
	if (!IsZipHandleU(m_hz))
	{
		CloseZip(m_hz);
		return false;
	}
	while (1)
	{
		int ret = GetZipItem(m_hz, i++, &ze);
		if (ret != ZR_OK)
			break;
		if (ze.unc_size <= 0)
			continue;

		zf.index = i - 1;
		zf.name = ze.name;
		zf.size = ze.unc_size;
		m_zfList.push_back(zf);

	}
	CloseZip(m_hz);
}

//{return m_sectionList.size();}
bool CEpubBookMark::ParseFromFile(CString fileName)
{
	int ret;
	InitFile(fileName);

	// read container
	ZipFileInfo zf;
	ret = FindzfFile(_T("container.xml"), zf);
	if (!ret)
		return ret;

	m_hz = OpenZip(fileName, 0);
	char *buf = new char[zf.size];
	ZRESULT er = UnzipItem(m_hz, zf.index, buf, zf.size);
	CString optn;
	if (er != ZR_FLATE)
	{
		//解析buf
		optn = ParseContainer(buf, zf.size);
		//	isRe = ParseToc(buf, len);
	}
	delete[]buf;
	if (!ret)return ret;
	
	// read opf
	OpfContent opf;
	ret = FindzfFile(optn, zf);
	if (!ret) return ret;
	buf = new char[zf.size];
	er = UnzipItem(m_hz, zf.index, buf, zf.size);
	if (er != ZR_FLATE)
	{
		//解析buf
		ret = ParseOpf(buf, zf.size,opf);
		//	isRe = ParseToc(buf, len);
	}
	delete[]buf;
	if (!ret)return ret;

	ret = FindzfFile(opf.ncxn, zf);
	if (!ret) return ret;
	buf = new char[zf.size];
	er = UnzipItem(m_hz, zf.index, buf, zf.size);
	if (er != ZR_FLATE)
	{
		//解析buf
		ret = ParseNcx(buf, zf.size);
		//	isRe = ParseToc(buf, len);
	}
	delete[]buf;
	if (!ret)return ret;

	ParseContent();
	CloseZip(m_hz);

	return true;
}

bool CEpubBookMark::ParseFromString()
{
	return false;
}

CString CEpubBookMark::GetSectionName(int sectionNum)
{
	if (sectionNum < m_sectionList.size())
		return m_sectionList[sectionNum].name;
	return L"";
}

const wchar_t* CEpubBookMark::GetSectionContent(int sectionNum, int& length)
{
	if (sectionNum < m_sectionList.size())
	{
		if (m_sectionList[sectionNum].cIndex > -1)
		{
			m_preGetBlock = sectionNum;
			length = m_textList[m_sectionList[sectionNum].cIndex].GetLength();
			return m_textList[m_sectionList[sectionNum].cIndex].GetBuffer();
		}
	}
	return NULL;
}

int CEpubBookMark::GetPreGetBlock()
{
	return m_preGetBlock;
}

int CEpubBookMark::ParseNcx(char* buf, int length)
{
	TiXmlDocument doc;
	doc.Parse(buf);
	TiXmlElement *root = doc.RootElement();
	TRACE(root->Value());

	TiXmlElement *pElt;
	for (pElt = root->FirstChildElement(); pElt; pElt = pElt->NextSiblingElement())
	{
		if (strncmp(pElt->Value(), "navMap", 6) == 0)
		{
			TiXmlElement *pSecElt;
			for (pSecElt = pElt->FirstChildElement(); pSecElt; pSecElt = pSecElt->NextSiblingElement())
			{
				TiXmlElement *pNode;
				SectionStruct node;
				for (pNode = pSecElt->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
				{
					if (strncmp(pNode->Value(), "navLabel", 8) == 0)
					{
						TiXmlElement *pText = pNode->FirstChildElement();

						TRACE(pText->Value());

						const char *pstr = pText->GetText();
						node.name = chartowstring(pstr, strlen(pstr));

					}
					else if (strncmp(pNode->Value(), "content", 7) == 0)
					{
						const char *pstr = pNode->Attribute("src");
						node.path = chartowstring(pstr, strlen(pstr));
					}
				}
				node.contentType = EText;
				node.cIndex = -1;
				this->m_sectionList.push_back(node);

			}

			break;
		}
	}



	return 1;
}

int CEpubBookMark::ParseOpf(char* buf, int length, OpfContent &opf)
{
	TiXmlDocument doc;
	doc.Parse(buf);
	TiXmlElement *root = doc.RootElement();
	TRACE(root->Value());


	TiXmlElement *pElt;

	for (pElt = root->FirstChildElement(); pElt; pElt = pElt->NextSiblingElement())
	{
		TRACE(pElt->Value());
		if (strncmp(pElt->Value(), "metadata", 8) == 0)
		{
			TiXmlElement *pSecElt;
			for (pSecElt = pElt->FirstChildElement(); pSecElt; pSecElt = pSecElt->NextSiblingElement())
			{
				TRACE(pSecElt->Value());
				if (strncmp(pSecElt->Value(), "dc:title", 8) == 0)
				{
					const char* text = pSecElt->GetText();
					opf.title = chartowstring(text, strlen(text));
				}
				else if (strncmp(pSecElt->Value(), "dc:creator", 10) == 0)
				{
					const char* text = pSecElt->GetText();
					opf.creator = chartowstring(text, strlen(text));
				}
				else if (strncmp(pSecElt->Value(), "dc:description", 14) == 0)
				{
					const char* text = pSecElt->GetText();
					opf.description = chartowstring(text, strlen(text));
				}
			}
		}
		else if (strncmp(pElt->Value(), "manifest", 8) == 0)
		{
			TiXmlElement *pSecElt;
			for (pSecElt = pElt->FirstChildElement(); pSecElt; pSecElt = pSecElt->NextSiblingElement())
			{
				TRACE(pSecElt->Value());
				if (strncmp(pSecElt->Value(), "item", 4) == 0)
				{
					const char *pstr = pSecElt->Attribute("id");
					if (pstr != 0)
					{
						if (strncmp(pstr, "ncx", 3) == 0)
						{
							const char *pstr1 = pSecElt->Attribute("href");
							if (pstr1 != 0)
							{
								opf.ncxn = chartowstring(pstr1, strlen(pstr1));
							}
						}
					}
				}
			}
		}
	}
	return 1;
}

CString CEpubBookMark::ParseContainer(char* buf, int length)
{
	TiXmlDocument doc;
	doc.Parse(buf);
	TiXmlElement *root = doc.RootElement();
	TRACE(root->Value());
	if(strncmp(root->Value(), "container", 9) != 0)
	{
		return 0;
	}

	TiXmlElement *pElt;

	for (pElt = root->FirstChildElement(); pElt; pElt = pElt->NextSiblingElement())
	{
		TRACE(pElt->Value());
		if (strncmp(pElt->Value(), "rootfiles", 9) == 0)
		{
			TiXmlElement *pSecElt;
			for (pSecElt = pElt->FirstChildElement(); pSecElt; pSecElt = pSecElt->NextSiblingElement())
			{
				if (strncmp(pSecElt->Value(), "rootfile", 8) == 0)
				{
					TiXmlElement *pNode;
					SectionStruct node;
					const char *pstr = pSecElt->Attribute("full-path");
					CString rs = chartowstring(pstr, strlen(pstr));
					return rs;
				}
			}
		}
	}
	return _T("");
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
	ZipFileInfo zf;
	m_textList.clear();

		//解析内容页
	for(std::vector<SectionStruct>::iterator it= m_sectionList.begin();it!=m_sectionList.end();it++)
	{
		if( it->contentType == EText)
		{
		//	FindZipItem(m_hz,it->path.GetBuffer(),true,&i,&ze);
			this->FindzfFile(it->path, zf);
			if(zf.index<0) continue;
			char *buf = new char[zf.size];
			ZRESULT er = UnzipItem(m_hz,zf.index,buf,zf.size);
			if( er!=ZR_FLATE) 
			{
				//解析buf
				it->cIndex=ParseSection(buf,zf.size);

			}
			delete[]buf;
		}
		else
		{

		}
	}
	return isRe;
}

