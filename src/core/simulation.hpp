#pragma once

#include "util/chance.hpp"

#include "vehicle.hpp"
#include "supply_depot.hpp"

namespace gtl {
  class Simulation {
  public:
    Simulation() = default;
    ~Simulation() = default;
    
    void run();
  private:
    bool shouldStop_{false};
    fge::Time time_{10.};
    u32 timeStep_{1};
    u32 maxTimeStep_{100};

    // Sim specific
    float avgVehicles_{3};
    float stdDevVehicles_{1};
    SupplyDepot depot_{};

    std::priority_queue<Vehicle> waitingVehicles_{};
    std::vector<Part> scrapParts_{};
    std::array<Vehicle, 3> repairBays_;
    std::vector<Vehicle> testingZone_{};

    void generateVehicles(i32 count);
    void repairVehicles();
    void testVehicles();

    void fixedUpdateLoop(const std::function<void()>& action);
  };
}