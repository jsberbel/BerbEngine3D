#pragma once 
#include "Transform.h"
#include "GLModel.h"
#include <string>

namespace brb {

	struct GameObject {
		Transform transform;
		GLModel *model;
		bool enabled{ true };
	};

	struct DebugObject {
		Transform transform;
		glm::vec3 color{ 0,0,0 };
		GLModel *model;
	};

}