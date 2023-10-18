#pragma once

#include "../gfx/all_sf.h"

#include <random>
#include <map>

// this class is used to generate random numbers
// when a dice is rolled it shows up in the top left corner of the screen in this format:
// DICE_ICON 1d6 ?? // the ?? are a number changing a bunch of times until it settles on a number as if it was rolling a dice
// if multiple dice are being rolled they will go down the left side of the screen, ordered by the time they were rolled

// roll for 1 second
#define _ROLL_FOR 1

struct DiceElement {
    int num=1; // 1d6 -> 1
    int type=6; // 1d6 -> 6
    double startTime=0; // when the dice was rolled
    bool finished=false; // if the dice has finished rolling
    int result=-1; // the result of the dice roll
    sf::Text text; // the text that shows up on the screen
    sf::Sprite sprite; // the dice icon
    std::string d;
    bool invis=false;
    double endTime=0;
    DiceElement(Engine& e, int num, int type, double startTime) : num(num), type(type), startTime(startTime) {
        std::stringstream ss;
        ss << num << "d" << type;
        d = ss.str();
        text = sf::Text(d, e.font, (int)(32));
        sprite = e.makeSprite("assets/img/dice1.png");
        sprite.setScale(1.2, 1.2);
    }

    void update(Engine& e, std::mt19937& gen) {
        if (finished) return;
        std::uniform_int_distribution<> dis(1, type);
        std::uniform_int_distribution<> dis6(1, 6);
        double time = e.getTime();
        if (time - startTime > _ROLL_FOR) {
            finished = true;
            endTime = time;
            result = 0;
            for (int i = 0; i < num; i++) {
                result += dis(gen);
            }
            std::stringstream ss;
            ss << d << ": " << result;
            text.setString(ss.str());
        } else {
            // make it happen every once in a while
            if ((int)(time * 1000) % 100 < 50) return;
            sprite.setTextureRect(e.getRect("assets/img/dice" + std::to_string(dis6(gen)) + ".png"));
        }
    }

    void draw(Engine& e, int i) {
        if (finished && e.getTime() - endTime > 2) {
            invis = true;
            return;
        }
        if (finished) {
            text.setPosition(0, i*32*1.2 - 6);
            e.draw(text);
        } else {
            sprite.setPosition(0, i*32*1.2);
            e.draw(sprite);
            text.setPosition(32*1.2, i*32*1.2 - 6);
            e.draw(text);
        }
    }

    DiceElement() = default;
};

class DiceRoller {
    std::map<std::string, DiceElement> m_dice;
    Engine& m_engine;
    std::random_device rd;
    std::mt19937 gen;

public:
    explicit DiceRoller(Engine& e) : m_engine(e), gen(rd()) { }

    void roll(const std::string& name, int type, int num=1) {
        DiceElement d(m_engine, num, type, m_engine.getTime());
        m_dice[name] = d;
    }

    bool rollDone(const std::string& name) {
        if (m_dice.find(name) == m_dice.end()) return false;
        return m_dice[name].finished;
    }

    int result(const std::string& name) {
        if (m_dice.find(name) == m_dice.end()) return -1;
        return m_dice[name].result;
    }

    void clear(const std::string& name) {
        m_dice.erase(name);
    }

    void draw() {
        int i = 0;
        for (auto& d : m_dice) {
            d.second.update(m_engine, gen);
            d.second.draw(m_engine, i);
            if (!d.second.invis)
                i++;

        }
    }




};