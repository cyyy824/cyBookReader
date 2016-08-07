#pragma once
#include <vector>
#include <string>

/*
���ཫ��Ϊ����Ŀ¼��ͬһ�࣬��ô����ҪЩʲô������
1.��������Ŀ¼���ĺ���GetAllDir()
2.����Ŀ¼�����ĺ���GetDirCount()
3.���ļ�������Ŀ¼�ĺ���ParseFromFile()��ParseFromString

*/


class MBookMark
{
public:
	MBookMark(void);
	~MBookMark(void);

public:	//�����غ���
	//virtual std::vector<std::wstring> GetAllSectionName();
	//virtual int GetSectionCount();
	//virtual bool ParseFromFile(std::wstring fileName);
	//virtual bool ParseFg GetSectionName(int sectionNum);
	//virtual const wcharromString(wchar_t* buffer);
	//virtual std::wstrin_t*  GetSectionContent(int sectionNum, int& length);

	virtual std::vector<CString> GetAllSectionName()=0;
	virtual int GetSectionCount()=0;
	virtual bool ParseFromFile(CString fileName)=0;
	virtual bool ParseFromString()=0;
	virtual CString GetSectionName(int sectionNum)=0;

	virtual const wchar_t* GetSectionContent(int sectionNum, int& length)=0;

	virtual int GetPreGetBlock()=0;
	virtual bool SetIsSplit(bool isSplit) = 0;
};

