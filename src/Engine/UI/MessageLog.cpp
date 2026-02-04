#include "Engine/UI/MessageLog.h"

#include <algorithm>

void MessageLog::AddMessage(std::string_view message, uint32_t color)
{
	m_entries.push_back(LogEntry{
		.text = std::string(message),
		.color = color,
		.timestamp = std::chrono::steady_clock::now() });

	// 최대 메시지 수 제한
	while (m_entries.size() > MaxMessages)
	{
		m_entries.erase(m_entries.begin());
		if (m_scrollOffset > 0)
		{
			--m_scrollOffset;
		}
	}
}

void MessageLog::Clear()
{
	m_entries.clear();
	m_scrollOffset = 0;
}

void MessageLog::ScrollUp(int lines)
{
	const int maxOffset = static_cast<int>(m_entries.size()) - 1;
	m_scrollOffset = std::min(m_scrollOffset + lines, std::max(0, maxOffset));
}

void MessageLog::ScrollDown(int lines)
{
	m_scrollOffset = std::max(0, m_scrollOffset - lines);
}

void MessageLog::ScrollToBottom()
{
	m_scrollOffset = 0;
}

void MessageLog::ScrollToTop()
{
	m_scrollOffset = std::max(0, static_cast<int>(m_entries.size()) - 1);
}

std::span<const LogEntry> MessageLog::GetVisibleEntries(int maxLines) const
{
	// 예외 처리
	if (m_entries.empty() || maxLines <= 0)
	{
		return {};
	}

	const int totalEntries = static_cast<int>(m_entries.size());

	// 화면에 표시할 마지막 인덱스
	const int endIdx = totalEntries - m_scrollOffset;

	// 화면에 표시할 시작 인덱스
	const int startIdx = std::max(0, endIdx - maxLines);

	const int count = endIdx - startIdx;
	if (count <= 0)
	{
		return {};
	}

	// totalEntries = 10, endIdx = 10 - 0 = 10, startIdx = max(0, 10 - 4) = 6, count = 10 - 6 = 4

	return std::span<const LogEntry>(
		&m_entries[static_cast<size_t>(startIdx)],
		static_cast<size_t>(count));
}