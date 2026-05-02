#include <string>

#include "components/meshcomponent.h"
#include "components/rigidbodycomponent.h"
#include "scene/scene.h"
#include "shapes/box.h"
#include "transformations/translation.h"
#include "utils/logger.h"
#include "window/window.h"

int main() {
    Window window;
    auto root = std::make_shared<Node>();
    auto scene = std::make_unique<Scene>(root);

    std::string sceneFile = "data/scenes/main.txt";

    if (!scene->load(sceneFile)) {
        Logger::Warn("Could not load scene from ", sceneFile);
    }

    // --- PHYSICS TEST ---
    // auto groundNode = std::make_shared<Node>();
    // Transformation groundTransform = Translation(Vec3(0, -2, 0));
    // groundNode->setTransform(groundTransform);
    // auto ground = std::make_unique<Box>(10.0f, 0.5f, 10.0f);
    // ground->setColor(Vec3(1, 0, 0));
    // groundNode->addComponent(std::make_shared<MeshComponent>(std::move(ground)));
    // groundNode->addComponent(std::make_shared<RigidBodyComponent>(
    //     scene->getPhysicsEngine(),
    //     0.0f, 
    //     std::make_unique<btBoxShape>(btVector3(5.0f, 0.25f, 5.0f))));
    // root->addChild(groundNode);

    // auto boxNode = std::make_shared<Node>();
    // Transformation boxTransform = Translation(Vec3(0, 5, 0));
    // boxNode->setTransform(boxTransform);
    // boxNode->addComponent(std::make_shared<MeshComponent>(std::make_unique<Box>(1.0f, 1.0f, 1.0f)));
    // boxNode->addComponent(std::make_shared<RigidBodyComponent>(
    //     scene->getPhysicsEngine(),
    //     1.0f, 
    //     std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f))));
    // root->addChild(boxNode);
    // --- END TEST ---

    if (window.initialize()) {
        window.loop(std::move(scene));
        window.finish();
    } else {
        Logger::Error("Failed to initialize application window!");
    }

    return 0;
}
