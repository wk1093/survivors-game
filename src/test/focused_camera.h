#pragma once

#include "../gfx/all_sf.h"
#include "../ecs/all.h"

enum class FocusType {
    CENTER, // obj is kept in the center of the screen, and the camera is moved to follow it always
    SOFT_CENTER, // obj is kept near the center of the screen, and the camera follows behind it
    CLICK, // camera only moves if the obj moves out of the screen, and the camera instantly moves in a grid-like fashion
};

void FocusedCamera(Engine& e, PhysicsObject& obj, FocusType type) {
    sf::View view = e.getView();
    sf::Vector2f pos = obj.getPosition();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f objSize = obj.getSize();
    sf::Vector2f viewCenter = view.getCenter();

    switch (type) {
        case FocusType::CENTER:
            view.setCenter(pos+objSize/2.0f);
            break;
        case FocusType::SOFT_CENTER:
            // smooth camera movement
            view.setCenter(viewCenter + (pos+objSize/2.0f - viewCenter)*0.1f);
            break;
        case FocusType::CLICK:
            // camera only moves if the obj moves out of the screen, and the camera instantly moves in a grid-like fashion
            if (pos.x+objSize.x/2 < viewCenter.x - viewSize.x/2.0f) {
                view.setCenter(viewCenter - sf::Vector2f(viewSize.x, 0));
            } else if (pos.x+objSize.x/2 > viewCenter.x + viewSize.x/2.0f) {
                view.setCenter(viewCenter + sf::Vector2f(viewSize.x, 0));
            }
            if (pos.y + objSize.y/2 < viewCenter.y - viewSize.y/2.0f) {
                view.setCenter(viewCenter - sf::Vector2f(0, viewSize.y));
            } else if (pos.y + objSize.y/2 > viewCenter.y + viewSize.y/2.0f) {
                view.setCenter(viewCenter + sf::Vector2f(0, viewSize.y));
            }
            break;
    }

    e.setView(view);
}