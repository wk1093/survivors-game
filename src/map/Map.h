#pragma once

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include <chrono>

#define TILE_SIZE 192

/*
 * Represent a list of tiles in a file. A tile can be a basic (passthough/background) tile, or a static (collidable) tile.
 * auto& gr1 = ecs.makeObject<StaticObject>("assets/img/tile/gravel.png");
 * auto& gr2 = ecs.makeObject<BasicObject>("assets/img/tile/gravel.png");
 * gr2.setPosition(0, 192);
 * tiles are 192x192 (for now)
 * Map will be in the following format:
 * FIRST LINE: (width) (height)
 * (S/B)(name)
 * S/B: S for static, B for basic
 * Sgravel = StaticObject("assets/img/tile/gravel.png")
 * seperated by spaces (meaning no spaces in tile names)
 */

class MapFile {
private:
    std::vector<std::pair<bool, std::string>> m_map;
    int width, height;
public:
    MapFile(const char* path) {
        FILE* file = fopen(path, "r");
        if (file == nullptr) {
            std::cerr << "Failed to open map file: " << path << std::endl;
            throw std::runtime_error("Failed to open map file");
        }
        // fscanf %c then %s over and over again until EOF
        fscanf(file, "%d %d", &width, &height);
        char type;
        char name[256];
        int i = 0;
        consumeWhitespace(file);
        while (fscanf(file, "%c%s", &type, name) != EOF) {
            // consume space
            consumeWhitespace(file);
            if (type == 'S') {
                m_map.emplace_back(true, name);
            } else if (type == 'B') {
                m_map.emplace_back(false, name);
            } else {
                std::cerr << "Invalid tile type: '" << type << "'" << std::endl;
                std::cerr << name << std::endl;
                std::cerr << m_map.size() << std::endl;
                throw std::runtime_error("Invalid tile type");
            }
            i++;
        }
        if (i != width*height) {
            std::cerr << "Map size does not match width and height" << std::endl;
            throw std::runtime_error("Map size does not match width and height");
        }
    }
private:

    static void consumeWhitespace(FILE* file) {
        char c;
        while ((c = fgetc(file)) == ' ' || c == '\n' || c == '\t') {
            // consume whitespace
        }
        ungetc(c, file);
    }
public:

    [[nodiscard]] int getWidth() const {
        return width;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }

    [[nodiscard]] const std::vector<std::pair<bool, std::string>>& getMap() const {
        return m_map;
    }
};

class Map {
private:
    std::vector<StaticObject*> m_staticObjects;
    std::vector<BasicObject*> m_basicObjects;

public:
    Map(const char* path, EntityComponentSystem& ecs) {
        MapFile mapFile(path);
        int width = mapFile.getWidth();
        int height = mapFile.getHeight();
        const std::vector<std::pair<bool, std::string>>& map = mapFile.getMap();
        for (int i = 0; i < map.size(); i++) {
            int x = i % width;
            int y = i / width;
            if (map[i].first) {
                m_staticObjects.emplace_back(&ecs.makeObject<StaticObject>("assets/img/tile/" + map[i].second + ".png"));
                m_staticObjects.back()->setPosition(x*TILE_SIZE, y*TILE_SIZE);
                std::cout << "static: " << x*TILE_SIZE << ", " << y*TILE_SIZE << std::endl;
            } else {
                m_basicObjects.emplace_back(&ecs.makeObject<BasicObject>("assets/img/tile/" + map[i].second + ".png"));
                m_basicObjects.back()->setPosition(x*TILE_SIZE, y*TILE_SIZE);
                std::cout << "basic: " << x*TILE_SIZE << ", " << y*TILE_SIZE << std::endl;
            }
        }
    }
};