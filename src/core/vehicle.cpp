//
// Created by galex on 12/11/2021.
//

#include "vehicle.hpp"

namespace gtl {
  bool Vehicle::tryRepair(std::vector<Part>& parts) {
//    std::cout << "> Parts list size: " << parts.size() << "\n";
    for (auto& part : parts_) {
      auto it = std::find(parts.begin(), parts.end(), part);
      if (part.broken && it != parts.end()) {
        //        std::cout << "> Fixed part!\n";
        part.broken = false;
        parts.erase(it);
        return true;
      }
    }
    return false;
  }

  bool Vehicle::hasBrokenParts() {
    return std::any_of(parts_.begin(), parts_.end(), [](const Part& p){ return p.broken; });
  }

  void Vehicle::generateParts() {
    for (i32 i = 1; i <= 40; ++i) {
      switch (type()) {
        case AV1:
          if ((i >= 1 && i <= 10) || (i >= 12 && i <= 20 && i % 2 == 0)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case AV2:
          if ((i >= 1 && i <= 10) || (i >= 11 && i <= 19 && i % 2 == 1)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case AV3:
          if ((i >= 2 && i <= 40 && i % 2 == 0)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case APC:
          if ((i >= 1 && i <= 5) || (i >= 20 && i <= 25)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case S1:
          if ((i >= 20 && i <= 30)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case S2:
          if ((i >= 30 && i <= 40)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case S3:
          if ((i >= 20 && i <= 40 && i % 2 == 0)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case S4:
          if ((i >= 35 && i <= 40)) {
            parts_.emplace_back(i, Chance::flip());
          }
          break;
        case None:
          break;
        default: throw std::range_error("Invalid part type!");
      }
    }
  }

  std::string Vehicle::display() const {
    std::stringstream buffer{};
    buffer << "Vehicle <";
    switch (type()) {
      case AV1:  buffer << "AV1"; break;
      case AV2:  buffer << "AV2"; break;
      case AV3:  buffer << "AV3"; break;
      case APC:  buffer << "APC"; break;
      case S1:   buffer << "S1";  break;
      case S2:   buffer << "S2";  break;
      case S3:   buffer << "S3";  break;
      case S4:   buffer << "S4";  break;
      case None: return "Vehicle <None>";
      default: throw std::range_error("Invalid vehicle type!");
    }

    buffer << "> (Repairable: " << repairable()
           << ", Priority: " << priority()
           << ")";

    return buffer.str();
  }

  std::ostream& operator<<(std::ostream& o, const Vehicle& v) {
    return o << v.display();
  }
}
