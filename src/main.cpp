#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"
#include "map/Map.h"

int main() {
    Engine e(1280, 720, "Survivors", "assets/img");

    auto ecs = EntityComponentSystem(e);

    auto v = e.getView();
    v.zoom(2);
    e.setView(v);
    auto m = Map("assets/map/test.map", ecs);
//    auto& gr1 = ecs.makeObject<StaticObject>("assets/img/gravel.png");
//    auto& gr2 = ecs.makeObject<BasicObject>("assets/img/gravel.png");
    auto& test = ecs.makeObject<PhysicsObject>("assets/img/bigshot.png");
    test.setPosition(200, 0);

    float speed = 2.0;

    while (e.isOpen()) {
        e.update();

        // wasd movement

        WasdMovement(test, speed, WasdMode::ACCELERATION);
        ecs.update();
        FocusedCamera(e, test, FocusType::SOFT_CENTER);

        e.clear();

        ecs.draw();

        e.render();
    }

}
