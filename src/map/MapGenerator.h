#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "../ecs/all.h"
#include "Map.h"

class MapGenerator {
public:
    static void generate(const std::string& path, int width, int height, const std::string& tileset) {
        std::ofstream file(path);
        file << width << " " << height << std::endl;
        for (int i = 0; i < width * height; i++) {
            file << "B";
            file << tileset;
            file << " 0.0";
            file << " ";
            if (i % width == width - 1) {
                file << std::endl;
            }
        }
        file.close();
    }
};