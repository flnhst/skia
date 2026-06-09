// Copyright 2019 Google LLC
#ifndef FontCollection_DEFINED
#define FontCollection_DEFINED

#include <memory>
#include <optional>
#include <set>
#include "include/core/SkFontMgr.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSpan.h"
#include "modules/skparagraph/include/FontArguments.h"
#include "modules/skparagraph/include/ParagraphCache.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "src/core/SkTHash.h"
#include "modules/skparagraph/include/ExportDefines.h"

namespace skia {
namespace textlayout {

class TextStyle;
class Paragraph;
class SKPARAGRAPH_API FontCollection : public SkRefCnt {
public:
    FontCollection();
    ~FontCollection() override;

    static sk_sp<FontCollection> Make();

    size_t getFontManagersCount() const;

    void setAssetFontManager(sk_sp<SkFontMgr> fontManager);
    void setDynamicFontManager(sk_sp<SkFontMgr> fontManager);
    void setTestFontManager(sk_sp<SkFontMgr> fontManager);
    void setDefaultFontManager(sk_sp<SkFontMgr> fontManager);
    void setDefaultFontManager(sk_sp<SkFontMgr> fontManager, const char defaultFamilyName[]);
    void setDefaultFontManager(sk_sp<SkFontMgr> fontManager, const skia_private::TArray<SkString>& defaultFamilyNames);

    sk_sp<SkFontMgr> getFallbackManager() const { return fDefaultFontManager; }

    skia_private::TArray<sk_sp<SkTypeface>> findTypefaces(const skia_private::TArray<SkString>& familyNames, SkFontStyle fontStyle);
    skia_private::TArray<sk_sp<SkTypeface>> findTypefaces(const skia_private::TArray<SkString>& familyNames, SkFontStyle fontStyle, const std::optional<FontArguments>& fontArgs);

    sk_sp<SkTypeface> defaultFallback(SkUnichar unicode, const std::vector<SkString>& families,
                                      SkFontStyle fontStyle, const SkString& locale,
                                      const std::optional<FontArguments>& fontArgs);
    sk_sp<SkTypeface> defaultEmojiFallback(SkUnichar emojiStart, SkFontStyle fontStyle, const SkString& locale);
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
        FamilyKey(const skia_private::TArray<SkString>& familyNames, SkFontStyle style, const std::optional<FontArguments>& args);

        FamilyKey();

        virtual ~FamilyKey();

        skia_private::TArray<SkString> fFamilyNames;
        SkFontStyle fFontStyle;
        std::optional<FontArguments> fFontArguments;

        bool operator==(const FamilyKey& other) const;

        struct SKPARAGRAPH_API Hasher {
            Hasher();
            virtual ~Hasher();

            size_t operator()(const FamilyKey& key) const;
        };
    };

    sk_sp<SkTypeface> cloneTypeface(const sk_sp<SkTypeface>& typeface, const FontArguments& args);

    struct FaceCache;
    std::unique_ptr<FaceCache> fFaceCache;
    struct VariationCache;
    std::unique_ptr<VariationCache> fVariationCache;
    bool fEnableFontFallback;
    skia_private::THashMap<FamilyKey, skia_private::TArray<sk_sp<SkTypeface>>, FamilyKey::Hasher> fTypefaces;
    sk_sp<SkFontMgr> fDefaultFontManager;
    sk_sp<SkFontMgr> fAssetFontManager;
    sk_sp<SkFontMgr> fDynamicFontManager;
    sk_sp<SkFontMgr> fTestFontManager;

    skia_private::TArray<SkString> fDefaultFamilyNames;
    ParagraphCache fParagraphCache;
};
}  // namespace textlayout
}  // namespace skia

#endif  // FontCollection_DEFINED
