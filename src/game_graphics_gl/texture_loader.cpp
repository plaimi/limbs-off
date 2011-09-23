/*
 * Copyright (C) 2011 Stian Ellingsen <stiell@stiell.org>
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

#include <png.h>
#include "game_graphics_gl.h"

TextureLoader* TextureLoader::instance_ = 0;

TextureLoader* TextureLoader::getInstance() {
    if (!instance_ && Screen::getInstance())
        instance_ = new TextureLoader();
    return instance_;
}

TextureLoader::TextureLoader() {
}

GLuint TextureLoader::loadTexture(const char* filename, bool premultiply) {
    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return 0;
    png_structp rsp = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!rsp) {
        fclose(fp);
        return 0;
    }
    png_infop isp = png_create_info_struct(rsp);
    if (!isp) {
        fclose(fp);
        png_destroy_read_struct(&rsp, &isp, 0);
        return 0;
    }
    png_init_io(rsp, fp);
    png_read_info(rsp, isp);
    png_uint_32 w, h;
    int d, t;
    png_get_IHDR(rsp, isp, &w, &h, &d, &t, 0, 0, 0);
    bool s = png_get_valid(rsp, isp, PNG_INFO_sRGB) && d <= 8;
    bool trns = png_get_valid(rsp, isp, PNG_INFO_tRNS);
    GLint ct;
    int a = 0;
    switch (t) {
    case PNG_COLOR_TYPE_GRAY:
        if (d < 8)
            png_set_expand_gray_1_2_4_to_8(rsp);
        if (!trns) {
            ct = s ? GL_SLUMINANCE : GL_LUMINANCE;
            break;
        } // Fall through.
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        ct = s ? GL_SLUMINANCE_ALPHA : GL_LUMINANCE_ALPHA;
        a = 1;
        break;
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(rsp); // Fall through.
    case PNG_COLOR_TYPE_RGB:
        if (!trns) {
            ct = s ? GL_SRGB : GL_RGB;
            break;
        } // Fall through.
    case PNG_COLOR_TYPE_RGBA:
        ct = s ? GL_SRGB_ALPHA : GL_RGBA;
        a = 3;
        break;
    }
    if (trns)
        png_set_tRNS_to_alpha(rsp);
    GLint dt = d <= 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT;
    if (d == 16)
        png_set_swap(rsp);
    int rb = png_get_rowbytes(rsp, isp);
    png_bytep data = (png_bytep) png_malloc(rsp, h * rb);
    png_bytep* rp = (png_bytep*) png_malloc(rsp, h * sizeof(png_bytep));
    for (int i = 0; i < h; i++)
        rp[i] = data + i * rb;
    png_read_image(rsp, rp);
    png_read_end(rsp, 0);
    fclose(fp);
    if (premultiply && a > 0) {
        if (d == 16) {
            png_uint_16p data16 = (png_uint_16p) data;
            for (int i = 0; i < h * w * (a + 1); i += (a + 1)) {
                for (int j = 0; j < a; j++)
                    data16[i + j] = ((png_uint_32) data16[i + j]
                            * (png_uint_32) data16[i + a] + 32767) / 65535;
            }
        } else {
            for (int i = 0; i < h * w * (a + 1); i += (a + 1)) {
                for (int j = 0; j < a; j++)
                    data[i + j] = ((png_uint_16) data[i + j]
                            * (png_uint_16) data[i + a] + 127) / 255;
            }
        }
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, ct, w, h, 0, ct, dt, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    png_destroy_read_struct(&rsp, &isp, 0);
    png_free(rsp, data);
    return texture;
}
