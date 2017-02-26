#include <glm/gtc/matrix_transform.hpp>
#include "Perspective.hpp"

Perspective::Perspective()
	:FoV_(45.0f), aspect_(4.0f / 3.0f)
	,nearClipping_(0.1f), farClipping_(100.0f)
{}

float Perspective::getFieldofView() const
{
	return FoV_;
}

float Perspective::getAspect() const
{
	return aspect_;
}

float Perspective::getNearClippingPlane() const
{
	return nearClipping_;
}

float Perspective::getFarClippingPlane() const
{
	return farClipping_;
}

glm::mat4 Perspective::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(FoV_), aspect_
		,nearClipping_, farClipping_);
	//return projectionMatrix_;
}

void Perspective::setFieldofView(float degree)
{
	FoV_ = degree;
}

void Perspective::setAspect(float aspect)
{
	aspect_ = aspect;
}

void Perspective::setNearClippingPlane(float src)
{
	nearClipping_ = src;
}

void Perspective::setFarClippingPlane(float src)
{
	farClipping_ = src;
}
