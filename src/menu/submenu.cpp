#include <GL/glu.h>
#include "menu.h"

Submenu::Submenu() {
}

void Submenu::addButton(const char* text, int position, bool selected) {
    buttons.push_back(new Button(text, position, selected));
}
