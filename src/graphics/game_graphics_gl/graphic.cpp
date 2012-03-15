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

#include <math.h>
#include <fontconfig/fontconfig.h>
#include "graphics/game_graphics_gl.h"
#include "geometry.h"

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
    double w = s->getGlWidth(), h = s->getGlHeight();
    double r = sqrt(w * w + h * h);
    glPushMatrix();
    glScaled(r, r, 1.0);
    glRotated(camera_->getRotation(), 0.0, 0.0, -1.0);
}

void BackgroundModifier::end() {
    glPopMatrix();
}

PositionModifier::PositionModifier(int position, int num) :
    position_(position),
    num_(num) {
}

void PositionModifier::begin() {
    glPushMatrix();
    double f = 2. / num_ * ((1. + num_) / 2. - position_);
    glTranslated(.0, f, .0);
}

void PositionModifier::end() {
    glPopMatrix();
}

void StackGraphic::addGraphic(Graphic* g) {
    graphics_.push_back(g);
}

void StackGraphic::doDraw() {
    // Draw all the graphics in the stack in the correct order.
    if (Screen::getInstance()->getDrawingMode() & Screen::DM_FRONT_TO_BACK) {
        std::vector<Graphic*>::reverse_iterator i;
        for (i = graphics_.rbegin(); i < graphics_.rend(); i++)
            (*i)->draw();
    } else {
        std::vector<Graphic*>::iterator i;
        for (i = graphics_.begin(); i < graphics_.end(); i++)
            (*i)->draw();
    }
}

Sprite::Sprite(GLuint texture, GLfloat w, GLfloat h) :
    texture_(texture), w_(w), h_(h) {
}

void Sprite::doDraw() {
    Screen::getInstance()->setDrawingMode(-1, Screen::DM_PREMUL);
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

double GuiGraphic::getHeight() {
    return height_;
}

double GuiGraphic::getWidth() {
    return width_;
}

GuiElement* GuiGraphic::getLogic() {
    return logic_;
}

void GuiGraphic::setSize(double w, double h) {
    width_ = w;
    height_ = h;
}

Label::Label(const char* face, const char* text, int size, double width, 
        double height) :
    size_(size),
    width_(width),
    height_(height) {
        face_ = (char*) malloc(strlen(face) + 1);
        strcpy(face_, face);
        text_ = (char*) malloc(strlen(text) + 1);
        strcpy(text_, text);
        make();
}

Label::~Label() {
    free(face_);
    free(text_);
}

char* Label::getText() {
    return text_;
}

double Label::getHeight() {
    return height_;
}

double Label::getWidth() {
    return width_;
}

void Label::doDraw() {
    Screen::getInstance()->setDrawingMode(0, Screen::DM_PREMUL);
    glColor3d(.0, .0, .0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glBegin(GL_QUADS);
    glTexCoord2d(.0, 1.);
    glVertex2d(-width_, -height_);
    glTexCoord2d(1., 1.);
    glVertex2d(width_, -height_);
    glTexCoord2d(1., .0);
    glVertex2d(width_, height_);
    glTexCoord2d(.0, .0);
    glVertex2d(-width_, height_);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3d(1.0, 1.0, 1.0);
}

void Label::make() {
    TTF_Init();
    font_ = TTF_OpenFont(face_, size_);
    if (!font_)
        printf("error opening font: %s\n", TTF_GetError());
    // Doesn't matter
    SDL_Color bg = {0, 0, 0};
    surface_ = TTF_RenderText_Shaded(font_, text_, bg, bg);
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, surface_->w, surface_->h, 0, 
            GL_ALPHA, GL_UNSIGNED_BYTE, surface_->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Label::setFace(const char* face) {
    free(face_);
    face_ = (char*) malloc(strlen(face) + 1);
    strcpy(face_, face);
    make();
}

void Label::setSize(int size) {
    size_ = size;
    make();
}

void Label::setText(const char* text) {
    free(text_);
    text_ = (char*) malloc(strlen(text) + 1);
    strcpy(text_, text);
    make();
}

Disk::Disk(GLfloat r, int n) :
    r_(r), n_(n), displayList_(0) {
}

void Disk::doDraw() {
    Screen::getInstance()->setDrawingMode(0, Screen::DM_PREMUL);
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
    disk_(r, n), square_(r, 4) {
    addGraphic(&disk_);
    addGraphic(&square_);
}

Disk* TestDisk::getDisk() {
    return &disk_;
}

Disk* TestDisk::getSquare() {
    return &square_;
}

ButtonGraphic::ButtonGraphic(double width, double height, GuiElement* logic, 
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
    if (logic_->isSelected())
        glColor4f(0.f, 1.f, 0.f, 1.f);
    else
        glColor4f(1.f, 0.f, 0.f, 1.f);
    glBegin(GL_QUADS);
    glVertex2d(width_, -height_);
    glVertex2d(-width_, -height_);
    glVertex2d(-width_, height_);
    glVertex2d(width_, height_);
    glEnd();
    glColor4f(1.f, 1.f, 1.f, 1.f);
}

SubmenuGraphic::SubmenuGraphic(Submenu* submenu) :
    submenu_(submenu) {
}

SubmenuGraphic::~SubmenuGraphic() {
    for (std::vector<ButtonGraphic*>::iterator i = buttonGraphics_.begin(); 
            i < buttonGraphics_.end(); delete (*i), ++i);
    for (std::vector<PositionModifier*>::iterator i = 
            positionModifiers_.begin(); i < positionModifiers_.end(); 
            delete (*i), ++i);
}

void SubmenuGraphic::addButton(GuiElement* logic, Label* label, bool selected) {
    // Create a ButtonGraphic
    buttonGraphics_.push_back(new ButtonGraphic(label->getWidth(), 
                label->getHeight(), logic, label, selected));
    // Create a PositionModifier
    positionModifiers_.push_back(new PositionModifier(
                buttonGraphics_.back()->getLogic()->getPosition(), 
                submenu_->buttons.size()));
    // Add the ButtonGraphic to graphics_
    addGraphic(buttonGraphics_.back());
    // Pair up the ButtonGraphic with the PositionModifier
    buttonGraphics_.back()->addModifier(positionModifiers_.back());
}

MenuGraphic::MenuGraphic(Menu* menu) :
    menu_(menu) {
        const char* fontFamily = "Anonymous Pro";
        const char* fontStyle = "Bold";
        const char* fontSpacing = "Monospace";
        char* font;
        FcPattern* fontPattern = FcPatternCreate();
        FcResult fontResult = FcResultMatch;
        int fontSize = 74;
        FcPatternAddString(fontPattern, FC_FAMILY, 
                (const FcChar8*) fontFamily);
        FcPatternAddDouble(fontPattern, FC_SIZE, fontSize);
        FcPatternAddString(fontPattern, FC_SPACING, 
                (const FcChar8*) fontSpacing);
        FcPatternAddString(fontPattern, FC_STYLE, 
                (const FcChar8*) fontStyle);
        FcDefaultSubstitute(fontPattern);
        FcPattern* fontMatch = FcFontMatch(NULL, fontPattern, &fontResult);
        FcPatternGetString(fontMatch, FC_FILE, 0, (FcChar8**) &font);
        for (int i = 0; i < Menu::NUM_MENU; ++i) {
            // Create a new SubmenuGraphic for each Submenu
            menuGraphics_[i] = new SubmenuGraphic(menu->getMenu(i));
            Submenu* submenu = menu->getMenu(i);
            // Add buttons and labels
            for (std::vector<GuiElement*>::iterator j = 
                    submenu->buttons.begin(); j < submenu->buttons.end(); ++j) {
                labels_.push_back(new Label(font, ((Button*) (*j))->getText(), 
                            fontSize, .5, .1));
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
