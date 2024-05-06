#pragma once

#include <BaseRenderEngine.hpp>

class MainWindow;

class LoadingRenderEngine : public BaseRenderEngine {
private:
    std::shared_ptr<MainWindow> mainWindow;

public:
    LoadingRenderEngine(std::shared_ptr<MainWindow> _mainWindow);

    void draw(
        const std::map<std::string, std::shared_ptr<Object>>& objects) override;
};
