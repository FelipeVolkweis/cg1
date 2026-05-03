#include <string>

#include "core/node.h"
#include "scene/scene.h"
#include "scene/sceneparser.h"
#include "utils/logger.h"
#include "window/window.h"

int main() {
    Window window;
    auto root = std::make_shared<Node>();
    auto scene = std::make_shared<Scene>(root);

    std::string sceneFile = "data/scenes/main.yaml";

    if (!window.initialize()) {
        Logger::Error("Failed to initialize application window!");
        return -1;
    }

    if (!SceneParser::load(sceneFile, root, scene->getPhysicsEngine(), window.getInputHandler())) {
        Logger::Warn("Could not load scene from ", sceneFile);
    }

    window.loop(std::move(scene));
    window.finish();

    return 0;
}
