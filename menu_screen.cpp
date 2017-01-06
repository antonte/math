#include "menu_screen.hpp"
#include "app.hpp"
#include "event_handler.hpp"
#include "rect.hpp"
#include "renderer.hpp"

MenuScreen::Item MenuScreen::exec(Renderer &r)
{
  Item item = Quit;
  bool buttonPressed = false;
  bool done;
  const auto &play = r.texture("play.bmp");
  const auto &playPressed = r.texture("play_pressed.bmp");

  EventHandler e;
  using namespace std::placeholders;
  e.draw = [&]()
    {
      SDL_SetRenderDrawColor(&r, 0x0, 0x0, 0x0, 0xff);
      r.clear();
      if (!buttonPressed)
        play.draw(makeRect(App::Width / 2 - 64, App::Height / 2 - 64, 128, 128));
      else
        playPressed.draw(makeRect(App::Width / 2 - 64, App::Height / 2 - 64, 128, 128));
      r.present();
    };
  e.buttonUp = [&](const SDL_MouseButtonEvent &e)
    {
      auto x1 = App::Width / 2 - 64;
      auto x2 = App::Width / 2 + 64;
      auto y1 = App::Height / 2 - 64;
      auto y2 = App::Height / 2 + 64;
      if (e.x > x1 && e.x < x2 && e.y > y1 && e.y < y2)
      {
        item = Play;
        done = true;
      }
      buttonPressed = false;
    };
  e.buttonDown = [&](const SDL_MouseButtonEvent &e)
    {
      auto x1 = App::Width / 2 - 64;
      auto x2 = App::Width / 2 + 64;
      auto y1 = App::Height / 2 - 64;
      auto y2 = App::Height / 2 + 64;
      buttonPressed = (e.x > x1 && e.x < x2 && e.y > y1 && e.y < y2);
    };
  e.quit = [&]()
    {
      item = Quit;
    };
  e.exec(done);
  return item;
}
