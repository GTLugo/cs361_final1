//
// Created by galex on 12/11/2021.
//

#pragma once

namespace gtl {
  class Chance {
  public:
    [[nodiscard]] static i32 roll(u32 sides) {
      return intBetween(1, static_cast<i32>(sides));
    }

    [[nodiscard]] static bool flip() {
      return roll(2) - 1;
    }

    [[nodiscard]] static float floatBetween(float lowerIncl, float upperExcl) {
      std::uniform_real_distribution<float> dist{lowerIncl, upperExcl};
      return dist(randomDevice());
    }

    [[nodiscard]] static i32 intBetween(i32 lowerIncl, i32 upperIncl) {
      std::uniform_int_distribution<i32> dist{lowerIncl, upperIncl};
      return dist(randomDevice());
    }

    [[nodiscard]] static float normalFloat(float mean, float stddev) {
      std::normal_distribution<float> dist{mean, stddev};
      return dist(randomDevice());
    }

    [[nodiscard]] static i32 normalInt(float mean, float stddev) {
      std::normal_distribution<float> dist{mean, stddev};
      return static_cast<i32>(std::floor(dist(randomDevice())));
    }

    [[nodiscard]] static float normalFloatBetween(float mean, float stddev, float lowerIncl, float upperIncl) {
      return std::clamp(normalFloat(mean, stddev), lowerIncl, upperIncl);
    }

    [[nodiscard]] static i32 normalIntBetween(float mean, float stddev, i32 lowerIncl, i32 upperIncl) {
      return std::clamp(normalInt(mean, stddev), lowerIncl, upperIncl);
    }

  private:
    static std::random_device& randomDevice() {
      static std::random_device device;
      return device;
    }
  };
}