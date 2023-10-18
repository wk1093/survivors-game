#pragma once

#include "../gfx/all_sf.h"
#include "../ecs/all.h"
#include <chrono>
#include <fstream>

#define TILE_SIZE 16
#define TILE_SIZEf 16.0f

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

struct MapDescriptor {
    MapObjectType type;
    std::string name;
    float rotation;
    MapDescriptor(MapObjectType type, std::string name, float rotation) : type(type), name(std::move(name)), rotation(rotation) {}
};


class MapFile {
private:
    std::vector<MapDescriptor> m_map;
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
        float rotation;
        int i = 0;
        consumeWhitespace(file);
        while (fscanf(file, "%c%s%f", &type, name, &rotation) != EOF) {
            // consume space
            consumeWhitespace(file);
            if (type == 'S') {
                m_map.emplace_back(STATIC, name, rotation);
            } else if (type == 'B') {
                m_map.emplace_back(BASIC, name, rotation);
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

    [[nodiscard]] const std::vector<MapDescriptor>& getMap() const {
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
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        MapFile mapFile(path);
        width = mapFile.getWidth();
        height = mapFile.getHeight();
        const std::vector<MapDescriptor>& map = mapFile.getMap();
        for (int i = 0; i < map.size(); i++) {
            int x = i % width;
            int y = i / width;
            if (map[i].type == STATIC) {
                m_staticObjects.emplace_back(&ecs.makeObject<StaticObject>("assets/img/tile/" + map[i].name + ".png"));
                m_staticObjects.back()->setPosition(x*TILE_SIZE, y*TILE_SIZE);
                m_staticObjects.back()->setRotation(map[i].rotation);
                m_staticObjects.back()->setOrigin(TILE_SIZE/2, TILE_SIZE/2);
                m_mapObjects.emplace_back(MapObject{STATIC, (int)m_staticObjects.size()-1, m_staticObjects.back()});
            } else if (map[i].type == BASIC) {
                m_basicObjects.emplace_back(&ecs.makeObject<BasicObject>("assets/img/tile/" + map[i].name + ".png"));
                m_basicObjects.back()->setPosition(x*TILE_SIZE, y*TILE_SIZE);
                m_basicObjects.back()->setOrigin(TILE_SIZE/2, TILE_SIZE/2);
                m_basicObjects.back()->setRotation(map[i].rotation);
                m_mapObjects.emplace_back(MapObject{BASIC, (int)m_basicObjects.size()-1, m_basicObjects.back()});
            } else {
                std::cerr << "Invalid tile type: '" << map[i].name << "'" << std::endl;
                throw std::runtime_error("Invalid tile type");
            }
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Map loaded in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
    }

    MapObject& getMapObject(int x, int y) {
        return m_mapObjects[y*width + x];
    }

    [[nodiscard]] int getWidth() const {
        return width;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }

    [[nodiscard]] Vector2i getSize() const {
        return Vector2i(width, height);
    }

    void save(Engine& engine, const std::string& directory) {
        std::ofstream file(directory);
        file << width << " " << height << std::endl;
        for (auto & m_mapObject : m_mapObjects) {
            if (m_mapObject.type == STATIC) {
                file << "S";
            } else if (m_mapObject.type == BASIC) {
                file << "B";
            } else {
                std::cerr << "Invalid tile type: '" << m_mapObject.type << "'" << std::endl;
                throw std::runtime_error("Invalid tile type");
            }
            // substr to remove "assets/img/tile/" and ".png" = substr(16, size-4
            std::string fn = engine.getAtlas().getFilename(((Object*)m_mapObject.obj)->getSprite().getTextureRect());
            file << fn.substr(16, fn.size()-16-4) << " " << ((Object*)m_mapObject.obj)->getRotation() << " ";
            if (m_mapObject.index % width == width-1) {
                file << std::endl;
            }
        }
    }


};