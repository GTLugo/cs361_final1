//
// Created by galex on 12/14/2021.
//

#include "target.hpp"

namespace gtl {
  Target::Target(bool enemy, i32 distance, char type)
      : enemy_{enemy}, distance_{distance}, type_{type} {}

  std::string Target::display() const {
    std::stringstream buffer{};

    buffer << type() << " (" << score() << ")";

    return buffer.str();
  }

  std::ostream& operator<<(std::ostream& o, const Target& v) {
    return o << v.display();
  }
}