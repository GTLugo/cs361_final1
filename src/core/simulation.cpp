#include "simulation.hpp"

namespace gtl {
  Simulation::Simulation(std::vector<std::string> args)
      : args_{std::move(args)} {}

  void Simulation::run() {
    std::cout << "Please enter number of timesteps to simulate: ";
    std::cin >> maxTimeStep_;
    while (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input! Please enter integer number of timesteps to simulate: ";
      std::cin >> maxTimeStep_;
    }

    std::cout << "Simulation started... max timesteps: " << maxTimeStep_ << '\n';

    fixedUpdateLoop([&]{
      std::cout << "-----------------------------\n";
      std::cout << "T" << timeStep_ << ":\n";
    });

    std::cout << "-----------------------------\n";
    std::cout << "Simulation complete!\n";
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