// src/Wave1DInteract.cpp
#include "Wave1DInteract.h"


Wave1DInteract::Wave1DInteract(float dt, float dx, float c, float damping, int size) :
	dt(dt), dx(dx), c2(c* c),
	C2(c* c* dt* dt / (dx * dx)),
	damping(damping), size(size)
{
	// Verify C < 1 for numerical stability
	if (C2 >= 1.0f)
	{
		std::cout << "Warning: C = c*dt/dx^2 >= 1.0, numerical instability!" << std::endl;
		exit(1);
	}

	u = std::vector<std::vector<float>>(3, std::vector<float>(size, 0.0f));


}



void Wave1DInteract::SimulateStep()
{
	// Get time states vectors
	std::vector<float>& u_new = u[(current_step + 1) % 3];
	std::vector<float>& u_curr = u[current_step % 3];
	std::vector<float>& u_prev = u[(current_step + 2) % 3];

	// Fixed boundary
	u_new[0] = target_left;
	u_new[size - 1] = 0;

	for (int i = 1; i < size-1; ++i)
	{
		u_new[i] = C2 * (u_curr[i + 1] - 2 * u_curr[i] + u_curr[i - 1])
			+ (1 - damping) * ( 2 * u_curr[i] - u_prev[i] );
	}

	current_step = (current_step + 1) % 3;
	
}


void Wave1DInteract::Disturb(int index, float amplitude) {
	if (index >= 0 && index < size) {
		u[current_step % 3][index] = amplitude;
	}
}


Wave1DInteract::~Wave1DInteract()
{
}