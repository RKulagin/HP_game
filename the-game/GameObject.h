#pragma once
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <Scene.h>
#include <cmath>
#include <iostream>
#include <string>

namespace rtf {
class Scene;

class GameObject {
public:
  enum class Tag {
    Unknown,
    Player,
    Enemy,
    PlayerBullet,
    EnemyBullet,
    Ally,
    Train,
    Dragon,
    Orc,
    Magician,
    TextBox,
    Unicorn
  };
  enum class GameState {
    Menu,
    Introduction,
    Action,
    Epilogue,
    GameOver,
    WaitingInput
  };

public:
  explicit GameObject(const std::string &textureFile);
  virtual ~GameObject() = default;

  GameObject(const GameObject &obj);
  GameObject() = default;
  GameObject &operator=(const GameObject &obj);

  virtual void SetPosition(float x, float y);
  virtual void SetPosition(const sf::Vector2f &pos);

  const sf::Sprite &Sprite() const { return sprite_; }
  void Rotate(float angle) { sprite_.rotate(angle * 180 / M_PI); }
  void LoadNewTexture(const std::string &file);

  virtual void Draw(sf::RenderWindow *window);

  virtual void Update(sf::RenderWindow *window, sf::Time time, Scene *scene);

  static void OnCollision(GameObject &a, GameObject &b);
  virtual void OnCollision(GameObject &obj);

  Tag GetTag() const { return tag_; }
  void set_tag(Tag tag) { tag_ = tag; }
  bool is(Tag tag) const { return tag == tag_; }

  inline bool isAlive() const { return isAlive_; }
  virtual void Die() { isAlive_ = false; }

  static GameState gameState_;

protected:
  static constexpr float SCALE_FACTOR = 0.5;
  Tag tag_ = Tag::Unknown;
  sf::Texture texture_;
  sf::Sprite sprite_;
  bool isAlive_ = true;
};
} // namespace rtf