#include "entity.h"

Entity::Entity() {
  reverse_ = false;
}

int Entity::set_cycle(int row, int width, int height, int qty, int speed) {
  Cycle tmp = {
    row - 1,
    width,
    height,
    qty,
    speed,
    0
  };
  animations_.push_back(tmp);
  return animations_.size() - 1;
}

void Entity::set_curr_anim(int anim) {
  begin = 0;
  cur_anim_ = anim;
}

int Entity::curr_anim() {
  return cur_anim_;
}

void Entity::reverse(bool reverse) {
  reverse_ = reverse;
}

void Entity::reverse(bool reverse, int new_anim) {
  reverse_ = reverse;
  is_new_anim_ = 1;
  new_anim = new_anim_;
}

void Entity::updateAnimation() {
  set_src(animations_[cur_anim_].width * animations_[cur_anim_].tick, animations_[cur_anim_].row * animations_[cur_anim_].height, animations_[cur_anim_].width, animations_[cur_anim_].height);
  if (begin > animations_[cur_anim_].speed) {
    if (!reverse_)
      animations_[cur_anim_].tick++;
    else
      animations_[cur_anim_].tick--;
    begin = 0;
  }
  begin++;
  if (animations_[cur_anim_].tick >= animations_[cur_anim_].qty)
    animations_[cur_anim_].tick = 0;
  if (animations_[cur_anim_].tick <= 0) {
    if (is_new_anim_) {
      cur_anim_ = new_anim_;
      is_new_anim_ = 0;
      reverse_ = 0;
    }
    else
      animations_[cur_anim_].tick = 0;
  }
}

void Entity::set_velocity(char axis, float value) {
  if (axis == 'x')
    velocity_x_ = value;
  else if (axis == 'y')
    velocity_y_ = value;
}

float Entity::velocity(char axis) {
  if (axis == 'x')
    return velocity_x_;
  else if (axis == 'y')
    return velocity_y_;
  return 0;
}
