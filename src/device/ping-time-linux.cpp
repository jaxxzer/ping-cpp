#include <ping-time.h>

#include <chrono>
#include <unistd.h>

int PingTime::timeMs() {
  // std::chrono::time_point
  auto now = std::chrono::high_resolution_clock::now();

  // std::chrono::duration
  auto duration    = now.time_since_epoch();
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

  return duration_ms.count();
}

void PingTime::microsecondDelay(unsigned int microseconds) { usleep(microseconds); }
