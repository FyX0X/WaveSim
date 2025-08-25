// include/Renderer.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include "Wave1D.h"
#include "Wave1DInteract.h"
#include "Wave2D.h"
#include <string>
#include <sstream>



class Renderer {
public:
    Renderer(sf::Vector2u window_size, std::string title);
    void Render(const Wave1DInteract& wave);  // General render interface
    void Render(const Wave2D& wave, int x, int y, float u);  // General render interface

    void Close() { window.close(); }


    bool IsOpen() const { return window.isOpen(); }
    void HandleEvents();
    sf::Vector2i Renderer::GetMousePosition() const { return sf::Mouse::getPosition(window); }
    bool Renderer::IsMousePressed(sf::Mouse::Button button) const { return sf::Mouse::isButtonPressed(button); }
    bool Renderer::IsKeyPressed(sf::Keyboard::Key key) const { return sf::Keyboard::isKeyPressed(key); }
    int Renderer::GetWidth() const { return width; }
    int Renderer::GetHeight() const { return height; }


    void InitializeWave2D(const Wave2D& wave);


private:
    void RenderWave1D(const Wave1D& wave, int frame);
    void RenderWave1DInteract(const Wave1DInteract& wave);
    void RenderWave2D(const Wave2D& u);
    
    void RenderInfo(int x, int y, float u);
    void RenderFPS();

    sf::Color ColorFromValue(float value) const;


    sf::RenderWindow window;
    int width, height;
    sf::Clock clock;
    sf::Font font;
    sf::Text fpsText;
    sf::Text text;
    int frameCount = 0;
    float elapsedTime = 0.0f;
    

    sf::VertexArray grid;
    const std::vector<bool>* walls = nullptr;
    float cellWidth, cellHeight;
    int cols, rows;


    inline int Index(int x, int y) const { return  y * cols + x; }
};
