// src/Wave1D.cpp
#include "Wave1D.h"


Wave1D::Wave1D(float dt, float dx, float c, int max_steps, int size):
	dt(dt), dx(dx), c2(c* c),
	C2(c* c* dt* dt / (dx * dx)),
	max_steps(max_steps), size(size)
{
	// Verify C < 1 for numerical stability
	if (C2 >= 1.0f)
	{
		std::cout << "Warning: C = c*dt/dx^2 >= 1.0, numerical instability!" << std::endl;
		exit(1);
	}

	u = std::vector<std::vector<float>>(max_steps, std::vector<float>(size, 0.0f));


}

void Wave1D::SetInitialCondition(const std::vector<float>& initial_condition)
{
	if (initial_condition.size() != size)
	{
		std::cerr << "Error: Initial condition size does not match the wave size." << std::endl;
		return;
	}

	u[0] = initial_condition;



}



void Wave1D::Simulate()
{
	// First step: simple estimate (e.g., zero velocity)
	for (int i = 1; i < size - 1; ++i)
	{
		u[1][i] = u[0][i] + 0.5f * C2 * (u[0][i + 1] - 2 * u[0][i] + u[0][i - 1]);
	}

	// Apply boundary conditions at step 1
	u[1][0] = 0;
	u[1][size - 1] = 0;

	// Main loop
	for (int n = 1; n < max_steps - 1; ++n)
	{
		u[n + 1][0] = 0;
		u[n + 1][size - 1] = 0;

		for (int i = 1; i < size - 1; ++i)
		{
			u[n + 1][i] = C2 * (u[n][i + 1] - 2 * u[n][i] + u[n][i - 1]) + 2 * u[n][i] - u[n - 1][i];
		}
	}
}


Wave1D::~Wave1D()
{
}