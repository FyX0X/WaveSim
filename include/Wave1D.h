// include/Wave1D.h
#pragma once
#include <vector>
#include <iostream>

class Wave1D
{
public:
	Wave1D(float dt, float dx, float c, int max_steps, int size);

	~Wave1D();

	void Simulate();

	// Getters
	const int GetMaxSteps() const { return max_steps; }
	const int GetSize() const { return size; }
	const std::vector<std::vector<float>>& Wave1D::GetWave() const { return u; }

	// Setters
	void SetInitialCondition(const std::vector<float>& initial_condition);

private:
	const float dt;
	const float dx;
	const float c2;
	const float C2;
	const int max_steps;
	const int size;
	std::vector<std::vector<float>> u;



};
