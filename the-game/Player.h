#pragma once

#include <Movable.h>
#include <TextBox.h>
namespace rtf {

class Player : public Shootable {
private:
  enum class State{Misunderstanding, Acting, WaitingNextTrain };
 public:
  explicit Player(const std::string& filename);

  void Controls(Scene *scene);
  void Attack(Scene* scene, const std::string& res) override ;
  void Update(sf::RenderWindow* window, sf::Time time, Scene* scene) override;

  void OnCollision(GameObject&) override;

 private:
  sf::Time timeTillNextText;
  State currentState = State::Misunderstanding;
  std::unique_ptr<TextBox> text;
  std::unique_ptr<TextBox> Say(const std::wstring &str, sf::Time time);
};
}  // namespace rtf
