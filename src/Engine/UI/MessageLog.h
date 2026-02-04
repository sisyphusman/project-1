#pragma once

#include <string>
#include <cstdint>
#include <chrono>
#include <span>

struct LogEntry
{
	std::string							  text;
	uint32_t							  color{ 0xFFFFFFFF };
	std::chrono::steady_clock::time_point timestamp{ std::chrono::steady_clock::now() };
};

// 스크롤 가능한 메시지 로그
class MessageLog
{
public:
	static constexpr size_t MaxMessages = 1000;
	static constexpr int	ScrollStep = 1;

	void AddMessage(std::string_view message, uint32_t color = 0xFFFFFFFF);
	void Clear();

	void ScrollUp(int lines = ScrollStep);
	void ScrollDown(int lines = ScrollStep);
	void ScrollToBottom();
	void ScrollToTop();

	[[nodiscard]] std::span<const LogEntry> GetVisibleEntries(int maxLines) const; // 로그 기록들의 일부를 읽기 전용으로 반환
	[[nodiscard]] size_t					GetTotalCount() const noexcept { return m_entries.size(); }
	[[nodiscard]] int						GetScrollOffset() const noexcept { return m_scrollOffset; }
	[[nodiscard]] bool						IsAtBottom() const noexcept { return m_scrollOffset == 0; }

private:
	std::vector<LogEntry> m_entries;
	int					  m_scrollOffset{ 0 }; // 최신 메시지 기준 스크롤 오프셋
};