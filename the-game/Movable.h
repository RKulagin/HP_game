#pragma once

#include "GameObject.h"

namespace rtf {

class Movable : public GameObject {
public:
  explicit Movable(const std::string &textureFile);
  Movable() = default;
  void ResetMoveDirection();

  virtual void MoveUp();
  virtual void MoveDown();
  virtual void MoveLeft();
  virtual void MoveRight();

  virtual void MoveTo(const GameObject *target);
  void MoveTo(sf::Vector2f, const GameObject &from);
  virtual void SetSpeed(float speed) { speedPixelsPerSecond_ = speed; }
  inline void SetMomentumSpeed(float speed) { speed_ = speed; }
  inline float GetSpeed() const { return speedPixelsPerSecond_; }
  void Draw(sf::RenderWindow *window) override;
  void MoveDirection(sf::Vector2f direction) { direction_ = direction; }

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

class Bar : public Movable {
private:
  constexpr static float height = 10;
  sf::RectangleShape bar;
  sf::RectangleShape leftPointsBar;
  float points = 100;

public:
  explicit Bar(float currentPoints, sf::Color activeColor = sf::Color::Red,
               sf::Vector2f pos = sf::Vector2f(0, 0));
  Bar &operator+=(int64_t value);
  Bar &operator-=(int64_t value);
  bool operator<(int64_t rhs) const { return points < rhs; }
  bool operator<(const Bar &rhs) const { return points < rhs.points; }
  void Draw(sf::RenderWindow *window) override;
  void SetPosition(float x, float y) override {
    SetPosition(sf::Vector2f(x, y));
  }
  Bar &operator=(float value) {
    if (value <= 100 && value >= 0) {
      points = value;
    } else if (value > 100) {
      points = 100;
    } else {
      points = 0;
    }
    UpdateBar();
    return *this;
  }
  void SetPosition(const sf::Vector2f &pos) override;
  void MoveUp() override { direction_.y -= speed_; }
  void MoveDown() override { direction_.y += speed_; }
  void MoveLeft() override { direction_.x -= speed_; }
  void MoveRight() override { direction_.x += speed_; }
  void Update(sf::RenderWindow *window, sf::Time time, Scene *scene) override;
  void UpdateBar();
};
bool operator>(int64_t lhs, const Bar &rhs);
bool operator>(const Bar &lhs, const Bar &rhs);

class Shootable : public Movable {
protected:
  sf::Time cd_;
  sf::Clock timer_;
  Bar HP;
  Bar MP;

public:
  explicit Shootable(const std::string &textureFile, float hp = 100,
                     float mp = 100);
  virtual void Attack(Scene *, const std::string &) { timer_.restart(); }
  inline bool CanAttack() const { return timer_.getElapsedTime() > cd_; }
  inline void Attacked() { timer_.restart(); }
  void SetPosition(float x, float y) override {
    GameObject::SetPosition(x, y);
    HP.SetPosition(x + 40, y - 20);
    MP.SetPosition(x + 40, y - 10);
  }
  void SetPosition(const sf::Vector2f &pos) override {
    GameObject::SetPosition(pos);
    HP.SetPosition(pos - sf::Vector2f(-40, 20));
    MP.SetPosition(pos - sf::Vector2f(-40, 10));
  }
  void Draw(sf::RenderWindow *window) override;
  void MoveUp() override {
    Movable::MoveUp();
    HP.MoveUp();
    MP.MoveUp();
  }
  void MoveDown() override {
    Movable::MoveDown();
    HP.MoveDown();
    MP.MoveDown();
  }
  void MoveLeft() override {
    Movable::MoveLeft();
    HP.MoveLeft();
    MP.MoveLeft();
  }
  void MoveRight() override {
    Movable::MoveRight();
    HP.MoveRight();
    MP.MoveRight();
  }
  void MoveTo(const GameObject *target) override {
    Movable::MoveTo(target);
    HP.MoveDirection(direction_);
    MP.MoveDirection(direction_);
  }
  void SetSpeed(float speed) override {
    Movable::SetSpeed(speed);
    HP.SetSpeed(speed);
    MP.SetSpeed(speed);
  }
};

} // namespace rtf