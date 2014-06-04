#ifndef INCLUDE_POTION_H
#define INCLUDE_POTION_H

#include "DynamicEntity.h"

/**
* Class for the bomb potion.
*/
class Potion : public DynamicEntity {
	public:
		/**
		* The constructor.
		*/
		Potion(const double x_, const double y_, Sprite* const sprite_, const int strength_, const int distance_, const bool isRight_);

		/**
		* The destructor.
		*/
		virtual ~Potion();

		/**
		*
		*/
		virtual void update(const double dt_);

		/**
		*
		*
		*/
		virtual void render(const double cameraX_, const double cameraY_);

		bool activated;

	private:
		/**
		*/
		virtual void handleCollision(std::array<bool, CollisionSide::SOLID_TOTAL> detections_);

		int strength;
		int distance;
		double flightTime;

};

#endif // INCLUDE_BOMBPOTION_H
