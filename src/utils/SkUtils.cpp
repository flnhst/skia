#include "include/utils/SkUtils.h"

void SkSpUtils::reset_sp(sk_sp<SkRefCnt>& sp) {
    sp.reset();
}
