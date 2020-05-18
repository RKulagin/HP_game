// Copyright 2020 RKulagin
#ifndef RUN_GAME_TEXTBOX_H
#define RUN_GAME_TEXTBOX_H
#include <Movable.h>
namespace rtf {
class TextBox : public Movable {
public:
  TextBox(const std::wstring &str, float x, float y, sf::Time ttl, float speed);
  TextBox(const std::wstring &str, sf::Vector2f, sf::Time ttl, float speed);
  sf::Text Text();
  void MoveUp() override;
  void MoveDown() override;
  void MoveRight() override;
  void MoveLeft() override;
  void Update(sf::RenderWindow *window, sf::Time time, Scene *scene) override;
private:
  sf::Font font;
  sf::Text text;
public:
  sf::Time timeToLive;
};
} // namespace rtf
#endif // RUN_GAME_TEXTBOX_H
