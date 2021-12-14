#include "core/simulation.hpp"

int main(const int argc, const char* argv[]) {
  try {
    // Convert args to more convenient format
    std::vector<std::string> args(argc);
    for (int i = 0; i < argc; ++i) {
      args[i] = argv[i];
    }

    gtl::Simulation sim{args};
    sim.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}