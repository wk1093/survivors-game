#pragma once

#include "SFML/Graphics.hpp"
#include <filesystem>
#include <iostream>


std::vector<std::string> recurseDirectory(const char* directory, const std::string& extension = ".png") {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
            std::vector<std::string> subFiles = recurseDirectory(entry.path().string().c_str());
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        } else {
            if (entry.path().extension() == extension) {
                files.push_back(entry.path().string());
            }
        }
    }
    return files;

}

class TextureAtlas {
private:
    sf::Texture textureAtlas;
    std::unordered_map<std::string, sf::IntRect> textureAtlasMap;
    const char* directory;

public:
    explicit TextureAtlas(const char* directory): directory(directory) { }

    void build() {
        std::vector<std::string> files = recurseDirectory(directory);
        std::vector<sf::Image> images;
        for (const std::string& file : files) {
            sf::Image image;
            image.loadFromFile(file);
            images.push_back(image);
        }

        sf::Image atlasImage;
        // the algorithm uses a simple algorithm that just puts the images next to each other horizontally
        // maybe in the future we can improve this algorithm to be more efficient
        unsigned int width = 0, height = 0;

        for (const sf::Image& image : images) {
            width += image.getSize().x;
            height = std::max(height, image.getSize().y);
        }

        atlasImage.create(width, height);

        unsigned int x = 0;
        unsigned int i = 0;
        for (const sf::Image& image : images) {
            atlasImage.copy(image, x, 0);
            auto rect = sf::IntRect((int)x, 0, (int)image.getSize().x, (int)image.getSize().y);
            textureAtlasMap[std::filesystem::path(files[i])] = rect;
            x += image.getSize().x;
            i++;
        }

        textureAtlas.loadFromImage(atlasImage);
    }

    sf::Sprite makeSprite(const std::string& file) {
        if (textureAtlasMap.find(std::filesystem::path(file)) == textureAtlasMap.end()) {
            std::cerr << "File not found in texture atlas" << std::endl;
            std::cerr << "Offending file: " << file << std::endl;
            std::cerr << "Options: " << std::endl;
            for (const auto& [key, value] : textureAtlasMap) {
                std::cerr << key << std::endl;
            }
            throw std::runtime_error("File not found in texture atlas");
        }
        sf::Sprite sprite(textureAtlas, textureAtlasMap[std::filesystem::path(file)]);
        return sprite;
    }

    sf::Texture& getTexture() {
        return textureAtlas;
    }
};