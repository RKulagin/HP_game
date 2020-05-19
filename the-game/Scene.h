#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
namespace rtf {
class GameObject;
class Player;

class Scene {
public:
  Scene(int w, int h);
  void Run();
  void AddObject(std::unique_ptr<GameObject> obj);
  Player *GetPlayer() { return player_; }

private:
  sf::RenderWindow window_;
  sf::Font font;
  std::list<std::unique_ptr<GameObject>> objects_;
  Player *player_ = nullptr;
  sf::Clock clock_;
  void Main_Action();
  void GameOver();
  void DrawMenu();
  void Win();
};

} // namespace rtf