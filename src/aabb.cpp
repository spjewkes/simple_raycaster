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

std::ostream& operator<<(std::ostream &out, const AABB &rhs)
{
	out << "( MIN: " << rhs.m_min << ", MAX: " << rhs.m_max << " )";
	return out;
}
