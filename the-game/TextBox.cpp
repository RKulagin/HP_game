// Copyright 2020 RKulagin
#include <TextBox.h>
#include <iostream>
namespace rtf {

TextBox::TextBox(const std::wstring &str, float x, float y, sf::Time ttl,
                 float speed)
    : TextBox(str, sf::Vector2f(x, y), ttl, speed) {}

sf::Text TextBox::Text() { return text; }

TextBox::TextBox(const std::wstring &str, sf::Vector2f vec, sf::Time ttl,
                 float speed)
    : Movable("../res/Herbalist/popup/PNG/popup_1.png") {
  font.loadFromFile("../res/arial.ttf");
  text.setString(str);
  if (str.size() > 70) {
    text.setCharacterSize(16);
  } else {
    text.setCharacterSize(20);
  }
  text.setFillColor(sf::Color::Black);
  text.setFont(font);
  text.move(vec + sf::Vector2f(10, 10));
  SetPosition(vec);
  SetSpeed(speed);
  timeToLive = ttl;
  set_tag(Tag::TextBox);
}
void TextBox::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    SetMomentumSpeed(GetSpeed() * time.asSeconds());
    timeToLive -= time;
    Draw(window);
    window->draw(text);
    ResetMoveDirection();

    if (timeToLive < sf::seconds(0)) {
      Die();
    }
  }
}
void TextBox::MoveUp() {
  direction_.y += -speed_;
  text.move(0, -speed_);
}
void TextBox::MoveDown() {
  direction_.y += speed_;
  text.move(0, +speed_);
}
void TextBox::MoveLeft() {
  direction_.x += -speed_;
  text.move(-speed_, 0);
}
void TextBox::MoveRight() {
  direction_.x += speed_;
  text.move(speed_, 0);
}

} // namespace rtf