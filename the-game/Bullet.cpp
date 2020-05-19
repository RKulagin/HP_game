#include "Bullet.h"
#include <cmath>

using namespace rtf;

Bullet::Bullet(const std::string &filename, sf::Vector2f start_position)
    : Movable(filename) {
  SetSpeed(200.f);
  SetPosition(start_position);

  set_tag(Tag::PlayerBullet);
}
void Bullet::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(speedPixelsPerSecond_ * time.asSeconds());
    Move();
    Draw(window);
    timeAlive += time;
  }
}
void Bullet::SetDirection(const GameObject &parent, sf::Vector2f from,
                          sf::Vector2f to) {
  auto direction = to - from;
  auto len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  direction /= len;
  directionPerSecond_ = direction;
  if (!parent.is(Tag::Player)) {
    set_tag(Tag::EnemyBullet);
  }
}
void Bullet::Move() { direction_ = directionPerSecond_ * speed_; }
void Bullet::SetDirection(sf::Vector2f direction) {
  directionPerSecond_ = direction * speedPixelsPerSecond_;
}

void Bullet::OnCollision(GameObject &obj) {
  if (isAlive()) {
    if (obj.GetTag() == Tag::EnemyBullet) {
      return;
    }
    if (GetTag() == Tag::PlayerBullet &&
        !(obj.is(Tag ::Player) || obj.is(Tag::Unknown) || obj.is(Tag::Ally))) {
      Die();
    }
    if (GetTag()==Tag::EnemyBullet && obj.GetTag() == Tag::Player){
      Die();
    }
  }
}
