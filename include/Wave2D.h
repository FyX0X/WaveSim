// include/Wave2D.h
#pragma once
#include <vector>
#include <array>
#include <iostream>


class Wave2D
{
public:
	Wave2D(float dt, float dx, float c, float damping, int width, int height);
	~Wave2D();

	void SimulateStep();


	void DisturbLeftSide(float amplitude);
	void Disturb(int x, int y, float amplitude, int radius);

	// Getters
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	const std::vector<float>& Wave2D::GetWaveState() const { return u[current_step % 3]; }
	float GetMaxAmplitude() const { return max_amplitude; }
	float GetMinAmplitude() const { return min_amplitude; }
	const std::vector<bool>& GetWalls() const { return walls; }

	// Setters
	void SetSpeedField(const std::vector<float>& speed_field) { C2 = speed_field; }
	void SetWalls(const std::vector<bool>& walls) { this->walls = walls; }

	void ResetSimulation();


	inline int Index(int x, int y) const { return y * width + x; }
	
	

private:
	const float dt;
	const float dx;
	const float c2_constant;
	const float C2_constant;
	const float damping;
	const int width, height;
	int current_step = 0;
	float target_left = 0.0f;
	float max_amplitude = 0.0f;
	float min_amplitude = 0.0f;

	std::array<std::vector<float>, 3> u; // each element is a flat (width) * (height) grid
	std::vector<float> C2;
	std::vector<bool> walls;



};
