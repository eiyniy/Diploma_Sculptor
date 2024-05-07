#include <ObjectsLoadedEvent.hpp>

#include <Enums.hpp>

#include <utility>

ObjectsLoadedEvent::ObjectsLoadedEvent(
    std::vector<std::unique_ptr<Object>> _objects)
    : objects(std::move(_objects))
{
}

EventType ObjectsLoadedEvent::getType() const
{
    return EventType::ObjectsLoaded;
}

std::vector<std::unique_ptr<Object>> ObjectsLoadedEvent::getObjects()
{
    return std::move(objects);
}
