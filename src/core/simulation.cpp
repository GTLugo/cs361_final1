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

      generateRandomTarget();
      std::cout << "Free Targets:\n";
      for (const auto& t : freeTargets_) {
        std::cout << "> " << *t << "\n";
      }

      assignTargetsToWeapons();
      std::cout << "Selected Targets:\n"
                << "> Type 1 <Surface>: " << (selectedTargets_[0] ? selectedTargets_[0]->display() : "None") << "\n"
                << "> Type 2 <Subsurface>: " << (selectedTargets_[1] ? selectedTargets_[1]->display() : "None") << "\n"
                << "> Type 3 <Aircraft>: " << (selectedTargets_[2] ? selectedTargets_[2]->display() : "None") << "\n"
                << "> Type 4 <Surface/Aircraft>: " << (selectedTargets_[3] ? selectedTargets_[3]->display() : "None") << "\n";

      fireWeapons();
      std::cout << "Hit Targets:\n";
      for (const auto& t : hitTargets_) {
        std::cout << "> " << *t << "\n";
      }

      hitTargets_.clear();
    });

    std::cout << "-----------------------------\n";
    std::cout << "Simulation complete!\n";
  }

  void Simulation::generateRandomTarget() {
    char c{' '};
    switch (Chance::intBetween(0, 2)) {
      case 0: c = 'A'; break;
      case 1: c = 'S'; break;
      case 2: c = 'U'; break;
      default: throw std::runtime_error("Invalid value for Target type!");
    }
    freeTargets_.push_back(std::make_unique<Target>(Chance::flip(), Chance::intBetween(0, 100), c));
  }

  void Simulation::assignTargetsToWeapons() {
    std::sort(freeTargets_.begin(),
              freeTargets_.end(),
              [](const std::unique_ptr<Target>& lhs, const std::unique_ptr<Target>& rhs){
      return *lhs > *rhs;
    });

    for (auto& target : freeTargets_) {
      if (target->score() < 0 && !alreadyAssigned(target)) {
        switch (target->type()) {
          case 'A': {
            if (selectedTargets_[2] == nullptr) {
              selectedTargets_[2] = std::move(target);
            } else if (selectedTargets_[3] == nullptr) {
              selectedTargets_[3] = std::move(target);
            }
            break;
          }
          case 'S': {
            if (selectedTargets_[0] == nullptr) {
              selectedTargets_[0] = std::move(target);
            } else if (selectedTargets_[3] == nullptr) {
              selectedTargets_[3] = std::move(target);
            }
            break;
          }
          case 'U': {
            if (selectedTargets_[1] == nullptr) {
              selectedTargets_[1] = std::move(target);
            }
            break;
          }
          default: throw std::runtime_error("Invalid type for Target!");
        }
      }
    }

    for (auto it = std::begin(freeTargets_); it != std::end(freeTargets_);) {
      if (*it == nullptr) {
        it = freeTargets_.erase(it);
      }
      if (it != std::end(freeTargets_)) {
        ++it;
      }
    }
  }

  void Simulation::fireWeapons() {
    if (selectedTargets_.size() != weaponRates_.size()) {
      throw std::range_error("Weapon size doesn't match rate size!");
    }

    for (u32 i = 0; i < selectedTargets_.size(); ++i) {
      if (selectedTargets_[i] != nullptr) {
        if (Chance::floatBetween(0, 1) <= weaponRates_[i]) {
          hitTargets_.push_back(std::move(selectedTargets_[i]));
        } else {
          selectedTargets_[i] = nullptr;
        }
      }
    }
  }

  bool Simulation::alreadyAssigned(std::unique_ptr<Target>& target) {
    return std::any_of(selectedTargets_.begin(), selectedTargets_.end(), [&target](std::unique_ptr<Target>& t) {
      return t == target;
    }) || std::any_of(hitTargets_.begin(), hitTargets_.end(), [&target](std::unique_ptr<Target>& t) {
      return t == target;
    });
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