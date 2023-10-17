#include <chrono>
#include <thread>

class Timer
{
public:
	template<class DurationType = std::chrono::nanoseconds>
	int64_t Time() const noexcept
	{
		return std::chrono::duration_cast<DurationType>(std::chrono::high_resolution_clock::now() - start).count();
	}

	template<class T, class DurationType = std::chrono::nanoseconds>
	void Sleep(T t)
	{
		std::this_thread::sleep_for((DurationType)t);
	}

	//void Timer::Accumulate() <- this will be how we add all the time we spent executing instructions

	void Reset() noexcept;

private:
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
};