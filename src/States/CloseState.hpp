#pragma once

#include <BaseState.hpp>

class CloseState : public BaseState {
public:
    CloseState();

    [[nodiscard]] StateTypes getType() const override;
};