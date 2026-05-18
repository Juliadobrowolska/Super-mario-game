#pragma once
#include <chrono>
#include <vector>
#include <array>

// Physics and Speed Constants
constexpr float BRICK_PARTICLE_SPEED = 4;
constexpr float ECTS_JUMP_SPEED = -6;
constexpr float GRADESHEET_SPEED = 1;
constexpr float GRAVITY = 0.25f;
constexpr float DEAN_LUGGAGE_SPEED = 4;
constexpr float DEAN_SPEED = 1;
constexpr float STUDENT_ACCELERATION = 0.25f;
constexpr float STUDENT_JUMP_SPEED = -4;
constexpr float STUDENT_WALK_SPEED = 2;
constexpr float MAX_VERTICAL_SPEED = 8;
constexpr float ENERGYDRINK_SPEED = 1;

// Rozmiary i Skalowanie
constexpr unsigned char CELL_SIZE = 16;
constexpr unsigned char SCREEN_RESIZE = 2;
constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 256;

// Timery i Mechaniki Studenta
constexpr unsigned char STUDENT_BLINKING = 4;
constexpr float _STUDENT_DEATH_DURATION_ = 32; 
constexpr unsigned char STUDENT_GROWTH_DURATION = 64; 
constexpr unsigned char STUDENT_INVINCIBILITY_DURATION = 128; 
constexpr unsigned char STUDENT_JUMP_TIMER = 8;

// Animacje i Przeciwnicy
constexpr unsigned short ECTS_ANIMATION_SPEED = 4;
constexpr unsigned short ENTITY_UPDATE_AREA = 64;
constexpr unsigned short GRADESHEET_DEATH_DURATION = 32;
constexpr unsigned short GRADESHEET_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short DEAN_GET_OUT_ANIMATION_SPEED = 4;
constexpr unsigned short DEAN_GET_OUT_DURATION = 512;
constexpr unsigned short DEAN_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short STUDENT_WALK_ANIMATION_SPEED = 4;
constexpr unsigned short MAIL_BLOCK_ANIMATION_SPEED = 8;

// Czas trwania klatki (60 FPS)
constexpr std::chrono::microseconds FRAME_DURATION(16667);

// Typy kafelków na mapie (Zostawiamy techniczne nazwy, łatwiej pisać kod)
enum Cell
{
	ActivatedQuestionBlock,
	Brick,
	Ects,
	Empty,
	Pipe,
	MailBlock,
	Wall
};

// Struktura dla prostych obiektów fizycznych (np. odłamki zniszczonych klocków/książek)
struct Object
{
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	Object(const float i_x, const float i_y, const float i_horizontal_speed = 0, const float i_vertical_speed = 0) :
		horizontal_speed(i_horizontal_speed),
		vertical_speed(i_vertical_speed),
		x(i_x),
		y(i_y)
	{

	}
};

// Definicja typu mapy
typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;