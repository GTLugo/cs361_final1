#pragma once

#include <utility>

#include "target.hpp"

namespace gtl {
  class Simulation {
  public:
    explicit Simulation(std::vector<std::string> args);
    ~Simulation() = default;
    
    void run();
  private:
    std::vector<std::string> args_;
    bool shouldStop_{false};
    fge::Time time_{128.};
    u32 timeStep_{1};
    u32 maxTimeStep_{32};

    // Sim specific
    std::vector<std::unique_ptr<Target>> hitTargets_{};
    std::vector<std::unique_ptr<Target>> freeTargets_{};
    std::array<std::unique_ptr<Target>, 4> selectedTargets_{};
    std::array<float, 4> weaponRates_{.96f, .98f, .9f, .88f};

    void generateRandomTarget();
    void assignTargetsToWeapons();
    void fireWeapons();
    bool alreadyAssigned(std::unique_ptr<Target>& target);

    void fixedUpdateLoop(const std::function<void()>& action);
  };
}