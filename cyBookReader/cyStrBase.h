#pragma once

#include<string>



int ParseNum(std::wstring str);

std::wstring WTrim(std::wstring str);

std::string Trim(std::string str);

int ParseNum(const wchar_t* pstr,int length);
const wchar_t* wcsnchr(const wchar_t* pstr,int length, wchar_t ch);

