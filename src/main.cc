#include <string>

#include "scene/scene.h"
#include "utils/logger.h"
#include "window/window.h"

int main() {
    Window window;
    auto scene = std::make_unique<Scene>(std::make_shared<Node>());

    std::string sceneFile = "data/scenes/main.txt";

    if (!scene->load(sceneFile)) {
        Logger::Warn("Could not load scene from ", sceneFile);
    }

    if (window.initialize()) {
        window.loop(std::move(scene));
        window.finish();
    } else {
        Logger::Error("Failed to initialize application window!");
    }

    return 0;
}
