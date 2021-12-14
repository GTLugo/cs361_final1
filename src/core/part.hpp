//
// Created by galex on 12/12/2021.
//

#pragma once

namespace gtl {
  struct Part {
    u32 id{};
    bool broken{false};

    Part(u32 id_, bool broken_) : id{id_}, broken{broken_} {}

//    Part& operator=(Part rhs) {
//      id = rhs.id;
//      repairable_ = rhs.repairable_;
//      priority_ = rhs.repairable_;
//      parts_.swap(rhs.parts_);
//      return *this;
//    }

    bool operator==(const Part& rhs) const { return id == rhs.id; }
  };
}