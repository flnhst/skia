#ifndef SkUtils_DEFINED
#define SkUtils_DEFINED

#include "include/core/SkTypes.h"
#include "include/core/SkRefCnt.h"

class SK_API SkSpUtils
{
public:
    static void reset_sp(sk_sp<SkRefCnt>& sp);
};

#endif
