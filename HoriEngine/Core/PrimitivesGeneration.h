#pragma once

#include <vector>
#include <numbers>

inline std::vector<float> generateCircleVertices(float radius, int segments)
{
	std::vector<float> vertices;
	vertices.reserve((segments + 1) * 2);

	for (int i = 0; i <= segments; i++)
	{
		float angle = 2.0f * std::numbers::pi_v<float> * i / segments;
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		vertices.push_back(x);
		vertices.push_back(y);
	}

	return vertices;
}