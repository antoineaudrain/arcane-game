#include "camera.h"

Camera::Camera(Game game) {
  this->game = game;
}

void Camera::Update() {
  int centered_pos_x = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
  int centered_pos_y = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;

  if (game.player_.dest_pos_x() != centered_pos_x) {
    game.player_.set_dest(centered_pos_x, game.player_.dest_pos_y());
    Move(-game.player_velocity_x_, 0);
    game.map_v_x_ += game.player_velocity_x_;
  }
  if (game.player_.dest_pos_y() != centered_pos_y) {
    game.player_.set_dest(game.player_.dest_pos_x(), centered_pos_y);
    Move(0, -game.player_velocity_y_);
    game.map_v_y_ += game.player_velocity_y_;
  }
}

void Camera::Move(int x, int y) {
  for (unsigned long i = 0; i < game.map_.size(); i++) {
    game.map_[i].set_dest(game.map_[i].dest_pos_x() + x, game.map_[i].dest_pos_y() + y);
  }
}
