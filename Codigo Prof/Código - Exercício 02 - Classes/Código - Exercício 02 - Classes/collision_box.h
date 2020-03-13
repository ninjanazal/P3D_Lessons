#pragma once

#include "rectangle.h"
#include <algorithm>    // std::max

namespace my_game {

	class CollisionBox : public Rectangle {
	 public:
		CollisionBox(float x, float y, float w, float h) : Rectangle(x + w / 2, y + h / 2, w, h), hit_{ false } {};
		bool ComputeCollision(CollisionBox&);
		bool hit() { return hit_; };

	 private:
		bool hit_;	// Indica se colidiu ou não, mas não com o quê.
					// false = Não há colisão
					// true  = Há colisão
	};

	// Deteção de colisão AABB (Axis-Aligned Bounding Box)
	bool CollisionBox::ComputeCollision(CollisionBox& box) {
		float min_x = std::min<float>(x_center_ - width_ / 2.0f, box.x_center_ - box.width_ / 2.0f);
		float max_x = std::max<float>(x_center_ + width_ / 2.0f, box.x_center_ + box.width_ / 2.0f);
		float min_y = std::min<float>(y_center_ - height_ / 2.0f, box.y_center_ - box.height_ / 2.0f);
		float max_y = std::max<float>(y_center_ + height_ / 2.0f, box.y_center_ + box.height_ / 2.0f);

		if ((max_x - min_x < width_ + box.width_) && (max_y - min_y < height_ + box.height_)) {
			// Sinaliza a colisão nos dois objetos
			hit_ = true;
			box.hit_ = true; // objeto que entra como argumento na função

			std::cout << "BOOOOM!\n";
		}
		else {
			hit_ |= false;
		}

		return hit_;
	};
}