#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "map/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"

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

    auto view = e.getView();
    auto gui = view;
    view.zoom(0.1);
    e.setView(view);



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
    player.setFriction(0.8);

    float speed = 0.2;
    float sprint_speed = 0.35;
    float stamina = 100; // 0 - 100
    MovementState state;

    RectangleShape stamina_bar({1280, 30});
    stamina_bar.setFillColor(Color::Red);
    stamina_bar.setPosition(0, 0);

    while (e.isOpen()) {
        e.update();

        // wasd movement
        if (e.isKeyPressed(Keyboard::LControl) && stamina > 0) {
            WasdMovement(state, e, player, sprint_speed, WasdMode::ACCELERATION, true);
            stamina -= 0.25f * e.dt;
        } else {
            WasdMovement(state, e, player, speed, WasdMode::ACCELERATION, true);
        }
        LockInMap(player, m);
        ecs.update();

        e.clear();
       view = FocusedCamera(e, player, FocusType::SOFT_CENTER, view);

        ecs.draw(true);

        if (stamina < 100) {
            stamina += 0.10f * e.dt;
        }
        if (stamina > 100) {
            stamina = 100;
        }

        stamina_bar.setSize({stamina*12.8f, 30});

        e.setView(gui);
        e.draw(stamina_bar);

        e.render();
    }

}
