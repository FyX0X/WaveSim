// src/main.cpp
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Renderer.h"
#include <chrono>

#define PI 3.14159265358979323846

int main()
{
    
    // PARAMETERS
    bool parabola = false;
    bool variable_speed = true;


    unsigned int width = 800;
    unsigned int height = 600;

    Renderer renderer({ width, height }, "WaveSim");

    // Simulation parameters
    float dt = 0.01f; // Time step
    float dx = 0.1f;  // Space step
    float c = 2.0f;   // Wave speed
    float damping = 0.00f; // Damping factor

    // Create the wave simulation object
    Wave2D wave(dt, dx, c, damping, width/4, height/4);
    renderer.InitializeWave2D(wave);


    // Create the terrain (speed field)
    if (variable_speed) {
        float base_speed = c * c * dt * dt / dx / dx; // Base speed
        std::vector<float> speed_field(wave.GetWidth() * wave.GetHeight(), base_speed);
        float m = -1.5f;
        int p = wave.GetHeight() * 3 / 2; // y-intercept (vertical shift)
        for (int i = 0; i < wave.GetWidth(); ++i) {
            // line equation
            int y = m * i + p;

		    for (int j = 0; j < wave.GetHeight(); ++j) {
                //std::cout << "i: " << i << " j: " << j << "y:" << y << std::endl;
                if (j < y) {
			        speed_field[wave.Index(i, j)] = 0.5f * base_speed;

                }
		    }
	    }
        wave.SetSpeedField(speed_field);
    }

    if (parabola) {
        // Create the walls
        std::vector<bool> walls(wave.GetWidth() * wave.GetHeight(), false);
        for (int i = 0; i < wave.GetWidth(); ++i) {
            for (int j = 0; j < wave.GetHeight(); ++j) {
                float a = 0.025f; // Controls curvature (smaller = wider)
                int h = wave.GetWidth() / 2; // Vertex x-position (center)
                int k = wave.GetHeight() / 2; // Vertex y-position (top quarter)

                // Calculate the parabola's X-position
                float parabolaX = -a * (j - k) * (j - k) + h;

                // Define a thickness range (e.g., a band around the curve)
                const float thickness = 2.0f;

                // If the current j (y index) is close to the curve, it's reflective
                if (std::abs(i - parabolaX) < thickness && i > h - wave.GetWidth()/10)
                    walls[wave.Index(i, j)] = true;
            }
        }
        wave.SetWalls(walls);
    }

    float left_height = 0.0f;

    int toBeRestored = 0;
    int count = 0;
    sf::Clock clock;


    float total_sim_time = 0.0f;
    float total_render_time = 0.0f;


    std::cout << "entering mainloop" << std::endl;

    while(renderer.IsOpen())
	{
		renderer.HandleEvents();
        
        if (renderer.IsKeyPressed(sf::Keyboard::Key::Escape))
			renderer.Close();
        if (renderer.IsKeyPressed(sf::Keyboard::Key::R))
            wave.ResetSimulation();
		
        auto mousePos = renderer.GetMousePosition();
        int i, j;
        // Clamp i and j to be within the bounds of the wave 
        if (mousePos.x < 0) i = 0;
        else if (mousePos.x >= width) i = wave.GetWidth() - 1;
        else i = mousePos.x * wave.GetWidth() / width;
        if (mousePos.y < 0) j = 0;
        else if (mousePos.y >= height) j = wave.GetHeight() - 1;
        else j = mousePos.y * wave.GetHeight() / height;


        if (renderer.IsMousePressed(sf::Mouse::Button::Left)) {
			wave.Disturb(i, j, 1.0f, 5);
		}

        count++;
        if (renderer.IsKeyPressed(sf::Keyboard::Key::Space)) {
            wave.DisturbLeftSide(sin(count * dt * 10));
            toBeRestored = 3;
		}
		else if (toBeRestored != 0){
			wave.DisturbLeftSide(0.0f);
            toBeRestored--;
		}




        /*if (renderer.IsMousePressed(sf::Mouse::Button::Left)) {
            auto mousePos = renderer.GetMousePosition();
            int index = static_cast<int>(mousePos.x / (800.0f / wave.GetSize()));
            float amplitude = (renderer.GetHeight()/2.0f - mousePos.y) / renderer.GetHeight() * 2.5f;
            wave.Disturb(index, amplitude);
        }
        if (renderer.IsKeyPressed(sf::Keyboard::Key::Space)) {
            left_height += 0.5f * (1.0f - left_height);
        }
        else {
            left_height -= 0.5f * (1.01f - left_height);
            if (left_height < 0.0f) left_height = 0.0f;
        }*/

        //wave.SetTargetLeft(left_height); // Set the target wave height



        // Timer to measure frame time
        auto start = std::chrono::high_resolution_clock::now();

        // 1. Simulation Step
        auto sim_start = std::chrono::high_resolution_clock::now();
        wave.SimulateStep();
        auto sim_end = std::chrono::high_resolution_clock::now();
        auto sim_duration = std::chrono::duration<float>(sim_end - sim_start).count();
        

        // 2. Rendering
        auto render_start = std::chrono::high_resolution_clock::now();
        
        renderer.Render(wave, i, j, wave.GetWaveState()[wave.Index(i,j)]);
        auto render_end = std::chrono::high_resolution_clock::now();
        auto render_duration = std::chrono::duration<float>(render_end - render_start).count();

        // Calculate total frame time
        auto end = std::chrono::high_resolution_clock::now();
        auto frame_duration = std::chrono::duration<float>(end - start).count();


        total_sim_time += sim_duration;
        total_render_time += render_duration;





        if (count % 100 == 0) {
            std::cout << "average FPS: " << count / clock.getElapsedTime().asSeconds() << std::endl;
            std::cout << "average simulation time: " << total_sim_time / count << " seconds" << std::endl;
            std::cout << "average render time: " << total_render_time / count << " seconds" << std::endl;
            std::cout << "Max amplitude: " << wave.GetMaxAmplitude() << std::endl;
            std::cout << "Min amplitude: " << wave.GetMinAmplitude() << std::endl;
        }




	}


}
