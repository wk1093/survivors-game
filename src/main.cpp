#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"

int main() {
    Engine e(1280, 720, "Survivors", "assets/img");

    auto ecs = EntityComponentSystem(e);

    auto& test2 = ecs.makeObject<BasicObject>("assets/img/gravel.png");
    auto& test = ecs.makeObject<PhysicsObject>("assets/img/bigshot.png");

    float speed = 2.0;

    while (e.isOpen()) {
        e.update();

        // wasd movement

        WasdMovement(test, speed, WasdMode::ACCELERATION);
        ecs.update();
        FocusedCamera(e, test, FocusType::CLICK);

        e.clear();

        ecs.draw();

        e.render();
    }

}
