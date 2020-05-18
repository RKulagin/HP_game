#pragma once

#include "Movable.h"

namespace rtf {

class Enemy : public Shootable {
  bool is_alive_ = true;

protected:
  GameObject *target_ = nullptr;
  enum class State { JustAppeared, WaitingForAttack, Attack, Hurts, Dying };
  State currentState = State::JustAppeared;

public:
  explicit Enemy(const std::string &filename);

  void set_target(GameObject *target) { target_ = target; }

  void Update(sf::RenderWindow *window, sf::Time, Scene *scene) override;
  void Attack(Scene *scene, const std::string &str) override;

  void OnCollision(GameObject &obj) override;
};

class Dragon : public Enemy {
private:
  static constexpr float realSpeed = 95;

public:
  explicit Dragon(const std::string &textureFile);
  void Update(sf::RenderWindow *window, sf::Time, Scene *scene) override;
  void OnCollision(GameObject &obj) override;
  void Die() override;
  friend class Scene;
};

class Magician : public Enemy {
private:
  static constexpr float realSpeed = 90;

public:
  explicit Magician(const std::string &textureFile);
  void Update(sf::RenderWindow *window, sf::Time, Scene *scene) override;
  void OnCollision(GameObject &obj) override;
  void Die() override;
  friend class Scene;
};

class Orc : public Enemy {
private:
  static constexpr float realSpeed = 120;

public:
  explicit Orc(const std::string &textureFile);
  void Update(sf::RenderWindow *window, sf::Time, Scene *scene) override;
  void OnCollision(GameObject &obj) override;
  void Die() override;
  void Attack(Scene *scene, const std::string &str) override;

  friend class Scene;
};

} // namespace rtf
