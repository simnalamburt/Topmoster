#pragma once

// Linker directive
#pragma comment(linker,"\"/manifestdependency:type='win32' " \
    "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' " \
    "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// windows 헤더
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// C++ 헤더
#include <string>
#include <sstream>
