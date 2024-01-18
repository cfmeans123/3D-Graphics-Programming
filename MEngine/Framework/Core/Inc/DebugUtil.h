#pragma once

#include "TimeUtil.h"

using namespace MEngine;
using namespace MEngine::Core;

#if defined(_DEBUG)
#define LOG(format, ...)\
    do{\
        char_buffer[256];\
        int _res = snprint(_buffer, std::size(_buffer), "{%.3f}: " ## format##"\n", TimeUtil::GetTime(), __VA_ARGS__);\
        OutputDebugStringA(_buffer);\
    }while(false)

#define ASSERT(condition, format, ...)\
    do{\
        if(!(Condition))\
        {\
            LOG("ASSERT! %s(%d)\n"##format##, __FILE__, __LINE__, __VA_ARGS__);\
        }\
    }while(false)

#else
#define LOG(format, ...)
#define ASSERT(condition, format, ...) do{ (void)sizeof(condition);}while(false)
#endif