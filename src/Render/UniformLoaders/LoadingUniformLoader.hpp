#pragma once

#include <IUniformLoader.hpp>
#include <MainWindow.hpp>

#include <memory>

class LoadingUniformLoader : public IUniformLoader {
private:
    std::shared_ptr<MainWindow> mainWindow;

public:
    LoadingUniformLoader(std::shared_ptr<MainWindow> _mainWindow);

    void load(ShaderProgram* shaderProgram) override;
};
