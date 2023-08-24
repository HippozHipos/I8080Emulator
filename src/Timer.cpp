#include "Timer.h"

void Timer::Reset() noexcept
{
	start = std::chrono::high_resolution_clock::now();
}

