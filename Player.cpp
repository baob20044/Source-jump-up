#include"Player.h"

Player::Player() {
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	on_ground = false;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
}
Player::~Player() {
	;
}
bool Player::LoadImg(std::string path, SDL_Renderer* gRenderer) {
	bool ret = BaseObject::LoadImg(path, gRenderer);
	if (ret == true) {
		//Load tung frame
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}
	return ret;
}

void Player::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2 * width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3 * width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		frame_clip_[4].x = 4 * width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		frame_clip_[5].x = 5 * width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		frame_clip_[6].x = 6 * width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;

		frame_clip_[7].x = 7 * width_frame_;
		frame_clip_[7].y = 0;
		frame_clip_[7].w = width_frame_;
		frame_clip_[7].h = height_frame_;
	}
}
void Player::Show(SDL_Renderer* des) {
	UpdateImagePlayer(des);
	if (input_type_.left_ == 1 ||
		input_type_.right_ == 1)
	{
		frame_++;
	}
	else {
		frame_ = 0;
	}
	if (frame_ >= 8) {
		frame_ = 0;
	}
	if (come_back_time_ == 0) {
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_];

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_,height_frame_ };

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
	}
}

void Player::HandleInputAction(SDL_Event e, SDL_Renderer* gRenderer) { // Xu li nut an
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImagePlayer(gRenderer);
		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImagePlayer(gRenderer);
		}
		break;
		case SDLK_SPACE:
		{
			input_type_.jump_ = 1;
			UpdateImagePlayer(gRenderer);
		}
		break;
		}
	}
	else if (e.type == SDL_KEYUP)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}
		break;
		}
	}
}
void Player::DoPlayer(Map& map_data) {
	if (come_back_time_ == 0) {
		x_val_ = 0;
		y_val_ += 0.8;

		if (y_val_ >= MAX_FALL_SPEED) {
			y_val_ = MAX_FALL_SPEED;
		}
		if (input_type_.left_ == 1) {
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1) {
			x_val_ += PLAYER_SPEED;
		}
		if (input_type_.jump_ == 1) {
			if (on_ground == true) {
				y_val_ = -PLAYER_JUMP;
			}
			on_ground = false;
			input_type_.jump_ = 0;
		}

		CheckToMap(map_data);
		CenterEntityOnMap(map_data);
	}
	if (come_back_time_ > 0) {
		come_back_time_--;
		if (come_back_time_ == 0) {
			on_ground = false;
			if (x_pos_ > 256) {
				x_pos_ -= 256;
			}
			else {
				x_pos_ = 0;
			}
			y_pos_ = 0;
			x_val_ = 0;
			y_val_ = 0;
		}
	}
}
//Di chuyen thi map cung di chuyen
void Player::CenterEntityOnMap(Map& map_data) {
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);
	//Mep man hinh ben trai
	if (map_data.start_x_ < 0) {
		map_data.start_x_ = 0;
	}
	//Mep man hinh ben phai
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) {
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}
	//Mep man hinh tren
	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0) {
		map_data.start_y_ = 0;
	}
	//Mep man hinh duoi
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_) {
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}
void Player::CheckToMap(Map& map_data) { // Va cham voi ban do
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//Check Horizontal
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAP_MAP_X && y1 >= 0 && y2 < MAP_MAP_Y) {
		if (x_val_ > 0) { //Neu dang qua phai 
			if (map_data.tile[y1 + 1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE) {
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
			}
		}
		else if (x_val_ < 0) { //Neu dang lui hoac qua trai
			if (map_data.tile[y1 + 1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE) {
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
			}
		}
	}
	//Check Vertical
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAP_MAP_X && y1 >= 0 && y2 < MAP_MAP_Y) {
		if (y_val_ > 0) { //Neu dang roi 
			if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE) {
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_val_ = 0;
				on_ground = true;
				if (status_ == WALK_NONE) {
					status_ = WALK_RIGHT;
				}
			}
		}
		else if (y_val_ < 0) { //Neu nhay len
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE) {
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
	}
	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0) {
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_) {
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}
	//Xu li vuc tham
	if (y_pos_ > map_data.max_y_) {
		come_back_time_ = 60;
	}
}
void Player::UpdateImagePlayer(SDL_Renderer* des) {
	if (on_ground == true) {
		if (status_ == WALK_LEFT) {
			LoadImg("Img/player_left.png", des);
		}
		else if (status_ == WALK_RIGHT) {
			LoadImg("Img/player_right.png", des);
		}
	}
	else {
		if (status_ == WALK_LEFT) {
			LoadImg("Img/jum_left.png", des);
		}
		else if (status_ == WALK_RIGHT) {
			LoadImg("Img/jum_right.png", des);
		}
	}
}