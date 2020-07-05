#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "object.h"

class Entity : public Object {
public:
  Entity();
  ~Entity() {}

  int set_cycle(int row, int width, int height, int qty, int speed);

  void set_curr_anim(int anim);
  int curr_anim();

  void reverse(bool reverse);
  void reverse(bool reverse, int new_anim);

  void updateAnimation();

  void set_velocity(char axis, float value);
  float velocity(char axis);

private:
  float velocity_x_;
  float velocity_y_;

private:
  struct Cycle {
    int row;
    int width;
    int height;
    int qty;
    int speed;
    int tick;
  };
  vector<Cycle> animations_;

private:
  int cur_anim_;
  int begin;
  bool reverse_, is_new_anim_;
  int new_anim_;
};

#endif // ENTITY_H
