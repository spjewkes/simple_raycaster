#ifndef __AABB_H__
#define __AABB_H__

#include <ostream>
#include "vector2d.h"

/**
 * CLass for axis-aligned bounding box (AABB) type.
 */
class AABB
{
public:
	friend std::ostream& operator<<(std::ostream &, const AABB &);

	/// Constructors.
	AABB() {}
	AABB(Vector2d &min, Vector2d &max) : m_min(min), m_max(max) {}

	/// Destructors.
	~AABB() {}

	/// Copy constructor.
	AABB(const AABB& rhs);

	/// Overridden operators.
	AABB& operator=(const AABB& rhs);

	/// Set and get operators.
	Vector2d& min() { return m_min; }
	Vector2d& max() { return m_max; }
	void min(const Vector2d& min) { m_min = min; }
	void max(const Vector2d& max) { m_max = max; }

private:
	/// Properties of AABB.
	Vector2d m_min;
	Vector2d m_max;
};

std::ostream& operator<<(std::ostream &out, const AABB &rhs);

#endif
