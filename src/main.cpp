#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"
#include "map/Map.h"
#include "map/Editor.h"
#include "map/MapGenerator.h"

#define NEW_MAP 0
#define EDIT_MAP 1
#define EXIT_AFTER_EDIT 0

int main() {
    Engine e(1280, 720, "Survivors", "assets/img");
    auto ecs = EntityComponentSystem(e);
#if NEW_MAP
    MapGenerator::generate("assets/map/test.map", 100, 100, "grass/sprite_00"); // creates a map filled with gravel, 100x100
#endif
    auto m = Map("assets/map/test.map", ecs);

#if EDIT_MAP
    MapEditor editor(e, m, ecs);
    editor.launch(); // launches the editor. Once the editor is closed (ESC), the below code will run
    e.reLaunch(1280, 720, "Survivors");
#if EXIT_AFTER_EDIT
    return 0;
#endif
#endif

    auto v = e.getView();
    v.zoom(0.1);
    e.setView(v);

    auto& player = ecs.makeObject<PhysicsObject>("assets/img/char/sprite_00.png");

    // TODO: fix boxes

    float speed = 0.2;
    while (e.isOpen()) {
        e.update();

        // wasd movement

        WasdMovement(e, player, speed, WasdMode::ACCELERATION);
        ecs.update();

        e.clear();

        FocusedCamera(e, player, FocusType::SOFT_CENTER);
        ecs.draw(true);

        e.render();
    }

}
