#include "Enemy.h"
#include <Ally.h>
#include <GameObject.h>
#include <Menu.h>
#include <Player.h>
#include <Scene.h>
using namespace rtf;

#define RK_DEBUG 1;

Scene::Scene(int w, int h)
    : window_(sf::VideoMode(w, h),
              "Harry Potter in nowhere!" , sf::Style::Fullscreen) {
  Main_Action();
  objects_.clear();
  GameObject::gameState_ = GameObject::GameState::GameOver;
}

void Scene::Run() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      // Window closed or escape key pressed: exit
      if ((event.type == sf::Event::Closed) ||
          ((event.type == sf::Event::KeyPressed) &&
           (event.key.code == sf::Keyboard::Escape))) {
        window_.close();
        break;
      }
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code)
        default:
          break;
      }
    }
      sf::Time elapsed = clock_.restart();
    if (GameObject::gameState_ == GameObject::GameState::Action) {
      player_->Controls(this);

      for (auto it = objects_.begin(); it != objects_.end(); ++it) {
        for (auto it2 = std::next(it); it2 != objects_.end(); ++it2) {
          const bool isCollision = (*it)->Sprite().getGlobalBounds().intersects(
              (*it2)->Sprite().getGlobalBounds());
          if (isCollision) {
            GameObject::OnCollision(*(*it), *(*it2));
          }
        }
      }

      window_.clear();

      for (auto &obj : objects_) {
        obj->Update(&window_, elapsed, this);
      }
      objects_.remove_if([](auto &&obj) {
        return !obj->isAlive() &&
               (obj->is(rtf::GameObject::Tag::PlayerBullet) ||
                obj->is(rtf::GameObject::Tag::Ally));
      });

      window_.display();
      if (GameObject::gameState_ == GameObject::GameState::GameOver) {
        objects_.clear();
      }
    } else if (GameObject::gameState_ == GameObject::GameState::GameOver) {
      if (objects_.empty()) {
        window_.clear();
        GameOver();
        window_.display();
      }
      if (dynamic_cast<Menu *>(objects_.front().get())->Controls(*this) ==
          Menu::Actions::RestartGame) {
        window_.clear();
        GameObject::gameState_ = GameObject::GameState ::Action;
        objects_.clear();
        Main_Action();
        window_.display();
      }
    }
  }
}

void Scene::GameOver() {
  auto font = sf::Font();
  font.loadFromFile("../res/arial.ttf");
  auto bg = std::make_unique<sf::RectangleShape>(sf::Vector2f(1920, 1080));
  bg->setFillColor(sf::Color(232, 246, 255));
  window_.draw(*bg);
  auto tryAgain = std::make_unique<sf::RectangleShape>(sf::Vector2f(300, 50));
  tryAgain->setPosition(810, 600);
  tryAgain->setFillColor(sf::Color::Black);
  auto tryAgainText = std::make_unique<sf::Text>();
  tryAgainText->setFont(font);
  tryAgainText->setCharacterSize(30);
  tryAgainText->setString("Try again");
  tryAgainText->setPosition(900, 607);
  tryAgainText->setFillColor(sf::Color::White);
  auto gameOver = std::make_unique<sf::Text>();
  gameOver->setString("GAME OVER!");
  gameOver->setFillColor(sf::Color::Red);
  gameOver->setFont(font);
  gameOver->setCharacterSize(72);
  gameOver->setPosition(740, 400);
  window_.draw(*tryAgain);
  window_.draw(*tryAgainText);
  window_.draw(*gameOver);
  AddObject(std::make_unique<Menu>());
  dynamic_cast<Menu *>(objects_.back().get())
      ->AddButton(std::move(tryAgain), Menu::Actions::RestartGame);
}
void Scene::AddObject(std::unique_ptr<GameObject> obj) {
  objects_.push_back(std::move(obj));
}
void Scene::Main_Action() {
  auto bg = std::make_unique<GameObject>("../res/rkulagin/background.png");
  bg->SetPosition(0, 0);

  auto doumbledor = std::make_unique<Ally>("../res/wizard/wizard/walk_1.png");
  doumbledor->SetPosition(700.f, 500.f);

  auto harry = std::make_unique<Player>(
      "../res/Herbalist/PNG/PNG Sequences/Idle/0_Herbalist_Idle_001.png");
  harry->SetPosition(300.f, 500.f);
  player_ = harry.get();

  auto train = std::make_unique<Train>("../res/rkulagin/train/train_1.png");
  train->SetPosition(1050.0f, 0.0f);
  train->set_tag(GameObject::Tag::Train);

  AddObject(std::move(bg));

  AddObject(std::move(doumbledor));
  AddObject(std::move(harry));
  AddObject(std::move(train));
}
