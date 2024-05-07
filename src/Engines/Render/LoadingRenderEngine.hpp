#pragma once

#include <BaseRenderEngine.hpp>

#include <map>
#include <memory>
#include <string>

class MainWindow;
class Object;

class LoadingRenderEngine : public BaseRenderEngine {
private:
    std::shared_ptr<MainWindow> mainWindow;

public:
    LoadingRenderEngine(std::shared_ptr<MainWindow> _mainWindow);

    void draw(
        const std::map<std::string, std::shared_ptr<Object>>& objects) override;
};
