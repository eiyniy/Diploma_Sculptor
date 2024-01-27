#include <future>
#include <thread>
#include <memory>
#include <Scene.hpp>
#include <Converter.hpp>
#include <Timer.hpp>
#include <Math.hpp>

Scene::Scene(
        Camera &_camera,
        BaseLightSource *_lightSource,
        const double _moveSpeed,
        const double _rotationSpeed)
        : camera(_camera),
          lightSource(_lightSource),
          defaultFrameTime(1000.f / 60),
          moveSpeed(_moveSpeed),
          rotationSpeed(_rotationSpeed) {
    // generateFloor(25, 20, Point(0, 0));
}

Scene::~Scene() {
    for (const auto &pair: objects)
        delete pair.second;
}

/*
void Scene::generateFloor() {
    double maxDimensionSize = 0;
    auto center = Point(0, 0);

    for (auto pair: objects) {
        if (pair.first == floorObjectName)
            continue;

        auto maxXZ = pair.second->getMaxXZ();
        auto minXZ = pair.second->getMinXZ();

        auto floorSize = maxXZ - minXZ;

        auto biggerDimensionSize = std::max(floorSize.cGetX(), floorSize.cGetZ());

        if (biggerDimensionSize > maxDimensionSize) {
            maxDimensionSize = biggerDimensionSize;

            center = {(int) pair.second->getCenter().cGetX(),
                      (int) pair.second->getCenter().cGetZ()};
        }
    }

    maxDimensionSize *= 3;

    auto step = maxDimensionSize / floorStepsCount;
    generateFloor(floorStepsCount, ceil(step), center);
}
*/

/*
void Scene::generateFloor(const int size, const int step, const Point &center) {
    auto color = sf::Color(255U, 255U, 255U, 64U);

    std::vector<Matrix<4, 1>> vertices;
    std::vector<Triangle> polygons;

    const auto evenSize = size % 2 == 0 ? size + 1 : size;
    const auto halfSize = evenSize / 2;

    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j)
            vertices.emplace_back(j * step + center.cGetX(), 0, i * step + center.cGetY());
    }

    for (int i = 0; i < evenSize - 1; ++i) {
        for (int j = 0; j < evenSize - 1; ++j) {
            auto vertexesIndexes = std::vector<VertexIds>{
                    VertexIds(j * evenSize + i + 1),
                    VertexIds(j * evenSize + i + 2),
                    VertexIds((j + 1) * evenSize + i + 2),
                    VertexIds((j + 1) * evenSize + i + 1)};

            polygons.emplace_back(vertexesIndexes);
        }
    }

    // auto floorPt = new Object(vertices, {}, {}, polygons, color);

    // objects.insert_or_assign(floorObjectName, floorPt);
}
*/

void Scene::addObject(const std::string &key, Object *object) {
    if (key == floorObjectName)
        throw std::invalid_argument("This object name is reserved!");

    selectedObjectName = key;

    objects[key] = object;

    // generateFloor();
}

std::vector<std::string> Scene::cGetAllObjectNames() const {
    auto res = std::vector<std::string>();

    for (const auto &pair: objects)
        res.emplace_back(pair.first);

    return res;
}

const std::string &Scene::cGetSelectedObjectName() const {
    return selectedObjectName;
}

const BaseLightSource *Scene::cGetLightSource() const {
    return lightSource;
}
