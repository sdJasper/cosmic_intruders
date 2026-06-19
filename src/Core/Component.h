#pragma once

struct Component {
    virtual ~Component() = default;
    bool active = true;
};
