// include/Wave1DInteract.h
#pragma once
#include <vector>
#include <iostream>

class Wave1DInteract
{
public:
	Wave1DInteract(float dt, float dx, float c, float damping, int size);

	~Wave1DInteract();

	void SimulateStep();

	void Disturb(int index, float amplitude);

	// Getters
	int GetSize() const { return size; }
	const std::vector<float>& Wave1DInteract::GetWaveState() const { return u[current_step%3]; }

	// Setters
	void SetTargetLeft(float target) { target_left = target; }

private:
	const float dt;
	const float dx;
	const float c2;
	const float C2;
	const float damping;
	const int size;
	int current_step = 0;
	float target_left = 0.0f;
	std::vector<std::vector<float>> u;



};
