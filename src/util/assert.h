#pragma once

#include <crtdbg.h> 

#ifdef _DEBUG
#define BLOWBOX_ASSERT(assertion) { if (!(assertion)) { _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, #assertion, "An assertion failed in blowbox."); _CrtDbgBreak(); } }
#define BLOWBOX_ASSERT_HR(assertion) { if (assertion != S_OK) { _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, #assertion, "An assertion failed in blowbox."); _CrtDbgBreak(); } }
#else
#define BLOWBOX_ASSERT(assertion) (assertion);
#define BLOWBOX_ASSERT_HR(assertion) (assertion);
#endif