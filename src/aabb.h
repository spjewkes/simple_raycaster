#ifndef __AABB_H__
#define __AABB_H__

#include <ostream>
#include "vector2d.h"
#include "ray.h"

/**
 * CLass for axis-aligned bounding box (AABB) type.
 */
class AABB
{
public:
	friend std::ostream& operator<<(std::ostream &, const AABB &);

	/// Constructors.
	AABB() {}
	AABB(Vector2d &min, Vector2d &max) :
		m_min(std::min(min.x(), max.x()), std::min(min.y(), max.y())),
		m_max(std::max(min.x(), max.x()), std::max(min.y(), max.y())) {}

	/// Destructors.
	~AABB() {}

	/// Copy constructor.
	AABB(const AABB& rhs);

	/// Overridden operators.
	AABB& operator=(const AABB& rhs);

	/// Set and get operators.
	Vector2d& min() { return m_min; }
	Vector2d& max() { return m_max; }

	/// Test for collision
	bool is_hit(Ray &ray, Vector2d &hit) const;

private:
	/// Properties of AABB.
	Vector2d m_min;
	Vector2d m_max;
};

std::ostream& operator<<(std::ostream &out, const AABB &rhs);

#endif
