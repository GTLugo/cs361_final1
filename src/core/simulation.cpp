#include "simulation.hpp"

namespace gtl {
  void Simulation::run() {
    std::cout << "Simulation started... max timesteps: " << maxTimeStep_ << '\n';

    fixedUpdateLoop([&]{
      std::cout << "-----------------------------\n";
      std::cout << "Day " << timeStep_ << ":\n";

      i32 vehicleCount{Chance::normalIntBetween(avgVehicles_, stdDevVehicles_, 0, 8)};
      generateVehicles(vehicleCount);

      repairVehicles();

      testVehicles();
    });

    std::cout << "-----------------------------\n";
    std::cout << "Simulation complete!\n";
  }

  void Simulation::generateVehicles(i32 count) {
    std::cout << "Generate Vehicles:\n";
    std::vector<Vehicle> toAdd;
    toAdd.reserve(count);
    for (int i = 0; i < count; ++i) {
      toAdd.emplace_back(Vehicle::generateType());
    }

    for (const auto& v : toAdd) {
      std::cout << "> " << v << "\n";
      if (v.repairable()) {
        waitingVehicles_.push(v);
      } else {
        for (const auto& p : v.parts()) {
          scrapParts_.emplace_back(p.id, false);
        }
      }
    }

    std::cout << "> Q size: " << waitingVehicles_.size() << "\n";
  }

  void Simulation::repairVehicles() {
    std::cout << "Fill Bays:\n";
    for (i32 i = 0; i < 3; ++i) {
      auto& bay = repairBays_[i];
      while (!waitingVehicles_.empty() && bay.type() == Vehicle::None) {
        //        std::cout << "Filling bay!\n";
        bay = waitingVehicles_.top();
        waitingVehicles_.pop();
        std::cout << "> Bay " << i << ": " << bay << "\n";
      }
    }

    std::cout << "Repair Vehicles:\n";
    for (i32 i = 0; i < 3; ++i) {
      auto& bay = repairBays_[i];
      if (bay.type() == Vehicle::None) continue;

      if (bay.hasBrokenParts()) {
        if (bay.tryRepair(scrapParts_)) {
          std::cout << "> Repaired Bay " << i << ": " << bay << "\n";
        } else if (bay.tryRepair(depot_.parts())) {
          std::cout << "> Repaired Bay " << i << ": " << bay << "\n";
          depot_.restock();
        } else {
          bay.setPriority(bay.priority() + 1);
          std::cout << "> Failed Bay " << i << ": " << bay << "\n";
        }
      } else {
        testingZone_.push_back(bay);
        std::cout << "> Fully Repaired Bay " << i << ": " << bay << "\n";
        bay.setType(Vehicle::None);
      }
    }
  }

  void Simulation::testVehicles() {
    std::cout << "Test Vehicles:\n";
    for (auto it = testingZone_.begin(); it != testingZone_.end();) {
      if (Chance::flip()) {
        std::cout << "> Passed Test: " << *it << "\n";
        it = testingZone_.erase(it);
      } else {
        for (auto& p : it->parts()) {
          p.broken = Chance::flip();
        }
        waitingVehicles_.push(*it);
        std::cout << "> Failed Test: " << *it << "\n";
      }

      if (it != testingZone_.end()) {
        ++it;
      }
    }
  }
  
  void Simulation::fixedUpdateLoop(const std::function<void()>& action) {
    while (!shouldStop_) {
      while (time_.shouldDoTick()) {
        if (timeStep_ <= maxTimeStep_) {
          action();

          ++timeStep_;
        } else {
          shouldStop_ = true;
        }
        time_.tick();
      }
      time_.update();
    }
  }
}