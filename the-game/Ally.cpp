// Copyright 2020 RKulagin

#include <Ally.h>
namespace rtf {
Ally::Ally(const std::string &filename) : Movable(filename) {
  set_tag(Tag::Ally);
  SetSpeed(80);
}

void Ally::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(GetSpeed() * time.asSeconds());
    if (gameState_ == GameState::Action) {
      MoveRight();
      if (text != nullptr) {
        text->MoveRight();
      }
    }
    if (currentState == State::TimeToSing && timeAlive > sf::milliseconds(500)) {
      text = Say(L"Большие города,\n пустые поезда...", sf::seconds(3));
      currentState = State::GoInsideTrain;
    }

    if (text != nullptr) {
      text->Update(window, time, scene);
    }

    Draw(window);
    ResetMoveDirection();
    timeAlive +=time;
  }
}

void Ally::OnCollision(rtf::GameObject &obj) {
  if (obj.is(Tag::Train)) {
    isAlive_ = false;
  }
}

std::unique_ptr<TextBox> Ally::Say(const std::wstring &str, sf::Time time) {
  return std::make_unique<TextBox>(
      str, Sprite().getPosition() - sf::Vector2f(125, 100), time, GetSpeed());
}

} // namespace rtf
