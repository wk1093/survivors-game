#include "gfx/all_sf.h"
#include "ecs/all.h"
#include "map/all.h"
#include "test/wasd.h"
#include "test/focused_camera.h"
#include "dev/DevMenu.h"
#include "gmae/DiceRoller.h"


int main() {
    Engine e(1280, 720, "Dev", "assets/img", "assets/snd");
    auto ecs = EntityComponentSystem(e);
    Map* map = nullptr;
    auto& player = ecs.makeObject<AnimatedPhysicsObject, 1>("assets/img/char/sprite_00.png");
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
    DiceRoller dr(e);
    Sound roll = e.makeSound("assets/snd/dice1.wav");

    while (true) {
        bool dmr = DevMenu(e, ecs, &map);
        if (dmr || map == nullptr) {
            return 0;
        }

        auto view = e.getView();
        auto gui = view;
        view.zoom(0.1);
        e.setView(view);

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

            if (e.isKeyDown(Keyboard::E)) {
                dr.roll("keyedown"+std::to_string(e.getTime()), 20);
                roll.play();
            }

            LockInMap(player, *map);
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

            stamina_bar.setSize({stamina * 12.8f, 30});

            e.setView(gui);
            e.draw(stamina_bar);
            dr.draw();

            e.render();
        }
        e.reLaunch(1280, 720, "Dev");
    }

}
