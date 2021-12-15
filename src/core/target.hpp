//
// Created by galex on 12/14/2021.
//

#pragma once

namespace gtl {
  class Target {
  public:
    Target(bool enemy, i32 distance, char type);

    [[nodiscard]] i32 score() const { return distance_ * (enemy_ ? -1 : 1); }
    [[nodiscard]] char type() const { return type_; }
    //[[nodiscard]] bool isEnemy() const { return enemy_; }

    [[nodiscard]] std::string display() const;

    auto operator<=>(const Target&) const = default;
    bool operator==(const Target& rhs) const { return distance_ == rhs.distance_; }
    bool operator<(const Target& rhs) const { return distance_ < rhs.distance_; }

    friend std::ostream& operator<<(std::ostream& o, const Target& v);
  private:
    bool enemy_;
    i32 distance_;
    char type_;
  };
}

