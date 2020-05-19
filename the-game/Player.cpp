#include <Bullet.h>
#include <Enemy.h>
#include <Player.h>
#include <SFML/Window/Mouse.hpp>
#include <Scene.h>
#include <cmath>

namespace rtf {
Player::Player(const std::string &filename) : Shootable(filename) {
  cd_ = sf::milliseconds(1000);
  set_tag(Tag::Player);
  SetSpeed(100);
}

void rtf::Player::Controls(Scene *scene) {
  if (currentState == State::Acting && CanAttack() &&
      sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    Attack(scene, "../res/laser.png");
  }
}

void Player::Attack(Scene *scene, const std::string &res) {
  auto pos = Sprite().getPosition() + sf::Vector2f(100, 75);
  auto mouseCoords = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
  std::cerr << mouseCoords.x << "|" << mouseCoords.y << std::endl;
  auto bullet = std::make_unique<Bullet>(res, pos);
  bullet->SetSpeed(300);
  bullet->SetDirection(*this, pos, mouseCoords);
  bullet->Rotate(bullet->Angle());
  //  std::cerr << bullet->Angle() * 180 / M_PI << "\n";
  scene->AddObject(std::move(bullet));
  timer_.restart();
}

void Player::Update(sf::RenderWindow *window, sf::Time time, Scene *scene) {
  if (isAlive()) {
    if (gameState_ == GameState::Action) {
      SetMomentumSpeed(speedPixelsPerSecond_ * time.asSeconds());
      HP.SetMomentumSpeed(HP.GetSpeed() * time.asSeconds());
      MP.SetMomentumSpeed(HP.GetSpeed() * time.asSeconds());
      MP = timer_.getElapsedTime().asSeconds() / cd_.asSeconds() * 100;
      timeTillNextText -= time;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        MoveUp();
        if (text != nullptr) {
          text->MoveUp();
        }
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        MoveDown();
        if (text != nullptr) {
          text->MoveDown();
        }
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        MoveRight();
        if (text != nullptr) {
          text->MoveRight();
        }
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        MoveLeft();
        if (text != nullptr) {
          text->MoveLeft();
        }
      }

      if (currentState == State::Misunderstanding &&
          timeAlive > sf::milliseconds(50)) {
        text = Say(L" Подождите,\n директор,\n какие стражи?", sf::seconds(2));
        currentState = State::WaitingNextTrain;
      }

      if (currentState == State::WaitingNextTrain &&
          timeAlive > sf::seconds(4.5)) {
        text = Say(L"Кажется, что-то\nедет...\nПора достать\nпалочку",
                   sf::seconds(2));
        currentState = State::Acting;

        texture_.loadFromFile(
            "../res/Herbalist/PNG/PNG Sequences/Idle/001.png");
        sprite_.setTexture(texture_);
        sprite_.setScale(SCALE_FACTOR, SCALE_FACTOR);
        timeTillNextText = sf::seconds(2.1);
      }
      if (currentState == State::WaitingNextTrain &&
          timeAlive > sf::seconds(7)) {
        text = Say(L"Чтобы стрелять,\nкликай ЛКМ по\nнаправлению удара.\nНе "
                   L"подпускай иx\nслишком близко.",
                   sf::seconds(2));
        currentState = State::Acting;
      }

      if (text != nullptr) {
        text->Update(window, time, scene);
        //      std::cerr<<text->timeToLive.asSeconds();
        if (!text->isAlive()) {
          text = nullptr;
        }
      }
      HP.Update(window, time, scene);
      MP.Update(window, time, scene);
      Draw(window);

      ResetMoveDirection();

    } else if (gameState_ == GameState::Introduction) {
      if (timeAlive > sf::seconds(0) && dialogue == DialoguePart::Hello) {
       text = Say(L"Профессор, \nгде мы?", sf::seconds(3));
        dialogue = DialoguePart::SeemsLikeRailway;
      }
      if (timeAlive > sf::seconds(3) &&
          dialogue == DialoguePart::SeemsLikeRailway) {
        text = Say(L"Это место, \nоно выглядит, \nкак вокзал \nКингс-Кросс.", sf::seconds(3));
        dialogue = DialoguePart::IHaveVariants;
      }
      if (timeAlive > sf::seconds(8) &&
          dialogue == DialoguePart::IHaveVariants) {
        text = Say(L"Разве у меня\nесть выбор?", sf::seconds(3));
        dialogue = DialoguePart::IWantToMeetFriends;
      }
      if (timeAlive > sf::seconds(11) &&
          dialogue == DialoguePart::IWantToMeetFriends) {
        text = Say(L"Я хочу помочь\nсвоим друзьям, \nхочу вернуться \nк ним.", sf::seconds(3));
        dialogue = DialoguePart::WhereDoesThisTrainGo;
      }
      if (timeAlive > sf::seconds(14) && dialogue == DialoguePart::WhereDoesThisTrainGo){
        text = Say(L"Подождите, а этот\n поезд куда идёт?", sf::seconds(3));
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
    timeAlive += time;
  }
}

void rtf::Player::OnCollision(GameObject &obj) {
  if (obj.isAlive()) {
    if (obj.is(Tag::Train) && Sprite().getPosition().x > 1700) {
      gameState_ = GameState ::Epilogue;
    }
    if (obj.is(Tag::Enemy) && dynamic_cast<Enemy &>(obj).CanAttack()) {
      HP -= 10;
      dynamic_cast<Shootable &>(obj).Attack(nullptr, "");
    }
    if (obj.is(Tag::EnemyBullet)) {
      HP -= 15;
    }
    if (HP < 0) {
      gameState_ = GameState::GameOver;
      Die();
    }
  }
}

std::unique_ptr<TextBox> Player::Say(const std::wstring &str, sf::Time time) {
  return std::make_unique<TextBox>(
      str, Sprite().getPosition() - sf::Vector2f(125, 100), time, GetSpeed());
}
} // namespace rtf