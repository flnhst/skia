// Copyright 2019 Google LLC.
#ifndef TextShadow_DEFINED
#define TextShadow_DEFINED

#include "include/core/SkColor.h"
#include "include/core/SkPoint.h"

#include "modules/skparagraph/include/ExportDefines.h"

namespace skia {
namespace textlayout {

class SKPARAGRAPH_API TextShadow {
public:
    SkColor fColor = SK_ColorBLACK;
    SkPoint fOffset;
    double fBlurRadius = 0.0;

    TextShadow();

    TextShadow(SkColor color, SkPoint offset, double blurRadius);

    bool operator==(const TextShadow& other) const;

    bool operator!=(const TextShadow& other) const;

    bool hasShadow() const;
};
}  // namespace textlayout
}  // namespace skia

#endif  // TextShadow_DEFINED
