// Copyright 2019 Google LLC.
#ifndef FontCollection_DEFINED
#define FontCollection_DEFINED

#include <memory>
#include <optional>
#include <set>
#include "include/core/SkFontMgr.h"
#include "include/core/SkRefCnt.h"
#include "include/private/SkTHash.h"
#include "modules/skparagraph/include/FontArguments.h"
#include "modules/skparagraph/include/ParagraphCache.h"
#include "modules/skparagraph/include/TextStyle.h"

#include "modules/skparagraph/include/ExportDefines.h"

namespace skia {
namespace textlayout {

class TextStyle;
class Paragraph;
class SKPARAGRAPH_API FontCollection : public SkRefCnt {
public:
    FontCollection();
    virtual ~FontCollection();

    static sk_sp<FontCollection> Make();

    size_t getFontManagersCount() const;

    void setAssetFontManager(sk_sp<SkFontMgr> fontManager);
    void setDynamicFontManager(sk_sp<SkFontMgr> fontManager);
    void setTestFontManager(sk_sp<SkFontMgr> fontManager);
    void setDefaultFontManager(sk_sp<SkFontMgr> fontManager);
    void setDefaultFontManager(sk_sp<SkFontMgr> fontManager, const char defaultFamilyName[]);
    void setDefaultFontManager(sk_sp<SkFontMgr> fontManager, const SkTArray<SkString>& defaultFamilyNames);

    sk_sp<SkFontMgr> getFallbackManager() const { return fDefaultFontManager; }

    SkTArray<sk_sp<SkTypeface>> findTypefaces(const SkTArray<SkString>& familyNames, SkFontStyle fontStyle);
    SkTArray<sk_sp<SkTypeface>> findTypefaces(const SkTArray<SkString>& familyNames, SkFontStyle fontStyle, const std::optional<FontArguments>& fontArgs);

    sk_sp<SkTypeface> defaultFallback(SkUnichar unicode, SkFontStyle fontStyle, const SkString& locale);
    sk_sp<SkTypeface> defaultFallback();

    void disableFontFallback();
    void enableFontFallback();
    bool fontFallbackEnabled() { return fEnableFontFallback; }

    ParagraphCache* getParagraphCache() { return &fParagraphCache; }

    void clearCaches();

    static int sizeofFontCollection();

private:
    std::vector<sk_sp<SkFontMgr>> getFontManagerOrder() const;

    sk_sp<SkTypeface> matchTypeface(const SkString& familyName, SkFontStyle fontStyle);

    struct SKPARAGRAPH_API FamilyKey {
        FamilyKey(const SkTArray<SkString>& familyNames, SkFontStyle style, const std::optional<FontArguments>& args);

        FamilyKey();

        virtual ~FamilyKey();

        SkTArray<SkString> fFamilyNames;
        SkFontStyle fFontStyle;
        std::optional<FontArguments> fFontArguments;

        bool operator==(const FamilyKey& other) const;

        struct SKPARAGRAPH_API Hasher {
            Hasher();
            virtual ~Hasher();

            size_t operator()(const FamilyKey& key) const;
        };
    };

    bool fEnableFontFallback;
    SkTHashMap<FamilyKey, SkTArray<sk_sp<SkTypeface>>, FamilyKey::Hasher> fTypefaces;
    sk_sp<SkFontMgr> fDefaultFontManager;
    sk_sp<SkFontMgr> fAssetFontManager;
    sk_sp<SkFontMgr> fDynamicFontManager;
    sk_sp<SkFontMgr> fTestFontManager;

    SkTArray<SkString> fDefaultFamilyNames;
    ParagraphCache fParagraphCache;
};
}  // namespace textlayout
}  // namespace skia

#endif  // FontCollection_DEFINED
