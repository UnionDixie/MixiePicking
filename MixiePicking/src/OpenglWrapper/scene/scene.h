#ifndef SCENE_H
#define SCENE_H

#include "../Object/Object.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;
public:
    void saveScene(const std::vector<Object>& data,const QString& filename);
    std::vector<Object>& loadScene(const QString& filename);
private:
    std::vector<Object> objects;
};

#endif // SCENE_H
