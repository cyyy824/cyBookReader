#pragma once
#include "mbookmark.h"
#include "zip_utils_src\\zip.h"
#include "zip_utils_src\\unzip.h"
#include<vector>
#include<string>

class CEpubBookMark :
	public MBookMark
{
public:
	CEpubBookMark(void);
	~CEpubBookMark(void);

public:
	std::vector<CString> GetAllSectionName();
	int GetSectionCount();//{return m_sectionList.size();}
	bool ParseFromFile(CString fileName);
	bool ParseFromString();
	CString GetSectionName(int sectionNum);

	const wchar_t* GetSectionContent(int sectionNum, int& length);

	int GetPreGetBlock();//{return m_preGetBlock;}
	bool SetIsSplit(bool isSplit) { return false; }

private:
	enum TContentType{
		EText,
		EImage
	};
	struct SectionStruct
	{
	CString name;
	CString path;
	TContentType contentType;
	int cIndex;
	};	
	struct ZipFileInfo {
		CString name;
		long size;
		int index;
	};
	struct OpfContent {
		CString title;
		CString creator;
		CString description;
		CString ncxn;
	};
private:
	bool ParseContent();
	bool ParseToc(char* buf, int length);
	int ParseSection(char* buf, int length);
	bool InitFile(CString fileName);
	void Clear();
	int FindzfFile(CString filename, ZipFileInfo &zf);
	CString ParseContainer(char* buf, int length);
	int ParseOpf(char* buf, int length, OpfContent &opf);
	int ParseNcx(char* buf, int length);

private:
	HZIP m_hz;
	std::vector<SectionStruct> m_sectionList;
	std::vector<ZipFileInfo> m_zfList;
	std::vector<CString> m_textList;
	int m_preGetBlock;
};

