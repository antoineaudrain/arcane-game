#ifndef CAMERA_H
#define CAMERA_H

#include "game.h"

class Camera {
public:
  Camera(Game game);
  ~Camera() {}

  void Update();

private:
  void Move(int x, int y);

private:
  Game game;
};

#endif // CAMERA_H
