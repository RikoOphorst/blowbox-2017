#pragma once

#include "util/assert.h"

#ifdef _DEBUG
#define BLOWBOX_RELEASE(object) { if (object != nullptr) { object->Release(); object = nullptr; } }
#define BLOWBOX_RELEASE_GUARANTEE(object) { if (object != nullptr) { object->Release(); object == nullptr; } else { BLOWBOX_ASSERT(false); } }
#else
#define BLOWBOX_RELEASE(object) { if (object != nullptr) { object->Release(); object = nullptr; } }
#define BLOWBOX_RELEASE_GUARANTEE(object)  { if (object != nullptr) { object->Release(); object = nullptr; } }
#endif