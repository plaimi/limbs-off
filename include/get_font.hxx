/*
 * Copyright (C) 2012 Alexander Berntsen <alexander@plaimi.net>
 *
 * This file is part of Limbs Off.
 *
 * Limbs Off is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Limbs Off is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Limbs Off.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GET_FONT_HXX_
#define GET_FONT_HXX_ 

#include <fontconfig/fontconfig.h>
#include <string.h>

void getFont(char* result, size_t size,
        const char* fontFamily = "Anonymous Pro",
        const char* fontStyle = "Bold", const char* fontSpacing = "Monospace");

inline void getFont(char* result, size_t size, const char* fontFamily,
        const char* fontStyle, const char* fontSpacing) {
    char* font;
    FcPattern* fontPattern = FcPatternCreate();
    FcResult fontResult = FcResultMatch;
    FcPatternAddString(fontPattern, FC_FAMILY, 
            (const FcChar8*) fontFamily);
    FcPatternAddDouble(fontPattern, FC_SIZE, 74);
    FcPatternAddString(fontPattern, FC_SPACING, 
            (const FcChar8*) fontSpacing);
    FcPatternAddString(fontPattern, FC_STYLE, 
            (const FcChar8*) fontStyle);
    FcDefaultSubstitute(fontPattern);
    FcPattern* fontMatch = FcFontMatch(NULL, fontPattern, &fontResult);
    FcPatternGetString(fontMatch, FC_FILE, 0, (FcChar8**) &font);
    strncpy(result, font, size);
    FcPatternDestroy(fontMatch);
    FcPatternDestroy(fontPattern);
}

#endif /* GET_FONT_HXX_ */
