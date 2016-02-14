#include "stdafx.h"
#include "EnvDb.h"


CEnvDb::CEnvDb(void)
{
	SetItem(0, "cx",              MYDB_TYPE_INT, &m_cx);
	SetItem(1, "cy",              MYDB_TYPE_INT, &m_cy);
	SetItem(2, "vs",              MYDB_TYPE_INT, &m_vs);
	SetItem(3, "fontH",              MYDB_TYPE_INT, &m_fontH);
	SetItem(4, "font",              MYDB_TYPE_TEXT, &m_font);
	SetItem(5, "color",              MYDB_TYPE_INT, &m_color);
	SetItem(6, "fontcolor",              MYDB_TYPE_INT, &m_fontColor);
}


CEnvDb::~CEnvDb(void)
{
}


int CEnvDb::ItemCount()
{
	return 7;
}

const char* CEnvDb::TableName() { return "envt"; }

bool CEnvDb::Open(const char* pFile)
{
    if (SQLiteTbl::Open(pFile)) {
        if (Query()) 
            return true;
    }
    //anything goes wrong, close and return false
    Close();
    return false;
}