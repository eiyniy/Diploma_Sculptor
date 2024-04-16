#pragma once

#include <memory>
#include <string>

template <class T> class IBuilder {
public:
    IBuilder<T>();

    std::unique_ptr<T> instance;

    std::string tName;

    virtual void reset();

    void create();

    [[nodiscard]] virtual bool isFinished() const;

    std::unique_ptr<T> build();
};

template <class T> class ConstructorPasskey {

    friend class IBuilder<T>;

private:
    ConstructorPasskey() = default;
    ConstructorPasskey(ConstructorPasskey const&) = default;
    ConstructorPasskey(ConstructorPasskey&&) = default;

public:
    ~ConstructorPasskey() = default;

    ConstructorPasskey& operator=(const ConstructorPasskey&) = delete;
    ConstructorPasskey& operator=(ConstructorPasskey&&) = delete;
};

template <class T>
IBuilder<T>::IBuilder()
    : instance(nullptr)
    , tName(typeid(T).name())
{
}

template <class T> void IBuilder<T>::reset()
{
    if (instance != nullptr) {
        const std::string error = "Can't reset builder of \"" + tName
            + "\". There is unfinished build.";

        throw std::logic_error(error);
    }
}

template <class T> void IBuilder<T>::create()
{
    if (instance != nullptr) {
        std::string error = "Can't create new instance of \"" + tName
            + "\". Previous build haven't been finished.";

        throw std::logic_error(error);
    }

    instance = std::make_unique<T>(ConstructorPasskey<T>());
}

template <class T> bool IBuilder<T>::isFinished() const
{
    return instance != nullptr;
}

template <class T> std::unique_ptr<T> IBuilder<T>::build()
{
    if (!isFinished()) {
        std::string error = "Can't build instance of \"" + tName
            + "\". Build steps haven't been completed.";

        throw std::logic_error(error);
    }

    auto result = std::move(instance);

    instance = nullptr;
    reset();

    return std::move(result);
}