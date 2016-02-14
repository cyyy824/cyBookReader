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
	//virtual bool ParseFromString(wchar_t* buffer);
	//virtual std::wstring GetSectionName(int sectionNum);
	//virtual const wchar_t*  GetSectionContent(int sectionNum, int& length);

	virtual std::vector<std::wstring> GetAllSectionName()=0;
	virtual int GetSectionCount()=0;
	virtual bool ParseFromFile(std::wstring fileName)=0;
//	virtual bool ParseFromString(wchar_t* buffer)=0;
	virtual std::wstring GetSectionName(int sectionNum)=0;

	virtual const wchar_t* GetSectionContent(int sectionNum, int& length)=0;

	virtual int GetPreGetBlock()=0;
};

