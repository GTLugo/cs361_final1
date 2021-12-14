#pragma once

#include <utility>

#include "util/chance.hpp"

namespace gtl {
  class Simulation {
  public:
    explicit Simulation(std::vector<std::string> args);
    ~Simulation() = default;
    
    void run();
  private:
    std::vector<std::string> args_;
    bool shouldStop_{false};
    fge::Time time_{32.};
    u32 timeStep_{1};
    u32 maxTimeStep_{32};

    // Sim specific

    void fixedUpdateLoop(const std::function<void()>& action);
  };
}