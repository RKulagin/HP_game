// Copyright 2020 RKulagin

#include <Ally.h>
namespace rtf {
Ally::Ally(const std::string &filename) : Movable(filename) {
  set_tag(Tag::Ally);
  SetSpeed(80);
}

void Ally::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    if (gameState_ == GameState::Action || gameState_ == GameState::Epilogue) {
      SetMomentumSpeed(GetSpeed() * time.asSeconds());
      if (gameState_ == GameState::Action ||
          gameState_ == GameState::Epilogue) {
        MoveRight();
        if (text != nullptr) {
          text->MoveRight();
        }
      }
      if (gameState_ == GameState::Action &&
          currentState == State::TimeToSing &&
          timeAlive > sf::milliseconds(500)) {
        text = Say(L"*Напевает Би-2*\nБольшие города,\n пустые поезда...",
                   sf::seconds(3));
        currentState = State::GoInsideTrain;
      }
      if (gameState_ == GameState::Epilogue && GetTag() != Tag::Unicorn) {
        if (Sprite().getPosition().x > 900) {
          Die();
        }
      }
      if (text != nullptr) {
        text->Update(window, time, scene);
      }

      Draw(window);
      ResetMoveDirection();
    } else if (gameState_ == GameState::Introduction) {
      if (timeAlive > sf::seconds(1) &&
          dialogue == DialoguePart::EverythingJustBegins) {
        text = Say(L"Мой мальчик, \nты там, где \nвсё только \nначинается...",
                   sf::seconds(3));
        dialogue = DialoguePart::RailwayIsAGoodName;
      }
      if (timeAlive > sf::seconds(4) &&
          dialogue == DialoguePart::RailwayIsAGoodName) {
        text = Say(L"Какое хорошее \nописание этого места. \nДля каждого оно\n"
                   L"выглядит по-разному.",
                   sf::seconds(3));
        dialogue = DialoguePart::LooksDifferent;
      }
      if (timeAlive > sf::seconds(7) &&
          dialogue == DialoguePart::LooksDifferent) {
        text = Say(
            L"Но суть остаётся\nодна, ты должен\nрешить,куда\nты отправишься.",
            sf::seconds(3));
        dialogue = DialoguePart::YouAlwaysCanChoose;
      }
      if (timeAlive > sf::seconds(10) &&
          dialogue == DialoguePart::YouAlwaysCanChoose) {
        text = Say(L"Выбор есть \nвсегда, мой \nмальчик.", sf::seconds(3));
        dialogue = DialoguePart::YouBrave;
      }
      if (timeAlive > sf::seconds(13) && dialogue == DialoguePart::YouBrave) {
        text = Say(L"Ты очень храбр, \nГарри. Тогда тебе\nстоит подождать\n"
                   L"следующий поезд",
                   sf::seconds(3));
        dialogue = DialoguePart::Further;
      }   if (timeAlive > sf::seconds(16) && dialogue == DialoguePart::Further) {
        text = Say(L"Вперед.",
                   sf::seconds(3));
        dialogue = DialoguePart::ItsTimeToGo;
      }if (timeAlive > sf::seconds(19) && dialogue == DialoguePart::ItsTimeToGo) {
        text = Say(L"Мне пора, мой мальчик.\nБудь осторожен, тот,\nкто решил вернуться,\nдолжен доказать что\nдостоин.",
                   sf::seconds(3));
        dialogue = DialoguePart::Defenders;
      }if (timeAlive > sf::seconds(22) && dialogue == DialoguePart::Defenders) {
        text = Say(L"Нужно победить \nстражей...",
                   sf::seconds(3));
        dialogue = DialoguePart::End;
      }
      if (timeAlive>sf::seconds(25) && dialogue == DialoguePart::End){
        gameState_ = GameState::Action;
      }
      if (text != nullptr) {
        text->Update(window, time, scene);
        //      std::cerr<<text->timeToLive.asSeconds();
        if (!text->isAlive()) {
          text = nullptr;
        }
      }

      Draw(window);

    }
    timeAlive+=time;
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
