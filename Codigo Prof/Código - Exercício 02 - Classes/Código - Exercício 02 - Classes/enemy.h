#pragma once

#include "collision_box.h"
#include <iomanip>

namespace my_game {

	enum class EnemyType { kSpaceShip, kSpaceMonster, kMeteor };

	class Enemy : public CollisionBox {
	 public:
		Enemy(float, float, float, float, EnemyType);
		void Move(float, float);
		void set_hide(bool);

		static int count_enemies_;

	 private:
		int index_;
		bool hide_;
		EnemyType type_of_enemy_;
	};

	int Enemy::count_enemies_ = 0;

	// posX: left
	// posY: bottom
	Enemy::Enemy(float posX, float posY, float sizeWidth, float sizeHeight, EnemyType type)
		: CollisionBox(posX, posY, sizeWidth, sizeHeight) {
		index_ = count_enemies_;
		hide_ = false;
		this->type_of_enemy_ = type;
		count_enemies_++;
	}

	void Enemy::Move(float speedX, float speedY) {
		std::cout << "[MOVE Enemy #" << index_ << "] ";
		std::cout << "Current Position = (" << x_center_ << "," << y_center_ << ")    ";

		x_center_ += speedX;
		y_center_ += speedY;
		
		std::cout << "Speed X:" << std::setw(3) << speedX << "Speed Y:" << std::setw(3) << speedY;
		std::cout << "    New Position = (" << x_center_ << "," << y_center_ << ")" << std::endl;
	}

	void Enemy::set_hide(bool hide) {
		hide_ = hide;
	}
}