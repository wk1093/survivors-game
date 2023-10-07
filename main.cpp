#include "engine/all_sf.h"

int main() {
    Engine e(1280, 720, "Survivors", "assets/img");
    Sprite sprite = e.makeSprite("assets/img/bigshot.png"); // retrieves the sprite from the atlas

    sprite.scale(0.2, 0.2);
    sprite.setPosition(1280/2, 720/2);

    float speed = 2.0;

    while (e.isOpen()) {
        e.update();

        // wasd movement

        if (Engine::isKeyPressed(sf::Keyboard::W)) {
            sprite.move(0, -speed*e.dt);
        }
        if (Engine::isKeyPressed(sf::Keyboard::A)) {
            sprite.move(-speed*e.dt, 0);
        }
        if (Engine::isKeyPressed(sf::Keyboard::S)) {
            sprite.move(0, speed*e.dt);
        }
        if (Engine::isKeyPressed(sf::Keyboard::D)) {
            sprite.move(speed*e.dt, 0);
        }

        e.clear();

        e.draw(sprite);

        e.render();
    }

}
