#pragma once

#include "../gfx/all_sf.h"
#include <unordered_map>
#include "Object.h"

class Component {
protected:
    Engine& m_engine;
    Object& m_object; // refrence to the object that this component is attached to

public:
    Component(Engine& e, Object& o): m_engine(e), m_object(o) { }

    virtual void update() = 0;
//    virtual void draw() {
//
//    }
};
