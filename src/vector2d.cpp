#include <math.h>
#include "vector2d.h"

/**
 * Copy constructor
 */
Vector2d::Vector2d(const Vector2d& rhs)
{
	if (this == &rhs)
	{
		return;
	}
	
	m_x = rhs.m_x;
	m_y = rhs.m_y;
}

/**
 * Operators.
 */
Vector2d& Vector2d::operator=(const Vector2d& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_x = rhs.m_x;
	m_y = rhs.m_y;
	
	return *this;
}

Vector2d Vector2d::operator+(const Vector2d& rhs) const
{
	return Vector2d(m_x + rhs.m_x, m_y + rhs.m_y);
}

Vector2d Vector2d::operator-(const Vector2d& rhs) const
{
	return Vector2d(m_x - rhs.m_x, m_y - rhs.m_y);
}

Vector2d Vector2d::operator*(const float scale) const
{
	return Vector2d(m_x * scale, m_y * scale);
}

Vector2d Vector2d::operator/(const float scale) const
{
	if (scale == 0.0f)
	{
		return Vector2d(0.0f, 0.0f);
	}
	else
	{
		return Vector2d(m_x / scale, m_y / scale);
	}
}

Vector2d& Vector2d::operator+=(const Vector2d& rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	return *this;
}

Vector2d& Vector2d::operator*=(const float scale)
{
	m_x *= scale;
	m_y *= scale;
	return *this;
}

Vector2d& Vector2d::operator/=(const float scale)
{
	if (scale == 0.0f)
	{
		m_x = 0.0f;
		m_y = 0.0f;
	}
	else
	{
		m_x /= scale;
		m_y /= scale;
	}
	
	return *this;
}

Vector2d Vector2d::operator-() const
{
	return Vector2d(-m_x, -m_y);
}

bool Vector2d::operator==(const Vector2d& rhs) const
{
	if (this == &rhs)	return true;
	
	if ((m_x != rhs.m_x) ||
		(m_y != rhs.m_y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector2d::operator!=(const Vector2d& rhs) const
{
	if (this == &rhs)	return false;
	
	if ((m_x == rhs.m_x) &&
		(m_y == rhs.m_y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector2d::operator<(const Vector2d& rhs) const
{
	if (this == &rhs)	return false;
	
	if ((m_x >= rhs.m_x) ||
		(m_y >= rhs.m_y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector2d::operator<=(const Vector2d& rhs) const
{
	if (this == &rhs)	return true;
	
	if ((m_x > rhs.m_x) ||
		(m_y > rhs.m_y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector2d::operator>(const Vector2d& rhs) const
{
	if (this == &rhs)	return false;
	
	if ((m_x <= rhs.m_x) ||
		(m_y <= rhs.m_y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Vector2d::operator>=(const Vector2d& rhs) const
{
	if (this == &rhs)	return true;
	
	if ((m_x < rhs.m_x) ||
		(m_y < rhs.m_y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * Get length of vector.
 */
float Vector2d::length() const
{
	return sqrtf(m_x * m_x + m_y * m_y);	
}

/**
 * Get squared length of vector.
 */
float Vector2d::length2() const
{
	return m_x * m_x + m_y * m_y;
}

/**
 * Normalize the vector.
 */
float Vector2d::normalize()
{
	float len = length();
	
	if (len > 0.0f)
	{
		float invLength = 1.0f / len;
		m_x *= invLength;
		m_y *= invLength;
	}
	else
	{
		m_x = 0.0f;
		m_y = 0.0f;
	}
	
	return len;
}

std::ostream& operator<<(std::ostream &out, const Vector2d &rhs)
{
    out << "(" << rhs.m_x << "," << rhs.m_y << ")";
    return out;
}
