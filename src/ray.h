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
	Ray(Vector2d &origin, Vector2d &dir) : m_origin(origin), m_dir(dir) {}

	/// Destructors.
	~Ray() {}

	/// Copy constructor.
	Ray(const Ray& rhs);

	/// Overridden operators.
	Ray& operator=(const Ray& rhs);

	/// Set and get operators.
	Vector2d& origin() { return m_origin; }
	Vector2d& dir() { return m_dir; }
	void origin(const Vector2d& origin) { m_origin = origin; }
	void dir(const Vector2d& dir) { m_dir = dir; }

	/// Get length of ray.
	float length() const { return m_dir.length(); }

	/// Get squared length of ray.
	float length2() const { return m_dir.length2(); }

private:
	/// Properties of ray.
	Vector2d m_origin;
	Vector2d m_dir;
};

std::ostream& operator<<(std::ostream &out, const Ray &rhs);

#endif
