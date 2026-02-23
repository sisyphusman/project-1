#include "MessageCatalog.h"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Engine/Core/Macros.h"

namespace
{
	using Json = nlohmann::json;
}

bool MessageCatalog::LoadFromManifestFile(const std::string& manifestPath, std::string& outError)
{
	MessageMap.clear();

	std::ifstream input(manifestPath);
	if (!input.is_open())
	{
		outError = "메시지 manifest 파일을 열 수 없습니다" + manifestPath;
		return false;
	}

	Json manifest;
	try
	{
		input >> manifest;
	}
	catch (const Json::parse_error& parseException)
	{
		outError = std::string("메시지 manifest 파싱 실패:") + parseException.what();
	}

	if (!manifest.contains("modules") || !manifest["modules"].is_array())
	{
		outError = "메시지 manifest에 modules 배열이 없습니다";
		return false;
	}

	const std::filesystem::path manifestFolder = std::filesystem::path(manifestPath).parent_path();

	for (const Json& moduleNode : manifest["modules"])
	{
		if (!moduleNode.is_string())
		{
			outError = "메시지 manifest의 modules 요소는 문자열이어야 합니다";
			return false;
		}

		// 오버로딩 / 연산자 -> 경로 결합
		const std::filesystem::path modulePath = manifestFolder / moduleNode.get<std::string>();
		if (!LoadMessageModule(modulePath.string(), outError))
		{
			return false;
		}
	}
	return true;
}

std::string MessageCatalog::Get(std::string_view key) const
{
	const auto found = MessageMap.find(std::string(key));

	// 텍스트 키 누락
	GAME_CHECK(found != MessageMap.end());
	return found->second;
}

std::string MessageCatalog::Format(std::string_view key, const FormatArgs& args) const
{
	std::string resolved = Get(key);

	for (const auto& [name, value] : args)
	{
		ReplaceAllInPlace(resolved, "{" + name + "}", value);
	}

	return resolved;
}

bool MessageCatalog::LoadMessageModule(const std::string& modulePath, std::string& outError)
{
	std::ifstream input(modulePath);
	if (!input.is_open())
	{
		outError = "메시지 모듈 파일을 열 수 없습니다: " + modulePath;
		return false;
	}

	Json module;
	try
	{
		input >> module;
	}
	catch (const Json::parse_error& parseException)
	{
		outError = std::string("메시지 모듈 파싱 실패: ") + parseException.what();
		return false;
	}

	for (const auto& [key, value] : module["messages"].items())
	{
		if (!value.is_string())
		{
			outError = "메시지 값은 문자열이어야 합니다: " + modulePath + " / " + key;
			return false;
		}

		MessageMap[key] = value.get<std::string>();
	}

	return true;
}

void MessageCatalog::ReplaceAllInPlace(std::string& source, const std::string& from, const std::string& to)
{
	if (from.empty())
	{
		return;
	}

	size_t startPos = 0;
	while ((startPos = source.find(from, startPos)) != std::string::npos)
	{
		source.replace(startPos, from.length(), to);
		startPos += to.length();
	}
}
