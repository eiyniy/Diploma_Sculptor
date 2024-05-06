#pragma once

class Brush {
private:
    float radius;
    float strength;

public:
    Brush(float _radius, float _strength);

    [[nodiscard]] float getRadius() const;
    [[nodiscard]] float getStrength() const;
};
