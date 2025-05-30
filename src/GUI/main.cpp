// baruh.ifraimov@gmail.com
#include "GUI/Window.hpp"

int main() {
    coup::Game game;
    Window gameWindow(game);
    gameWindow.run();
    return 0;
}
