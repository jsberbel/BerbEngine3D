#include "AIPhysics.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <ctime>

#define PATH_DISTANCE_DETECTION 10.0f

namespace SerraPlo {

	static inline glm::vec2 DoKinematicSeek(glm::vec2 targetPosition, glm::vec2 agentPosition, float agentMaxSpeed) {
		glm::vec2 desiredVelocity = glm::normalize(targetPosition - agentPosition); // Calculate desired velocity
		desiredVelocity *= agentMaxSpeed; // Scale by agentMaxSpeed
		return desiredVelocity;
	}

	static inline glm::vec2 DoSteeringSeek(glm::vec2 targetPosition, glm::vec2 agentPosition, glm::vec2 agentSpeed, float agentMaxSpeed, float agentMaxForce) {
		glm::vec2 desiredVelocity = DoKinematicSeek(targetPosition, agentPosition, agentMaxSpeed); // Calculate desired velocity		
		glm::vec2 steeringForce = desiredVelocity - agentSpeed; // Calculate Steering Force
		steeringForce /= agentMaxSpeed; // Divide by agentMaxSpeed to get the speed factor
		steeringForce *= agentMaxForce; // Scale this factor by agentMaxForce
		return steeringForce;
	}

	bool AIPhysics::carColCalc(glm::vec3 a, glm::vec3 b) {
		float rad = 5.0f;
		if (glm::length(a - b) <= rad) return true;
		else return false;
	}

	void AIPhysics::Update(float deltaTime){
		glm::vec2 steeringForce{};
		glm::vec2 acceleration{};
		float angle{ .0f };
		glm::vec3 posP = glm::vec3(0.0f,0.0f,0.0f);
		if (playerOn) posP = aiCarArray[0].playerRef->position;
		for (size_t i = 0; i < aiCarArray.size(); i++) aiCarArray[i].collisionCar = -1;
		for (size_t i = 0; i < aiCarArray.size(); i++) {
			glm::vec3 posI = aiCarArray[i].transformRef->position;
			if (carColCalc(posI, posP)) aiCarArray[i].collisionCar = -10;
			else if(aiCarArray[i].collisionCar == -1){
				for (size_t j = 0; j < aiCarArray.size()-i; j++) {
					if (i != j && carColCalc(posI, aiCarArray[j].transformRef->position)) {
						aiCarArray[i].collisionCar = j; aiCarArray[j].collisionCar = i;
						break;
					}
					else aiCarArray[i].collisionCar = -1;
				}
			}
		}
		for (auto &aiCar : aiCarArray) {
			if (!(*aiCar.stunned)) {
				glm::vec2 positionIA = { aiCar.transformRef->position.x, aiCar.transformRef->position.z };
				glm::vec2 newPos = { aiCar.transformRef->position.x, aiCar.transformRef->position.z };
				glm::vec2 targetSegment = aiPathRef->at(aiCar.curPathNode); // Find Current segment to target
				if (glm::distance(positionIA, targetSegment) < PATH_DISTANCE_DETECTION) { // Are we near enough targetSegment
					if (!aiCar.curPathNode) ++(*aiCar.laps);
					++aiCar.curPathNode; ++(*aiCar.pointsDone);  // Update targetSegment and car score
					if (aiCar.curPathNode >= int(aiPathRef->size())) aiCar.curPathNode = 0;
				}

				if (glm::distance(positionIA, { 140, 110 }) < 30.0f && boxOn) {//recollir caixa
					steeringForce = DoSteeringSeek({ 140, 110 }, positionIA, aiCar.speed, aiCar.maxSpeed, aiCar.maxSteerForce);
				}else{//esquivar
					bool flee = false;
					static glm::vec2 fleePos = { 0.0f,0.0f };
					if (playerOn) {
						for (auto &aiCarro : aiCarArray) {
							if (!flee && glm::distance(positionIA, { aiCarro.transformRef->position.x,aiCarro.transformRef->position.z }) < 50.0f) {
								glm::vec2 temporalPos = positionIA - (glm::vec2(aiCarro.transformRef->position.x, aiCarro.transformRef->position.z)- positionIA);
								if (glm::distance(positionIA, targetSegment) >= glm::distance(temporalPos, targetSegment)) {
									flee = true;
									fleePos = { aiCarro.transformRef->position.x,aiCarro.transformRef->position.z };
									break;
								}
							}
						}
					}
					if (flee) {
						steeringForce = DoSteeringSeek(fleePos, positionIA, aiCar.speed, aiCar.maxSpeed, aiCar.maxSteerForce)*-1.0f;
					}
					steeringForce = DoSteeringSeek(targetSegment, positionIA, aiCar.speed, aiCar.maxSpeed, aiCar.maxSteerForce);//seguir cami
				}
				

				acceleration = (steeringForce / aiCar.mass);
				aiCar.speed += acceleration * deltaTime*0.1f;
				newPos += aiCar.speed;
				//std::cout << aiCar.speed.x << ", " << aiCar.speed.y << std::endl;
				glm::vec2 direction = glm::normalize(aiCar.speed);
				angle = float(atan2f(direction.y, -direction.x) * RAD2DEG) - 90.0f;
				if (glm::length(angle) > 1.0f) aiCar.transformRef->rotation.y = angle;
				//detect collisions and correct it
				if (aiCar.collisionForce > 0.0f) aiCar.collisionForce -= 100.0f*deltaTime;
				else aiCar.collisionForce = 0.0f;
				if (aiCar.collisionCarForce > 0.0f) aiCar.collisionCarForce -= 100.0f*deltaTime;
				else aiCar.collisionCarForce = 0.0f;
				glm::vec2 pFront = glm::vec2(-direction.y, direction.x);
				if (aiCar.collisionCar >= 0) {
					aiCar.speed /= 10.0f;
					aiCar.collisionCarDirection = glm::normalize(positionIA - glm::vec2(aiCarArray[aiCar.collisionCar].transformRef->position.x, aiCarArray[aiCar.collisionCar].transformRef->position.z));
					aiCar.collisionCarForce = 40.0f;
				} else if (aiCar.collisionCar == -10) {//player
					aiCar.speed /= 10.0f;
					aiCar.collisionCarDirection = glm::normalize(positionIA - glm::vec2(aiCar.playerRef->position.x, aiCar.playerRef->position.z));
					aiCar.collisionCarForce = 40.0f;
				}
				glm::vec2 newPosT = newPos + aiCar.collisionDirection*aiCar.collisionForce*deltaTime;
				positionsCol[0] = newPosT + direction*2.0f + pFront*1.25f;	positionsCol[1] = newPosT + direction*2.0f - pFront*1.25f;
				positionsCol[2] = newPosT + direction*2.0f + pFront*1.25f;	positionsCol[3] = newPosT + direction*2.0f - pFront*1.25f;
				int i = collisions.CalculateCollision(positionsCol);
				if (i != -1) {
					aiCar.collisionCarForce = 0.0f;
					if (i < collisions.nBoxs) {
						aiCar.speed == glm::vec2(0.0f, 0.0f);
						aiCar.collisionDirection = glm::normalize(positionIA - newPosT);
						aiCar.collisionForce = 40.0f;
					} else {
						aiCar.speed /= 2.0f;
						aiCar.collisionDirection = glm::normalize(newPosT - collisions.circles[i - collisions.nBoxs].c);
						aiCar.collisionForce = 40.0f;
					}
				}
				aiCar.transformRef->position = { (newPos + (aiCar.collisionDirection*aiCar.collisionForce + aiCar.collisionCarDirection*aiCar.collisionCarForce)*deltaTime).x,
					0.0f,(newPos + (aiCar.collisionDirection*aiCar.collisionForce + aiCar.collisionCarDirection*aiCar.collisionCarForce)*deltaTime).y };
			} else {
				aiCar.transformRef->rotation.y = float((clock()) % 360);
			}
		}
	}
}
