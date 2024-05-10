#pragma once

#include <BaseInputEngine.hpp>
#include <Enums.hpp>
#include <ObjParseResult.hpp>
#include <ObjParser.hpp>
#include <ObjectBuilder.hpp>

#include <Object.hpp>

#include <chrono>
#include <future>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

class ShaderProgramManager;
class IEvent;

class LoadingInputEngine : public BaseInputEngine {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> loadingStart;

    bool isLoadingFinished;

    ObjParser objParser;
    ObjectBuilder objectBuilder;

    std::shared_ptr<ShaderProgramManager> shaderProgramManager;

    std::vector<std::unique_ptr<Object>> result;

    std::future<ObjParseResult> modelParseTask;
    std::unique_ptr<ObjParseResult> modelParseResult;
    bool isModelParseStarted;

    std::future<void> modelLoadingTask;
    bool isModelLoadingStarted;

    std::future<ObjParseResult> selectorParseTask;
    std::unique_ptr<ObjParseResult> selectorParseResult;
    bool isSelectorParseStarted;

    std::future<void> selectorLoadingTask;
    bool isSelectorLoadingStarted;

public:
    LoadingInputEngine(
        std::shared_ptr<std::queue<std::unique_ptr<IEvent>>> _eventBus,
        std::shared_ptr<ShaderProgramManager> _shaderProgramManager);

    LoadingInputEngine(const LoadingInputEngine&) = delete;
    LoadingInputEngine(LoadingInputEngine&&) = delete;
    LoadingInputEngine& operator=(const LoadingInputEngine&) = delete;
    LoadingInputEngine& operator=(LoadingInputEngine&&) = delete;

    ~LoadingInputEngine();

    std::optional<StateType> update(float dt) override;
};
