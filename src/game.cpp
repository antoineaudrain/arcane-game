#include "game.h"

Game::Game() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    SDL_Log("Failed to initialize SDL_image: %s", SDL_GetError());

  window_ = SDL_CreateWindow("Arcane", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (window_ == nullptr)
    SDL_Log("Failed to create window: %s", SDL_GetError());

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr)
    SDL_Log("Failed to create renderer: %s", SDL_GetError());

  GenerateTerrain();

  player_.set_tex("res/gfx/cobblestone.png", renderer_);
  player_.set_src(0, 0, 16, 16);
  player_.set_dest(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);

  Loop();
}

Game::~Game() {
  IMG_Quit();
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void Game::Loop() {
  static int lastTime = 0;
  while (running_) {
    last_frame_ = SDL_GetTicks();
    if (last_frame_ >= (lastTime + 1000)) {
      lastTime = last_frame_;
      frame_count_ = 0;
    }

    Input();
    Update();
    Render();
  }
}

void Game::Render() {
  SDL_RenderClear(renderer_);
  SDL_SetRenderDrawColor(renderer_, 126, 192, 238, 255);

  Map();
  drawMap();
  Draw(player_);

  frame_count_++;
  int timerFPS = SDL_GetTicks() - last_frame_;
  if (timerFPS < (1000 / 60))
    SDL_Delay((1000 / 60) - timerFPS);

  SDL_RenderPresent(renderer_);
}

void Game::Draw(Object object) {
  SDL_Rect dest = object.dest();
  SDL_Rect src = object.src();
  SDL_RenderCopyEx(renderer_, object.tex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

void Game::Input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_QUIT)
      running_ = false;

    else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_e:
        cout << "Action key pressed!" << endl;
        break;

      case SDLK_ESCAPE:
        running_ = false;
        break;

      case SDLK_a:
        keyboard_.left = ACTIVE;
        if (keyboard_.right == ACTIVE)
          keyboard_.right = PASSIVE;
        break;

      case SDLK_w:
        keyboard_.up = ACTIVE;
        if (keyboard_.down == ACTIVE)
          keyboard_.down = PASSIVE;
        break;

      case SDLK_s:
        keyboard_.down = ACTIVE;
        if (keyboard_.up == ACTIVE)
          keyboard_.up = PASSIVE;
        break;

      case SDLK_d:
        keyboard_.right = ACTIVE;
        if (keyboard_.left == ACTIVE)
          keyboard_.left = PASSIVE;
        break;

      default:
        break;
      }
    }

    else if (event.type == SDL_KEYUP) {
      switch (event.key.keysym.sym) {
        case SDLK_a:
          keyboard_.left = OFF;
          if (keyboard_.right == PASSIVE)
            keyboard_.right = ACTIVE;
          break;

        case SDLK_w:
          keyboard_.up = OFF;
          if (keyboard_.down == PASSIVE)
            keyboard_.down = ACTIVE;
          break;

        case SDLK_s:
          keyboard_.down = OFF;
          if (keyboard_.up == PASSIVE)
            keyboard_.up = ACTIVE;
          break;

        case SDLK_d:
          keyboard_.right = OFF;
          if (keyboard_.left == PASSIVE)
            keyboard_.left = ACTIVE;
          break;

        default:
          break;
      }
    }

    SDL_GetMouseState(&mouse_x_, &mouse_y_);
  }
}

void Game::Update() {
  playerControl();
  cameraControl();
}

void Game::playerControl() {
  // cout << (keyboard_.up == ACTIVE) << (keyboard_.down == ACTIVE) << (keyboard_.left == ACTIVE) << (keyboard_.right == ACTIVE) << endl;

  if (keyboard_.up == ACTIVE || keyboard_.down == ACTIVE) {
    if (keyboard_.down == ACTIVE)
      player_velocity_y_ = RUNNING_SPEED;
    else if (keyboard_.up == ACTIVE)
      player_velocity_y_ = -RUNNING_SPEED;
    player_.set_dest(player_.dest_pos_x(), player_.dest_pos_y() + player_velocity_y_);
  }

  if (keyboard_.right == ACTIVE || keyboard_.left == ACTIVE) {
    if (keyboard_.right == ACTIVE)
      player_velocity_x_ = RUNNING_SPEED;
    else if (keyboard_.left == ACTIVE)
      player_velocity_x_ = -RUNNING_SPEED;
    player_.set_dest(player_.dest_pos_x() + player_velocity_x_, player_.dest_pos_y());
  }

  for (unsigned long i = 0; i < map_.size(); i++) {
    SDL_Rect rect_player = player_.dest();
    SDL_Rect rect_object = map_[i].dest();

    if (map_[i].solid() && SDL_HasIntersection(&rect_player, &rect_object)) {
      // int player_left = player_.dest_pos_x();
      // int player_right = player_left + player_.dest_width();
      int player_top = player_.dest_pos_y();
      int player_bottom = player_top + player_.dest_height();

      // int tile_left = map_[i].dest_pos_x();
      // int tile_right = tile_left + map_[i].dest_width();
      int tile_top = map_[i].dest_pos_y();
      int tile_bottom = tile_top + map_[i].dest_height();

      if (player_bottom >= tile_top && player_velocity_y_ > 0)
        player_velocity_y_ =
            player_bottom - tile_top != RUNNING_SPEED
                ? RUNNING_SPEED - (player_bottom - tile_top)
                : 0;

      else if (player_top <= tile_bottom && player_velocity_y_ < 0)
        player_velocity_y_ =
            tile_bottom - player_top != RUNNING_SPEED
                ? -RUNNING_SPEED + (tile_bottom - player_top)
                : 0;

      // if (player_left <= tile_right)
      //   cout << "left collision" << endl;

      // else if (player_right >= tile_left)
      //   cout << "right collision" << endl;
    }
  }
}

// ============================== CAMERA START =================================== //

void Game::cameraControl() {
  int window_center_pos_x = (WINDOW_WIDTH - PLAYER_WIDTH) / 2;
  int window_center_pos_y = (WINDOW_HEIGHT - PLAYER_HEIGHT) / 2;

  if (player_.dest_pos_x() != window_center_pos_x) {
    player_.set_dest(window_center_pos_x, player_.dest_pos_y());
    moveCamera(-player_velocity_x_, 0);
    map_v_x_ += player_velocity_x_;
  }

  if (player_.dest_pos_y() != window_center_pos_y) {
    player_.set_dest(player_.dest_pos_x(), window_center_pos_y);
    moveCamera(0, -player_velocity_y_);
    map_v_y_ += player_velocity_y_;
  }

  cout << map_v_x_ << ", " << map_v_y_ << endl;
}

void Game::moveCamera(int x, int y) {
  for (unsigned long i = 0; i < map_.size(); i++) {
    map_[i].set_dest(map_[i].dest_pos_x() + x, map_[i].dest_pos_y() + y);
  }
}

// ================================ MAP START ==================================== //

void Game::Map() {
  int test = CHUNK_COUNT;
  if (map_v_x_ % UNIT_SIZE == 0) {
    if ((map_v_x_ / UNIT_SIZE) % CHUNK_SIZE == 0 && player_velocity_x_ > 0) {
      int chunk_id = (map_v_x_ / UNIT_SIZE / CHUNK_SIZE + CHUNK_ORIGIN + 1) % test;
      cout << "load right: " << chunk_id << endl;
      AddChunkToMap(chunk_id, 0);
    }

    else if ((map_v_x_ / UNIT_SIZE) % CHUNK_SIZE == 0 && player_velocity_x_ < 0) {
      int qwwe = (map_v_x_ / UNIT_SIZE / CHUNK_SIZE + CHUNK_ORIGIN - 1) % test;
      cout << "load left: " << qwwe << endl;
    }
  }
}

void Game::GenerateTerrain() {
  // BELOW STAND FOR HEIGHT MAP GENERATION FROM PERLIN NOISE ALGORITHM
  int seed = 269;
  // int seed = rand();
  int geographical_height = 30;
  float x_unit_section = (2 * M_PI) / MAP_WIDTH;
  int noise_map[MAP_WIDTH];

  for (float i = 0; i < (2 * M_PI); i += x_unit_section) {
    double xoff = cos(i) + 1;
    double yoff = sin(i) + 1;
    double n = perlin_.noise(seed, xoff, yoff, 0);
    int y_pos = geographical_height * n;
    int pos = round(i / x_unit_section);

    if (pos == 0)
      noise_map[0] = geographical_height;
    else
      noise_map[pos] = y_pos;
  }

  // BELOW STAND FOR OBJECT FILLING MAP FROM HEIGHT MAP GENERATED BEFORE
  Object tile;

  for (int i = 0; i < CHUNK_COUNT; i++) {
    int offset = i * CHUNK_SIZE;
    int limit = offset + CHUNK_SIZE;

    for (int y = 0; y < geographical_height; y++) {
      for (int x = offset; x < limit; x++) {
        if (y >= noise_map[x] || y > geographical_height) {
          tile.set_id(2);
          tile.set_solid(1);
          tile.set_tex("res/gfx/dirt.png", renderer_);
          tile.set_src(0, 0, 32, 32);

          int tile_pos_x = (x - (CHUNK_SIZE - WINDOW_WIDTH / UNIT_SIZE) / 2 - CHUNK_SIZE * CHUNK_ORIGIN) * UNIT_SIZE;
          int tile_pos_y = (y + 10) * UNIT_SIZE;

          tile.set_dest(tile_pos_x, tile_pos_y, UNIT_SIZE, UNIT_SIZE);

          chunks_map_[i].push_back(tile);
        }
      }
    }
  }

  // BELOW STAND FOR RENDERING THE FIRST CHUNK
  AddChunkToMap(CHUNK_ORIGIN, 0);
}

void Game::AddChunkToMap(int id, int offset) {
  for (unsigned long i = 0; i < chunks_map_[id].size(); i++) {
    Object obj = chunks_map_[id][i];
    if (offset)
      obj.set_dest(obj.dest_pos_x() + offset, obj.dest_pos_y());
    map_.push_back(obj);
  }
}

void Game::drawMap() {
  int x = 0;
  int y = 0;
  for (unsigned long i = 0; i < map_.size(); i++) {
    if (map_[i].dest_pos_x() >= x - UNIT_SIZE & map_[i].dest_pos_y() >= y - UNIT_SIZE & map_[i].dest_pos_x() <= x + WINDOW_WIDTH + UNIT_SIZE & map_[i].dest_pos_y() <= y + WINDOW_HEIGHT + UNIT_SIZE)
      Draw(map_[i]);
  }
}
