#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H

#include <string>

class BaseShape;

class ModelImporter {
public:
    static void loadObjInto(const std::string &filePath, BaseShape *shape);

private:
};

#endif