#pragma once

#include <Enums.hpp>
#include <IEvent.hpp>
#include <Object.hpp>

#include <memory>
#include <vector>

class ObjectsLoadedEvent : public IEvent {
private:
    std::vector<std::unique_ptr<Object>> objects;

public:
    ObjectsLoadedEvent(std::vector<std::unique_ptr<Object>> _objects);

    [[nodiscard]] EventType getType() const override;

    [[nodiscard]] std::vector<std::unique_ptr<Object>> getObjects();
};
