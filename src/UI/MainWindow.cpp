#include <iostream>
#include <MainWindow.hpp>
#include <Timer.hpp>
#include <Converter.hpp>
#include <ThreadPool.hpp>
#include <SHClipper.hpp>
#include <BarycentricRasterizer.hpp>

MainWindow::MainWindow(
    Point &_resolution,
    const BaseLightingModel *_lightingModel,
    const BaseLightSource *_lightSource,
    const ShadingModel &_shadingModel)
    : window(sf::RenderWindow{
          sf::VideoMode(_resolution.cGetX(), _resolution.cGetY()),
          "SFML Graphics",
          sf::Style::Fullscreen}),
      isFullscreen(true),
      resolution(_resolution),
      lastResolution{1280, 720},
      pixels(new sf::Uint8[resolution.cGetX() * resolution.cGetY() * 4]),
      pixelLocks(new omp_lock_t[resolution.cGetX() * resolution.cGetY()]),
      depthBuffer(new double[resolution.cGetX() * resolution.cGetY()]),
      rasterizer(BarycentricRasterizer{
          _lightingModel,
          _lightSource,
          _shadingModel,
          _resolution,
          &drawPixel,
          pixels,
          pixelLocks,
          depthBuffer})
{
    std::fill(depthBuffer, depthBuffer + resolution.cGetX() * resolution.cGetY(), INT_MAX);

    for (int i = 0; i < resolution.cGetX() * resolution.cGetY(); ++i)
        omp_init_lock(&pixelLocks[i]);

    bufferTexture.create(resolution.cGetX(), resolution.cGetY());
    bufferSprite.setTexture(bufferTexture, true);
}

void MainWindow::drawModel(
    Object &objInfo,
    const Matrix<4, 1> cameraPosition)
{
    const auto color = &objInfo.cGetColor();
    auto &polygons = objInfo.getPolygons();
    // int backwardClippedCount{};

    // #pragma omp parallel for
    for (int j = 0; j < polygons.size(); ++j)
    {
        auto sightDir = polygons[j].getCenter(objInfo.cGetVertices()) - cameraPosition;
        sightDir.normalize();

        // std::cout << "Polygon: " << j << std::endl;
        if (!isPolygonFrontOriented(polygons[j], objInfo.cGetVertices(), sightDir))
        {
            // #pragma omp atomic
            // ++backwardClippedCount;
            continue;
        }

        // Timer::start();
        drawPolygon(
            polygons[j],
            sightDir,
            objInfo.cGetVertices(),
            objInfo.cGetNVertices(),
            objInfo.cGetDrawable(),
            color);
        // Timer::stop();
    }

    // std::cout << "backwardClippedCount: " << backwardClippedCount << std::endl;

    // std::cout << std::endl;

    //    */

    /*
for (auto vertex: viewportVertices) {
    if (!vertex.IsVisible())
        continue;

    int x = vertex.CGetX();
    int y = vertex.CGetY();

//        if (x - 1 < 0 || x + 1 >= resolution.cGetX() || y - 1 < 0 || y + 1 >= resolution.cGetY())
//            continue;

    drawPixel(x - 1, y - 1, &vColor, resolution.cGetX());
    drawPixel(x - 1, y, &vColor, resolution.cGetX());
    drawPixel(x - 1, y + 1, &vColor, resolution.cGetX());
    drawPixel(x, y - 1, &vColor, resolution.cGetX());
    drawPixel(x, y, &vColor, resolution.cGetX());
    drawPixel(x, y + 1, &vColor, resolution.cGetX());
    drawPixel(x + 1, y - 1, &vColor, resolution.cGetX());
    drawPixel(x + 1, y, &vColor, resolution.cGetX());
    drawPixel(x + 1, y + 1, &vColor, resolution.cGetX());
}
*/

    /*
    for (int j = 0; j < polygons.size(); ++j)
        drawPolygon(polygons[j], viewportVertices, color);
    */
}

void MainWindow::switchVideoMode(const bool isEscape)
{
    if (!isFullscreen && isEscape)
        return;

    sf::VideoMode videoMode;

    if (isFullscreen)
        videoMode = sf::VideoMode(lastResolution.cGetX(), lastResolution.cGetY());
    else
    {
        lastResolution = {(int)window.getSize().x, (int)window.getSize().y};
        videoMode = sf::VideoMode::getDesktopMode();
    }

    isFullscreen = !isFullscreen;

    window.create(
        videoMode,
        "SFML Graphics",
        isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);

    resize((int)videoMode.width, (int)videoMode.height);
}

void MainWindow::resize(const int width, const int height)
{
    resolution = Point(width, height);

    delete[] pixels;
    pixels = new sf::Uint8[width * height * 4];

    depthBuffer = new double[width * height];
    std::fill(depthBuffer, depthBuffer + resolution.cGetX() * resolution.cGetY(), INT_MAX);

    bufferTexture.create(width, height);
    bufferSprite.setTexture(bufferTexture, true);

    sf::View view{{0, 0, (float)width, (float)height}};
    window.setView(view);
}

void MainWindow::clear()
{
    window.clear();
    std::fill(pixels, pixels + resolution.cGetX() * resolution.cGetY() * 4, 0x53u);
    std::fill(depthBuffer, depthBuffer + resolution.cGetX() * resolution.cGetY(), INT_MAX);
}

void MainWindow::drawPixels()
{
    bufferTexture.update(pixels);
    window.draw(bufferSprite);
    window.display();
}

void MainWindow::drawPolygon(
    Polygon &polygon,
    const Matrix<4, 1> &sightDir,
    const std::vector<Matrix<4, 1>> &vertices,
    const std::vector<Matrix<4, 1>> &nVertices,
    const std::vector<DrawableVertex> &drawableVertices,
    const sf::Color *color)
{
    auto vIndexesCount = polygon.cGetVertexIdsCount();
    //    /*
    bool isPolygonVisible = true;

    for (int i = 0; i < vIndexesCount; ++i)
        isPolygonVisible &= !drawableVertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1).IsWNegative();

    if (!isPolygonVisible)
        return;
    //    */

    /*
    for (int i = 0; i < vIndexesCount; ++i) {
        const auto j = (i + 1) % vIndexesCount;

        auto firstDrawablePoint = drawableVertices[polygon.cGetVertexIds(i).cGetVertexId() - 1];
        auto secondDrawablePoint = drawableVertices[polygon.cGetVertexIds(j).cGetVertexId() - 1];

        if (firstDrawablePoint.IsWNegative() || secondDrawablePoint.IsWNegative())
            continue;

        const auto result = clipper->clipLine(
                firstDrawablePoint.GetX(),
                firstDrawablePoint.GetY(),
                secondDrawablePoint.GetX(),
                secondDrawablePoint.GetY());

        if (result == ClipLineResult::Invisible)
            continue;

        drawLineBr(
                firstDrawablePoint,
                secondDrawablePoint,
                color);
    }
    */

    //    /*
    rasterizer.rasterize(
        polygon,
        sightDir,
        vertices,
        nVertices,
        drawableVertices,
        *color
        //            colors.at(colorNumber)
    );

    //    for (const auto &i: result) {
    //        drawPixel(i.CGetX(), i.CGetY(), &colors.at(colorNumber), resolution.cGetX());
    //    }

    //    colorNumber = (colorNumber + 1) % colors.size();
    //    */
}

void MainWindow::drawLineBr(
    const DrawableVertex &p1,
    const DrawableVertex &p2,
    const sf::Color *color)
{
    int x1 = (int)p1.CGetX();
    int y1 = (int)p1.CGetY();

    const int x2 = (int)p2.CGetX();
    const int y2 = (int)p2.CGetY();

    const int xSize = resolution.cGetX();

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);

    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    // if (x2 < resolution.cGetX() && y2 < resolution.cGetY() && x2 > 0 && y2 > 0)
    drawPixel(pixels, y2 * xSize + x2, color);

    while (x1 != x2 || y1 != y2)
    {
        // if (x1 < resolution.cGetX() && y1 < resolution.cGetY() && x1 > 0 && y1 > 0)
        drawPixel(pixels, y1 * xSize + x1, color);

        const int error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}
