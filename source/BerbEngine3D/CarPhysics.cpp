#include "CarPhysics.h"
#include "PathLoader.h"

namespace SerraPlo {

	// Math functions
	static float max(float a, float b) { return(a >= b) ? a : b; }
	static float min(float a, float b) { return(a <= b) ? a : b; }
	static float sign(float n) { return n = (n < 0.0f) ? -1.0f : 1.0f; }
	static float clamp(float n, float _min, float _max) { return min(max(n, _min), _max); }

	CarPhysics::CarPhysics() {
		collisions.InitStructures(GetPathToAsset("models/circuit_col/colisions.txt"));
		collActive = true;
		collisionDirection = { 0.0f,0.0f };
		collisionForce = 0.0f; 
		collisionCarDirection = glm::vec2(0.0f,0.0f);
		collisionCarForce = 0.0f;
	}

	float CarPhysics::applySmoothSteer(float steerInput, float dt) {
		if (abs(steerInput) > 0.001f) {
			steer = clamp(steer + steerInput * dt * 3.0f, -1.0f, 1.0f);
		} else {
			if (steer > 0.0f) steer = max(steer - dt * 3.0f, 0.0f);
			else if (steer < 0.0f)	steer = min(steer + dt * 3.0f, 0.0f);
		}return steer;
	};

	float CarPhysics::applySafeSteer(float steerInput) const {
		float avel = min(velocity, 60.0f);  // m/s
		float steer = steerInput * (1.0f - (avel / 1260.0f));
		return steer;
	};

	void CarPhysics::processInput(bool arrayK[5], float dt) {
		throttle = arrayK[0] ? 1 : 0;
		brake = arrayK[1] ? 1 : 0;
		ebrake = arrayK[4] ? 1 : 0;
		float steerInput = 0.0f;
		if (arrayK[2]) steerInput += -1.0f;
		if (arrayK[3]) steerInput += 1.0f;
		if (smoothSteer) steer = applySmoothSteer(steerInput, dt);
		else steer = steerInput;
		if (safeSteer)	steer = applySafeSteer(steer);
		steerAngle = steer * cfg.maxSteer;
	}

	void CarPhysics::doPhysics(float deltaTime, glm::vec2 colCarVector) {
		
		front = glm::vec3(sin((transform->rotation.y*M_PI) / 180), 0.0f, cos((transform->rotation.y*M_PI) / 180));

		float _brake = min(brake * BRAKE_FORCE + ebrake * cfg.eBrakeForce, BRAKE_FORCE);
		float _throttle = throttle * cfg.engineForce;

		float tractionForce = _throttle - _brake * sign(velocity);
		float dragForce = -cfg.rollResist * velocity - cfg.airResist * velocity * abs(velocity);

		float totalForce = dragForce + tractionForce;
		accel = totalForce / cfg.mass;  // forward/reverse accel

		velocity += accel * deltaTime;
		if (velocity < 0.5 && throttle == 0) velocity = 0.0f;
		if (velocity > 0.0f) transform->rotation.y += -(float((steerAngle*180.0f) / M_PI))*1.5f *deltaTime;

		if (collisionForce > 0.0f) collisionForce -= 100.0f*deltaTime;
		else collisionForce = 0.0f;

		if (colCarVector.x == 0.0f && colCarVector.y == 0.0f) {
			if (collisionCarForce > 0.0f) collisionCarForce -= 100.0f*deltaTime;
			else collisionCarForce = 0.0f;
		}else {
			collisionCarDirection = colCarVector;
			collisionCarForce = 40.0f;
		}
		glm::vec3 rawNewPos = transform->position + front*velocity*deltaTime;
		glm::vec2 newPos = glm::vec2(rawNewPos.x, rawNewPos.z) + (collisionDirection*collisionForce + collisionCarDirection*collisionCarForce)*deltaTime;
		glm::vec2 front2 = glm::normalize(glm::vec2(front.x, front.z));
		glm::vec2 pFront2 = glm::vec2(-front2.y, front2.x);
		positionsCol[0] = newPos + front2*2.0f + pFront2*1.25f;	positionsCol[1] = newPos + front2*2.0f - pFront2*1.25f;
		positionsCol[2] = newPos + front2*2.0f + pFront2*1.25f;	positionsCol[3] = newPos + front2*2.0f - pFront2*1.25f;
		//positionsCol[2] = newPos - front2*2.0f + pFront2*1.25f;	positionsCol[3] = newPos - front2*2.0f - pFront2*1.25f;
		//if (collActive) std::cout << true << std::endl;
		if (collisions.CalculateCollision(positionsCol) == -1 || !collActive) {
			transform->position = glm::vec3(newPos.x, 0.0f, newPos.y);
		}else{
			int i = collisions.CalculateCollision(positionsCol);
			if (i < collisions.nBoxs) {
				velocity = 0.0f;
				collisionDirection = glm::normalize(glm::vec2(transform->position.x,transform->position.z)-newPos);
				collisionForce = 40.0f;
			}else {
				velocity /= 2.0f;
				collisionDirection = glm::normalize(newPos - collisions.circles[i-collisions.nBoxs].c);
				collisionForce = 40.0f;
			}
			newPos = glm::vec2((transform->position + front* velocity *deltaTime).x, (transform->position + front* velocity *deltaTime).z) + collisionDirection*collisionForce*deltaTime;
			transform->position = glm::vec3(newPos.x, 0.0f, newPos.y);
		}
	}

	glm::vec2 CarPhysics::ColideCars(std::vector<glm::vec3> a){
		glm::vec2 directionVec = {0.00f,0.00f};
		float rad = 5.0f;
		for (size_t i = 0; i < a.size(); i++) {
			if (glm::length(a[i] - transform->position) <= rad) {
				directionVec = glm::normalize(glm::vec2((transform->position - a[i]).x, (transform->position - a[i]).z));
				//std::cout << "coliding with : " << i << std::endl;
				break;
			}
		}
		return directionVec;
	}

	void CarPhysics::Update(bool arrayK[5], float deltaTime, glm::vec2 colCarVector) {
		processInput(arrayK, deltaTime);
		doPhysics(deltaTime, colCarVector);
	}
}