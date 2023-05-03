#ifndef STATS_H
#define STATS_H

#include <chrono>

template <typename T>
class Average
{
private:
    T average;
    long long count;
public:
    void AddValue(T value);
    T GetValue();
};

//typename std::enable_if<std::is_arithmetic<T>::value>::type Average<T>::AddValue(T value)
template <typename T>
void Average<T>::AddValue(T value)
{
    double newCount = static_cast<double>(count + 1);
    average = static_cast<T>(average * (count / newCount) + value / newCount);
    count += 1;
}

template <typename T>
T Average<T>::GetValue()
{
    return average;
}


class Timer
{
public:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    void Start();
    void Stop();
    std::chrono::duration<double, std::micro> GetDuration() const;
};

class Stats
{
private:
    Stats();

public:
    std::chrono::duration<double, std::micro> totalTime { 0 };
    Average<double> averageTime;
    Average<double> averageQueries;
    ~Stats();
    static Stats& GetInstance();
    void AddTimer(const Timer& timer);

};





#endif // STATS_H