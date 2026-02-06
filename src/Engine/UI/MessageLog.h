#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <vector>

// 로그 데이터
struct Message
{
	std::string Text;
	uint32_t	Color;
};

class MessageLog
{
public:
	MessageLog() = default;

	void AddMessage(std::string_view message, uint32_t color = 0xFFFFFFFF);
	void Clear();

	// 저장/로드용
	const std::vector<Message>& GetMessages() const { return Messages; }
	int							GetMessageCount() const { return static_cast<int>(Messages.size()); }

	// 최근 N개 가져오기
	std::vector<Message> GetRecentMessages(int count) const;

private:
	std::vector<Message> Messages;

	static constexpr int MaxStoredMessages = 1000; // 저장할 수 있는 메시지 개수
};