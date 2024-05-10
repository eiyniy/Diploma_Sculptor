#include <LoadingInputEngine.hpp>

#include <BaseInputEngine.hpp>
#include <Enums.hpp>
#include <IEvent.hpp>
#include <ObjParseResult.hpp>
#include <ObjParser.hpp>
#include <Object.hpp>
#include <ObjectBuilder.hpp>
#include <ObjectsLoadedEvent.hpp>
#include <Settings.hpp>
#include <ShaderProgramManager.hpp>
#include <Triangle.hpp>

#include <GL/glew.h>

#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>

LoadingInputEngine::LoadingInputEngine(
    std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
    std::shared_ptr<ShaderProgramManager> _shaderProgramManager)
    : BaseInputEngine(std::move(_eventBus))
    , shaderProgramManager(std::move(_shaderProgramManager))
    , isLoadingFinished(false)
    , isModelLoadingStarted(false)
    , isSelectorParseStarted(false)
    , isModelParseStarted(false)
    , isSelectorLoadingStarted(false)
{
    loadingStart = std::chrono::high_resolution_clock::now();

    glfwSwapInterval(1);
}

LoadingInputEngine::~LoadingInputEngine()
{
    glfwSwapInterval(static_cast<int>(Settings::get()->isVSyncEnabled()));
}

std::optional<StateType> LoadingInputEngine::update(const float dt)
{
    if (!isModelParseStarted) {
        modelParseTask = std::async([&parser = objParser] {
            const std::string projectPath
                = R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\)";

            // TODO: Move paths to Settings
            std::string path = projectPath +
                // R"(resources\baseModels\cube\cube.obj)";
                R"(resources\baseModels\sphere\sphere.obj)";
            // R"(resources\models\car\car.obj)";
            // R"(resources\models\AstonMartin\AstonMartin2.obj)";
            // R"(resources\models\angel\angel.obj)";

            return std::move(parser.parse(path));
        });

        isModelParseStarted = true;
    }

    if (!isModelLoadingStarted) {
        auto modelParseStatus
            = modelParseTask.wait_for(std::chrono::nanoseconds(0));
        if (modelParseStatus != std::future_status::ready) {
            return std::nullopt;
        }

        std::cout << "modelParseTask ended" << std::endl;
        modelParseResult
            = std::make_unique<ObjParseResult>(modelParseTask.get());

        objectBuilder.create();
        objectBuilder.init(
            "MODEL", GL_TRIANGLES, std::move(modelParseResult->getVertices()));

        modelLoadingTask = std::async(
            [&builder = objectBuilder, &manager = shaderProgramManager](
                std::unique_ptr<ObjParseResult> parseResult) {
                builder.addTriangles(std::move(parseResult->getTriangles()));

                builder.addNVertices(std::move(parseResult->getNVertices()));

                builder.addShaderProgram(manager->getShaderProgram(
                    ShaderProgramManager::modelSPName));

                builder.transform();
                std::cout << "modelLoadingStatus ended INNER" << std::endl;
            },
            std::move(modelParseResult));

        isModelLoadingStarted = true;
    }

    if (!isSelectorParseStarted) {
        auto modelLoadingStatus
            = modelLoadingTask.wait_for(std::chrono::nanoseconds(0));
        if (modelLoadingStatus != std::future_status::ready) {
            return std::nullopt;
        }

        std::cout << "modelLoadingStatus ended BEFORE" << std::endl;
        modelLoadingTask.get();
        std::cout << "modelLoadingStatus ended AFTER" << std::endl;

        objectBuilder.setupVAO();

        std::cout << "HERE" << std::endl;

        result.emplace_back(std::move(objectBuilder.build()));

        std::cout << "HERE TWO" << std::endl;

        selectorParseTask = std::async([&parser = objParser] {
            const std::string projectPath
                = R"(C:\Users\Natallia\Documents\Labs\Diploma\Diploma_Sculptor\)";

            std::string path = projectPath
                + R"(resources\baseModels\armillarySphere\ArmillarySphere.obj)";

            return parser.parse(path);
        });

        isSelectorParseStarted = true;
    }

    if (!isSelectorLoadingStarted) {
        auto selectorParseStatus
            = selectorParseTask.wait_for(std::chrono::nanoseconds(0));
        if (selectorParseStatus != std::future_status::ready) {
            return std::nullopt;
        }

        std::cout << "selectorParseTask ended" << std::endl;
        selectorParseResult
            = std::make_unique<ObjParseResult>(selectorParseTask.get());

        objectBuilder.create();
        objectBuilder.init(
            "SELECTOR", GL_LINES, selectorParseResult->getVertices());

        selectorLoadingTask = std::async(
            [&builder = objectBuilder, &manager = shaderProgramManager](
                std::unique_ptr<ObjParseResult> parseResult) {
                builder.addLines(parseResult->getLinesId());

                builder.addShaderProgram(manager->getShaderProgram(
                    ShaderProgramManager::linesSPName));

                builder.transform();
            },
            std::move(selectorParseResult));

        isSelectorLoadingStarted = true;
    }

    auto selectorLoadingStatus
        = selectorLoadingTask.wait_for(std::chrono::nanoseconds(0));
    if (selectorLoadingStatus != std::future_status::ready) {
        return std::nullopt;
    }

    std::cout << "selectorLoadingTask ended" << std::endl;
    selectorLoadingTask.get();

    objectBuilder.setupVAO();

    result.push_back(std::move(objectBuilder.build()));

    std::cout << "Total loading time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - loadingStart)
              << std::endl;

    pushEvent(std::make_unique<ObjectsLoadedEvent>(std::move(result)));
    return StateType::View;
}
