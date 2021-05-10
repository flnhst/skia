// Copyright 2019 Google LLC.
#include "include/core/SkColor.h"
#include "include/core/SkFontStyle.h"
#include "modules/skparagraph/include/TextStyle.h"

#include <iostream>

namespace skia {
namespace textlayout {

const SkTArray<SkString> TextStyle::kDefaultFontFamilies = { SkString(DEFAULT_FONT_FAMILY) };

TextStyle::TextStyle() = default;

TextStyle::~TextStyle() {}

TextStyle& TextStyle::operator=(const TextStyle& rhs) = default;

TextStyle::TextStyle(const TextStyle& orig) = default;

TextStyle::TextStyle(TextStyle&& orig) = default;

TextStyle::TextStyle(const TextStyle& other, bool placeholder) {
    fColor = other.fColor;
    fFontSize = other.fFontSize;
    fFontFamilies = other.fFontFamilies;
    fDecoration = other.fDecoration;
    fHasBackground = other.fHasBackground;
    fHasForeground = other.fHasForeground;
    fBackground = other.fBackground;
    fForeground = other.fForeground;
    fHeightOverride = other.fHeightOverride;
    fIsPlaceholder = placeholder;
    fFontFeatures = other.fFontFeatures;
    fHalfLeading = other.fHalfLeading;
}

bool TextStyle::equals(const TextStyle& other) const {

    if (fIsPlaceholder || other.fIsPlaceholder) {
        return false;
    }

    if (fColor != other.fColor) {
        return false;
    }
    if (!(fDecoration == other.fDecoration)) {
        return false;
    }
    if (!(fFontStyle == other.fFontStyle)) {
        return false;
    }
    if (fFontFamilies != other.fFontFamilies) {
        return false;
    }
    if (fLetterSpacing != other.fLetterSpacing) {
        return false;
    }
    if (fWordSpacing != other.fWordSpacing) {
        return false;
    }
    if (fHeight != other.fHeight) {
        return false;
    }
    if (fHalfLeading != other.fHalfLeading) {
        return false;
    }
    if (fFontSize != other.fFontSize) {
        return false;
    }
    if (fLocale != other.fLocale) {
        return false;
    }
    if (fHasForeground != other.fHasForeground || fForeground != other.fForeground) {
        return false;
    }
    if (fHasBackground != other.fHasBackground || fBackground != other.fBackground) {
        return false;
    }
    if (fTextShadows.size() != other.fTextShadows.size()) {
        return false;
    }
    for (size_t i = 0; i < fTextShadows.size(); ++i) {
        if (fTextShadows[i] != other.fTextShadows[i]) {
            return false;
        }
    }
    if (fFontFeatures.size() != other.fFontFeatures.size()) {
        return false;
    }
    for (size_t i = 0; i < fFontFeatures.size(); ++i) {
        if (!(fFontFeatures[i] == other.fFontFeatures[i])) {
            return false;
        }
    }

    return true;
}

bool TextStyle::equalsByFonts(const TextStyle& that) const {

    return !fIsPlaceholder && !that.fIsPlaceholder &&
           fFontStyle == that.fFontStyle &&
           fFontFamilies == that.fFontFamilies &&
           fFontFeatures == that.fFontFeatures &&
           nearlyEqual(fLetterSpacing, that.fLetterSpacing) &&
           nearlyEqual(fWordSpacing, that.fWordSpacing) &&
           nearlyEqual(fHeight, that.fHeight) &&
           nearlyEqual(fFontSize, that.fFontSize) &&
           fLocale == that.fLocale;
}

bool TextStyle::matchOneAttribute(StyleType styleType, const TextStyle& other) const {
    switch (styleType) {
        case kForeground:
            return (!fHasForeground && !other.fHasForeground && fColor == other.fColor) ||
                   ( fHasForeground &&  other.fHasForeground && fForeground == other.fForeground);

        case kBackground:
            return (!fHasBackground && !other.fHasBackground) ||
                   ( fHasBackground &&  other.fHasBackground && fBackground == other.fBackground);

        case kShadow:
            if (fTextShadows.size() != other.fTextShadows.size()) {
                return false;
            }

            for (int32_t i = 0; i < SkToInt(fTextShadows.size()); ++i) {
                if (fTextShadows[i] != other.fTextShadows[i]) {
                    return false;
                }
            }
            return true;

        case kDecorations:
            return this->fDecoration == other.fDecoration;

        case kLetterSpacing:
            return fLetterSpacing == other.fLetterSpacing;

        case kWordSpacing:
            return fWordSpacing == other.fWordSpacing;

        case kAllAttributes:
            return this->equals(other);

        case kFont:
            // TODO: should not we take typefaces in account?
            return fFontStyle == other.fFontStyle &&
                   fLocale == other.fLocale &&
                   fFontFamilies == other.fFontFamilies &&
                   fFontSize == other.fFontSize &&
                   fHeight == other.fHeight &&
                   fHalfLeading == other.fHalfLeading;
        default:
            SkASSERT(false);
            return false;
    }
}

SkColor TextStyle::getColor() const { return fColor; }

void TextStyle::setColor(SkColor color) { fColor = color; }

bool TextStyle::hasForeground() const { return fHasForeground; }

SkPaint TextStyle::getForeground() const { return fForeground; }

void TextStyle::setForegroundColor(SkPaint paint) {
    fHasForeground = true;
    fForeground = std::move(paint);
}

void TextStyle::clearForegroundColor() { fHasForeground = false; }

bool TextStyle::hasBackground() const { return fHasBackground; }

SkPaint TextStyle::getBackground() const { return fBackground; }

void TextStyle::setBackgroundColor(SkPaint paint) {
    fHasBackground = true;
    fBackground = std::move(paint);
}

void TextStyle::clearBackgroundColor() { fHasBackground = false; }

void TextStyle::getFontMetrics(SkFontMetrics* metrics) const {
    SkFont font(fTypeface, fFontSize);
    font.setEdging(SkFont::Edging::kAntiAlias);
    font.setSubpixel(true);
    font.setHinting(SkFontHinting::kSlight);
    font.getMetrics(metrics);
    if (fHeightOverride) {
        auto multiplier = fHeight * fFontSize;
        auto height = metrics->fDescent - metrics->fAscent + metrics->fLeading;
        metrics->fAscent = (metrics->fAscent - metrics->fLeading / 2) * multiplier / height;
        metrics->fDescent = (metrics->fDescent + metrics->fLeading / 2) * multiplier / height;

    } else {
        metrics->fAscent = (metrics->fAscent - metrics->fLeading / 2);
        metrics->fDescent = (metrics->fDescent + metrics->fLeading / 2);
    }
}

bool PlaceholderStyle::equals(const PlaceholderStyle& other) const {
    return nearlyEqual(fWidth, other.fWidth) &&
           nearlyEqual(fHeight, other.fHeight) &&
           fAlignment == other.fAlignment &&
           fBaseline == other.fBaseline &&
           (fAlignment != PlaceholderAlignment::kBaseline ||
            nearlyEqual(fBaselineOffset, other.fBaselineOffset));
}

int TextStyle::sizeofTextStyle()
{
    return sizeof(TextStyle);
}

int TextStyle::sizeofVectorSkString()
{
    return sizeof(std::vector<SkString>);
}

int TextStyle::sizeofVectorInt()
{
    return sizeof(std::vector<int>);
}

}  // namespace textlayout
}  // namespace skia
