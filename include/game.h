#ifndef GAME_H
#define GAME_H

#include <iostream>
using namespace std;
#include <SDL2/SDL.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <vector>

#include "object.h"
#include "entity.h"
#include "perlin_noise.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define UNIT_SIZE 20

#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
// #define WALKING_SPEED 2
#define RUNNING_SPEED 20

// #define GRAVITY 10
#define MAP_WIDTH 1000
#define CHUNK_SIZE 100
#define CHUNK_COUNT MAP_WIDTH / CHUNK_SIZE
#define CHUNK_ORIGIN CHUNK_COUNT / 2

enum KeyStatus {
  ACTIVE,
  PASSIVE,
  OFF,
};

struct KeyboardLayout {
  KeyStatus left = OFF;
  KeyStatus right = OFF;
  KeyStatus up = OFF;
  KeyStatus down = OFF;
};

class Game {
public:
  Game();
  ~Game();

  void Loop();
  void Update();
  void Input();
  void Render();
  void Draw(Object object);

  void Map();
  // void loadMap();
  void drawMap();
  void GenerateTerrain();
  void AddChunkToMap(int id, int offset);
  // void SetHeightMap();
  // void SetHeightMap(int seed);
  // void SetChunkMap(int id);

  void cameraControl();
  void moveCamera(int x, int y);

  void playerControl();

public:
  KeyboardLayout keyboard_;
  int mouse_x_, mouse_y_;

public:
  Object player_;
  double player_velocity_x_;
  double player_velocity_y_;

public:
  // int noise_map_[MAP_WIDTH];
  vector<Object> map_;
  vector<Object> chunks_map_[CHUNK_COUNT];
  int map_v_x_ = 0;
  int map_v_y_ = 0;

  double map_index = 0;
  double tile_rendered = 0;

private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  bool running_ = true;
  int frame_count_, last_frame_;
  PerlinNoise perlin_;
};

#endif // GAME_H
