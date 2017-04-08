#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>

enum class MeterSize { Small, Normal };

class GUIMeter : public sf::Drawable
{
    std::shared_ptr<sf::Texture> icon;
    
    MeterSize size;
    size_t height = 0, current = 0, target = 0;
    sf::Color fillColor, targetColor, backdropColor;

    sf::Vertex vertices[20];
    sf::Vector2f position;

    void setRect(size_t ind, sf::FloatRect rect);
    void setQuadColor(size_t ind, sf::Color color);

public:
    GUIMeter(MeterSize size);
    virtual ~GUIMeter() {}

    size_t getHeight() const { return height; }
    void setHeight(size_t h) { height = h; }

    size_t getCurrent() const { return current; }
    void setCurrent(size_t c) { current = c; }

    size_t getTarget() const { return target; }
    void setTarget(size_t t) { target = t; }

    void setColors(sf::Color fill, sf::Color target, sf::Color backdrop)
    {
        fillColor = fill;
        targetColor = target;
        backdropColor = backdrop;
        updateVertices();
    }

    auto getIcon() const { return icon; }
    void setIcon(const std::shared_ptr<sf::Texture>& i) { icon = i; }

    auto getPosition() const { return position; }
    void setPosition(sf::Vector2f pos) { position = pos; }
    void setPosition(float x, float y) { position = sf::Vector2f(x, y); }

    void update(std::chrono::steady_clock::time_point);
    void updateVertices();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
