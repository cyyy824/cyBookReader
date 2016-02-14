#pragma once
#include "mbookmark.h"




class CTxtBookMark :
	public MBookMark
{
public:
	CTxtBookMark(void);
	~CTxtBookMark(void);

public:
	std::vector<std::wstring> GetAllSectionName();
	int GetSectionCount(){return m_sectionList.size();}
	bool ParseFromFile(std::wstring fileName);
	bool ParseFromString(wchar_t* buffer);
	std::wstring GetSectionName(int sectionNum);

	const wchar_t* GetSectionContent(int sectionNum, int& length);

	int GetPreGetBlock(){return m_preGetBlock;}

private:
	void ProcSectionList();

private:
	struct SectionStruct
	{
	std::wstring name;
	int sectionnum;
	int startpos;
	int length;
	};	

private:
	std::vector<SectionStruct> m_sectionList;
	wchar_t* m_txtbuffer;
	int m_txtlen;
	int m_preGetBlock;

};

