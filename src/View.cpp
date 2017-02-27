#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "View.hpp"

View::View(glm::vec3 position, glm::vec3 up)
	:position_(position), up_(up)
	,horizontalAngle_(acos(0.0) * 2.0), verticalAngle_(0.0)
{
	calcDirection();
	right_ = glm::normalize(glm::cross(direction_ , up_));
}

glm::mat4 View::getViewMatrix() const
{
	// right hand coordinate
	glm::vec3 const f(glm::normalize(direction_));
	glm::vec3 const s(glm::normalize(glm::cross(f, up_)));
	glm::vec3 const u(glm::cross(s, f));
	glm::mat4 ret(1);
	ret[0][0] = s.x;
	ret[0][0] = s.x;
	ret[1][0] = s.y;
	ret[2][0] = s.z;
	ret[0][1] = u.x;
	ret[1][1] = u.y;
	ret[2][1] = u.z;
	ret[0][2] =-f.x;
	ret[1][2] =-f.y;
	ret[2][2] =-f.z;
	ret[3][0] =-glm::dot(s, position_);
	ret[3][1] =-glm::dot(u, position_);
	ret[3][2] = glm::dot(f, position_);

	return ret;
	//return viewMatrix_;
}

glm::vec3 View::getUp() const
{
	return up_;
}

glm::vec3 View::getRight() const
{
	return right_;
}

glm::vec3 View::getDirection() const
{
	return direction_;
}

void View::setPosition(glm::vec3 position)
{
	position_ = position;
}

void View::setUp(glm::vec3 up)
{
	up_ = up;
}

void View::setDirectionHorizontalAngle(float angle)
{
	horizontalAngle_ = angle;
	calcDirection();
	right_ = glm::normalize(glm::cross(direction_, up_));
}

void View::setDirectionVerticalAngle(float angle)
{
	verticalAngle_ = angle;
	calcDirection();
	right_ = glm::normalize(glm::cross(direction_, up_));
}

void View::move(glm::vec3 translation)
{
	position_ += translation;
}

glm::vec3 View::rotateUpHorizontal(float angle)
{
	glm::vec3 up(glm::rotate(glm::mat4(), angle, -glm::normalize(direction_))
		* glm::vec4(up_, 1) * glm::vec4(glm::vec3(), 1));
	up_ = up;
	right_ = glm::normalize(glm::cross(direction_, up_));
	return up_;
}

glm::vec3 View::rotateUpVertical(float angle)
{
	glm::vec3 up(glm::rotate(glm::mat4(), angle, -glm::normalize(direction_))
		* glm::vec4(up_, 1) * glm::vec4(glm::vec3(), 1));
	up_ = up;
	right_ = glm::normalize(glm::cross(direction_, up_));
	return up_;
}

glm::vec3 View::rotateDirectionHorizontal(float angle)
{
	horizontalAngle_ += angle;
	calcDirection();
	right_ = glm::normalize(glm::cross(direction_, up_));
	return direction_;
}

glm::vec3 View::rotateDirectionVertical(float angle)
{
	verticalAngle_ += angle;
	calcDirection();
	right_ = glm::normalize(glm::cross(direction_, up_));
	return direction_;
}

void View::calcDirection()
{
	direction_ = glm::vec3(
		cos(verticalAngle_) * sin(horizontalAngle_),
		sin(verticalAngle_),
		cos(verticalAngle_) * cos(horizontalAngle_)
	);
}
