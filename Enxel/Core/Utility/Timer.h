#pragma once
#include <chrono>

namespace Enxel
{
    class Timer
    {
	public:
		Timer();
		virtual ~Timer() = default;

		void Reset();		
		float GetCurTime() const;
		float GetStartToStopDuration() const;
		void Stop();

	private:
		std::chrono::high_resolution_clock::time_point m_StartTime;
		std::chrono::high_resolution_clock::time_point m_StopTime;

    };
}