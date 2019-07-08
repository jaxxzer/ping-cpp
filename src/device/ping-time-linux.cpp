#include "ping-time.h"
#include <chrono>

int time_ms()
{
    // std::chrono::time_point
    auto now = std::chrono::high_resolution_clock::now();

    // std::chrono::duration
    auto duration = now.time_since_epoch();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    return duration_ms.count();
}
