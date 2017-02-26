#pragma once

#include <glm/glm.hpp>
//#include <glm/glc/matrix_transform.hpp>

class View
{
public:
	View(glm::vec3 position, glm::vec3 up = glm::vec3(0.0, 1.0, 0.0));
	View(const View& other) = delete;
	View& operator=(const View& other) = delete;
	//View(View&& other);
	//View& operator=(View&& other);
	~View() = default;

	glm::mat4 getViewMatrix() const;
	glm::vec3 getUp() const;
	glm::vec3 getRight() const;
	glm::vec3 getDirection() const;

	void setPosition(glm::vec3 position);
	void setUp(glm::vec3 up);
	void setDirectionHorizontalAngle(float angle);
	void setDirectionVerticalAngle(float angle);
	void move(glm::vec3 translation);

	// カメラの上の方向に対して
	glm::vec3 rotateUpHorizontal(float angle);
	glm::vec3 rotateUpVertical(float angle);

	// 注視点の方向に対して
	glm::vec3 rotateDirectionHorizontal(float angle);
	glm::vec3 rotateDirectionVertical(float angle);
private:
	//glm::mat4 viewMatrix_;

	glm::vec3 position_;
	// カメラからの見ている場所
	glm::vec3 direction_;
	glm::vec3 up_;
	glm::vec3 right_;
	// z 軸方向に対して
	float horizontalAngle_;
	float verticalAngle_;

	void calcDirection();
};
