#ifndef Q3_GLM_CONVERSIONS_INLINE_
#define Q3_GLM_CONVERSIONS_INLINE_

#include <GLM/glm.hpp>
#include <q3.h>

inline glm::vec3 q2g(const q3Vec3 & in) {
	return glm::vec3(in.x, in.y, in.z);
}

inline q3Vec3 g2q(const glm::vec3 & in){
	return q3Vec3(in.x, in.y, in.z);
}

inline glm::mat4 q2g_mat3_2_mat4(const q3Mat3 & in){
	return glm::mat4(
		in.ex.v[0], in.ex.v[1], in.ex.v[2], 0,
		in.ey.v[0], in.ey.v[1], in.ey.v[2], 0,
		in.ez.v[0], in.ez.v[1], in.ez.v[2], 0,
		    0     ,    0      ,     0     , 1
		);
}

#endif // include guard