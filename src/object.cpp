#include "object.h"

Object::Object() {
  solid_ = true;
}

SDL_Rect Object::dest() {
  return dest_;
}

void Object::set_dest(const int x, const int y, const int w, const int h) {
  dest_.x = x;
  dest_.y = y;
  dest_.w = w;
  dest_.h = h;
}

void Object::set_dest(const int x, const int y) {
  dest_.x = x;
  dest_.y = y;
}

SDL_Rect Object::src() {
  return src_;
}

void Object::set_src(const int x, const int y, const int w, const int h) {
  src_.x = x;
  src_.y = y;
  src_.w = w;
  src_.h = h;
}

SDL_Texture *Object::tex() {
  return tex_;
}

void Object::set_tex(const string asset_path, SDL_Renderer *renderer) {
  SDL_Surface *surface = IMG_Load(asset_path.c_str());
  tex_ = SDL_CreateTextureFromSurface(renderer, surface);
}

int Object::id() {
  return id_;
}

void Object::set_id(const int id) {
  id_ = id;
}

bool Object::solid() {
  return solid_;
}

void Object::set_solid(bool is_solid) {
  solid_ = is_solid;
}

int Object::dest_pos_x() {
  return dest_.x;
}

int Object::dest_pos_y() {
  return dest_.y;
}

int Object::dest_width() {
  return dest_.w;
}

int Object::dest_height() {
  return dest_.h;
}
