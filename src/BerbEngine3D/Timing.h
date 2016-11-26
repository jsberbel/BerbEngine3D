#pragma once

#define TIME FPSLimiter::Instance()

namespace brb {

	class FPSLimiter {
		~FPSLimiter() = default;

		float m_targetFPS;
		unsigned __int32 m_startTicks;
		unsigned __int32 m_frameTicks;

		void calculateFPS();
	public:
		static FPSLimiter& Instance() {
			static FPSLimiter instance;
			return instance;
		}

		float fps;
		float deltaTime;

		explicit FPSLimiter();
		explicit FPSLimiter(float targetFPS);

		void setTargetFPS(float targetFPS) { m_targetFPS = targetFPS; };
		void printFPS() const;

		void begin();
		void end();
	};

}

