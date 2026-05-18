#pragma once
#include <SFML/Graphics.hpp>
#include "Global.hpp"

class MapManager 
{
public:
    MapManager() {}
    void update() {}
    void draw_map(bool layer, bool is_underground, unsigned int view_x, sf::RenderWindow& window) {}
    unsigned short get_map_width() const { return 100; }
};
