#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"
#include "map/Map.h"
#include "map/Editor.h"
#include "map/MapGenerator.h"

#define NEW_MAP 0
#define EDIT_MAP 0
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



    auto& player = ecs.makeObject<AnimatedPhysicsObject>("assets/img/char/sprite_00.png");
    player.addAnimation("idle_down", std::vector<std::string>{"char/sprite_00.png", "char/sprite_01.png"}, 0.2);
    player.addAnimation("walk_down", std::vector<std::string>{"char/sprite_02.png", "char/sprite_03.png"});
    player.addAnimation("idle_up", std::vector<std::string>{"char/sprite_04.png", "char/sprite_05.png"}, 0.2);
    player.addAnimation("walk_up", std::vector<std::string>{"char/sprite_06.png", "char/sprite_07.png"});
    player.addAnimation("idle_left", std::vector<std::string>{"char/sprite_08.png", "char/sprite_09.png"}, 0.2);
    player.addAnimation("walk_left", std::vector<std::string>{"char/sprite_10.png", "char/sprite_11.png"});
    player.addAnimation("idle_right", std::vector<std::string>{"char/sprite_12.png", "char/sprite_13.png"}, 0.2);
    player.addAnimation("walk_right", std::vector<std::string>{"char/sprite_14.png", "char/sprite_15.png"});
    player.setAnimation("idle_down");
    player.getAnim().setSpeed(0.01);

    float speed = 0.2;
    MovementState state;
    while (e.isOpen()) {
        e.update();

        // wasd movement

        WasdMovement(state, e, player, speed, WasdMode::ACCELERATION, true);
        ecs.update();

        e.clear();

        FocusedCamera(e, player, FocusType::SOFT_CENTER);
        ecs.draw(true);

        e.render();
    }

}
