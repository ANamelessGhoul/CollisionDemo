#include "stats.h"

using namespace std::chrono; 

Stats::Stats(/* args */)
{
}

Stats::~Stats()
{
}

void Stats::AddTimer(const Timer& timer)
{
    averageTime.AddValue(timer.GetDuration().count());
    //totalTime += timer.GetDuration();
}

Stats& Stats::GetInstance()
{
    static Stats *instance = new Stats();
    return *instance;
}


void Timer::Start()
{
    startTime = steady_clock::now();
}

void Timer::Stop()
{
    endTime = steady_clock::now();
}

duration<double, std::micro> Timer::GetDuration() const
{
    return duration<double, std::micro>(endTime - startTime);
}