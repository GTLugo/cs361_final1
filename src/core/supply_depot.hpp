//
// Created by galex on 12/12/2021.
//

#pragma once

#include "vehicle.hpp"

namespace gtl {
  class SupplyDepot {
  public:
    SupplyDepot();

    [[nodiscard]] const std::vector<Part>& parts() const { return parts_; }
    [[nodiscard]] std::vector<Part>& parts() { return parts_; }

    void restock();
  private:
    std::vector<Part> parts_{};
  };
}

