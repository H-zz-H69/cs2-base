// These are cheat offsets. If they are not from the latest update the cheat wont work!
// Here the 2 files u prob need when updating:
// offsets.hpp:		https://github.com/a2x/cs2-dumper/blob/main/output/offsets.hpp
// client_dll.hpp:	https://github.com/a2x/cs2-dumper/blob/main/output/client_dll.hpp
// Should always be upto date! if not then use: https://github.com/a2x/cs2-dumper/
// Tutorial for the github dumper: https://www.youtube.com/watch?v=AlzZ52FFdFQ (Weird ahh tutorial but working)
#pragma once

#include <cstddef>

namespace Offsets {
	// Changes every update:
	constexpr std::ptrdiff_t EntityList = 0x1A359B0;	// dwEntityList			(offsets.hpp)
	constexpr std::ptrdiff_t LocalPlayer = 0x1889F20;	// dwLocalPlayerPawn	(offsets.hpp)
	
	constexpr std::ptrdiff_t ViewMatrix = 0x1AA17B0;	// dwViewMatrix			(offsets.hpp)
	constexpr std::ptrdiff_t ViewAngles = 0x1AABA40;	// dwViewAngles			(offsets.hpp)

	// Dont change often:
	constexpr std::ptrdiff_t Player = 0x80C;			// m_hPlayerPawn		(client_dll.hpp)

	constexpr std::ptrdiff_t Name = 0x660;				// m_iszPlayerName		(client_dll.hpp)
	constexpr std::ptrdiff_t Health = 0x344;			// m_iHealth			(client_dll.hpp)
	constexpr std::ptrdiff_t Team = 0x3E3;				// m_iTeamNum			(client_dll.hpp)

	constexpr std::ptrdiff_t OldOrigin = 0x1324;		// m_vOldOrigin			(client_dll.hpp)

    constexpr std::ptrdiff_t SceneNode = 0x8;			// m_pSceneNode			(client_dll.hpp)
	constexpr std::ptrdiff_t ModelState = 0x170;		// m_modelState			(client_dll.hpp)

	constexpr std::ptrdiff_t ViewOffset = 0xCB0;		// m_vecViewOffset		(client_dll.hpp)

	constexpr std::ptrdiff_t m_fFlags = 0x3EC;
	constexpr std::ptrdiff_t m_iJump = 0x1855CE8;
	constexpr std::ptrdiff_t m_nTickBase = 0x628;
	constexpr std::ptrdiff_t normal_jump = 0x184E9E0;
}