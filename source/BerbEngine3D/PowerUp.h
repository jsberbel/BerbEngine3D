#pragma once
#include "GameObject.h"
#include "GLCamera.h"

namespace SerraPlo {

#define MAX_POWERUPS 4
#define SHELL_STUN_DISTANCE 5.0f

	class ShaderProgram;

	struct PowerUp : GameObject {
		float lifeTimeCounter = 0;
		virtual void Init(glm::vec3 *pos, glm::vec3 *front) = 0;
		virtual void Activate(void) = 0;
		virtual void Update(float dt) = 0;
		virtual void Draw(ShaderProgram & program, GLCamera & camera) = 0;
		virtual ~PowerUp() = default;
	};

#define GREEN_SHELL_STUN_DELAY 1000
#define GREEN_SHELL_LIFETIME_DELAY 4000

	struct GreenShell : PowerUp {
		glm::vec3 *carPos;
		glm::vec3 *carFront;
		glm::vec3 front;
		float speed = 70.0f; ///TODO: change to player car trully speed
		void Init(glm::vec3 *pos, glm::vec3 *front) override;
		void Activate(void) override;
		void Update(float dt) override;
		void Draw(ShaderProgram & program, GLCamera & camera) override;
	};

#define RED_SHELL_STUN_DISTANCE 5.0f
#define RED_SHELL_STUN_DELAY 1000
#define PATH_DISTANCE_DETECTION 10.0f

	struct RedShell : PowerUp {
		glm::vec3 *carPos;
		glm::vec3 *carFront;
		glm::vec3 front;
		float speed = 70.0f; ///TODO: change to player car trully speed
		void Init(glm::vec3 *pos, glm::vec3 *front) override;
		void Activate(void) override;
		void Update(float dt) override;
		void Draw(ShaderProgram & program, GLCamera & camera) override;
		std::vector<glm::vec2> *pathRef;
		void AddPath(std::vector<glm::vec2> *pathR) { pathRef = pathR; }
		int curPathNode = 0;
		std::vector<glm::vec3*> karts;
		void AddKarts(std::vector<glm::vec3*> k) { karts = k; }
	};

	struct Banana : PowerUp {
		glm::vec3 *carPos;
		glm::vec3 *carFront;
		glm::vec3 front;
		float speed = 70.0f; ///TODO: change to player car trully speed
		void Init(glm::vec3 *pos, glm::vec3 *front) override;
		void Activate(void) override;
		void Update(float dt) override;
		void Draw(ShaderProgram & program, GLCamera & camera) override;
	};

#define BULLET_BILL_LIFETIME_DELAY 5000

	struct BulletBill : PowerUp {
		Transform *carTransform;
		glm::vec3 *carFront;
		glm::vec3 front;
		bool *carEnabled;
		float speed = 70.0f; ///TODO: change to player car trully speed
		void Init(glm::vec3 *pos, glm::vec3 *front) override;
		void Activate(void) override;
		void Update(float dt) override;
		void Draw(ShaderProgram & program, GLCamera & camera) override;
		std::vector<glm::vec2> *pathRef;
		void AddPath(std::vector<glm::vec2> *pathR) { pathRef = pathR; }
		int curPathNode = 0;
	};

}
