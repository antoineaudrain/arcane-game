#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
using namespace std;
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Object {
public:
  Object();
  ~Object() {}

  SDL_Rect dest();
  void set_dest(const int x, const int y, const int w, const int h);
  void set_dest(const int x, const int y);

  SDL_Rect src();
  void set_src(const int x, const int y, const int w, const int h);

  SDL_Texture *tex();
  void set_tex(const string asset_path, SDL_Renderer *ren);

  int id();
  void set_id(const int id);

  bool solid();
  void set_solid(bool is_solid);

  int dest_pos_x();
  int dest_pos_y();
  int dest_width();
  int dest_height();

private:
  int id_;
  bool solid_;
  SDL_Rect dest_;
  SDL_Rect src_;
  SDL_Texture *tex_;
};

#endif // OBJECT_H
