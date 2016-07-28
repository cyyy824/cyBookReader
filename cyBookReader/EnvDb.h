#pragma once
#include "SQLiteTbl.h"

class CEnvDb  : public SQLiteTbl
{
public:
	CEnvDb(void);
	~CEnvDb(void);

	virtual bool Open(const char* pFile);
    virtual const char* TableName();
    virtual int ItemCount();
 
    //data members of the table
    int m_cx;
	int m_cy;
	int m_fontH;
	int m_vs;//ÐÐ¾à
    SQLiteString m_font;
	int m_color;
	int m_fontColor;
	int m_issplit;
};

