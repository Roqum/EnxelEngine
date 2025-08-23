#include "Timer.h"
namespace Enxel
{
    Timer::Timer() : m_StartTime(std::chrono::high_resolution_clock::now()) 
    {
    }
    void Timer::Reset()
    {
        m_StartTime = std::chrono::high_resolution_clock::now();
        m_StopTime = std::chrono::high_resolution_clock::time_point{};
    }
    float Timer::GetCurTime() const
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - m_StartTime).count();
    }
    float Timer::GetStartToStopDuration() const
    {
        return std::chrono::duration<float, std::chrono::milliseconds::period>(m_StartTime - m_StopTime).count();
    }
    void Timer::Stop()
    {
		m_StopTime = std::chrono::high_resolution_clock::now();
    }
}