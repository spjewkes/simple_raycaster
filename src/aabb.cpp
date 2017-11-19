#include <cmath>
#include <cfloat>
#include "aabb.h"

/**
 * Copy constructor.
 */
AABB::AABB(const AABB& rhs)
{
	if (this == &rhs)
	{
		return;
	}

	m_min = rhs.m_min;
	m_max = rhs.m_max;
}

/**
 * Operators.
 */
AABB& AABB::operator=(const AABB& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_min = rhs.m_min;
	m_max = rhs.m_max;

	return *this;
}

bool AABB::is_hit(Ray &ray, Vector2d &hit) const
{
	float half[2] = { std::fabs((m_max.x() - m_min.x()) / 2.0f),
					  std::fabs((m_max.y() - m_min.y()) / 2.0f) };
	
	float t_min = FLT_MIN;
	float t_max = FLT_MAX;

	Vector2d p = ((m_max - m_min) / 2.0f) - ray.origin();

	for(int i=0; i<2; i++)
	{
		float e = p[i];
		float f = ray.dir()[i];

		if (std::fabs(f) > FLT_EPSILON)
		{
			float t1 = (e + half[i]) / f;
			float t2 = (e - half[i]) / f;

			if (t1 > t2)
			{
				std::swap(t1, t2);
			}

			if (t1 > t_min)
			{
				t_min  = t1;
			}

			if (t2 < t_max)
			{
				t_max = t2;
			}

			if (t_min > t_max)
			{
				return false;
			}

			if (t_max < 0.0f)
			{
				return false;
			}

		}
		else if ((-e - half[i] > 0.0f) || (-e + half[i] < 0))
		{
			return false;
		}
	}

	// If we get here then there is an intersection

	if (t_min > 0)
	{
		hit = ray.origin() + (ray.dir() * t_min);
	}
	else
	{
		hit = ray.origin() + (ray.dir() * t_max);
	}

	return true;
}

std::ostream& operator<<(std::ostream &out, const AABB &rhs)
{
	out << "( MIN: " << rhs.m_min << ", MAX: " << rhs.m_max << " )";
	return out;
}
