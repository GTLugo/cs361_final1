//
// Created by galex on 12/11/2021.
//

#pragma once

#include "part.hpp"

namespace gtl {
  class Vehicle {
  public:
    enum Type {
      None,
      AV1, AV2, AV3,
      APC,
      S1, S2, S3, S4,
    };

    explicit Vehicle() : Vehicle{None} {}

    explicit Vehicle(Type type) : Vehicle{type, 0, Chance::flip()} {}

    explicit Vehicle(Type type, i32 priority, bool repairable = true)
        : type_{type}, priority_{priority}, repairable_{repairable} {
      generateParts();
    }

    Vehicle(const Vehicle& rhs)
        : Vehicle{rhs.type(), rhs.priority(), rhs.repairable()} {
      for (const auto& p : rhs.parts()) {
        parts_.emplace_back(p.id, p.broken);
      }
    }

    [[nodiscard]] Type type() const { return type_; }
    [[nodiscard]] bool repairable() const { return repairable_; }
    [[nodiscard]] i32 priority() const { return priority_; }
    [[nodiscard]] const std::vector<Part>& parts() const { return parts_; }
    [[nodiscard]] std::vector<Part>& parts() { return parts_; }

    void setType(Type type) { type_ = type; }
    void setPriority(i32 priority) { priority_ = priority; }

    bool tryRepair(std::vector<Part>& parts);
    bool hasBrokenParts();

    [[nodiscard]] std::string display() const;

    static Type generateType() {
      switch (Chance::intBetween(0, 7)) {
        case 0: return AV1;
        case 1: return AV2;
        case 2: return AV3;
        case 3: return APC;
        case 4: return S1;
        case 5: return S2;
        case 6: return S3;
        case 7: return S4;
        default: throw std::range_error("Invalid vehicle type!");
      }
    }

    bool operator<(const Vehicle& rhs) const { return priority() < rhs.priority(); }

    Vehicle& operator=(Vehicle rhs) {
      type_ = rhs.type_;
      repairable_ = rhs.repairable_;
      priority_ = rhs.repairable_;
      parts_.swap(rhs.parts_);
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& o, const Vehicle& v);
  private:
    Type type_;
    bool repairable_;
    i32 priority_;
    std::vector<Part> parts_{};

    void generateParts();
  };
}