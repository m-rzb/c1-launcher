#include "Library/CrashLogger.h"
#include "Library/OS.h"
#include "Project.h"

#include "../LauncherCommon.h"
#include "../MemoryPatch.h"

#include "DedicatedServerLauncher.h"

#define DEFAULT_LOG_FILE_NAME "Server.log"

static std::FILE* OpenLogFile()
{
	return LauncherCommon::OpenLogFile(DEFAULT_LOG_FILE_NAME);
}

DedicatedServerLauncher::DedicatedServerLauncher() : m_pGameStartup(NULL), m_params(), m_dlls()
{
}

DedicatedServerLauncher::~DedicatedServerLauncher()
{
	if (m_pGameStartup)
	{
		m_pGameStartup->Shutdown();
	}
}

int DedicatedServerLauncher::Run()
{
	m_params.hInstance = OS::Module::GetEXE();
	m_params.logFileName = DEFAULT_LOG_FILE_NAME;
	m_params.isDedicatedServer = true;

	LauncherCommon::SetParamsCmdLine(m_params, OS::CmdLine::Get());

	CrashLogger::Enable(&OpenLogFile);

	this->LoadEngine();
	this->PatchEngine();

	m_pGameStartup = LauncherCommon::StartEngine(m_dlls.pCryGame, m_params);

	gEnv = m_params.pSystem->GetGlobalEnvironment();

	CryLogAlways("%s", PROJECT_BANNER);

	return m_pGameStartup->Run(NULL);
}

void DedicatedServerLauncher::LoadEngine()
{
	m_dlls.pCrySystem = LauncherCommon::LoadModule("CrySystem.dll");

	m_dlls.gameBuild = LauncherCommon::GetGameBuild(m_dlls.pCrySystem);

	LauncherCommon::VerifyGameBuild(m_dlls.gameBuild);

	m_dlls.pCryGame = LauncherCommon::LoadModule("CryGame.dll");
	m_dlls.pCryNetwork = LauncherCommon::LoadModule("CryNetwork.dll");
}

void DedicatedServerLauncher::PatchEngine()
{
	if (m_dlls.pCryNetwork)
	{
		MemoryPatch::CryNetwork::EnablePreordered(m_dlls.pCryNetwork, m_dlls.gameBuild);
		MemoryPatch::CryNetwork::AllowSameCDKeys(m_dlls.pCryNetwork, m_dlls.gameBuild);
		MemoryPatch::CryNetwork::FixInternetConnect(m_dlls.pCryNetwork, m_dlls.gameBuild);
	}

	if (m_dlls.pCrySystem)
	{
		MemoryPatch::CrySystem::UnhandledExceptions(m_dlls.pCrySystem, m_dlls.gameBuild);
		MemoryPatch::CrySystem::HookError(m_dlls.pCrySystem, m_dlls.gameBuild, &CrashLogger::OnEngineError);

		if (OS::CPU::IsAMD() && !OS::CPU::Has3DNow())
		{
			MemoryPatch::CrySystem::Disable3DNow(m_dlls.pCrySystem, m_dlls.gameBuild);
		}
	}
}
