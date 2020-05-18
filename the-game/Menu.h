// Copyright 2020 RKulagin

#ifndef RUN_GAME_MENU_H
#define RUN_GAME_MENU_H
#include <Scene.h>
#include <GameObject.h>
namespace rtf {
class Menu : public GameObject {
public:
  enum class Actions { RunGame, RestartGame, Quit, Nothing };
  Menu() = default;
  void AddButton(std::unique_ptr<sf::RectangleShape> rect, Actions action);
  Menu::Actions Controls(Scene &scene);

private:
  std::list<std::pair<std::unique_ptr<sf::RectangleShape>, Actions>> buttons;
};
void Menu::AddButton(std::unique_ptr<sf::RectangleShape> rect, Actions action) {
  buttons.emplace_back(std::move(rect), action);
}
Menu::Actions Menu::Controls(Scene &scene) {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    auto coords = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
    for (const auto &[rect, action] : buttons) {
      if (rect->getPosition().x < coords.x && rect->getPosition().y < coords.y &&
          rect->getPosition().x + rect->getSize().x > coords.x &&
          rect->getPosition().y + rect->getSize().y > coords.y) {
        return action;
      }
    }
  }
  return Actions::Nothing;
}
} // namespace rtf
#endif // RUN_GAME_MENU_H
