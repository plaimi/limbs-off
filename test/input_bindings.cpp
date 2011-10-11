#include <string>
#include "../src/input_handler.h"
#include "../src/init.h"

int main(int argc, char const *argv[]) {
    InputHandler inputHandler;
    const char* filename = "src/controllers.conf";
    Init::readBindings(&inputHandler, filename);
    for (int i = 0; i < SDLK_LAST; ++i) {
        if (InputHandler::getHandler((SDLKey)i))
            printf("\n%d\n", i);
    }

    return 0;
}
