#pragma once

#include "util/assert.h"

#ifdef _DEBUG
#define BLOWBOX_DELETE(ptr) { if (ptr != nullptr) { delete ptr; ptr = nullptr; } }
#define BLOWBOX_DELETE_GUARANTEE(ptr) { if (ptr != nullptr) { delete ptr; ptr = nullptr; } else { BLOWBOX_ASSERT(false); }}
#elif
#define BLOWBOX_DELETE(ptr)
#define BLOWBOX_DELETE_GUARANTEE(ptr)
#endif