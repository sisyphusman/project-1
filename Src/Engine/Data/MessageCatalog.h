#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

class MessageCatalog
{
public:
	using FormatArgs = std::unordered_map<std::string, std::string>;

	// manifast.json에서 locale에 맞는 modules 순서대로 병합 로드, 문자열 치환은 {enemy} 같은 placeholder를 사용
	bool LoadFromManifestFile(const std::string& manifestPath, std::string& outError);

	[[nodiscard]] std::string Get(std::string_view key) const; // key의 value 값 반환
	[[nodiscard]] std::string Format(std::string_view key, const FormatArgs& args) const;

private:
	bool		LoadMessageModule(const std::string& modulePath, std::string& outError);				// 모듈 1개 로드 후 MessageMap에 병합
	static void ReplaceAllInPlace(std::string& source, const std::string& from, const std::string& to); // 문자열 치환

	std::unordered_map<std::string, std::string> MessageMap;
};