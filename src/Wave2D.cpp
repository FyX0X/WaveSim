#include "Wave2D.h"
#include <cassert>

Wave2D::Wave2D(float dt, float dx, float c, float damping, int width, int height) :
	dt(dt), dx(dx), c2_constant(c* c),
	C2_constant(c* c* dt* dt / (dx * dx)),
	damping(damping), width(width), height(height)
{
	// Verify C < 1 for numerical stability
	if (C2_constant >= 1.0f)
	{
		std::cout << "Warning: C = c*dt/dx^2 >= 1.0, numerical instability!" << std::endl;
		exit(1);
	}

	for (auto& layer : u)
		layer.resize(width * height, 0.0f);

    // Initialize the speed field
	C2.resize(width * height, C2_constant);
	// Initialize the walls
	walls.resize(width * height, false);

}


void Wave2D::SimulateStep()
{
	// Get time states vectors
	std::vector<float>& u_new = u[(current_step + 1) % 3];
	std::vector<float>& u_curr = u[current_step % 3];
	std::vector<float>& u_prev = u[(current_step + 2) % 3];

	max_amplitude = 0.0f;
	min_amplitude = 0.0f;


	for (int i = 1; i < width - 1; ++i)
	{
		for (int j = 1; j < height - 1; ++j)
		{
			int idx = Index(i, j);

			if (walls[idx])
				continue; // Skip walls

			float value = C2[idx] * (
				u_curr[Index(i + 1, j)]
				+ u_curr[Index(i - 1, j)]
				+ u_curr[Index(i, j + 1)]
				+ u_curr[Index(i, j - 1)]
				- 4 * u_curr[idx])
				+ (1 - damping) * (2 * u_curr[idx] - u_prev[idx]);
			u_new[idx] = value;
			if (value > max_amplitude) max_amplitude = value;
			if (value < min_amplitude) min_amplitude = value;

		}
	}

	

	current_step = (current_step + 1) % 3;

}

void Wave2D::Disturb(int cx, int cy, float amplitude, int radius)
{
	int r2 = radius * radius;

	for (int i = cx - radius; i <= cx + radius; ++i)
	{
		for (int j = cy - radius; j <= cy + radius; ++j)
		{
			if (i > 0 && i < width - 1 && j > 0 && j < height - 1)
			{
				if (walls[Index(i, j)])
					continue; // Skip walls

				int dx = i - cx;
				int dy = j - cy;
				float dist2 = static_cast<float>(dx * dx + dy * dy);

				if (dist2 <= r2)
				{
					float falloff = std::exp(-dist2 / (2.0f * (radius * 0.5f) * (radius * 0.5f)));
					u[current_step % 3][Index(i,j)] = amplitude * falloff;
				}
			}
		}
	}
}


void Wave2D::DisturbLeftSide(float amplitude) {
	for (int j = 0; j < height; ++j) {
		u[current_step % 3][Index(0, j)] = amplitude;
	}
}


void Wave2D::ResetSimulation()
{
	for (auto& layer : u)
		std::fill(layer.begin(), layer.end(), 0.0f);
	current_step = 0;
	max_amplitude = 0.0f;
	min_amplitude = 0.0f;
}



Wave2D::~Wave2D()
{
	// Destructor
}
