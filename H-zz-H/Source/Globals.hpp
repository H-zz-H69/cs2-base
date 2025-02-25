
#pragma once

namespace Globals {
	namespace Interface {
		inline bool Open = false;
	}

	namespace ESP {
		inline bool Enabled = false;

		inline bool Box = false;

		inline bool Name = false;
		inline bool Health = false;
		inline bool HealthText = false;
		inline bool Arrows = false;
		inline bool ArrowAsTriangle = false;
		inline bool Crosshair = false;
		inline bool WorldColor = false;
		inline bool TeamCheck = true;
		inline float DarknessLevel = 0.5f;


	}

	namespace Chams {
		inline bool Enable = false;
		inline bool VisibleChams = false;
		inline bool InvisibleChams = false;
		inline bool HealthChams = false;
		inline bool ClownChams = false;
	}

	namespace Aimbot {
		inline bool Enabled = false;

		inline float FOV = 30.0f;
		inline float Smoothing = 0.5f;

		inline bool DrawFOV = false;

	}

	namespace Misc {
		inline bool TeamCheck = false;
		inline bool Bunnyhop = false;
		inline bool Thirdperson = false;
		inline bool AntiOBS = false;
	}
}