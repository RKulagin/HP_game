#include "Movable.h"
#include <Enemy.h>
#include <cmath>
#include <iostream>
using namespace rtf;

Movable::Movable(const std::string &textureFile) : GameObject(textureFile) {}

void Movable::ResetMoveDirection() { direction_ = sf::Vector2f{}; }

void Movable::MoveUp() { direction_.y += -speed_; }
void Movable::MoveDown() { direction_.y += speed_; }
void Movable::MoveLeft() { direction_.x += -speed_; }
void Movable::MoveRight() { direction_.x += speed_; }

void Movable::MoveTo(const GameObject *target) {
  // ����������� � ��������.
  auto direction = target->Sprite().getPosition() - Sprite().getPosition();
  // ����� �������.
  auto len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  // ������ ����� 1, �� ����������� � ����.
  direction /= len;

  // ��� � �������� �����������.
  direction_ = direction * speed_;
}
void Movable::MoveTo(sf::Vector2f target, const GameObject &from) {
  // ����������� � ��������.
  auto direction = target - from.Sprite().getPosition();
  // ����� �������.
  auto len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  // ������ ����� 1, �� ����������� � ����.
  direction /= len;

  // ��� � �������� �����������.
  direction_ = direction * speedPixelsPerSecond_;
}

void Movable::Draw(sf::RenderWindow *window) {
  sprite_.move(direction_);
  GameObject::Draw(window);
}

Train::Train(const std::string &textureFile) : Movable(textureFile) {
  speedPixelsPerSecond_ = 1000 / 2;
  tag_ = Tag ::Train;
}
void Train::OnCollision(GameObject &obj) {
  if (obj.is(Tag::Ally)) {
    currentState = State::MovingUp;
  }
}

void Train::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(GetSpeed() * time.asSeconds());
    if (currentState == State::MovingUp) {
      MoveUp();
      if (Sprite().getPosition().y < -1200) {
        currentState = State::GotAwayForever;

        auto newTrain =
            std::make_unique<Train>("../res/rkulagin/train/train_2.png");
        newTrain->set_tag(Tag::Train);
        newTrain->currentState = State::MovingDown;
        newTrain->SetPosition(1720, -1200);
        scene->AddObject(std::move(newTrain));
      }
    }
    if (currentState == State::MovingDown) {
      MoveDown();
      if (Sprite().getPosition().y >= 0) {
        currentState = State::Stopped;
        auto dragon = std::make_unique<Dragon>("../res/dragon/idle1.png");
        auto magician =
            std::make_unique<Magician>("../res/wizard/wizardice/idle1.png");
        auto orc = std::make_unique<Orc>("../res/orcs/idle1.png");
        dragon->SetPosition(1720, 200);
        magician->SetPosition(1720, 500);
        orc->SetPosition(1720, 800);
        scene->AddObject(std::move(dragon));
        scene->AddObject(std::move(magician));
        scene->AddObject(std::move(orc));
      }
    }

    Draw(window);
    ResetMoveDirection();
  }
}

Shootable::Shootable(const std::string &textureFile, float hp, float mp)
    : Movable(textureFile), HP(hp), MP(mp, sf::Color::Blue) {}
void Shootable::Draw(sf::RenderWindow *window) {
  if (gameState_ == GameState::Action) {
    HP.Draw(window);
    MP.Draw(window);
  }
  Movable::Draw(window);
}

Bar::Bar(float currentPoints, sf::Color activeColor, sf::Vector2f pos) {
  bar.setSize(sf::Vector2f(points, height));
  leftPointsBar.setSize(sf::Vector2f(currentPoints, height));
  points = currentPoints;
  bar.setFillColor(sf::Color::Black);
  leftPointsBar.setFillColor(activeColor);
  bar.setPosition(pos);
  leftPointsBar.setPosition(pos);
}
Bar &Bar::operator+=(int64_t value) {
  points -= value;
  UpdateBar();
  return *this;
}
void Bar::UpdateBar() {
  if (points < 0) {
    leftPointsBar.setSize(sf::Vector2f(0, 0));
    bar.setSize(sf::Vector2f(0, 0));
  } else {
    leftPointsBar.setSize(sf::Vector2f(points, height));
  }
}
Bar &Bar::operator-=(int64_t value) {
  points -= value;
  UpdateBar();
  return *this;
}
void Bar::Draw(sf::RenderWindow *window) {
  window->draw(bar);
  window->draw(leftPointsBar);
  ResetMoveDirection();
}
void Bar::SetPosition(const sf::Vector2f &pos) {
  bar.setPosition(pos);
  leftPointsBar.setPosition(pos);
}
void Bar::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  bar.move(direction_);
  leftPointsBar.move(direction_);
  Draw(window);
  ResetMoveDirection();
}

bool operator>(int64_t lhs, const Bar &rhs) { return rhs < lhs; }
bool operator>(const Bar &lhs, const Bar &rhs) { return rhs < lhs; }
