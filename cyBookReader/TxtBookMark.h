#pragma once
#include "mbookmark.h"




class CTxtBookMark :
	public MBookMark
{
public:
	CTxtBookMark(void);
	~CTxtBookMark(void);

public:
	std::vector<CString> GetAllSectionName();
	int GetSectionCount(){return m_sectionList.size();}
	bool ParseFromFile(CString fileName);
	bool ParseFromString();
	CString GetSectionName(int sectionNum);

	const wchar_t* GetSectionContent(int sectionNum, int& length);

	int GetPreGetBlock(){return m_preGetBlock;}
	void AutoSetVS();
	bool SetIsSplit(bool isSplit) { m_isSplit = isSplit; return true; }

private:
	void ProcSectionList();

private:
	struct SectionStruct
	{
	CString name;
	int sectionnum;
	int startpos;
	int length;
	};	

private:
	std::vector<SectionStruct> m_sectionList;
	wchar_t* m_txtbuffer;
	int m_txtlen;
	int m_preGetBlock;
	bool m_isSplit;

};

