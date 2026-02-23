#include "Engine/UI/MessageLog.h"

#include <algorithm>

void MessageLog::AddMessage(std::string_view message, uint32_t color)
{
	Messages.push_back(Message{ std::string(message), color });

	while (Messages.size() > MaxStoredMessages)
	{
		Messages.erase(Messages.begin());
	}
}

void MessageLog::Clear()
{
	Messages.clear();
}

std::vector<Message> MessageLog::GetRecentMessages(int count) const
{
	int total = static_cast<int>(Messages.size());
	int start = std::max(0, total - count);

	return std::vector<Message>(Messages.begin() + start, Messages.end());
}