/*
 * Copyright (C) 2011 Stian Ellingsen <stian@plaimi.net>
 * Copyright (C) 2012, 2013 Alexander Berntsen <alexander@plaimi.net>
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
#include "graphics/game_graphics_gl.hxx"

const GLint FORMAT[] = { GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };

TextureLoader* TextureLoader::_instance_ = 0;

TextureLoader* TextureLoader::getInstance() {
    if (!_instance_ && Screen::getInstance())
        _instance_ = new TextureLoader();
    return _instance_;
}

TextureLoader::TextureLoader() :
        loaded_() {
}

GLuint TextureLoader::getTexture(const char* filename) {
    std::map<std::string, GLuint>::iterator it = loaded_.find(filename);
    if (it != loaded_.end())
        return it->second;
    GLuint tex = loadTexture(filename, true);
    loaded_.insert(std::pair<std::string, GLuint>(filename, tex));
    return tex;
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
    int d, t, nc;
    png_get_IHDR(rsp, isp, &w, &h, &d, &t, 0, 0, 0);
    bool trns = png_get_valid(rsp, isp, PNG_INFO_tRNS);
    switch (t) {
    case PNG_COLOR_TYPE_GRAY:
        if (d < 8)
            png_set_expand_gray_1_2_4_to_8(rsp);
        if (!trns) {
            nc = 1;
            break;
        } // Fall through.
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        nc = 2;
        break;
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(rsp); // Fall through.
    case PNG_COLOR_TYPE_RGB:
        if (!trns) {
            nc = 3;
            break;
        } // Fall through.
    case PNG_COLOR_TYPE_RGBA:
        nc = 4;
        break;
    }
    GLint ct = FORMAT[nc - 1];
    if (trns)
        png_set_tRNS_to_alpha(rsp);
    GLint dt = d <= 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT;
    if (d == 16)
        png_set_swap(rsp);
    int rb = w * nc * (d <= 8 ? 1 : 2);
    png_bytep data = (png_bytep) png_malloc(rsp, h * rb);
    png_bytep* rp = (png_bytep*) png_malloc(rsp, h * sizeof(png_bytep));
    for (png_uint_32 i = 0; i < h; i++)
        rp[i] = data + i * rb;
    png_read_image(rsp, rp);
    png_read_end(rsp, 0);
    fclose(fp);
    if (premultiply && (nc & 1) == 0) {
        int a = nc - 1, l = h * w * nc;
        if (d == 16) {
            png_uint_16p data16 = (png_uint_16p) data;
            for (int i = 0; i < l; i += nc) {
                for (int j = 0; j < a; j++)
                    data16[i + j] = ((png_uint_32) data16[i + j]
                            * (png_uint_32) data16[i + a] + 32767) / 65535;
            }
        } else {
            for (int i = 0; i < l; i += nc) {
                for (int j = 0; j < a; j++)
                    data[i + j] = ((png_uint_16) data[i + j]
                            * (png_uint_16) data[i + a] + 127) / 255;
            }
        }
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    GLint tw = (nc & 1) == 0 ? GL_CLAMP_TO_BORDER : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tw);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tw);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, ct, w, h, 0, ct, dt, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    png_free(rsp, data);
    png_destroy_read_struct(&rsp, &isp, 0);
    return texture;
}
