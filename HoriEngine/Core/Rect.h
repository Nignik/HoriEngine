#pragma once

#include <glm/glm.hpp>

struct Rect
{
	float x, y;
	float w, h;

	bool Contains(glm::vec2& p) const
	{
		return ((p.x >= x) && (p.x < (x + w)) &&
			(p.y >= y) && (p.y < (y + h))) ? true : false;
	}

	bool Intersects(const Rect& other) const
	{
		if (x + w < other.x)         return false;
		if (x > other.x + other.w)   return false;
		if (y + h < other.y)         return false;
		if (y > other.y + other.h)   return false;

		return true;
	}
};