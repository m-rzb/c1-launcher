#include <cstring>

#include "Library/OS.h"
#include "Library/StringTools.h"

#include "MemoryPatch.h"

static void* ByteOffset(void* base, std::size_t offset)
{
	return static_cast<unsigned char*>(base) + offset;
}

static void FillNop(void* base, std::size_t offset, std::size_t size)
{
	void* address = ByteOffset(base, offset);

	if (!OS::Hack::FillNop(address, size))
	{
		throw StringTools::OSError("Failed to apply patch at 0x%IX", reinterpret_cast<std::size_t>(address));
	}
}

static void FillMem(void* base, std::size_t offset, const void* data, std::size_t dataSize)
{
	void* address = ByteOffset(base, offset);

	if (!OS::Hack::FillMem(address, data, dataSize))
	{
		throw StringTools::OSError("Failed to apply patch at 0x%IX", reinterpret_cast<std::size_t>(address));
	}
}

////////////////////////////////////////////////////////////////////////////////
// CryAction
////////////////////////////////////////////////////////////////////////////////

/**
 * Allows connecting to DX10 servers with game running in DX9 mode.
 */
void MemoryPatch::CryAction::AllowDX9ImmersiveMultiplayer(void* pCryAction, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryAction, 0x2AF92D, 0x1E);
			FillNop(pCryAction, 0x2B24DD, 0x1A);
			break;
		}
		case 5879:
		{
			FillNop(pCryAction, 0x2AF6ED, 0x1E);
			FillNop(pCryAction, 0x2B239D, 0x1A);
			break;
		}
		case 6115:
		{
			FillNop(pCryAction, 0x2B349D, 0x1E);
			FillNop(pCryAction, 0x2B6361, 0x1A);
			break;
		}
		case 6156:
		{
			FillNop(pCryAction, 0x2B394D, 0x1E);
			FillNop(pCryAction, 0x2B6860, 0x1A);
			break;
		}
		case 6566:
		{
			FillNop(pCryAction, 0x2B06AD, 0x1E);
			FillNop(pCryAction, 0x2B3EAA, 0x16);
			break;
		}
		case 6586:
		{
			FillNop(pCryAction, 0x2B529D, 0x1E);
			FillNop(pCryAction, 0x2B7F7A, 0x16);
			break;
		}
		case 6627:
		{
			FillNop(pCryAction, 0x2B39FD, 0x1E);
			FillNop(pCryAction, 0x2B66DA, 0x16);
			break;
		}
		case 6670:
		{
			FillNop(pCryAction, 0x2B6F6D, 0x1E);
			FillNop(pCryAction, 0x2B9C21, 0x16);
			break;
		}
		case 6729:
		{
			FillNop(pCryAction, 0x2B6F3D, 0x1E);
			FillNop(pCryAction, 0x2B9BF1, 0x16);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryAction, 0x1D4ADA, 0x1A);
			FillNop(pCryAction, 0x1D6B03, 0x15);
			break;
		}
		case 5879:
		{
			FillNop(pCryAction, 0x1D4B0A, 0x1A);
			FillNop(pCryAction, 0x1D6B33, 0x15);
			break;
		}
		case 6115:
		{
			FillNop(pCryAction, 0x1D6EDA, 0x1A);
			FillNop(pCryAction, 0x1D8F32, 0x15);
			break;
		}
		case 6156:
		{
			FillNop(pCryAction, 0x1D698A, 0x1A);
			FillNop(pCryAction, 0x1D89FC, 0x15);
			break;
		}
		case 6527:
		{
			FillNop(pCryAction, 0x1D854A, 0x1A);
			FillNop(pCryAction, 0x1DA5BC, 0x15);
			break;
		}
		case 6566:
		{
			FillNop(pCryAction, 0x1F09AA, 0x1A);
			FillNop(pCryAction, 0x1F2DEC, 0x15);
			break;
		}
		case 6586:
		{
			FillNop(pCryAction, 0x1D81DA, 0x1A);
			FillNop(pCryAction, 0x1DA1CC, 0x15);
			break;
		}
		case 6627:
		{
			FillNop(pCryAction, 0x1D826A, 0x1A);
			FillNop(pCryAction, 0x1DA25C, 0x15);
			break;
		}
		case 6670:
		{
			FillNop(pCryAction, 0x1D9FCA, 0x1A);
			FillNop(pCryAction, 0x1DBFBC, 0x15);
			break;
		}
		case 6729:
		{
			FillNop(pCryAction, 0x1D9F6A, 0x1A);
			FillNop(pCryAction, 0x1DBF5C, 0x15);
			break;
		}
#endif
	}
}

/**
 * Disables automatic creation of "gameplaystatsXXX.txt" files.
 *
 * The "dump_stats" console command can still be used to create these files manually.
 */
void MemoryPatch::CryAction::DisableGameplayStats(void* pCryAction, int gameBuild)
{
#ifdef BUILD_64BIT
	const unsigned char code[] = {
		0xC3,  // ret
		0x90,  // nop
		0x90,  // nop
		0x90,  // nop
		0x90   // nop
	};
#endif

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillMem(pCryAction, 0x2F21D6, code, sizeof code);
			break;
		}
		case 5879:
		{
			FillMem(pCryAction, 0x2F59E6, code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCryAction, 0x2FA686, code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCryAction, 0x2FA976, code, sizeof code);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryAction, 0x2016ED, 0x7);
			break;
		}
		case 5879:
		{
			FillNop(pCryAction, 0x203EBD, 0x7);
			break;
		}
		case 6115:
		{
			FillNop(pCryAction, 0x20668D, 0x7);
			break;
		}
		case 6156:
		{
			FillNop(pCryAction, 0x20605D, 0x7);
			break;
		}
#endif
		case 6527:
		case 6566:
		case 6586:
		case 6627:
		case 6670:
		case 6729:
		{
			// Crysis Wars has no automatically created "gameplaystatsXXX.txt" files
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// CryGame
////////////////////////////////////////////////////////////////////////////////

/**
 * Disables useless startup video ads.
 */
void MemoryPatch::CryGame::DisableIntros(void* pCryGame, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryGame, 0x2EDF9D, 0x10);
			break;
		}
		case 5879:
		{
			FillNop(pCryGame, 0x2ED05D, 0x10);
			break;
		}
		case 6115:
		{
			FillNop(pCryGame, 0x2F695D, 0x10);
			break;
		}
		case 6156:
		{
			FillNop(pCryGame, 0x2F6F4D, 0x10);
			break;
		}
		case 6566:
		{
			FillNop(pCryGame, 0x336402, 0x10);
			break;
		}
		case 6586:
		{
			FillNop(pCryGame, 0x3274E2, 0x10);
			break;
		}
		case 6627:
		{
			FillNop(pCryGame, 0x3275B2, 0x10);
			break;
		}
		case 6670:
		{
			FillNop(pCryGame, 0x327CC2, 0x10);
			break;
		}
		case 6729:
		{
			FillNop(pCryGame, 0x3291A2, 0x10);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryGame, 0x21A91D, 0xD);
			FillNop(pCryGame, 0x21A92B, 0x2);
			break;
		}
		case 5879:
		{
			FillNop(pCryGame, 0x21ACDD, 0xD);
			FillNop(pCryGame, 0x21ACEB, 0x2);
			break;
		}
		case 6115:
		{
			FillNop(pCryGame, 0x220CAD, 0xD);
			FillNop(pCryGame, 0x220CBB, 0x2);
			break;
		}
		case 6156:
		{
			FillNop(pCryGame, 0x220BFD, 0xD);
			FillNop(pCryGame, 0x220C0B, 0x2);
			break;
		}
		case 6527:
		{
			FillNop(pCryGame, 0x23C9F0, 0xC);
			FillNop(pCryGame, 0x23C9FF, 0x2);
			break;
		}
		case 6566:
		{
			FillNop(pCryGame, 0x24D101, 0xC);
			FillNop(pCryGame, 0x24D110, 0x2);
			break;
		}
		case 6586:
		{
			FillNop(pCryGame, 0x23D650, 0xC);
			FillNop(pCryGame, 0x23D65F, 0x2);
			break;
		}
		case 6627:
		{
			FillNop(pCryGame, 0x23D250, 0xC);
			FillNop(pCryGame, 0x23D25F, 0x2);
			break;
		}
		case 6670:
		{
			FillNop(pCryGame, 0x23D760, 0xC);
			FillNop(pCryGame, 0x23D76F, 0x2);
			break;
		}
		case 6729:
		{
			FillNop(pCryGame, 0x23EEE0, 0xC);
			FillNop(pCryGame, 0x23EEEF, 0x2);
			break;
		}
#endif
	}
}

/**
 * Prevents DX10 servers in the server list from being grayed-out when the game is running in DX9 mode.
 */
void MemoryPatch::CryGame::CanJoinDX10Servers(void* pCryGame, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryGame, 0x327B3C, 0xF);
			break;
		}
		case 5879:
		{
			FillNop(pCryGame, 0x32689C, 0xF);
			break;
		}
		case 6115:
		{
			FillNop(pCryGame, 0x3343C1, 0x18);
			break;
		}
		case 6156:
		{
			FillNop(pCryGame, 0x334791, 0x18);
			break;
		}
		case 6566:
		{
			FillNop(pCryGame, 0x35BC57, 0x18);
			break;
		}
		case 6586:
		{
			FillNop(pCryGame, 0x34B4F7, 0x18);
			break;
		}
		case 6627:
		{
			FillNop(pCryGame, 0x34B097, 0x18);
			break;
		}
		case 6670:
		{
			FillNop(pCryGame, 0x34B9A7, 0x18);
			break;
		}
		case 6729:
		{
			FillNop(pCryGame, 0x34D047, 0x18);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryGame, 0x23A4BC, 0xA);
			break;
		}
		case 5879:
		{
			FillNop(pCryGame, 0x23AB5C, 0xA);
			break;
		}
		case 6115:
		{
			FillNop(pCryGame, 0x242CAC, 0xF);
			break;
		}
		case 6156:
		{
			FillNop(pCryGame, 0x242F1C, 0xF);
			break;
		}
		case 6527:
		{
			FillNop(pCryGame, 0x250E10, 0xF);
			break;
		}
		case 6566:
		{
			FillNop(pCryGame, 0x262D50, 0xF);
			break;
		}
		case 6586:
		{
			FillNop(pCryGame, 0x2514D0, 0xF);
			break;
		}
		case 6627:
		{
			FillNop(pCryGame, 0x2510D0, 0xF);
			break;
		}
		case 6670:
		{
			FillNop(pCryGame, 0x251960, 0xF);
			break;
		}
		case 6729:
		{
			FillNop(pCryGame, 0x252E10, 0xF);
			break;
		}
#endif
	}
}

/**
 * Forces true value for DX10 flag in Flash UI scripts (ActionScript).
 *
 * It unlocks DX10 features in "CREATE GAME" menu in DX9 game.
 */
void MemoryPatch::CryGame::EnableDX10Menu(void* pCryGame, int gameBuild)
{
	const unsigned char code[] = {
		0xB0, 0x01,  // mov al, 0x1
		0x90         // nop
	};

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillMem(pCryGame, 0x2ECE24, code, sizeof code);
			FillMem(pCryGame, 0x2ED3FE, code, sizeof code);
			break;
		}
		case 5879:
		{
			FillMem(pCryGame, 0x2EBEE4, code, sizeof code);
			FillMem(pCryGame, 0x2EC4BE, code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCryGame, 0x2F5792, code, sizeof code);
			FillMem(pCryGame, 0x2F5DBC, code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCryGame, 0x2F5D7D, code, sizeof code);
			FillMem(pCryGame, 0x2F63B7, code, sizeof code);
			break;
		}
		case 6566:
		{
			FillMem(pCryGame, 0x3150C1, code, sizeof code);
			FillMem(pCryGame, 0x3156F7, code, sizeof code);
			break;
		}
		case 6586:
		{
			FillMem(pCryGame, 0x30AED1, code, sizeof code);
			FillMem(pCryGame, 0x30B507, code, sizeof code);
			break;
		}
		case 6627:
		{
			FillMem(pCryGame, 0x30AF91, code, sizeof code);
			FillMem(pCryGame, 0x30B5C7, code, sizeof code);
			break;
		}
		case 6670:
		{
			FillMem(pCryGame, 0x30B6A1, code, sizeof code);
			FillMem(pCryGame, 0x30BCD7, code, sizeof code);
			break;
		}
		case 6729:
		{
			FillMem(pCryGame, 0x30CBA1, code, sizeof code);
			FillMem(pCryGame, 0x30D1D7, code, sizeof code);
			break;
		}
#else
		case 5767:
		{
			FillMem(pCryGame, 0x21A00E, code, sizeof code);
			FillMem(pCryGame, 0x21A401, code, sizeof code);
			break;
		}
		case 5879:
		{
			FillMem(pCryGame, 0x21A3CE, code, sizeof code);
			FillMem(pCryGame, 0x21A7C1, code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCryGame, 0x22034F, code, sizeof code);
			FillMem(pCryGame, 0x220789, code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCryGame, 0x22029A, code, sizeof code);
			FillMem(pCryGame, 0x2206E2, code, sizeof code);
			break;
		}
		case 6527:
		{
			FillMem(pCryGame, 0x22C35E, code, sizeof code);
			FillMem(pCryGame, 0x22C7A2, code, sizeof code);
			break;
		}
		case 6566:
		{
			FillMem(pCryGame, 0x23936E, code, sizeof code);
			FillMem(pCryGame, 0x2397B2, code, sizeof code);
			break;
		}
		case 6586:
		{
			FillMem(pCryGame, 0x22CEAE, code, sizeof code);
			FillMem(pCryGame, 0x22D2F2, code, sizeof code);
			break;
		}
		case 6627:
		{
			FillMem(pCryGame, 0x22C9CE, code, sizeof code);
			FillMem(pCryGame, 0x22CE12, code, sizeof code);
			break;
		}
		case 6670:
		{
			FillMem(pCryGame, 0x22CDCE, code, sizeof code);
			FillMem(pCryGame, 0x22D212, code, sizeof code);
			break;
		}
		case 6729:
		{
			FillMem(pCryGame, 0x22E64E, code, sizeof code);
			FillMem(pCryGame, 0x22EA92, code, sizeof code);
			break;
		}
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
// CryNetwork
////////////////////////////////////////////////////////////////////////////////

/**
 * Unlocks advantages of pre-ordered version for everyone.
 *
 * This is both server-side and client-side patch.
 */
void MemoryPatch::CryNetwork::EnablePreordered(void* pCryNetwork, int gameBuild)
{
	unsigned char code[] = {
#ifdef BUILD_64BIT
		0xC6, 0x83, 0x70, 0xFA, 0x00, 0x00, 0x01  // mov byte ptr ds:[rbx + 0xFA70], 0x1
#else
		0xC6, 0x83, 0xC8, 0xF3, 0x00, 0x00, 0x01  // mov byte ptr ds:[ebx + 0xF3C8], 0x1
#endif
	};

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillMem(pCryNetwork, 0x17F0C7, code, sizeof code);
			break;
		}
		case 5879:
		{
			code[2] = 0x68;  // 0xFA68 instead of 0xFA70

			FillMem(pCryNetwork, 0x1765F0, code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCryNetwork, 0x17C077, code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCryNetwork, 0x17C377, code, sizeof code);
			break;
		}
#else
		case 5767:
		{
			FillMem(pCryNetwork, 0x42C10, code, sizeof code);
			break;
		}
		case 5879:
		{
			FillMem(pCryNetwork, 0x412FD, code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCryNetwork, 0x430A8, code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCryNetwork, 0x43188, code, sizeof code);
			break;
		}
#endif
		case 6527:
		case 6566:
		case 6586:
		case 6627:
		case 6670:
		case 6729:
		{
			// Crysis Wars does not have pre-ordered version
			break;
		}
	}
}

/**
 * Prevents server from kicking players with the same CD-Key.
 *
 * This is a server-side patch.
 */
void MemoryPatch::CryNetwork::AllowSameCDKeys(void* pCryNetwork, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryNetwork, 0xE4858, 0x47);
			break;
		}
		case 5879:
		{
			FillNop(pCryNetwork, 0xE5628, 0x47);
			break;
		}
		case 6115:
		{
			FillNop(pCryNetwork, 0xE0188, 0x47);
			break;
		}
		case 6156:
		{
			FillNop(pCryNetwork, 0xE0328, 0x47);
			break;
		}
		case 6566:
		{
			FillNop(pCryNetwork, 0xE9034, 0x6B);
			break;
		}
		case 6586:
		{
			FillNop(pCryNetwork, 0xE0838, 0x47);
			break;
		}
		case 6627:
		case 6670:
		case 6729:
		{
			FillNop(pCryNetwork, 0xDFE48, 0x47);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryNetwork, 0x608CE, 0x4);
			break;
		}
		case 5879:
		{
			FillNop(pCryNetwork, 0x5DE79, 0x4);
			break;
		}
		case 6115:
		{
			FillNop(pCryNetwork, 0x60EF2, 0x4);
			break;
		}
		case 6156:
		{
			FillNop(pCryNetwork, 0x606A5, 0x4);
			break;
		}
		case 6527:
		{
			FillNop(pCryNetwork, 0x60768, 0x4);
			break;
		}
		case 6566:
		{
			FillNop(pCryNetwork, 0x73F90, 0x4);
			break;
		}
		case 6586:
		case 6627:
		case 6670:
		{
			FillNop(pCryNetwork, 0x60CFE, 0x4);
			break;
		}
		case 6729:
		{
			FillNop(pCryNetwork, 0x60CF9, 0x4);
			break;
		}
#endif
	}
}

/**
 * Allows connecting to Internet servers without GameSpy account.
 */
void MemoryPatch::CryNetwork::FixInternetConnect(void* pCryNetwork, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryNetwork, 0x18C716, 0x18);
			break;
		}
		case 5879:
		{
			FillNop(pCryNetwork, 0x184136, 0x18);
			break;
		}
		case 6115:
		{
			FillNop(pCryNetwork, 0x189596, 0x18);
			break;
		}
		case 6156:
		{
			FillNop(pCryNetwork, 0x189896, 0x18);
			break;
		}
		case 6566:
		{
			FillNop(pCryNetwork, 0x19602B, 0x18);
			break;
		}
		case 6586:
		{
			FillNop(pCryNetwork, 0x18B0A6, 0x18);
			break;
		}
		case 6627:
		case 6670:
		case 6729:
		{
			FillNop(pCryNetwork, 0x18B0B6, 0x18);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryNetwork, 0x3F4B5, 0xD);
			break;
		}
		case 5879:
		{
			FillNop(pCryNetwork, 0x3DBCC, 0xD);
			break;
		}
		case 6115:
		{
			FillNop(pCryNetwork, 0x3FA9C, 0xD);
			break;
		}
		case 6156:
		{
			FillNop(pCryNetwork, 0x3FB7C, 0xD);
			break;
		}
		case 6527:
		{
			FillNop(pCryNetwork, 0x3FB77, 0xD);
			break;
		}
		case 6566:
		{
			FillNop(pCryNetwork, 0x50892, 0xD);
			break;
		}
		case 6586:
		case 6627:
		case 6670:
		case 6729:
		{
			FillNop(pCryNetwork, 0x3FF87, 0xD);
			break;
		}
#endif
	}
}

/**
 * Fixes the sporadic crash when file check (sv_cheatProtection) is enabled.
 *
 * Both client and server are affected. Although server is much less prone to crashing. This patch fixes both.
 */
void MemoryPatch::CryNetwork::FixFileCheckCrash(void* pCryNetwork, int gameBuild)
{
#ifdef BUILD_64BIT
	const unsigned char codeA[] = {
		0x48, 0x89, 0x0A,  // mov qword ptr ds:[rdx], rcx
		0x90               // nop
	};

	const unsigned char codeB[] = {
		0x48, 0x89, 0x4A, 0x08  // mov qword ptr ds:[rdx+0x8], rcx
	};
#else
	const unsigned char clientCode[] = {
		0x8B, 0x4D, 0xC0,  // mov ecx, dword ptr ss:[ebp-0x40]
		0xFF, 0x49, 0xF4,  // dec dword ptr ds:[ecx-0xC]
		0x8B, 0x4D, 0xBC,  // mov ecx, dword ptr ss:[ebp-0x44]
		0x89, 0x4D, 0xC0   // mov dword ptr ss:[ebp-0x40], ecx
	};

	const unsigned char serverCode[] = {
		0x90,              // nop
		0x90,              // nop
		0xEB, 0x02,        // jmp -------------------------------+
		0x33, 0xC0,        // xor eax, eax                       |
		0x8B, 0x4F, 0x04,  // mov ecx, dword ptr ds:[edi+0x4] <--+
		0xFF, 0x49, 0xF4,  // dec dword ptr ds:[ecx-0xC]
		0x8B, 0x0F,        // mov ecx, dword ptr ds:[edi]
		0x89, 0x4F, 0x04,  // mov dword ptr ds:[edi+0x4], ecx
		0x90,              // nop
		0x90,              // nop
		0x90               // nop
	};
#endif

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			// client
			FillMem(pCryNetwork, 0x1540C1, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x1540D9, codeB, sizeof codeB);
			// server
			FillMem(pCryNetwork, 0x154411, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x154429, codeB, sizeof codeB);
			break;
		}
		case 5879:
		{
			// Crysis 1.1 does not have file check
			break;
		}
		case 6115:
		{
			// client
			FillMem(pCryNetwork, 0x14F151, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x14F169, codeB, sizeof codeB);
			// server
			FillMem(pCryNetwork, 0x14F481, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x14F499, codeB, sizeof codeB);
			break;
		}
		case 6156:
		{
			// client
			FillMem(pCryNetwork, 0x14F5B1, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x14F5C9, codeB, sizeof codeB);
			// server
			FillMem(pCryNetwork, 0x14F8E1, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x14F8F9, codeB, sizeof codeB);
			break;
		}
		case 6566:
		{
			// client
			FillMem(pCryNetwork, 0x158991, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x1589A9, codeB, sizeof codeB);
			// server
			FillMem(pCryNetwork, 0x158CC1, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x158CD9, codeB, sizeof codeB);
			break;
		}
		case 6586:
		{
			// client
			FillMem(pCryNetwork, 0x151571, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x151589, codeB, sizeof codeB);
			// server
			FillMem(pCryNetwork, 0x1518A1, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x1518B9, codeB, sizeof codeB);
			break;
		}
		case 6627:
		case 6670:
		case 6729:
		{
			// client
			FillMem(pCryNetwork, 0x151301, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x151319, codeB, sizeof codeB);
			// server
			FillMem(pCryNetwork, 0x151641, codeA, sizeof codeA);
			FillMem(pCryNetwork, 0x151659, codeB, sizeof codeB);
			break;
		}
#else
		case 5767:
		{
			// client
			FillNop(pCryNetwork, 0x49E66, 0xC);
			FillMem(pCryNetwork, 0x49EB5, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x49A7F, 0xC);
			FillMem(pCryNetwork, 0x30D62, serverCode, sizeof serverCode);
			break;
		}
		case 5879:
		{
			// Crysis 1.1 does not have file check
			break;
		}
		case 6115:
		{
			// client
			FillNop(pCryNetwork, 0x4A268, 0xC);
			FillMem(pCryNetwork, 0x4A2B7, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x49E81, 0xC);
			FillMem(pCryNetwork, 0x30E1C, serverCode, sizeof serverCode);
			break;
		}
		case 6156:
		{
			// client
			FillNop(pCryNetwork, 0x4A34F, 0xC);
			FillMem(pCryNetwork, 0x4A39E, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x49F68, 0xC);
			FillMem(pCryNetwork, 0x30E7B, serverCode, sizeof serverCode);
			break;
		}
		case 6527:
		{
			// client
			FillNop(pCryNetwork, 0x4A361, 0xC);
			FillMem(pCryNetwork, 0x4A3B0, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x49F7A, 0xC);
			FillMem(pCryNetwork, 0x31123, serverCode, sizeof serverCode);
			break;
		}
		case 6566:
		{
			// client
			FillNop(pCryNetwork, 0x5B3A6, 0xC);
			FillMem(pCryNetwork, 0x5B3F5, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x5ADE1, 0xC);
			FillMem(pCryNetwork, 0x3D633, serverCode, sizeof serverCode);
			break;
		}
		case 6586:
		{
			// client
			FillNop(pCryNetwork, 0x4A9B5, 0xC);
			FillMem(pCryNetwork, 0x4AA04, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x4A3CB, 0xC);
			FillMem(pCryNetwork, 0x31333, serverCode, sizeof serverCode);
			break;
		}
		case 6627:
		case 6670:
		case 6729:
		{
			// client
			FillNop(pCryNetwork, 0x4A9B5, 0xC);
			FillMem(pCryNetwork, 0x4AA04, clientCode, sizeof clientCode);
			// server
			FillNop(pCryNetwork, 0x4A3CB, 0xC);
			FillMem(pCryNetwork, 0x3141A, serverCode, sizeof serverCode);
			break;
		}
#endif
	}
}

/**
 * Disables creation of the "server_profile.txt" file.
 */
void MemoryPatch::CryNetwork::DisableServerProfile(void* pCryNetwork, int gameBuild)
{
#ifdef BUILD_64BIT
	// already disabled in 64-bit version
#else
	switch (gameBuild)
	{
		case 5767:
		{
			FillNop(pCryNetwork, 0x9F435, 0x5);
			break;
		}
		case 5879:
		{
			FillNop(pCryNetwork, 0x9CA81, 0x5);
			break;
		}
		case 6115:
		{
			FillNop(pCryNetwork, 0x9C665, 0x5);
			break;
		}
		case 6156:
		{
			FillNop(pCryNetwork, 0x9BE2E, 0x5);
			break;
		}
		case 6527:
		{
			FillNop(pCryNetwork, 0x9BEE6, 0x5);
			break;
		}
		case 6566:
		{
			FillNop(pCryNetwork, 0xB3419, 0x5);
			break;
		}
		case 6586:
		case 6627:
		case 6670:
		{
			FillNop(pCryNetwork, 0x9C4DC, 0x5);
			break;
		}
		case 6729:
		{
			FillNop(pCryNetwork, 0x9C4D7, 0x5);
			break;
		}
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CrySystem
////////////////////////////////////////////////////////////////////////////////

/**
 * Disables the SecuROM crap in 64-bit CrySystem DLL.
 */
void MemoryPatch::CrySystem::RemoveSecuROM(void* pCrySystem, int gameBuild)
{
#ifdef BUILD_64BIT
	switch (gameBuild)
	{
		case 5767:
		{
			FillNop(pCrySystem, 0x4659E, 0x16);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x47B6E, 0x16);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x46FFD, 0x16);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x470B9, 0x16);
			break;
		}
		case 6566:
		case 6586:
		case 6627:
		case 6670:
		case 6729:
		{
			// Crysis Wars has no SecuROM crap in its CrySystem DLL
			break;
		}
	}
#endif
}

/**
 * Allows Very High settings in DX9 mode.
 */
void MemoryPatch::CrySystem::AllowDX9VeryHighSpec(void* pCrySystem, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCrySystem, 0x45C31, 0x54);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x47201, 0x54);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x46690, 0x54);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x4674C, 0x54);
			break;
		}
		case 6566:
		{
			FillNop(pCrySystem, 0x4D7B5, 0x54);
			break;
		}
		case 6586:
		{
			FillNop(pCrySystem, 0x47DBB, 0x54);
			break;
		}
		case 6627:
		{
			FillNop(pCrySystem, 0x4A90B, 0x54);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCrySystem, 0x59F08, 0x4B);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x5A488, 0x4B);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x5A268, 0x4B);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x59DA8, 0x4B);
			break;
		}
		case 6527:
		{
			FillNop(pCrySystem, 0x5A778, 0x4B);
			break;
		}
		case 6566:
		{
			FillNop(pCrySystem, 0x5D1A9, 0x4B);
			break;
		}
		case 6586:
		{
			FillNop(pCrySystem, 0x5A659, 0x4B);
			break;
		}
		case 6627:
		{
			FillNop(pCrySystem, 0x5B5E9, 0x4B);
			break;
		}
#endif
		case 6670:
		case 6729:
		{
			// Crysis Wars 1.4+ allows Very High settings in DX9 mode
			break;
		}
	}
}

/**
 * Allows running multiple instances of Crysis at once.
 *
 * Note that the first check if any instance is already running is normally done in launcher.
 */
void MemoryPatch::CrySystem::AllowMultipleInstances(void* pCrySystem, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCrySystem, 0x420DF, 0x68);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x436AF, 0x68);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x42B5F, 0x68);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x42BFF, 0x68);
			break;
		}
		case 6566:
		{
			FillNop(pCrySystem, 0x49D1F, 0x68);
			break;
		}
		case 6586:
		{
			FillNop(pCrySystem, 0x4420F, 0x68);
			break;
		}
		case 6627:
		{
			FillNop(pCrySystem, 0x46D5F, 0x68);
			break;
		}
		case 6670:
		case 6729:
		{
			FillNop(pCrySystem, 0x46EEF, 0x68);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCrySystem, 0x57ABF, 0x58);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x5802F, 0x58);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x57E1F, 0x58);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x5794F, 0x58);
			break;
		}
		case 6527:
		{
			FillNop(pCrySystem, 0x5831F, 0x58);
			break;
		}
		case 6566:
		{
			FillNop(pCrySystem, 0x5AC4F, 0x58);
			break;
		}
		case 6586:
		{
			FillNop(pCrySystem, 0x5834F, 0x58);
			break;
		}
		case 6627:
		{
			FillNop(pCrySystem, 0x592DF, 0x58);
			break;
		}
		case 6670:
		{
			FillNop(pCrySystem, 0x595CF, 0x58);
			break;
		}
		case 6729:
		{
			FillNop(pCrySystem, 0x595DF, 0x58);
			break;
		}
#endif
	}
}

/**
 * Prevents the engine from installing its own broken unhandled exceptions handler.
 */
void MemoryPatch::CrySystem::UnhandledExceptions(void* pCrySystem, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCrySystem, 0x22986, 0x6);
			FillNop(pCrySystem, 0x22992, 0x7);
			FillNop(pCrySystem, 0x45C8A, 0x16);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x232C6, 0x6);
			FillNop(pCrySystem, 0x232D2, 0x7);
			FillNop(pCrySystem, 0x4725A, 0x16);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x22966, 0x6);
			FillNop(pCrySystem, 0x22972, 0x7);
			FillNop(pCrySystem, 0x466E9, 0x16);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x22946, 0x6);
			FillNop(pCrySystem, 0x22952, 0x7);
			FillNop(pCrySystem, 0x467A5, 0x16);
			break;
		}
		case 6566:
		{
			FillNop(pCrySystem, 0x298AE, 0x6);
			FillNop(pCrySystem, 0x298BA, 0x7);
			FillNop(pCrySystem, 0x4D80E, 0x16);
			break;
		}
		case 6586:
		{
			FillNop(pCrySystem, 0x24026, 0x6);
			FillNop(pCrySystem, 0x24032, 0x7);
			FillNop(pCrySystem, 0x47E14, 0x16);
			break;
		}
		case 6627:
		{
			FillNop(pCrySystem, 0x25183, 0x6);
			FillNop(pCrySystem, 0x2518F, 0x7);
			FillNop(pCrySystem, 0x4A964, 0x16);
			break;
		}
		case 6670:
		case 6729:
		{
			FillNop(pCrySystem, 0x253B3, 0x6);
			FillNop(pCrySystem, 0x253BF, 0x7);
			FillNop(pCrySystem, 0x4AAA0, 0x16);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCrySystem, 0x182B7, 0x5);
			FillNop(pCrySystem, 0x182C2, 0xC);
			FillNop(pCrySystem, 0x59F58, 0x13);
			break;
		}
		case 5879:
		{
			FillNop(pCrySystem, 0x18437, 0x5);
			FillNop(pCrySystem, 0x18442, 0xC);
			FillNop(pCrySystem, 0x5A4D8, 0x13);
			break;
		}
		case 6115:
		{
			FillNop(pCrySystem, 0x18217, 0x5);
			FillNop(pCrySystem, 0x18222, 0xC);
			FillNop(pCrySystem, 0x5A2B8, 0x13);
			break;
		}
		case 6156:
		{
			FillNop(pCrySystem, 0x17D67, 0x5);
			FillNop(pCrySystem, 0x17D72, 0xC);
			FillNop(pCrySystem, 0x59DF8, 0x13);
			break;
		}
		case 6527:
		{
			FillNop(pCrySystem, 0x18767, 0x5);
			FillNop(pCrySystem, 0x18772, 0xC);
			FillNop(pCrySystem, 0x5A7C8, 0x13);
			break;
		}
		case 6566:
		{
			FillNop(pCrySystem, 0x1AD57, 0x5);
			FillNop(pCrySystem, 0x1AD62, 0xC);
			FillNop(pCrySystem, 0x5D1F9, 0x13);
			break;
		}
		case 6586:
		{
			FillNop(pCrySystem, 0x18A27, 0x5);
			FillNop(pCrySystem, 0x18A32, 0xC);
			FillNop(pCrySystem, 0x5A6A9, 0x13);
			break;
		}
		case 6627:
		{
			FillNop(pCrySystem, 0x19327, 0x5);
			FillNop(pCrySystem, 0x19332, 0xC);
			FillNop(pCrySystem, 0x5B639, 0x13);
			break;
		}
		case 6670:
		{
			FillNop(pCrySystem, 0x19607, 0x5);
			FillNop(pCrySystem, 0x19612, 0xC);
			FillNop(pCrySystem, 0x5B8DC, 0x13);
			break;
		}
		case 6729:
		{
			FillNop(pCrySystem, 0x19617, 0x5);
			FillNop(pCrySystem, 0x19622, 0xC);
			FillNop(pCrySystem, 0x5B8EC, 0x13);
			break;
		}
#endif
	}
}

/**
 * Hooks CryEngine fatal error handler.
 */
void MemoryPatch::CrySystem::HookError(void* pCrySystem, int gameBuild, void (*handler)(const char* format, ...))
{
	// convert thiscall into a normal function call
	// and call our handler
#ifdef BUILD_64BIT
	unsigned char code[] = {
		0x48, 0x89, 0x54, 0x24, 0x10,                                // mov qword ptr ss:[rsp+0x10], rdx
		0x4C, 0x89, 0x44, 0x24, 0x18,                                // mov qword ptr ss:[rsp+0x18], r8
		0x4C, 0x89, 0x4C, 0x24, 0x20,                                // mov qword ptr ss:[rsp+0x20], r9
		0x48, 0x83, 0xEC, 0x28,                                      // sub rsp, 0x28
		0x48, 0x8B, 0xCA,                                            // mov rcx, rdx
		0x48, 0x8D, 0x54, 0x24, 0x40,                                // lea rdx, qword ptr ss:[rsp+0x40]
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // mov rax, 0x0
		0xFF, 0xD0,                                                  // call rax
		0x48, 0x83, 0xC4, 0x28,                                      // add rsp, 0x28
		0xC3                                                         // ret
	};

	std::memcpy(&code[29], &handler, 8);
#else
	unsigned char code[] = {
		0x8B, 0x4C, 0x24, 0x08,        // mov ecx, dword ptr ss:[esp+0x8]
		0x8D, 0x44, 0x24, 0x0C,        // lea eax, dword ptr ss:[esp+0xC]
		0x50,                          // push eax
		0x51,                          // push ecx
		0xB8, 0x00, 0x00, 0x00, 0x00,  // mov eax, 0x0
		0xFF, 0xD0,                    // call eax
		0x83, 0xC4, 0x08,              // add esp, 0x8
		0xC3                           // ret
	};

	std::memcpy(&code[11], &handler, 4);
#endif

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillMem(pCrySystem, 0x52180, &code, sizeof code);
			break;
		}
		case 5879:
		{
			FillMem(pCrySystem, 0x53850, &code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCrySystem, 0x52D50, &code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCrySystem, 0x52D00, &code, sizeof code);
			break;
		}
		case 6566:
		{
			FillMem(pCrySystem, 0x59A90, &code, sizeof code);
			break;
		}
		case 6586:
		{
			FillMem(pCrySystem, 0x543F0, &code, sizeof code);
			break;
		}
		case 6627:
		{
			FillMem(pCrySystem, 0x570E0, &code, sizeof code);
			break;
		}
		case 6670:
		case 6729:
		{
			FillMem(pCrySystem, 0x571A0, &code, sizeof code);
			break;
		}
#else
		case 5767:
		{
			FillMem(pCrySystem, 0x655C0, &code, sizeof code);
			break;
		}
		case 5879:
		{
			FillMem(pCrySystem, 0x65C50, &code, sizeof code);
			break;
		}
		case 6115:
		{
			FillMem(pCrySystem, 0x65920, &code, sizeof code);
			break;
		}
		case 6156:
		{
			FillMem(pCrySystem, 0x63290, &code, sizeof code);
			break;
		}
		case 6527:
		{
			FillMem(pCrySystem, 0x63F90, &code, sizeof code);
			break;
		}
		case 6566:
		{
			FillMem(pCrySystem, 0x668A0, &code, sizeof code);
			break;
		}
		case 6586:
		{
			FillMem(pCrySystem, 0x63C90, &code, sizeof code);
			break;
		}
		case 6627:
		{
			FillMem(pCrySystem, 0x64C20, &code, sizeof code);
			break;
		}
		case 6670:
		{
			FillMem(pCrySystem, 0x64D30, &code, sizeof code);
			break;
		}
		case 6729:
		{
			FillMem(pCrySystem, 0x64D40, &code, sizeof code);
			break;
		}
#endif
	}
}

/**
 * Disables use of 3DNow! instructions.
 *
 * This patch correctly fixes the well-known crash of 32-bit Crysis on modern AMD processors.
 */
void MemoryPatch::CrySystem::Disable3DNow(void* pCrySystem, int gameBuild)
{
	// default processor feature flags without CPUF_3DNOW flag
	const unsigned char flags = 0x18;

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillMem(pCrySystem, 0xA1AF, &flags, sizeof flags);
			break;
		}
		case 5879:
		{
			FillMem(pCrySystem, 0xA0FF, &flags, sizeof flags);
			break;
		}
		case 6115:
		{
			FillMem(pCrySystem, 0xA0BF, &flags, sizeof flags);
			break;
		}
		case 6156:
		{
			FillMem(pCrySystem, 0xA0FF, &flags, sizeof flags);
			break;
		}
		case 6566:
		{
			FillMem(pCrySystem, 0xAD3F, &flags, sizeof flags);
			break;
		}
		case 6586:
		{
			FillMem(pCrySystem, 0xA32F, &flags, sizeof flags);
			break;
		}
		case 6627:
		{
			FillMem(pCrySystem, 0xA26F, &flags, sizeof flags);
			break;
		}
		case 6670:
		case 6729:
		{
			FillMem(pCrySystem, 0xA32F, &flags, sizeof flags);
			break;
		}
#else
		case 5767:
		{
			FillMem(pCrySystem, 0x9432, &flags, sizeof flags);
			break;
		}
		case 5879:
		{
			FillMem(pCrySystem, 0x9472, &flags, sizeof flags);
			break;
		}
		case 6115:
		{
			FillMem(pCrySystem, 0x9412, &flags, sizeof flags);
			break;
		}
		case 6156:
		{
			FillMem(pCrySystem, 0x93E2, &flags, sizeof flags);
			break;
		}
		case 6527:
		{
			FillMem(pCrySystem, 0x9472, &flags, sizeof flags);
			break;
		}
		case 6566:
		{
			FillMem(pCrySystem, 0x9942, &flags, sizeof flags);
			break;
		}
		case 6586:
		{
			FillMem(pCrySystem, 0x93D2, &flags, sizeof flags);
			break;
		}
		case 6627:
		{
			FillMem(pCrySystem, 0x9402, &flags, sizeof flags);
			break;
		}
		case 6670:
		case 6729:
		{
			FillMem(pCrySystem, 0x9412, &flags, sizeof flags);
			break;
		}
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
// CryRenderD3D10
////////////////////////////////////////////////////////////////////////////////

/**
 * Prevents the DX10 renderer from using the lowest refresh rate available.
 *
 * Thanks to Guzz and Vladislav for this patch.
 */
void MemoryPatch::CryRenderD3D10::FixLowRefreshRateBug(void* pCryRenderD3D10, int gameBuild)
{
	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryRenderD3D10, 0x1C5ED5, 0x4);
			break;
		}
		case 5879:
		{
			FillNop(pCryRenderD3D10, 0x1C5DC5, 0x4);
			break;
		}
		case 6115:
		{
			FillNop(pCryRenderD3D10, 0x1C8B65, 0x4);
			break;
		}
		case 6156:
		{
			FillNop(pCryRenderD3D10, 0x1C8F45, 0x4);
			break;
		}
		case 6566:
		{
			FillNop(pCryRenderD3D10, 0x1BAA25, 0x4);
			break;
		}
		case 6586:
		{
			FillNop(pCryRenderD3D10, 0x1CA335, 0x4);
			break;
		}
		case 6627:
		case 6670:
		case 6729:
		{
			FillNop(pCryRenderD3D10, 0x1CA345, 0x4);
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryRenderD3D10, 0x16CE00, 0x6);
			break;
		}
		case 5879:
		{
			FillNop(pCryRenderD3D10, 0x16E390, 0x6);
			break;
		}
		case 6115:
		{
			FillNop(pCryRenderD3D10, 0x16F470, 0x6);
			break;
		}
		case 6156:
		{
			FillNop(pCryRenderD3D10, 0x16F3E0, 0x6);
			break;
		}
		case 6527:
		{
			FillNop(pCryRenderD3D10, 0x16F290, 0x6);
			break;
		}
		case 6566:
		{
			FillNop(pCryRenderD3D10, 0x1798D0, 0x6);
			break;
		}
		case 6586:
		{
			FillNop(pCryRenderD3D10, 0x16F110, 0x6);
			break;
		}
		case 6627:
		{
			FillNop(pCryRenderD3D10, 0x16F150, 0x6);
			break;
		}
		case 6670:
		case 6729:
		{
			FillNop(pCryRenderD3D10, 0x16F170, 0x6);
			break;
		}
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
// CryRenderNULL
////////////////////////////////////////////////////////////////////////////////

/**
 * Disables the debug renderer in CryRenderNULL DLL.
 *
 * This patch gets rid of the wasteful hidden debug renderer window with OpenGL context.
 *
 * The 1st FillNop disables debug renderer stuff in CNULLRenderAuxGeom constructor.
 * The 2nd FillNop disables debug renderer stuff in CNULLRenderAuxGeom destructor.
 * The 3rd FillMem disables the CNULLRenderAuxGeom::BeginFrame call in CNULLRenderer::BeginFrame.
 * The 4th FillMem disables the CNULLRenderAuxGeom::EndFrame call in CNULLRenderer::EndFrame.
 */
void MemoryPatch::CryRenderNULL::DisableDebugRenderer(void* pCryRenderNULL, int gameBuild)
{
	const unsigned char code[] = {
		0xC3,  // ret
#ifdef BUILD_64BIT
		0x90,  // nop
#endif
		0x90,  // nop
		0x90,  // nop
		0x90,  // nop
		0x90,  // nop
		0x90   // nop
	};

	unsigned int renderAuxGeomVTableOffset = 0;

	switch (gameBuild)
	{
#ifdef BUILD_64BIT
		case 5767:
		{
			FillNop(pCryRenderNULL, 0xD2B9, 0x175);
			FillNop(pCryRenderNULL, 0xD473, 0x35);
			FillMem(pCryRenderNULL, 0x16BE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x16D0, code, sizeof code);
			renderAuxGeomVTableOffset = 0x97578;
			break;
		}
		case 5879:
		{
			FillNop(pCryRenderNULL, 0xD489, 0x175);
			FillNop(pCryRenderNULL, 0xD393, 0x35);
			FillMem(pCryRenderNULL, 0x16CE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x16E0, code, sizeof code);
			renderAuxGeomVTableOffset = 0x97538;
			break;
		}
		case 6115:
		{
			FillNop(pCryRenderNULL, 0xD049, 0x175);
			FillNop(pCryRenderNULL, 0xD203, 0x35);
			FillMem(pCryRenderNULL, 0x16BE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x16D0, code, sizeof code);
			renderAuxGeomVTableOffset = 0x974A8;
			break;
		}
		case 6156:
		{
			FillNop(pCryRenderNULL, 0xD379, 0x175);
			FillNop(pCryRenderNULL, 0xD533, 0x35);
			FillMem(pCryRenderNULL, 0x16CE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x16E0, code, sizeof code);
			renderAuxGeomVTableOffset = 0x97588;
			break;
		}
		case 6566:
		{
			FillNop(pCryRenderNULL, 0xC332, 0x175);
			FillNop(pCryRenderNULL, 0xC4EC, 0x35);
			FillMem(pCryRenderNULL, 0x176E, code, sizeof code);
			FillMem(pCryRenderNULL, 0x1780, code, sizeof code);
			renderAuxGeomVTableOffset = 0x98918;
			break;
		}
		case 6586:
		{
			FillNop(pCryRenderNULL, 0xCFC9, 0x175);
			FillNop(pCryRenderNULL, 0xD183, 0x35);
			FillMem(pCryRenderNULL, 0x16FE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x1710, code, sizeof code);
			renderAuxGeomVTableOffset = 0x984B8;
			break;
		}
		case 6627:
		{
			FillNop(pCryRenderNULL, 0xD369, 0x175);
			FillNop(pCryRenderNULL, 0xD523, 0x35);
			FillMem(pCryRenderNULL, 0x16FE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x1710, code, sizeof code);
			renderAuxGeomVTableOffset = 0x984B8;
			break;
		}
		case 6670:
		case 6729:
		{
			FillNop(pCryRenderNULL, 0xD0D9, 0x175);
			FillNop(pCryRenderNULL, 0xD293, 0x35);
			FillMem(pCryRenderNULL, 0x16FE, code, sizeof code);
			FillMem(pCryRenderNULL, 0x1710, code, sizeof code);
			renderAuxGeomVTableOffset = 0x984B8;
			break;
		}
#else
		case 5767:
		{
			FillNop(pCryRenderNULL, 0x1CF3E, 0x101);
			FillNop(pCryRenderNULL, 0x1D051, 0xE);
			FillMem(pCryRenderNULL, 0x1895, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18A9, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA677C;
			break;
		}
		case 5879:
		{
			FillNop(pCryRenderNULL, 0x1CF78, 0x101);
			FillNop(pCryRenderNULL, 0x1CEFE, 0xE);
			FillMem(pCryRenderNULL, 0x1895, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18A9, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA6734;
			break;
		}
		case 6115:
		{
			FillNop(pCryRenderNULL, 0x1CF4F, 0x101);
			FillNop(pCryRenderNULL, 0x1D062, 0xE);
			FillMem(pCryRenderNULL, 0x1895, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18A9, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA6784;
			break;
		}
		case 6156:
		{
			FillNop(pCryRenderNULL, 0x1CEE6, 0x101);
			FillNop(pCryRenderNULL, 0x1CFF9, 0xE);
			FillMem(pCryRenderNULL, 0x1895, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18A9, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA778C;
			break;
		}
		case 6527:
		{
			FillNop(pCryRenderNULL, 0x1CE41, 0x101);
			FillNop(pCryRenderNULL, 0x1CF54, 0xE);
			FillMem(pCryRenderNULL, 0x189B, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18AF, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA779C;
			break;
		}
		case 6566:
		{
			FillNop(pCryRenderNULL, 0x1D3D9, 0x10C);
			FillNop(pCryRenderNULL, 0x1D4F7, 0xE);
			FillMem(pCryRenderNULL, 0x18A0, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18B4, code, sizeof code);
			renderAuxGeomVTableOffset = 0xB078C;
			break;
		}
		case 6586:
		{
			FillNop(pCryRenderNULL, 0x1CF67, 0x101);
			FillNop(pCryRenderNULL, 0x1D07A, 0xE);
			FillMem(pCryRenderNULL, 0x18A0, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18B4, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA779C;
			break;
		}
		case 6627:
		case 6670:
		case 6729:
		{
			FillNop(pCryRenderNULL, 0x1CF7C, 0x101);
			FillNop(pCryRenderNULL, 0x1D08F, 0xE);
			FillMem(pCryRenderNULL, 0x18AD, code, sizeof code);
			FillMem(pCryRenderNULL, 0x18C1, code, sizeof code);
			renderAuxGeomVTableOffset = 0xA779C;
			break;
		}
#endif
	}

	if (renderAuxGeomVTableOffset)
	{
		void** oldVTable = static_cast<void**>(ByteOffset(pCryRenderNULL, renderAuxGeomVTableOffset));

		// create a new CNULLRenderAuxGeom vtable
		void* newVTable[27] = {};

		// keep CNULLRenderAuxGeom::SetRenderFlags
		// keep CNULLRenderAuxGeom::GetRenderFlags
		newVTable[0] = oldVTable[0];
		newVTable[1] = oldVTable[1];

		// CNULLRenderAuxGeom::SetRenderFlags is empty and returns nothing
		void* emptyFunc = newVTable[0];

		// make the rest of CNULLRenderAuxGeom functions empty
		// note that all the functions return nothing
		for (unsigned int i = 2; i < (sizeof newVTable / sizeof newVTable[0]); i++)
		{
			newVTable[i] = emptyFunc;
		}

		// inject the new vtable
		FillMem(pCryRenderNULL, renderAuxGeomVTableOffset, newVTable, sizeof newVTable);
	}
}