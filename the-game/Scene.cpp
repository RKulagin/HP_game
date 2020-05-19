#include "Enemy.h"
#include <Ally.h>
#include <GameObject.h>
#include <Menu.h>
#include <Player.h>
#include <Scene.h>
using namespace rtf;

#define RK_DEBUG 1;

Scene::Scene(int w, int h)
    : window_(sf::VideoMode(w, h), "Harry Potter in nowhere!",
              sf::Style::Fullscreen) {
  objects_.clear();
  font.loadFromFile("../res/arial.ttf");
  GameObject::gameState_ = GameObject::GameState::Menu;
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
      if (GameObject::gameState_ == GameObject::GameState::Epilogue) {
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
    } else if (GameObject::gameState_ == GameObject::GameState::Epilogue) {
      if (objects_.empty()) {
        Win();
      }
      window_.clear(sf::Color(232, 246, 255));
      for (auto &obj : objects_) {
        obj->Update(&window_, elapsed, this);
        if (obj->Sprite().getPosition().x > 2000){
          GameObject::gameState_ = GameObject::GameState::Menu;
        }
      }
      sf::Text text("You win!", font, 72);
      text.setFillColor(sf::Color::Black);
      text.setPosition(960 - text.getCharacterSize() * 2, 200);
      window_.draw(text);
      window_.display();
      if (GameObject::gameState_ == GameObject::GameState::Menu){
        objects_.clear();
      }
    } else if (GameObject::gameState_ == GameObject::GameState::Menu) {
      if (objects_.empty()) {
        window_.clear();
        DrawMenu();
        window_.display();
      }
      if (dynamic_cast<Menu *>(objects_.front().get())->Controls(*this) ==
          Menu::Actions::RunGame) {
        window_.clear();
        GameObject::gameState_ = GameObject::GameState ::Introduction;
        objects_.clear();
        window_.display();
      }

    } else if (GameObject::gameState_ == GameObject::GameState::Introduction) {
      if (objects_.empty()){
        Main_Action();

      }
      window_.clear();
      for (auto& obj: objects_){
        obj->Update(&window_, elapsed, this);
      }
      window_.display();

      if (GameObject::gameState_ == GameObject::GameState::Action) {
        objects_.clear();
        Main_Action();
      }
    }
  }
}

void Scene::GameOver() {
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
void Scene::Win() {

  auto doumbledor = std::make_unique<Ally>("../res/wizard/wizard/idle_3.png");
  doumbledor->SetPosition(-100, 500);
  doumbledor->SetSpeed(100);
  doumbledor->set_tag(GameObject::Tag::Ally);
  objects_.push_back(std::move(doumbledor));
  auto unicorn = std::make_unique<Ally>("../res/unicorn/walk_1.png");
  unicorn->SetPosition(-140, 530);
  unicorn->SetSpeed(100);
  unicorn->set_tag(GameObject::Tag::Unicorn);
  objects_.push_back(std::move(unicorn));
}
void Scene::DrawMenu() {
  auto bg = std::make_unique<sf::RectangleShape>(sf::Vector2f(1920, 1080));
  bg->setFillColor(sf::Color(232, 246, 255));
  window_.draw(*bg);
  auto play = std::make_unique<sf::RectangleShape>(sf::Vector2f(300, 50));
  play->setPosition(810, 650);
  play->setFillColor(sf::Color::Black);
  auto playText = std::make_unique<sf::Text>();
  playText->setFont(font);
  playText->setCharacterSize(30);
  playText->setString("Play");
  playText->setPosition(960 - playText->getCharacterSize(), 657);
  playText->setFillColor(sf::Color::White);
  auto title = std::make_unique<sf::Text>();
  title->setString(" Harry Potter in nowhere!");
  title->setFillColor(sf::Color::Red);
  title->setFont(font);
  title->setCharacterSize(108);
  title->setPosition(960 - title->getCharacterSize() * 6, 400);
  auto description = std::make_unique<sf::Text>();
  description->setString(L"Что произошло, после того, как Волан-де-Морт убил Гарри?");
  description->setFillColor(sf::Color::Black);
  description->setFont(font);
  description->setCharacterSize(36);
  description->setPosition(960 - description->getCharacterSize() * 14, 550);

  window_.draw(*play);
  window_.draw(*playText);
  window_.draw(*title);
  window_.draw(*description);
  AddObject(std::make_unique<Menu>());
  dynamic_cast<Menu *>(objects_.back().get())
      ->AddButton(std::move(play), Menu::Actions::RunGame);
}
void Scene::AddObject(std::unique_ptr<GameObject> obj) {
  objects_.push_back(std::move(obj));
}
void Scene::Main_Action() {
  auto bg = std::make_unique<GameObject>("../res/rkulagin/background.png");
  bg->SetPosition(0, 0);
  AddObject(std::move(bg));

  if (GameObject::gameState_ == GameObject::GameState::Action) {
    auto doumbledor = std::make_unique<Ally>("../res/wizard/wizard/walk_1.png");
    doumbledor->SetPosition(700.f, 500.f);
    AddObject(std::move(doumbledor));

  }
  else{
    auto doumbledor = std::make_unique<Ally>("../res/wizard/wizard_reverse/walk_1.png");
    doumbledor->SetPosition(700.f, 500.f);
    AddObject(std::move(doumbledor));

  }
  auto harry = std::make_unique<Player>(
      "../res/Herbalist/PNG/PNG Sequences/Idle/0_Herbalist_Idle_001.png");
  harry->SetPosition(300.f, 500.f);
  player_ = harry.get();

  auto train = std::make_unique<Train>("../res/rkulagin/train/train_1.png");
  train->SetPosition(1050.0f, 0.0f);
  train->set_tag(GameObject::Tag::Train);


  AddObject(std::move(harry));
  AddObject(std::move(train));
}
