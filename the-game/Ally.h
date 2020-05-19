//
// Created by rkulagin on 17.05.2020.
//

#ifndef RUN_GAME_ALLY_H
#define RUN_GAME_ALLY_H
#include <Movable.h>
#include <TextBox.h>
namespace rtf {

class Ally : public Movable {
private:
  enum class State{
    TimeToSing, GoInsideTrain
  };
  enum class DialoguePart{
    EverythingJustBegins,
    RailwayIsAGoodName,
    LooksDifferent,
    YouHaveToChoose,
    YouAlwaysCanChoose,
    YouBrave,
    Further,
    ItsTimeToGo,
    Defenders,
    End,
  };
public:
  explicit Ally(const std::string &filename);

  void Update(sf::RenderWindow *window, sf::Time time, Scene *scene) override;
  void OnCollision(GameObject &) override;


private:
  //  sf::Time cd_;
  State currentState = State::TimeToSing;
  std::unique_ptr<TextBox> text;

  std::unique_ptr<TextBox> Say(const std::wstring &str, sf::Time time);
DialoguePart dialogue = DialoguePart::EverythingJustBegins;
};

} // namespace rtf

#endif // RUN_GAME_ALLY_H
