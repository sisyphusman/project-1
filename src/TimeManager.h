#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class TimeManager
{
public:
	// 메이어스 싱글톤
	static TimeManager& GetInstance()
	{
		static TimeManager time_manager;
		return time_manager;
	}

	void Update();
	float GetDeltaTime() const;
	float GetElapsedTime() const;

private:
	// 메이어스 싱글톤
	TimeManager();
	~TimeManager();

	TimeManager(const TimeManager& ref) = delete;
	TimeManager& operator=(const TimeManager& ref) = delete;

	// 변수
	sf::Clock clock_;
	float delta_time_;
	float elapsed_time_;
};

