#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include <ostream>

/**
 * Class for vector 2d type.
 */
class Vector2d
{
public:
    friend std::ostream& operator<<(std::ostream &, const Vector2d &);

	/// Constructors.
	Vector2d() : m_x(0.0f), m_y(0.0f) {}
	Vector2d(float x, float y) : m_x(x), m_y(y) {}

	/// Destructors.
	~Vector2d() {}

	/// Copy constructor
	Vector2d(const Vector2d& rhs);

	/// Overridden operators
	Vector2d& operator=(const Vector2d& rhs);
	Vector2d operator+(const Vector2d& rhs) const;
	Vector2d operator-(const Vector2d& rhs) const;
	Vector2d operator*(const float scale) const;
	Vector2d operator/(const float scale) const;
	Vector2d& operator+=(const Vector2d& rhs);
	Vector2d& operator-=(const Vector2d& rhs);
	Vector2d& operator*=(const float scale);
	Vector2d& operator/=(const float scale);
	Vector2d operator-() const;
	bool operator==(const Vector2d& rhs) const;
	bool operator!=(const Vector2d& rhs) const;
	bool operator<(const Vector2d& rhs) const;
	bool operator<=(const Vector2d& rhs) const;
	bool operator>(const Vector2d& rhs) const;
	bool operator>=(const Vector2d& rhs) const;
	float operator[](const int rhs) const;
	
	/// Set and get operators.
	float x() const { return m_x; }
	float y() const { return m_y; }
	void x(const float x) { m_x = x; }
	void y(const float y) { m_y = y; }
	
	/// Get length of vector.
	float length() const;
	
	/// Get squared length of vector.
	float length2() const;
	
	/// Normalize the vector.
	float normalize();
	
private:
	/// Properties of 2d vector
	float m_x;
	float m_y;
};

std::ostream& operator<<(std::ostream &out, const Vector2d &rhs);

#endif
