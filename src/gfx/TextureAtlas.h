#pragma once

#include "SFML/Graphics.hpp"
#include <filesystem>
#include <iostream>
#include <unordered_map>


std::vector<std::string> recurseDirectory(const char* directory) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
            std::vector<std::string> subFiles = recurseDirectory(entry.path().generic_string().c_str());
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        } else {
            files.push_back(entry.path().generic_string());
        }
    }
    return files;
}

std::vector<std::string> recurseDirectory(const char* directory, const std::string& extension) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
            std::vector<std::string> subFiles = recurseDirectory(entry.path().generic_string().c_str(), extension);
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        } else {
            if (entry.path().extension() == extension) {
                files.push_back(entry.path().generic_string());
            }
        }
    }
    return files;
}

std::vector<std::string> recurseDirectory(const char* directory, const std::vector<std::string>& extensions) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
            std::vector<std::string> subFiles = recurseDirectory(entry.path().generic_string().c_str(), extensions);
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        } else {
            for (const std::string& extension : extensions) {
                if (entry.path().extension() == extension) {
                    files.push_back(entry.path().generic_string());
                }
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
        std::vector<std::string> files = recurseDirectory(directory, ".png");
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
        if (width > 16384) {
            std::cerr << "Texture atlas width is too large" << std::endl;
            std::cerr << "Bro, make a better algorithm" << std::endl;
            throw std::runtime_error("Texture atlas width is too large");
        }
        if (height > 16384) {
            std::cerr << "Texture atlas height is too large" << std::endl;
            std::cerr << "Bro, make a better algorithm" << std::endl;
            throw std::runtime_error("Texture atlas height is too large");
        }
        atlasImage.create(width, height);

        unsigned int x = 0;
        unsigned int i = 0;
        for (const sf::Image& image : images) {
            atlasImage.copy(image, x, 0);
            auto rect = sf::IntRect((int)x, 0, (int)image.getSize().x, (int)image.getSize().y);
            textureAtlasMap[std::filesystem::path(files[i]).generic_string()] = rect;
            x += image.getSize().x;
            i++;
        }

        textureAtlas.loadFromImage(atlasImage);
    }

    sf::Sprite makeSprite(const std::string& file) {
        if (textureAtlasMap.find(std::filesystem::path(file).generic_string()) == textureAtlasMap.end()) {
            std::cerr << "File not found in texture atlas" << std::endl;
            std::cerr << "Offending file: " << file << std::endl;
            std::cerr << "Options: " << std::endl;
            for (const auto& [key, value] : textureAtlasMap) {
                std::cerr << key << std::endl;
            }
            throw std::runtime_error("File not found in texture atlas");
        }
        sf::Sprite sprite(textureAtlas, textureAtlasMap[std::filesystem::path(file).generic_string()]);
        return sprite;
    }

    sf::IntRect makeRect(const std::string& file) {
        if (textureAtlasMap.find(std::filesystem::path(file).generic_string()) == textureAtlasMap.end()) {
            std::cerr << "File not found in texture atlas" << std::endl;
            std::cerr << "Offending file: " << file << std::endl;
            std::cerr << "Options: " << std::endl;
            for (const auto& [key, value] : textureAtlasMap) {
                std::cerr << key << std::endl;
            }
            throw std::runtime_error("File not found in texture atlas");
        }
        return textureAtlasMap[std::filesystem::path(file).generic_string()];
    }

    sf::Texture& getTexture() {
        return textureAtlas;
    }

    std::string getFilename(const sf::IntRect& rect) {
        for (const auto& [key, value] : textureAtlasMap) {
            if (value == rect) {
                return key;
            }
        }
        std::cerr << "Rect not found in texture atlas" << std::endl;
        throw std::runtime_error("Rect not found in texture atlas");
    }
};