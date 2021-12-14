#pragma once
//
// Created by Gabriel Lugo on 3/27/2021.
// Flugel Game Engine: https://github.com/GTLugo/flugel_engine
//

#include <chrono>
#include <iostream>

namespace fge {
  // duration types
  using NanoSeconds = std::chrono::duration<double, std::nano>;
  using MicroSeconds = std::chrono::duration<double, std::micro>;
  using MilliSeconds = std::chrono::duration<double, std::milli>;
  using Seconds = std::chrono::duration<double>;
  using Minutes = std::chrono::duration<double, std::ratio<60>>;
  using Hours = std::chrono::duration<double, std::ratio<3600>>;
  // clock types
  using ClockSystem = std::chrono::system_clock;
  using ClockSteady = std::chrono::steady_clock;
  using ClockAccurate = std::chrono::high_resolution_clock;
  // time point
  using TimePoint = ClockSteady::time_point;
  using TimePointAccurate = ClockAccurate::time_point;
  using TimePointSystem = ClockSystem::time_point;

  class Stopwatch {
  public:
    Stopwatch() {
      start();
    }

    explicit Stopwatch(TimePoint timePoint) {
      start(timePoint);
    }

    void start(TimePoint timePoint) {
      start_ = timePoint;
    }

    void start() { start(ClockSteady::now()); }

    template<typename Duration>
    [[nodiscard]] double startTime() const {
      return Duration::duration((start_).time_since_epoch()).count();
    }

    template<typename Duration>
    [[nodiscard]] double getTimeElapsed() const {
      return Duration::duration((ClockSteady::now() - start_)).count();
    }
  private:
    TimePoint start_{};
  };

  class Time {
  public:
    explicit Time(double tickRate, uint32_t bailCount = 1024U)
      : tickRate_{tickRate},
        bailCount_{bailCount},
        stopwatch_{ClockSteady::now()}, 
        gameLast_{ClockSteady::now()}, 
        gameCurrent_{ClockSteady::now()} {
      fixedTimeStep_ = Seconds{1. / tickRate_};
    }
    ~Time() = default;

    [[nodiscard]] double tickRate() const { return tickRate_; }

    template<class Duration>
    [[nodiscard]] double fixedStep() const {
      return Duration::duration((fixedTimeStep_)).count();
    }

    template<class Duration>
    [[nodiscard]] double start() const {
      return stopwatch_.startTime<Duration>();
    }

    template<class Duration>
    [[nodiscard]] double sinceStart() const {
      return stopwatch_.getTimeElapsed<Duration>();
    }

    template<typename Duration>
    static double now() {
      return Duration::duration(ClockSteady::now().time_since_epoch()).count();
    }

    template<class Duration>
    [[nodiscard]] double delta() const {
      return Duration::duration((delta_)).count();
    }

    template<class Duration>
    [[nodiscard]] double lag() const {
      return Duration::duration((lag_)).count();
    }

    void update() {
      // FLUGEL_ENGINE_TRACE("Update!");
      gameCurrent_ = ClockSteady::now();
      // Seconds::duration()
      delta_ = std::chrono::duration_cast<Seconds>(gameCurrent_ - gameLast_);
      gameLast_ = gameCurrent_;
      lag_ += delta_;
      stepCount_ = 0U;
    }

    void tick() {
      // FLUGEL_ENGINE_TRACE("Tick!");
      lag_ -= fixedTimeStep_;
      ++stepCount_;
    }

    [[nodiscard]] bool shouldDoTick() const {
      #ifndef NDEBUG
        if (stepCount_ >= bailCount_) {
          std::cerr << "Struggling to catch up with physics rate!\n";
        }
      #endif
      
      return lag_.count() >= fixedTimeStep_.count() && stepCount_ < bailCount_;
    }
  private:
    // fixed number of ticks per second. this will be used for physics and anything else in fixed update
    double tickRate_{};
    Seconds fixedTimeStep_{};
    // bail out of the fixed updates if iterations exceeds this amount to prevent lockups
    // on extremely slow systems where updateFixed may be longer than fixedTimeStep_
    uint32_t bailCount_{};

    const Stopwatch stopwatch_;
    TimePoint gameLast_{}; // when last frame started
    TimePoint gameCurrent_{}; // when this frame started
    Seconds delta_{Seconds{1. / 60.}}; // how much time last frame took
    Seconds lag_{Seconds::zero()}; // how far behind the game is from real world
    uint32_t stepCount_{0U};
  };
}