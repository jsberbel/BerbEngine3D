#pragma once 
#include "Transform.h"
#include "GLMesh.h"
#include "GLMaterial.h"
#include <string>

namespace SerraPlo {

	struct GameObject {
		Transform transform;
		GlobalMesh *meshRef;
		GlobalMaterial *materialRef;
		bool enabled{ true };
	};

	struct DebugObject {
		Transform transform;
		glm::vec3 color{ 0,0,0 };
		GlobalMesh *meshRef;
	};

}