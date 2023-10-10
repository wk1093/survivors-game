#pragma once

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include <chrono>

#define TILE_SIZE 192
#define TILE_SIZEf 192.0f

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
 * E for empty
 */

enum MapObjectType {
    STATIC, BASIC
};


class MapFile {
private:
    std::vector<std::pair<MapObjectType, std::string>> m_map;
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
                m_map.emplace_back(STATIC, name);
            } else if (type == 'B') {
                m_map.emplace_back(BASIC, name);
            } else {
                std::cerr << "Invalid tile type: '" << type << "'" << std::endl;
                std::cerr << name << std::endl;
                std::cerr << m_map.size() << std::endl;
                throw std::runtime_error("Invalid tile type");
            }
            i++;
            if (i == width*height) {
                break;
            }
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

    [[nodiscard]] const std::vector<std::pair<MapObjectType, std::string>>& getMap() const {
        return m_map;
    }
};

struct MapObject {
    MapObjectType type;
    int index;
    void* obj;
};

class Map {
private:
    std::vector<StaticObject*> m_staticObjects;
    std::vector<BasicObject*> m_basicObjects;
    std::vector<MapObject> m_mapObjects;
    int width, height;

public:
    Map(const char* path, EntityComponentSystem& ecs) {
        MapFile mapFile(path);
        width = mapFile.getWidth();
        height = mapFile.getHeight();
        const std::vector<std::pair<MapObjectType, std::string>>& map = mapFile.getMap();
        for (int i = 0; i < map.size(); i++) {
            int x = i % width;
            int y = i / width;
            if (map[i].first == STATIC) {
                m_staticObjects.emplace_back(&ecs.makeObject<StaticObject>("assets/img/tile/" + map[i].second + ".png"));
                m_staticObjects.back()->setPosition(x*TILE_SIZE, y*TILE_SIZE);
                m_mapObjects.emplace_back(MapObject{STATIC, (int)m_staticObjects.size()-1, m_staticObjects.back()});
            } else if (map[i].first == BASIC) {
                m_basicObjects.emplace_back(&ecs.makeObject<BasicObject>("assets/img/tile/" + map[i].second + ".png"));
                m_basicObjects.back()->setPosition(x*TILE_SIZE, y*TILE_SIZE);
                m_mapObjects.emplace_back(MapObject{BASIC, (int)m_basicObjects.size()-1, m_basicObjects.back()});
            } else {
                std::cerr << "Invalid tile type: '" << map[i].first << "'" << std::endl;
                throw std::runtime_error("Invalid tile type");
            }
        }
    }

    MapObject getMapObject(int x, int y) {
        return m_mapObjects[y*width + x];
    }

    [[nodiscard]] int getWidth() const {
        return width;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }



};