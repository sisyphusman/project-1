#pragma once

#include "Engine/UI/Panel/UIPanel.h"
#include "Engine/UI/MessageLog.h"

#include <memory>

// 메시지 로그 패널
class MessageLogPanel : public UIPanel
{
public:
	MessageLogPanel(float x, float y, float width, float height);

	void Update(float deltaTime) override; // 스크롤 기능
	void Render(sf::RenderWindow& window, const sf::Font& font) override;

	// MessageLog 접근
	MessageLog&		  GetLog() { return Log; }		 // non-const 객체에서 호출 가능
	const MessageLog& GetLog() const { return Log; } // 읽기 전용

private:
	MessageLog Log;
	int		   MaxVisibleLines; // 볼 수 있는 로그의 개수
};