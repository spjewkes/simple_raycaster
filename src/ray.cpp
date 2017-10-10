#include "ray.h"

/**
 * Copy constructor.
 */
Ray::Ray(const Ray& rhs)
{
	if (this == &rhs)
	{
		return;
	}

	m_origin = rhs.m_origin;
	m_dir = rhs.m_dir;
}

/**
 * Operators.
 */
Ray& Ray::operator=(const Ray& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_origin = rhs.m_origin;
	m_dir = rhs.m_dir;

	return *this;
}

std::ostream& operator<<(std::ostream &out, const Ray &rhs)
{
	out << "( O: " << rhs.m_origin << ", D: " << rhs.m_dir << " )";
	return out;
}
