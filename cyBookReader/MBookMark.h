#pragma once
#include <vector>
#include <string>

/*
改类将作为各书目录的同一类，那么它需要些什么函数了
1.返回所有目录名的函数GetAllDir()
2.返回目录数量的函数GetDirCount()
3.从文件分析出目录的函数ParseFromFile()和ParseFromString

*/


class MBookMark
{
public:
	MBookMark(void);
	~MBookMark(void);

public:	//需重载函数
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

