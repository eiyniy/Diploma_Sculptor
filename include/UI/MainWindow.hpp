#pragma once

#include <cstring>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>
#include <mutex>
#include <memory>
#include <CSClipper.hpp>
#include <BaseLightingModel.hpp>
#include <BaseLightSource.hpp>
#include <BarycentricRasterizer.hpp>
#include <Enums.hpp>
#include <omp.h>

class MainWindow
{
public:
    MainWindow(
        Point &_resolution,
        const BaseLightingModel *_lightingModel,
        const BaseLightSource *_lightSource,
        const ShadingModel &_shadingModel);

    void switchVideoMode(bool isEscape = false);

    void resize(int width, int height);

    void clear();

    void drawPixels();

    void drawModel(
        Object &objInfo,
        const Matrix<4, 1> cameraPosition);

    sf::RenderWindow &getWindow();

    const Point &cGetResolution() const;

private:
    static bool isPolygonFrontOriented(
        Polygon &polygon,
        const std::vector<Matrix<4, 1>> &vertices,
        const Matrix<4, 1> &sightDir);

    void drawPolygon(
        Polygon &polygon,
        const Matrix<4, 1> &sightDir,
        const std::vector<Matrix<4, 1>> &vertices,
        const std::vector<Matrix<4, 1>> &nVertices,
        const std::vector<DrawableVertex> &drawableVertices,
        const sf::Color *color);

    void drawLineBr(
        const DrawableVertex &p1,
        const DrawableVertex &p2,
        const sf::Color *color);

    static void drawPixel(
        sf::Uint8 *pixels,
        int pos,
        const sf::Color *color);

    bool isFullscreen;

    Point &resolution;
    Point lastResolution;

    sf::Uint8 *pixels;
    omp_lock_t *pixelLocks;

    double *depthBuffer;

    BarycentricRasterizer rasterizer;

    sf::RenderWindow window;
    sf::Texture bufferTexture;
    sf::Sprite bufferSprite;
};

inline sf::RenderWindow &MainWindow::getWindow()
{
    return window;
}

inline const Point &MainWindow::cGetResolution() const
{
    return resolution;
}

inline void MainWindow::drawPixel(
    sf::Uint8 *pixels,
    const int pos,
    const sf::Color *color)
{
    static const auto colorSize = (int)sizeof(sf::Color);
    std::memcpy(pixels + (colorSize * pos), color, colorSize);
}

inline bool MainWindow::isPolygonFrontOriented(
    Polygon &polygon,
    const std::vector<Matrix<4, 1>> &vertices,
    const Matrix<4, 1> &sightDir)
{
    return polygon.getNormal(vertices).scalarMultiply(sightDir) < 0;
}
