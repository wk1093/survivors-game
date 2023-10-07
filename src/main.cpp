#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "test/wasd.h"

int main() {
    Engine e(1280, 720, "Survivors", "assets/img");
//    Sprite sprite = e.makeSprite("assets/img/bigshot.png"); // retrieves the sprite from the atlas
//
//    sprite.scale(0.2, 0.2);
//    sprite.setPosition(1280/2, 720/2);

    auto ecs = EntityComponentSystem(e);

    auto& test = ecs.makeObject<PhysicsObject>("assets/img/bigshot.png");

    float speed = 2.0;

    while (e.isOpen()) {
        e.update();

        // wasd movement

        WasdMovement(test, speed, WasdMode::ACCELERATION);

        e.clear();

        test.update();
        test.draw();

        e.render();
    }

}
