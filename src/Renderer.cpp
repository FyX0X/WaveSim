// src/Renderer.cpp
#include "Renderer.h"


Renderer::Renderer(sf::Vector2u window_size, std::string title) :
	window(sf::VideoMode(window_size), title),
	width(window_size.x), height(window_size.y),
    fpsText(font), text(font)
{
	window.setFramerateLimit(500);


    if (!font.openFromFile("C:\\Users\\marti\\code\\WaveSim\\resource\\arial.ttf")) {
        // Handle error
    }

    fpsText.setCharacterSize(14);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition({ 10.f, 10.f });
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition({ 10.0f, 30.f });
    
}


void Renderer::InitializeWave2D(const Wave2D& wave)
{

    cols = wave.GetWidth();
    rows = wave.GetHeight();


    cellWidth = static_cast<float>(width) / cols;
    cellHeight = static_cast<float>(height) / rows;


    // Create the grid
    grid = sf::VertexArray(sf::PrimitiveType::Triangles, 6 * cols * rows);


    walls = &wave.GetWalls();

    // Precomputes triangles locations
    

    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            float px = i * cellWidth;
            float py = j * cellHeight;


            sf::Vector2f TL(px, py);
            sf::Vector2f TR(px + cellWidth, py);
            sf::Vector2f BL(px, py + cellHeight);
            sf::Vector2f BR(px + cellWidth, py + cellHeight);

            int cellIndex = Index(i, j);
            int vi = cellIndex * 6;

            // Set the vertices for the two triangles
            grid[vi + 0].position = TL;
            grid[vi + 1].position = TR;
            grid[vi + 2].position = BR;

            grid[vi + 3].position = TL;
            grid[vi + 4].position = BR;
            grid[vi + 5].position = BL;

            if ((*walls)[cellIndex]) {
                grid[vi + 0].color = sf::Color::White;
                grid[vi + 1].color = sf::Color::White;
                grid[vi + 2].color = sf::Color::White;

                grid[vi + 3].color = sf::Color::White;
                grid[vi + 4].color = sf::Color::White;
                grid[vi + 5].color = sf::Color::White;
			}
        }
    }


}



void Renderer::Render(const Wave1DInteract& wave)
{
    //const int time_steps = wave.GetMaxSteps();

    HandleEvents();
    window.clear(sf::Color::Black);

    //RenderWave1D(wave, frame);
    RenderWave1DInteract(wave);

    RenderFPS();

    window.display();

}

void Renderer::Render(const Wave2D& wave, int x, int y, float u)
{

    HandleEvents();
    window.clear(sf::Color::Black);

    RenderWave2D(wave);

    RenderFPS();
    RenderInfo(x, y, u);

    window.display();

}

void Renderer::RenderFPS()
{
    // At the start of each frame
    float deltaTime = clock.restart().asSeconds();
    elapsedTime += deltaTime;
    frameCount++;

    if (elapsedTime >= 1.0f) {
        float fps = frameCount / elapsedTime;

        std::ostringstream ss;
        ss.precision(2);
        ss << std::fixed << "FPS: " << fps;

        fpsText.setString(ss.str());

        frameCount = 0;
        elapsedTime = 0.0f;
    }
    window.draw(fpsText);
}


void Renderer::RenderInfo(int x, int y, float u)
{
    std::ostringstream ss;
    ss.precision(2);
    ss << std::fixed << "U[" << x << ", " << y << "] = " << u;
    text.setString(ss.str());        
    window.draw(text);
}


void Renderer::RenderWave1D(const Wave1D& wave, int frame)
{
    const auto& data = wave.GetWave();
    const int size = wave.GetSize();

    float x_spacing = static_cast<float>(width) / (size - 1);
    float y_mid = height / 2.0f;
    float y_scale = height / 2.5f;

    sf::VertexArray line(sf::PrimitiveType::LineStrip, size);
    for (int i = 0; i < size; ++i)
    {
        float x = i * x_spacing;
        float y = y_mid - data[frame][i] * y_scale;
        line[i].position = sf::Vector2f(x, y);
        line[i].color = sf::Color::Cyan;
    }

    window.draw(line);
}

void Renderer::RenderWave1DInteract(const Wave1DInteract& wave)
{
    const auto& data = wave.GetWaveState();
    const int size = wave.GetSize();

    float x_spacing = static_cast<float>(width) / (size - 1);
    float y_mid = height / 2.0f;
    float y_scale = height / 2.5f;


    sf::VertexArray line(sf::PrimitiveType::LineStrip, size);
    for (int i = 0; i < size; ++i)
    {
        float x = i * x_spacing;
        float y = y_mid - data[i] * y_scale;
        line[i].position = sf::Vector2f(x, y);
        line[i].color = sf::Color::Cyan;
    }

    window.draw(line);
}


void Renderer::RenderWave2D(const Wave2D& wave)
{
    const auto& u = wave.GetWaveState();

    // Find min and max values for normalization
    float minVal = wave.GetMinAmplitude();
    float maxVal = wave.GetMaxAmplitude();

    float range;
    if (maxVal > -minVal) range = maxVal;
	else range = -minVal;

    if (range == 0) range = 1.0f; // Avoid division by zero
    
    float inv_range = 1.0f / range;


    for (int idx = 0; idx < cols * rows; ++idx)
	{
        if ((*walls)[idx])
            continue; // Skip walls

        float normalized = u[idx] * inv_range;
        sf::Color color = ColorFromValue(normalized);

        int vi = idx * 6;

        // Set the colors for the vertices
        grid[vi + 0].color = color;
        grid[vi + 1].color = color;
        grid[vi + 2].color = color;
        grid[vi + 3].color = color;
        grid[vi + 4].color = color;
        grid[vi + 5].color = color;
	}
    

    window.draw(grid);

    
}


sf::Color Renderer::ColorFromValue(float value) const
{
    std::uint8_t r = 0, g = 0, b = 0;


    if (value > 0.0f) {
        value = std::min(value, 1.0f);
        r = static_cast<std::uint8_t>(255 * value);
    }
    else {
        value = std::min(-value, 1.0f);
        b = static_cast<std::uint8_t>(255 * value);
    }

    return sf::Color(r, g, b);
}



void Renderer::HandleEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}
