#include <algorithm>

#include "CryCommon/CrySystem/CryColorCode.h"
#include "CryCommon/CrySystem/IConsole.h"
#include "CryCommon/CrySystem/ISystem.h"

#include "Library/PathTools.h"
#include "Library/StringTools.h"
#include "Library/StringView.h"

#include "Logger.h"

Logger::Logger() : m_verbosity(0), m_cvars(), m_mainThreadID(OS::GetCurrentThreadID())
{
}

Logger::~Logger()
{
}

void Logger::OnUpdate()
{
	OS::LockGuard<OS::Mutex> lock(m_mutex);

	for (std::size_t i = 0; i < m_messages.size(); i++)
	{
		WriteMessage(m_messages[i]);
	}

	m_messages.clear();
}

static StringView ExtractBackupNameAttachment(StringView header)
{
	const StringView prefix = "BackupNameAttachment=";

	if (header.StartsWith(prefix))
	{
		header.RemovePrefix(prefix.length);

		std::size_t pos;

		pos = 0;
		if (header.Find('"', pos))
		{
			header.RemovePrefix(pos + 1);
		}

		pos = 0;
		if (header.Find('"', pos) || header.Find('\r', pos) || header.Find('\n', pos))
		{
			header.RemoveSuffix(header.length - pos);
		}

		return header;
	}
	else
	{
		return "";
	}
}

static void BackupLogFile(OS::File& file, const char* filePath)
{
	bool readError;
	char buffer[256];
	const StringView header(buffer, file.Read(buffer, sizeof buffer, &readError));

	if (readError)
	{
		throw StringTools::OSError("Failed to read the existing log file!");
	}

	if (header.IsEmpty())
	{
		// the existing log file is empty, so no backup is needed
		return;
	}

	const StringView backupNameAttachment = ExtractBackupNameAttachment(header);

	std::string backupPath;

	backupPath += PathTools::DirName(filePath);
	backupPath += OS_PATH_SLASH;
	backupPath += "LogBackups";

	if (!OS::Directory::Create(backupPath.c_str()))
	{
		throw StringTools::OSError("Failed to create log backup directory!\n=> %s", backupPath.c_str());
	}

	backupPath += OS_PATH_SLASH;
	backupPath += PathTools::RemoveFileExtension(PathTools::BaseName(filePath));
	backupPath += backupNameAttachment;
	backupPath += PathTools::GetFileExtension(filePath);

	if (!OS::File::Copy(filePath, backupPath.c_str()))
	{
		throw StringTools::OSError("Failed to copy the existing log file!\n<= %s\n=> %s",
			filePath,
			backupPath.c_str()
		);
	}
}

void Logger::OpenFile(const char* filePath)
{
	bool created = false;
	OS::File file;

	if (!file.Open(filePath, OS::File::READ_WRITE_CREATE, &created))
	{
		throw StringTools::OSError("Failed to open log file!\n=> %s", filePath);
	}

	if (!created)
	{
		BackupLogFile(file, filePath);

		if (!file.Resize(0))
		{
			throw StringTools::OSError("Failed to clear the existing log file!\n=> %s", filePath);
		}
	}

	m_file.Swap(file);
	m_filePath = filePath;
}

void Logger::CloseFile()
{
	m_file.Close();
	m_filePath.clear();
}

std::FILE* Logger::ReleaseFile()
{
	if (m_file.IsOpen())
	{
		// we have exclusive write access
		m_file.Close();

		std::FILE* file = std::fopen(m_filePath.c_str(), "a");

		m_filePath.clear();

		return file;
	}
	else
	{
		return NULL;
	}
}

void Logger::SetPrefix(const char* prefix)
{
	if (m_cvars.prefix)
	{
		m_cvars.prefix->Set(prefix);
	}

	m_prefix = prefix;
}

void Logger::LogV(ILog::ELogType type, const char* format, va_list args)
{
	PushMessageV(type, Message::FLAG_FILE | Message::FLAG_CONSOLE, format, args);
}

void Logger::Log(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	LogV(ILog::eMessage, format, args);
	va_end(args);
}

void Logger::LogWarning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	LogV(ILog::eWarning, format, args);
	va_end(args);
}

void Logger::LogError(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	LogV(ILog::eError, format, args);
	va_end(args);
}

void Logger::Release()
{
	// don't let the engine to delete us
}

bool Logger::SetFileName(const char* fileName)
{
	// don't let the engine to mess with the log file name
	return true;
}

const char* Logger::GetFileName()
{
	return PathTools::BaseName(m_filePath).string;
}

void Logger::LogPlus(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	PushMessageV(ILog::eMessage, Message::FLAG_FILE | Message::FLAG_CONSOLE | Message::FLAG_APPEND, format, args);
	va_end(args);
}

void Logger::LogToFile(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	PushMessageV(ILog::eMessage, Message::FLAG_FILE, format, args);
	va_end(args);
}

void Logger::LogToFilePlus(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	PushMessageV(ILog::eMessage, Message::FLAG_FILE | Message::FLAG_APPEND, format, args);
	va_end(args);
}

void Logger::LogToConsole(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	PushMessageV(ILog::eMessage, Message::FLAG_CONSOLE, format, args);
	va_end(args);
}

void Logger::LogToConsolePlus(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	PushMessageV(ILog::eMessage, Message::FLAG_CONSOLE | Message::FLAG_APPEND, format, args);
	va_end(args);
}

void Logger::UpdateLoadingScreen(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	LogV(ILog::eMessage, format, args);
	va_end(args);

	// server has no loading screen, so nothing to do here
}

void Logger::RegisterConsoleVariables()
{
	if (!gEnv)
	{
		return;
	}

	IConsole* pConsole = gEnv->pConsole;

	if (!pConsole)
	{
		return;
	}

	m_cvars.verbosity = pConsole->RegisterInt("log_Verbosity", m_verbosity, VF_DUMPTODISK,
		"Defines the verbosity level for console log messages (use log_FileVerbosity for file logging).\n"
		"Usage: log_Verbosity [-1/0/1/2/3/4]\n"
		" -1 = Suppress all logs (including eAlways).\n"
		"  0 = Suppress all logs (except eAlways).\n"
		"  1 = Additional errors.\n"
		"  2 = Additional warnings.\n"
		"  3 = Additional messages.\n"
		"  4 = Additional comments."
	);

	m_cvars.fileVerbosity = pConsole->RegisterInt("log_FileVerbosity", m_verbosity, VF_DUMPTODISK,
		"Defines the verbosity level for file log messages (if log_Verbosity is higher, this one is used).\n"
		"Usage: log_FileVerbosity [-1/0/1/2/3/4]\n"
		" -1 = Suppress all logs (including eAlways).\n"
		"  0 = Suppress all logs (except eAlways).\n"
		"  1 = Additional errors.\n"
		"  2 = Additional warnings.\n"
		"  3 = Additional messages.\n"
		"  4 = Additional comments."
	);

	m_cvars.prefix = pConsole->RegisterString("log_Prefix", m_prefix.c_str(), VF_NOT_NET_SYNCED,
		"Defines prefix of each message written to the log file.\n"
		"Usage: log_Prefix FORMAT\n"
		"The format string consists of normal characters and the following conversion specifiers:\n"
		"  %% = %\n"
		"  %d = Day of the month (01..31)\n"
		"  %m = Month (01..12)\n"
		"  %Y = Year (e.g. 2007)\n"
		"  %H = Hour (00..23)\n"
		"  %M = Minute (00..59)\n"
		"  %S = Second (00..60)\n"
		"  %N = Millisecond (000..999)\n"
		"  %z = Offset from UTC (time zone) in the ISO 8601 format (e.g. +0100)\n"
		"  %F = Equivalent to \"%Y-%m-%d\" (the ISO 8601 date format)\n"
		"  %T = Equivalent to \"%H:%M:%S\" (the ISO 8601 time format)\n"
		"  %t = Thread ID where the message was logged"
	);
}

void Logger::UnregisterConsoleVariables()
{
	// keep all cvars registered and reuse them
}

void Logger::SetVerbosity(int verbosity)
{
	if (m_cvars.verbosity)
	{
		m_cvars.verbosity->Set(verbosity);
	}

	if (m_cvars.fileVerbosity)
	{
		m_cvars.fileVerbosity->Set(verbosity);
	}

	m_verbosity = verbosity;
}

int Logger::GetVerbosityLevel()
{
	return (m_cvars.verbosity) ? m_cvars.verbosity->GetIVal() : m_verbosity;
}

void Logger::AddCallback(ILogCallback* pCallback)
{
	if (pCallback && std::count(m_callbacks.begin(), m_callbacks.end(), pCallback) == 0)
	{
		m_callbacks.push_back(pCallback);
	}
}

void Logger::RemoveCallback(ILogCallback* pCallback)
{
	m_callbacks.erase(std::remove(m_callbacks.begin(), m_callbacks.end(), pCallback), m_callbacks.end());
}

void Logger::PushMessage(ILog::ELogType type, unsigned int flags, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	PushMessageV(type, flags, format, args);
	va_end(args);
}

void Logger::PushMessageV(ILog::ELogType type, unsigned int flags, const char* format, va_list args)
{
	if (!format)
	{
		// drop messages with null format
		return;
	}

	const int currentVerbosity = GetVerbosityLevel();
	const int requiredVerbosity = GetRequiredVerbosity(type);

	if (currentVerbosity < requiredVerbosity)
	{
		// drop messages above the current verbosity level
		return;
	}

	const int currentFileVerbosity = (m_cvars.fileVerbosity) ? m_cvars.fileVerbosity->GetIVal() : currentVerbosity;

	if (currentFileVerbosity < requiredVerbosity)
	{
		flags &= ~Message::FLAG_FILE;
	}

	Message message;
	message.type = type;
	message.flags = flags;

	BuildMessagePrefix(message);
	BuildMessageContent(message, format, args);

	if (OS::GetCurrentThreadID() == m_mainThreadID)
	{
		WriteMessage(message);
	}
	else
	{
		OS::LockGuard<OS::Mutex> lock(m_mutex);

		m_messages.push_back(message);
	}
}

int Logger::GetRequiredVerbosity(ILog::ELogType type)
{
	switch (type)
	{
		case ILog::eAlways:
		case ILog::eWarningAlways:
		case ILog::eErrorAlways:
		case ILog::eInput:
		case ILog::eInputResponse:
		{
			return 0;
		}
		case ILog::eError:
		{
			return 1;
		}
		case ILog::eWarning:
		{
			return 2;
		}
		case ILog::eMessage:
		{
			return 3;
		}
		case ILog::eComment:
		{
			return 4;
		}
	}

	return 0;
}

static void AddTimeZoneOffset(std::string& result)
{
	long bias = OS::GetCurrentTimeZoneBias();

	if (bias == 0)
	{
		result += 'Z';  // UTC
	}
	else
	{
		char sign = '-';

		if (bias < 0)
		{
			bias = -bias;
			sign = '+';
		}

		StringTools::FormatTo(result, "%c%02u%02u", sign, bias / 60, bias % 60);
	}
}

static void ExpandMessagePrefixSpecifier(std::string& result, const OS::DateTime& time, char specifier)
{
	switch (specifier)
	{
		case '%':
		{
			result += '%';
			break;
		}
		case 't':
		{
			StringTools::FormatTo(result, "%04x", OS::GetCurrentThreadID());
			break;
		}
		case 'd':
		{
			StringTools::FormatTo(result, "%02u", time.day);
			break;
		}
		case 'm':
		{
			StringTools::FormatTo(result, "%02u", time.month);
			break;
		}
		case 'Y':
		{
			StringTools::FormatTo(result, "%04u", time.year);
			break;
		}
		case 'F':
		{
			StringTools::FormatTo(result, "%04u-%02u-%02u", time.year, time.month, time.day);
			break;
		}
		case 'H':
		{
			StringTools::FormatTo(result, "%02u", time.hour);
			break;
		}
		case 'M':
		{
			StringTools::FormatTo(result, "%02u", time.minute);
			break;
		}
		case 'S':
		{
			StringTools::FormatTo(result, "%02u", time.second);
			break;
		}
		case 'T':
		{
			StringTools::FormatTo(result, "%02u:%02u:%02u", time.hour, time.minute, time.second);
			break;
		}
		case 'N':
		{
			StringTools::FormatTo(result, "%03u", time.millisecond);
			break;
		}
		case 'z':
		{
			AddTimeZoneOffset(result);
			break;
		}
	}
}

void Logger::BuildMessagePrefix(Message& message)
{
	if (!m_cvars.prefix)
	{
		// no log prefix until cvars are registered in the engine
		return;
	}

	const StringView prefix = m_cvars.prefix->GetString();

	if (prefix.IsEmpty() || prefix == "0")
	{
		// empty string or "0" means log prefix is disabled
		return;
	}

	const OS::DateTime currentTime = OS::GetCurrentDateTimeLocal();

	for (std::size_t i = 0; i < prefix.length; i++)
	{
		if (prefix[i] == '%')
		{
			if ((i + 1) < prefix.length)
			{
				const char specifier = prefix[++i];

				ExpandMessagePrefixSpecifier(message.prefix, currentTime, specifier);
			}
		}
		else
		{
			message.prefix += prefix[i];
		}
	}

	if (!message.prefix.empty())
	{
		message.prefix += ' ';
	}
}

void Logger::BuildMessageContent(Message& message, const char* format, va_list args)
{
	switch (message.type)
	{
		case ILog::eWarning:
		case ILog::eWarningAlways:
		{
			message.content += CRY_COLOR_CODE_YELLOW_STRING "[Warning] ";
			break;
		}
		case ILog::eError:
		case ILog::eErrorAlways:
		{
			message.content += CRY_COLOR_CODE_RED_STRING "[Error] ";
			break;
		}
		case ILog::eComment:
		{
			message.content += CRY_COLOR_CODE_GRAY_STRING;
			break;
		}
		case ILog::eMessage:
		case ILog::eAlways:
		case ILog::eInput:
		case ILog::eInputResponse:
		{
			break;
		}
	}

	StringTools::FormatToV(message.content, format, args);
}

void Logger::WriteMessage(const Message& message)
{
	if (message.flags & Message::FLAG_FILE)
	{
		WriteMessageToFile(message);
	}

	if (message.flags & Message::FLAG_CONSOLE)
	{
		WriteMessageToConsole(message);
	}
}

void Logger::WriteMessageToFile(const Message& message)
{
	if (!m_file.IsOpen())
	{
		return;
	}

	const bool isAppend = (message.flags & Message::FLAG_APPEND) != 0;

	std::string buffer;
	buffer.reserve(message.prefix.length() + message.content.length() + OS_NEWLINE_LENGTH);

	if (!isAppend)
	{
		buffer += message.prefix;
	}

	for (std::size_t i = 0; i < message.content.length(); i++)
	{
		switch (message.content[i])
		{
			case '\n':
			{
				buffer += OS_NEWLINE;
				break;
			}
			case '$':
			{
				// drop color codes
				i++;

				// "$$" => "$"
				if (i < message.content.length() && message.content[i] == '$')
				{
					buffer += '$';
				}

				break;
			}
			default:
			{
				buffer += message.content[i];
				break;
			}
		}
	}

	buffer += OS_NEWLINE;

	if (isAppend)
	{
		m_file.Seek(OS::File::END, -OS_NEWLINE_LENGTH);
	}

	m_file.Write(buffer.c_str(), buffer.length());

	for (std::size_t i = 0; i < m_callbacks.size(); i++)
	{
		m_callbacks[i]->OnWriteToFile(message.content.c_str(), !isAppend);
	}
}

void Logger::WriteMessageToConsole(const Message& message)
{
	if (!gEnv)
	{
		return;
	}

	IConsole* pConsole = gEnv->pConsole;

	if (!pConsole)
	{
		return;
	}

	const bool isAppend = (message.flags & Message::FLAG_APPEND) != 0;

	if (isAppend)
	{
		pConsole->PrintLinePlus(message.content.c_str());
	}
	else
	{
		pConsole->PrintLine(message.content.c_str());
	}

	for (std::size_t i = 0; i < m_callbacks.size(); i++)
	{
		m_callbacks[i]->OnWriteToConsole(message.content.c_str(), !isAppend);
	}
}
