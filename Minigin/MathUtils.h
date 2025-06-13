#pragma once
#include "glm.hpp"

namespace dae::MathUtils
{
	constexpr static bool CheckPointInSquare(const glm::ivec4& rect, const glm::ivec2& point) {
		return point.x >= rect.x &&
			point.y >= rect.y &&
			point.x < rect.x + rect.z &&
			point.y < rect.y + rect.w;
	}

	inline bool IsNearlyEqual(float a, float b, float tolerance = 0.001f) {
		return std::fabs(a - b) <= tolerance;
	}
	constexpr static glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t) {
		return a + (b - a) * t;
	}

	
}

namespace std {
	// hash specialization for glm::ivec2
	template <>
	struct hash<glm::ivec2> {
		size_t operator()(const glm::ivec2& v) const noexcept {
			size_t h1 = std::hash<int>{}(v.x);
			size_t h2 = std::hash<int>{}(v.y);
			return h1 ^ (h2 << 1);
		}
	};
}