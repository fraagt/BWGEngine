#pragma once

#include "camera.hpp"
#include "input.hpp"
#include "time.hpp"

class Object {
public:
    virtual void update(const Input &input, const Time &time) {}

    virtual void draw(const Camera &camera) {}
};