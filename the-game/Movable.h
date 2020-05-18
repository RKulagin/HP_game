#pragma once

#include "GameObject.h"

namespace rtf {

class Movable : public GameObject {
public:
  explicit Movable(const std::string &textureFile);

  void ResetMoveDirection();

  virtual void MoveUp();
  virtual void MoveDown();
  virtual void MoveLeft();
  virtual void MoveRight();

  void MoveTo(const GameObject *target);
  void MoveTo(sf::Vector2f, const GameObject &from);
  void SetSpeed(float speed) { speedPixelsPerSecond_ = speed; }
  inline void SetMomentumSpeed(float speed) { speed_ = speed; }
  inline float GetSpeed() const { return speedPixelsPerSecond_; }
  void Draw(sf::RenderWindow *window) override;

protected:
  float speedPixelsPerSecond_ = 10;
  float speed_ = 0;
  sf::Time timeAlive;
  sf::Vector2f direction_;
};

class Train : public Movable {
private:
  enum class State { MovingUp, MovingDown, Stopped, GotAwayForever };

public:
  explicit Train(const std::string &textureFile);
  void OnCollision(GameObject &) override;
  void Update(sf::RenderWindow *window, sf::Time time, Scene *scene) override;

private:
  State currentState = State::Stopped;
};

class Shootable : public Movable {
protected:
  sf::Time cd_;
  sf::Clock timer_;
  float HP = 100;

public:
  explicit Shootable(const std::string &textureFile);
  virtual void Attack(Scene *, const std::string &) { timer_.restart(); }
  inline bool CanAttack() const { return timer_.getElapsedTime() > cd_; }
  inline void Attacked(){timer_.restart();}
};

} // namespace rtf