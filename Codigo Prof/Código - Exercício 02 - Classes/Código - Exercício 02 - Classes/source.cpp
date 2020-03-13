#include <iostream>
#include "enemy.h"

int main() {
	using namespace std;
	using namespace my_game;
	
	Enemy spaceship(500.0f, 2600.0f, 80.0f, 90.0f, EnemyType::kSpaceShip);
	Enemy meteor = Enemy(520.0f, 2700.0f, 50.0f, 50.0f, EnemyType::kMeteor);
	bool end_game = false;

	while (!end_game){
		cout << "Move:\n";
		//spaceship.Move(0.0f, -5.0f);
		spaceship.Move(0.0f, 0.0f);
		meteor.Move(0.0f, -10.0f);
		cout << "\nTest collision: ";
		bool hit = spaceship.ComputeCollision(meteor);
		end_game = (spaceship.hit() && meteor.hit() && hit);
		if (!end_game) cout << "No hit\n\n";
	}

	return 0;
}