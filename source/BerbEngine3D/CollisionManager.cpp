#include "CollisionManager.h"
#include <fstream>

namespace SerraPlo {

	static float projectionScalar(glm::vec2 a, glm::vec2 b) {
		float projection = ((b.x*a.x) + (b.y*a.y)) / (a.x*a.x + a.y*a.y);
		return (projection*a.x*a.x) + (projection *a.y*a.y);
	}

	void CollisionManager::InitStructures(std::string path) {
		std::ifstream myReadFile;
		myReadFile.open(path);
		if (myReadFile.is_open()) {
			char actual;
			std::string nBox;
			std::string nCircle;

			myReadFile.get(actual);
			while (actual != ' ') {
				nBox += actual;
				myReadFile.get(actual);
			} myReadFile.get(actual);
			while (actual != '\n') {
				nCircle += actual;
				myReadFile.get(actual);
			} myReadFile.get(actual);

			nBoxs = atoi(nBox.c_str());
			nCircles = atoi(nCircle.c_str());
			boxs = new Box[nBoxs];
			circles = new Circle[nCircles];

			std::string temp = "";
			for (int i = 0; i < nBoxs; i++) {
				for (int j = 0; j < 4; j++) {

					while (actual != '/') {
						temp += actual;
						myReadFile.get(actual);
					} myReadFile.get(actual);
					boxs[i].v[j].x = float(atof(temp.c_str()));
					temp = "";

					while (actual != '\n') {
						temp += actual;
						myReadFile.get(actual);
					} myReadFile.get(actual);
					boxs[i].v[j].y = float(atof(temp.c_str()));
					temp = "";
				}
				glm::vec2 vectorToCenter = (boxs[i].v[2] - boxs[i].v[0])*0.5f;
				boxs[i].ahorro.c = boxs[i].v[0] + vectorToCenter;
				boxs[i].ahorro.r = float(vectorToCenter.length());
			}
			for (int i = 0; i < nCircles; i++) {
				//center
				while (actual != '/') {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				circles[i].c.x = float(atof(temp.c_str()));
				temp = "";
				while (actual != '/') {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				circles[i].c.y = float(atof(temp.c_str()));
				temp = "";
				//rad
				glm::vec2 temporal;
				while (actual != '/') {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				temporal.x = float(atof(temp.c_str()));
				temp = "";
				while (actual != '\n' && !myReadFile.eof()) {
					temp += actual;
					myReadFile.get(actual);
				}myReadFile.get(actual);
				temporal.y = float(atof(temp.c_str()));
				temporal.x *= -1;
				temporal.y *= -1;
				circles[i].c.x *= -1;
				circles[i].c.y *= -1;
				temp = "";
				circles[i].r = glm::length(temporal - circles[i].c);
			}

			for (int i = 0; i < nBoxs; i++) {
				for (int j = 0; j < 4; j++) { boxs[i].v[j].y *= -1; boxs[i].v[j].x *= -1; }
				boxs[i].axis[0] = boxs[i].v[1] - boxs[i].v[0]; boxs[i].axis[1] = boxs[i].v[2] - boxs[i].v[1];
			}
		}
		myReadFile.close();

	}

	int CollisionManager::CalculateCollision(glm::vec2 v[4]) const {
		for (int i = 0; i < nBoxs; i++) {
			bool colliding = true;
			glm::vec2 axis[4];
			axis[0] = boxs[i].axis[0];
			axis[1] = boxs[i].axis[1];
			axis[2] = v[1] - v[0];
			axis[3] = v[2] - v[1];
			for (int j = 0; j < 4; j++) {
				float bMax; float oMax;
				float bMin; float oMin;
				if (colliding) {
					float temp[8];
					for (int k = 0; k < 8; k++) {
						if (k < 4)temp[k] = projectionScalar(axis[j], v[k % 4]);
						else temp[k] = projectionScalar(axis[j], boxs[i].v[k % 4]);
					}
					oMax = temp[0]; bMax = temp[4];
					oMin = temp[0]; bMin = temp[4];
					for (int k = 1; k < 4; k++) { if (temp[k] > oMax) oMax = temp[k]; if (temp[k] < oMin) oMin = temp[k]; }
					for (int k = 5; k < 8; k++) { if (temp[k] > bMax) bMax = temp[k]; if (temp[k] < bMin) bMin = temp[k]; }
					if (!(oMin <= bMax && oMin >= bMin || bMin <= oMax && bMin >= oMin)) {
						colliding = false;
					}
				}
			}
			if (colliding) return i;
		}
		for (int i = 0; i < nCircles; i++) {
			for (int j = 0; j < 4; j++) {
				if (glm::length(v[j] - circles[i].c) <= circles[i].r)return nBoxs + i;
			}
		}
		return -1;
	}

}