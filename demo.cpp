#include <glm/gtx/euler_angles.hpp>

// 计算切线方向向量 tangent_vector

// 将切线方向向量转化为欧拉角
glm::vec3 euler_angles = glm::eulerAngles(glm::quat_cast(glm::mat3(glm::normalize(tangent_vector), binormal_vector, normal_vector)));

// 将弧度制转化为角度制
float roll = glm::degrees(euler_angles.x);
float yaw = glm::degrees(euler_angles.y);
float pitch = glm::degrees(euler_angles.z);
