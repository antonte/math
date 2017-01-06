#pragma once

class Renderer;
class MenuScreen
{
public:
  enum Item { Play, Settings, Credits, Quit };
  static Item exec(Renderer &renderer);
};
