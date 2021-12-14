#include "core/simulation.hpp"

int main() {
  try {
    gtl::Simulation sim{};
    sim.run();
  } catch (const std::exception& e) {
    std::cerr << e.what();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}