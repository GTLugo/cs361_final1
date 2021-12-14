//
// Created by galex on 12/12/2021.
//

#include "supply_depot.hpp"

namespace gtl {
  SupplyDepot::SupplyDepot() {
    restock();
  }

  void SupplyDepot::restock() {
    for (u32 i = 1; i <= 40; ++i) {
      if (!std::any_of(parts_.begin(), parts_.end(), [i](const Part& p){ return p.id == i; })) {
        parts_.emplace_back(i, false);
      }
    }
  }
}