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

#include <algorithm>
#include <math.h>
#include "get_font.hxx"
#include "graphics/game_graphics_gl.hxx"
#include "geometry.hxx"

GraphicFixture::GraphicFixture(Body* body) :
        body_(body) {
}

void GraphicFixture::begin() {
    phys_t x, y;
    body_->getPosition(x, y);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(body_->getOrientation() * IN_DEG, 0.0, 0.0, 1.0);
}

void GraphicFixture::end() {
    glPopMatrix();
}

ColorModifier::ColorModifier(const float* color) :
        color_(color) {
}

void ColorModifier::begin() {
    glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
    glColor3fv(color_);
}

void ColorModifier::end() {
    glPopAttrib();
}

BackgroundModifier::BackgroundModifier(Camera* camera) :
        camera_(camera) {
}

void BackgroundModifier::begin() {
    Screen* s = Screen::getInstance();
    GLfloat w = s->getGlWidth(), h = s->getGlHeight();
    GLfloat r = sqrt(w * w + h * h);
    glPushMatrix();
    glScalef(r, r, 1.0);
    glRotatef(camera_->getRotation(), 0.0, 0.0, -1.0);
}

void BackgroundModifier::end() {
    glPopMatrix();
}

PositionModifier::PositionModifier(int position, int num, bool horizontalP,
        float offset) :
        position_(position),
        num_(num),
        horizontalP_(horizontalP),
        offset_(offset) {
}

void PositionModifier::begin() {
    glPushMatrix();
    // Evenly distribute the elements horizontally or vertically
    GLfloat f = 2.0 / num_ * ((1.0 + num_) / 2.0 - position_);
    if (horizontalP_)
        glTranslatef(-f, offset_, 0.0);
    else
        glTranslatef(offset_, f, 0.0);
}

void PositionModifier::end() {
    glPopMatrix();
}

SizeModifier::SizeModifier(Shape<phys_t>* shape) :
        shape_((Circle<phys_t>*) shape)  {
}

void SizeModifier::begin() {
    glPushMatrix();
    GLfloat f = shape_->getRadius();
    glScalef(f, f, 1.0);
}

void SizeModifier::end() {
    glPopMatrix();
}

StackGraphic::StackGraphic() :
        graphics_() {
}

void StackGraphic::addGraphic(Graphic* graphic, std::size_t index) {
    if (index == -1)
        graphics_.push_back(graphic);
    else
        graphics_.insert(graphics_.begin() + index, graphic);
}

std::size_t StackGraphic::removeGraphic(Graphic* g) {
    std::vector<Graphic*>::iterator it=std::find(graphics_.begin(),
            graphics_.end(), g);
    std::size_t i=std::distance(graphics_.begin(), it);
    if (it != graphics_.end())
        graphics_.erase(it);
    return i;
}

void StackGraphic::doDraw() {
    // Draw all the graphics in the stack in the correct order.
    if (Screen::getInstance()->getDrawingMode() & Screen::_DM_FRONT_TO_BACK) {
        std::vector<Graphic*>::reverse_iterator i;
        for (i = graphics_.rbegin(); i < graphics_.rend(); ++i)
            (*i)->draw();
    } else {
        std::vector<Graphic*>::iterator i;
        for (i = graphics_.begin(); i < graphics_.end(); ++i)
            (*i)->draw();
    }
}

Sprite::Sprite(GLuint texture, GLfloat w, GLfloat h) :
        texture_(texture),
        w_(w),
        h_(h) {
}

void Sprite::doDraw() {
    Screen::getInstance()->setDrawingMode(-1, Screen::_DM_PREMUL);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(-w_, -h_);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(w_, -h_);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(w_, h_);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(-w_, h_);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

ScreenGraphic::ScreenGraphic() :
        width_(0),
        height_(0),
        logic_(NULL) {
}

GLfloat ScreenGraphic::getHeight() {
    return height_;
}

GLfloat ScreenGraphic::getWidth() {
    return width_;
}

ScreenElement* ScreenGraphic::getLogic() {
    return logic_;
}

void ScreenGraphic::setSize(GLfloat w, GLfloat h) {
    width_ = w;
    height_ = h;
}

Label::Label(const char* face, const char* text, int size, GLfloat width,
        GLfloat height) :
        face_(NULL),
        size_(size),
        width_(width),
        height_(height),
        font_(NULL),
        text_(NULL),
        texture_(0) {
    TTF_Init();
    if (!TTF_WasInit() && TTF_Init() == -1) {
        printf("error initialising fontconfig: %s\n", TTF_GetError());
        exit(1);
    }
    text_ = (char*) malloc(strlen(text) + 1);
    strcpy(text_, text);
    setFace(face);
}

Label::~Label() {
    free(face_);
    free(text_);
}

char* Label::getText() {
    return text_;
}

GLfloat Label::getHeight() {
    return height_;
}

GLfloat Label::getWidth() {
    return width_;
}

void Label::doDraw() {
    Screen::getInstance()->setDrawingMode(0, Screen::_DM_PREMUL);
    glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBegin(GL_QUADS);
    glTexCoord2f(.0, 1.);
    glVertex2f(-width_, -height_);
    glTexCoord2f(1., 1.);
    glVertex2f(width_, -height_);
    glTexCoord2f(1., .0);
    glVertex2f(width_, height_);
    glTexCoord2f(.0, .0);
    glVertex2f(-width_, height_);
    glEnd();
    drawShadow();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopAttrib();
}

void Label::drawShadow() {
    // TODO: Implement shadow properly...
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(.0, 1.);
    glVertex2f(-width_ + width_ / 50.0, -height_ - height_ / 10.0);
    glTexCoord2f(1., 1.);
    glVertex2f(width_ + width_ / 50.0, -height_ - height_ / 10.0);
    glTexCoord2f(1., .0);
    glVertex2f(width_ + width_ / 50.0, height_ - height_ / 10.0);
    glTexCoord2f(.0, .0);
    glVertex2f(-width_ + width_ / 50.0, height_ - height_ / 10.0);
    glEnd();
}

void Label::make() {
    if (font_ == NULL)
        font_ = TTF_OpenFont(face_, size_);
    if (font_ == NULL)
        printf("error opening font: %s\n", TTF_GetError());
    // Doesn't matter
    SDL_Color bg = {0, 0, 0};
    SDL_Surface* surface = TTF_RenderText_Shaded(font_, text_, bg, bg);
    if (texture_)
        glDeleteTextures(1, &texture_);
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, surface->w, surface->h, 0,
            GL_ALPHA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(surface);
}

void Label::setFace(const char* face) {
    if (face_ != NULL)
        free(face_);
    face_ = (char*) malloc(strlen(face) + 1);
    strcpy(face_, face);
    TTF_CloseFont(font_);
    font_ == NULL;
    make();
}

void Label::setSize(int size) {
    size_ = size;
    make();
}

void Label::setText(const char* text) {
    if (strcmp(text_, text) == 0)
            return;
    if (text_ != NULL)
        free(text_);
    text_ = (char*) malloc(strlen(text) + 1);
    strcpy(text_, text);
    make();
}

Disk::Disk(GLfloat r, int n) :
        r_(r),
        n_(n),
        displayList_(0) {
}

void Disk::doDraw() {
    Screen::getInstance()->setDrawingMode(0, Screen::_DM_PREMUL);
    if (displayList_ == 0)
        makeDisplayList();
    else
        glCallList(displayList_);
}

void Disk::makeDisplayList() {
    float t = 2 * PI / n_;
    float c = cosf(t), s = sinf(t), x = 0.0, y = r_;
    displayList_ = glGenLists(1);
    glNewList(displayList_, GL_COMPILE_AND_EXECUTE);
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex2f(x, y);
    for (int i = 1; i < n_; i++) {
        if (i & 1) {
            float z = x;
            x = c * x - s * y;
            y = s * z + c * y;
        }
        glVertex2f(i & 1 ? x : -x, y);
    }
    glEnd();
    glEndList();
}

TestDisk::TestDisk(GLfloat r, int n) :
        disk_(r, n),
        square_(r, 4) {
    addGraphic(&disk_);
    addGraphic(&square_);
}

Disk* TestDisk::getDisk() {
    return &disk_;
}

Disk* TestDisk::getSquare() {
    return &square_;
}

MassIndicatorGraphic::MassIndicatorGraphic(GLfloat width, GLfloat height,
        MassIndicator* logic, Label* label) :
        label(label) {
    setSize(width, height);
    logic_ = logic;
}

void MassIndicatorGraphic::doDraw() {
    // This draws front to back
    if (label) {
        label->draw();
    }
    glBegin(GL_QUADS);
    glVertex2f(width_, -height_);
    glVertex2f(-width_, -height_);
    glVertex2f(-width_, height_);
    glVertex2f(width_, height_);
    glEnd();
}

ButtonGraphic::ButtonGraphic(GLfloat width, GLfloat height, Button* logic,
        Label* label, bool selected) :
        label(label) {
    setSize(width, height);
    logic_ = logic;
}

ButtonGraphic::~ButtonGraphic() {
}

void ButtonGraphic::doDraw() {
    // This draws front to back
    if (label) {
        label->draw();
    }
    glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
    if (((Button*)logic_)->isSelected())
        glColor4f(0.0, 1.0, 0.0, 1.0);
    else
        glColor4f(0.25, 0.0, 0.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(width_, -height_);
    glVertex2f(-width_, -height_);
    glVertex2f(-width_, height_);
    glVertex2f(width_, height_);
    glEnd();
    glPopAttrib();
}

SubmenuGraphic::SubmenuGraphic(Submenu* submenu) :
        submenu_(submenu),
        buttonGraphics_(),
        positionModifiers_() {
}

SubmenuGraphic::~SubmenuGraphic() {
    for (std::vector<ButtonGraphic*>::iterator i = buttonGraphics_.begin(); 
            i < buttonGraphics_.end(); delete (*i), ++i);
    for (std::vector<PositionModifier*>::iterator i = 
            positionModifiers_.begin(); i < positionModifiers_.end(); 
            delete (*i), ++i);
}

void SubmenuGraphic::addButton(Button* logic, Label* label,
        bool selected) {
    // Create a ButtonGraphic
    buttonGraphics_.push_back(new ButtonGraphic(label->getWidth(), 
                label->getHeight(), logic, label, selected));
    // Create a PositionModifier
    positionModifiers_.push_back(new PositionModifier(
                buttonGraphics_.back()->getLogic()->getPosition(), 
                submenu_->buttons.size(), false));
    // Add the ButtonGraphic to graphics_
    addGraphic(buttonGraphics_.back());
    // Pair up the ButtonGraphic with the PositionModifier
    buttonGraphics_.back()->addModifier(positionModifiers_.back());
}

MenuGraphic::MenuGraphic(Menu* menu) :
        menu_(menu),
        labels_() {
    for (int i = 0; i < Menu::NUM_MENU; ++i) {
        // Create a new SubmenuGraphic for each Submenu
        menuGraphics_[i] = new SubmenuGraphic(menu->getMenu(i));
        Submenu* submenu = menu->getMenu(i);
        // Add buttons and labels
        char font[256];
        getFont(font, sizeof(font));
        for (std::vector<Button*>::iterator j =
                submenu->buttons.begin(); j < submenu->buttons.end(); ++j) {
            labels_.push_back(new Label(font, ((Button*) (*j))->getText(),
                        74, .5, .1));
            menuGraphics_[i]->addButton((*j), labels_.back(),
                    (*j)->isSelected());
        }
    }
}

MenuGraphic::~MenuGraphic() {
    for (int i = 0; i < Menu::NUM_MENU; delete menuGraphics_[i++]);
    for (std::vector<Label*>::iterator i = 
            labels_.begin(); i < labels_.end(); delete (*i), ++i);
}

void MenuGraphic::doDraw() {
    // Draw the active menu
    menuGraphics_[menu_->getActiveMenu()]->draw();
}
