#include <iostream>

#include "window/window.h" 

int main() {
    Window window;
    
    if (window.initialize()) {
        window.loop();
        window.finish();
    
    } else {
        std::cerr << "Failed to initialize aplication window!" << '\n';
    }

    return 0;
}
