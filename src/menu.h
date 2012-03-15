#ifndef MENU_H_
#define MENU_H_

#include <vector>
#include "event_handler.h"

class GuiElement {
public:
    bool isSelected();
    int getPosition();
    void setPosition(int pos);
    void setSelected(bool selected);
protected:
    bool selected_;
    int position_;
};

class Submenu {
public:
    Submenu();
    void addButton(const char* text, int position, bool selected);
    /** Buttons are hardcoded. */
    std::vector<GuiElement*> buttons;
};

class Menu: public EventHandler {
public:
    Menu();
    enum {
        MAINMENU, NUM_MENU
    };
    bool handle(const SDL_Event &event);
    GuiElement* getGuiElement(std::vector<GuiElement*> vector, int position);
    GuiElement* getSelected();
    int getActiveMenu();
    Submenu* getMenu(int menu);
    void setSelected(GuiElement* selected);
private:
    GuiElement* activeElement_;
    int activeMenu_;
    /** Menus are hardcoded. */
    Submenu* menus_[NUM_MENU];
};

class Button: public GuiElement {
public:
    Button(const char* text, int position, bool selected = false);
    ~Button();
    char* getText();
    int getPosition();
private:
    bool selected_;
    char* text_;
    int position_;
};

#endif /* MENU_H_ */
