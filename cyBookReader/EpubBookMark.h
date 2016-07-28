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
	std::vector<std::wstring> GetAllSectionName();
	int GetSectionCount();//{return m_sectionList.size();}
	bool ParseFromFile(std::wstring fileName);
	bool ParseFromString();
	std::wstring GetSectionName(int sectionNum);

	const wchar_t* GetSectionContent(int sectionNum, int& length);

	int GetPreGetBlock();//{return m_preGetBlock;}
	bool SetIsSplit(bool isSplit) { return false; }
private:
	bool ParseContent();
	bool ParseToc(char* buf,int length);
	int ParseSection(char* buf,int length);
	void Clear();
private:
	enum TContentType{
		EText,
		EImage
	};
	struct SectionStruct
	{
	std::wstring name;
	std::wstring path;
	TContentType contentType;
	int cIndex;
	};	

private:
	HZIP m_hz;
	std::vector<SectionStruct> m_sectionList;
	std::vector<std::wstring> m_textList;
	int m_preGetBlock;
};

