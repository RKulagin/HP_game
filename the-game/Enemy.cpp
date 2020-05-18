#include "Enemy.h"
#include "Bullet.h"
#include "Movable.h"
#include <Player.h>
namespace rtf {

Enemy::Enemy(const std::string &filename) : Shootable(filename) {}

void Enemy::Update(sf::RenderWindow *window, sf::Time, Scene *scene) {
  if (!is_alive_) {
    Draw(window);
    return;
  }

  if (target_) {
    MoveTo(target_);
  }

  Movable::Draw(window);
}

void rtf::Enemy::OnCollision(GameObject &obj) {

  // ���� obj �������� PlayerBullet, �� p != nullptr;
  if (obj.is(Tag::PlayerBullet)) {
    Die();
  }
}

Dragon::Dragon(const std::string &textureFile) : Enemy(textureFile) {
  SetSpeed(100);
  set_tag(Tag::Dragon);
  cd_ = sf::milliseconds(2500);
}
Magician::Magician(const std::string &textureFile) : Enemy(textureFile) {
  SetSpeed(100);
  set_tag(Tag::Magician);
  cd_ = sf::milliseconds(3000);
}
Orc::Orc(const std::string &textureFile) : Enemy(textureFile) {
  SetSpeed(100);
  set_tag(Tag::Orc);
  cd_ = sf::milliseconds(800);
}

void Dragon::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(GetSpeed() * time.asSeconds());
    if (currentState == Enemy::State::JustAppeared) {
      MoveLeft();
    }
    if (Sprite().getPosition().x < 1500) {
      currentState = Enemy::State::WaitingForAttack;
    }
    if (currentState == Enemy::State::WaitingForAttack) {
      currentState= Enemy::State::Attack;
    }
    if (currentState == Enemy::State::Attack) {
      if (CanAttack()) {
        set_target(scene->GetPlayer());
        Attack(scene, "../res/dragon/fire_attack3.png");
        timer_.restart();
      }
    }
    Draw(window);
    ResetMoveDirection();
  } else {
    Draw(window);
  }
}
void Dragon::OnCollision(GameObject &obj) {
  if (obj.is(Tag::PlayerBullet) && obj.isAlive()) {

    HP -= 15;
    if (HP < 0) {
      Die();
    }
  }
}
void Magician::OnCollision(GameObject &obj) {
  if (obj.is(Tag::Unknown)) {
    return;
  }
  if (obj.is(Tag::PlayerBullet) && obj.isAlive()) {

    HP -= 20;
    std::cerr << HP << "HP\n";

    if (HP < 0) {
      Die();
    }
  }
}
void Orc::OnCollision(GameObject &obj) {
  if (obj.is(Tag::PlayerBullet) && obj.isAlive()) {
    HP -= 15;
    std::cerr << HP << "HP\n";
    //  Freese(sf::seconds(0.5));
    if (HP < 0) {
      Die();
    }
  }
}

void Magician::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(GetSpeed() * time.asSeconds());

    if (currentState == Enemy::State::JustAppeared) {
      MoveLeft();
    }
    if (Sprite().getPosition().x < 1500) {
      currentState = Enemy::State::WaitingForAttack;
    }
    if (currentState == Enemy::State::WaitingForAttack &&
        timeAlive > sf::seconds(3)) {
      currentState = Enemy::State::Attack;
    }
    if (currentState == Enemy::State::Attack) {
      if (CanAttack()) {
        set_target(scene->GetPlayer());
        Attack(scene, "../res/green_laser.png");
        timer_.restart();
      }
    }
    Draw(window);
    ResetMoveDirection();
    timeAlive += time;
  } else {
    Draw(window);
  }
}

void Orc::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(GetSpeed() * time.asSeconds());
    if (currentState == Enemy::State::JustAppeared) {
      MoveLeft();
      if (Sprite().getPosition().x < 1500) {
        currentState = Enemy::State::WaitingForAttack;
      }
    }
    if (currentState == Enemy::State::WaitingForAttack) {
      set_target(scene->GetPlayer());
      currentState = Enemy::State::Attack;
    }
    if (currentState == Enemy::State::Attack) {
      MoveTo(target_);
    }
    Draw(window);
    ResetMoveDirection();
  } else {
    Draw(window);
  }
}

void Orc::Die() {
  currentState = Enemy::State::Dying;
  texture_.loadFromFile("../res/orcs/die7.png");
  sprite_.setTexture(texture_);
  sprite_.setScale(SCALE_FACTOR, SCALE_FACTOR);
  isAlive_ = false;
}

void Magician::Die() {
  currentState = Enemy::State::Dying;
  texture_.loadFromFile("../res/wizard/wizardice/dead4.png");
  sprite_.setTexture(texture_);
  sprite_.setScale(SCALE_FACTOR, SCALE_FACTOR);
  isAlive_ = false;
}

void Dragon::Die() {
  currentState = Enemy::State::Dying;
  texture_.loadFromFile("../res/dragon/death5.png");
  sprite_.setTexture(texture_);
  sprite_.setScale(SCALE_FACTOR, SCALE_FACTOR);
  isAlive_ = false;
}

void Enemy::Attack(Scene *scene, const std::string &str) {
  auto pos = Sprite().getPosition() + sf::Vector2f(-10, 75);
  auto targetCoords = target_->Sprite().getPosition();
  std::cerr << targetCoords.x << "|" << targetCoords.y << std::endl;
  auto bullet = std::make_unique<Bullet>(str, pos);
  bullet->SetSpeed(300);
  bullet->SetDirection(*this, targetCoords);
  bullet->Rotate(bullet->Angle());
  std::cerr << bullet->Angle() * 180 / M_PI << "\n";
  scene->AddObject(std::move(bullet));
  timer_.restart();
}

void Orc::Attack(Scene *scene, const std::string &str) {
  // Attack Animation
  scene->GetPlayer()->OnCollision(*this);
}


} // namespace rtf