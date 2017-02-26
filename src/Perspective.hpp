#pragma once

#include <glm/glm.hpp>

class Perspective
{
public:
	Perspective();
	Perspective(const Perspective& other) = delete;
	Perspective& operator=(const Perspective& other) = delete;
	~Perspective() = default;

	float getFieldofView() const;
	float getAspect() const;
	float getNearClippingPlane() const;
	float getFarClippingPlane() const;
	glm::mat4 getProjectionMatrix() const;

	void setFieldofView(float degree);
	void setAspect(float aspect);
	void setNearClippingPlane(float src);
	void setFarClippingPlane(float src);
private:
	float FoV_;
	float aspect_;
	float nearClipping_;
	float farClipping_;
};
