#include "scene.h"

#include <stack>

#include "components/cameracomponent.h"
#include "components/meshcomponent.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "utils/logger.h"

void Scene::populateRenderer(Renderer &renderer) {
    std::stack<std::pair<std::shared_ptr<Node>, Transformation>> stack;
    stack.push({root_, Transformation()});

    while (!stack.empty()) {
        auto [currentNode, transform] = stack.top();
        stack.pop();

        if (!currentNode) {
            continue;
        }

        Transformation currentTransform = transform * currentNode->getTransformation();
        auto meshComponent = currentNode->getComponent<MeshComponent>();

        if (meshComponent) {
            renderer.addShape(meshComponent->getShape(), currentTransform);
        }

        for (auto child : currentNode->getChildren()) {
            stack.push({child, currentTransform});
        }
    }
}

void Scene::start() {
    cameras_.clear();
    std::stack<std::shared_ptr<Node>> stack;
    if (root_) {
        stack.push(root_);
    }

    while (!stack.empty()) {
        auto node = stack.top();
        stack.pop();

        auto cameraComp = node->getComponent<CameraComponent>();
        if (cameraComp) {
            cameras_.push_back(cameraComp);
        }

        for (auto &comp : node->getComponents()) {
            comp->onStart();
        }

        for (auto &child : node->getChildren()) {
            stack.push(child);
        }
    }
}

void Scene::update(float dt) {
    physicsEngine_.stepSimulation(dt);

    std::stack<std::shared_ptr<Node>> stack;
    if (root_) {
        stack.push(root_);
    }

    while (!stack.empty()) {
        auto node = stack.top();
        stack.pop();

        for (auto &comp : node->getComponents()) {
            comp->onPreUpdate();
            comp->onUpdate(dt);
            comp->onPostUpdate();
        }

        for (auto &child : node->getChildren()) {
            stack.push(child);
        }
    }
}

void Scene::end() {
    std::stack<std::shared_ptr<Node>> stack;
    if (root_) {
        stack.push(root_);
    }

    while (!stack.empty()) {
        auto node = stack.top();
        stack.pop();

        for (auto &comp : node->getComponents()) {
            comp->onEnd();
        }

        for (auto &child : node->getChildren()) {
            stack.push(child);
        }
    }
}

std::shared_ptr<Camera> Scene::getActiveCamera() {
    if (cameras_.empty()) {
        return nullptr;
    }

    return cameras_[0]->getCamera();
}
