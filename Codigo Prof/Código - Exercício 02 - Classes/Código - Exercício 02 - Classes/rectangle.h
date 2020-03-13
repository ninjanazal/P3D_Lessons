#pragma once

namespace my_game {

	class Rectangle {
	 public:
		Rectangle(float x_center = 0.0f, float y_center = 0.0f, float width = 0.0f, float height = 0.0f)
			: x_center_{ x_center }, y_center_{ y_center }, width_{ width }, height_{ height } {};

	 protected:
		float x_center_, y_center_;
		float width_, height_;
	};
}
