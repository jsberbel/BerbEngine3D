#pragma once
#include <vector>
#include "Transform.h"
#include "CollisionManager.h"

namespace SerraPlo {
	
#define BRAKE_FORCE 100000.0f

	struct Config {
		float gravity = 9.81f;   // m/s^2
		float mass = 1200.0f; // kg
		float engineForce = 25000.0;
		float eBrakeForce = BRAKE_FORCE / 2.5f;
		float maxSteer = 1.0f;    // Maximum steering angle in radians
		float airResist = 5.5f;   // air resistance (* vel)
		float rollResist = 8.0f;   // rolling resistance force (* vel)
	};

	class CarPhysics {
	public:
		bool collActive;
		CollisionManager collisions;
		Transform *transform{ nullptr };
		glm::vec3 front;
		float collisionForce;
		glm::vec2 collisionDirection;
		int collisionCD = 0;
		float collisionCarForce;
		glm::vec2 collisionCarDirection;
		float accel = 0.0f;
		float velocity = 0.0f;
		bool smoothSteer = true;
		bool safeSteer = true;
		Config cfg;
		int throttle = 0;
		int brake = 0;
		int ebrake = 0;
		float steer = 0.0;
		float steerAngle = 0.0;
		glm::vec2 positionsCol[4];

		CarPhysics();
		~CarPhysics() = default;

		void AddTransform(Transform* _transform) { transform = _transform; }
		void Update(bool arrayK[5], float deltaTime, glm::vec2 colCarVector);
		glm::vec2 ColideCars(std::vector<glm::vec3> a);
	private:
		float applySmoothSteer(float steerInput, float dt);
		float applySafeSteer(float steerInput) const;
		void processInput(bool arrayK[5], float dt);
		void doPhysics(float deltaTime, glm::vec2 colCarVector);
	};

}