#ifndef __RAY_H__
#define __RAY_H__

#include <ostream>
#include "vector2d.h"

/**
 * CLass for ray type.
 */
class Ray
{
public:
	friend std::ostream& operator<<(std::ostream &, const Ray &);

	/// Constructors.
	Ray() {}
	Ray(Vector2d &origin, Vector2d &dir) : m_origin(origin), m_dir(dir) { calc_end(); }

	/// Destructors.
	~Ray() {}

	/// Copy constructor.
	Ray(const Ray& rhs);

	/// Overridden operators.
	Ray& operator=(const Ray& rhs);

	/// Set and get operators.
	Vector2d& origin() { return m_origin; }
	Vector2d& dir() { return m_dir; }
	Vector2d& end() { return m_end; }
	void origin(const Vector2d& origin) { m_origin = origin; calc_end(); }
	void dir(const Vector2d& dir) { m_dir = dir; calc_end(); }

	/// Get length of ray.
	float length() const { return m_dir.length(); }

	/// Get squared length of ray.
	float length2() const { return m_dir.length2(); }

private:
	/// Calculate end point based on the other two values.
	void calc_end() { m_end = m_origin + m_dir; }

	/// Properties of ray.
	Vector2d m_origin;
	Vector2d m_dir;
	Vector2d m_end;
};

std::ostream& operator<<(std::ostream &out, const Ray &rhs);

#endif
