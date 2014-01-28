// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <regex>
#include <deque>
#include <list>
#include <memory>
#include <map>
#include <complex>
#include <algorithm>
//#include <boost\algorithm\searching\boyer_moore.hpp>
//#include <boost\regex.hpp>
// TODO: reference additional headers your program requires here

namespace std {
#ifdef UNICODE
	typedef wstring tstring;
	static wostream& tcout = wcout;
#else
	typedef string tstring;
	static ostream& tcout = cout;
#endif
//}
//namespace boost {
typedef std::regex_iterator<std::tstring::const_iterator> tRegexIterator;
}