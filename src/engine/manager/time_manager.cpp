#include "engine/manager/time_manager.h"

TimeManager::TimeManager()
	: delta_time_(0.0f), elapsed_time_(0.0f)
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::Update()
{
	// 프레임동안 지난 시간
	sf::Time frame_time = clock_.restart();
	
	// 타입 변환
	delta_time_ = frame_time.asSeconds();

	// 시간 누적
	elapsed_time_ += delta_time_;
}

float TimeManager::GetDeltaTime() const
{
	return delta_time_;
}

float TimeManager::GetElapsedTime() const
{
	return elapsed_time_;
}
