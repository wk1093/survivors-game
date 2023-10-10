#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"
#include "map/Map.h"
#include "map/Editor.h"

int main() {
    Engine e(1280, 720, "Editor", "assets/img");

    auto ecs = EntityComponentSystem(e);
    auto m = Map("assets/map/test.map", ecs);

    MapEditor editor(e, m, ecs);

    editor.launch(); // launches the editor. Once the editor is closed (ESC), the below code will run
    exit(0);
    e.reLaunch(1280, 720, "Survivors");

    auto v = e.getView();
    v.zoom(2);
    e.setView(v);

    auto& player = ecs.makeObject<PhysicsObject>("assets/img/bigshot.png");
    player.setPosition(200, 0);

    float speed = 2.0;

    while (e.isOpen()) {
        e.update();

        // wasd movement

        WasdMovement(e, player, speed, WasdMode::ACCELERATION);
        ecs.update();

        e.clear();

        FocusedCamera(e, player, FocusType::SOFT_CENTER);

        ecs.draw();

        e.render();
    }

}
