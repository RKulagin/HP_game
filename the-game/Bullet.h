#pragma once

#include "Movable.h"
#include <cmath>

namespace rtf {

class Bullet : public Movable {
private:
public:
  Bullet(const std::string &filename, sf::Vector2f start_position);
  void Update(sf::RenderWindow *window, sf::Time time, Scene *scene) override;
  void SetDirection(const GameObject &parent, sf::Vector2f from,
                    sf::Vector2f to);
  void SetDirection(sf::Vector2f direction);
  void Move();
  float Angle() const {
    return std::asin((directionPerSecond_.x)) < 0
               ? std::acos(directionPerSecond_.y)
               : -std::acos((directionPerSecond_).y);
  }
  void OnCollision(GameObject &obj) override;

private:
  sf::Vector2f directionPerSecond_;
};
} // namespace rtf