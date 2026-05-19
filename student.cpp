#include "Headers/Global.hpp"
#include "Headers/Animation.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/EnergyDrink.hpp"
#include "Headers/Student.hpp"

#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


// Konstruktor klasy Student - ustawia domyślne statystyki i stan postaci na starcie gry
Student::Student() :
	can_jump(0),          // Na starcie nie można skoczyć (aż dotkniemy ziemi)
	dead(0),              // Student zaczyna grę żywy
	flipped(0),           // Patrzy w prawo (brak odbicia lustrzanego tekstury)
	on_ground(0),         // Zaczyna w powietrzu, zanim opadnie na platformę
	crouching(0),         // Nie kuca
	death_timer(0),       // Reset licznika czasu śmierci
	growth_timer(0),      // Reset czasu działania energetyka (speed-boosta)
	invincible_timer(0),  // Brak chwilowej nietykalności po oberwaniu
	powerup_state(0),     // Stan początkowy: brak ulepszeń (czysty student)
	x(100),               // Startujemy kawałek od lewej krawędzi screena
	y(100),               // Startujemy w powietrzu, żeby sprawdzić spadanie
	enemy_bounce_speed(0),// Brak siły odbicia od wroga
	horizontal_speed(0),  // Postać stoi w miejscu (brak ruchu w poziomie)
	vertical_speed(0),    // Postać nie spada ani nie leci w górę
	sprite(texture)
{
}

void Student::update(const unsigned i_view_x, MapManager& i_map_manager)
{
	// Aktualizacja tymczasowych energetyków 
	for (EnergyDrink& energy_drink : energy_drinks)
	{
		energy_drink.update();
	}

	if (0 == dead)
	{
		bool moving = 0;

		// Ustalamy maksymalną prędkość w zależności od energetyka
		float current_max_speed = STUDENT_WALK_SPEED;
		if (0 < powerup_state)
		{
			current_max_speed = STUDENT_WALK_SPEED * 1.5f; // +50% prędkości po kofeinie
		}

		// Obsługa ruchu w lewo/prawo za pomocą strzałek
		if (0 == crouching)
		{
			if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
				1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			{
				moving = 1;
				horizontal_speed = std::max(horizontal_speed - STUDENT_ACCELERATION, -current_max_speed);
			}

			if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) &&
				1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			{
				moving = 1;
				horizontal_speed = std::min(STUDENT_ACCELERATION + horizontal_speed, current_max_speed);
			}
		}

		// Hamowanie (powrót do zera), gdy puszcisz klawisze
		if (0 == moving)
		{
			if (0 < horizontal_speed)
			{
				horizontal_speed = std::max<float>(0, horizontal_speed - STUDENT_ACCELERATION);
			}
			else if (0 > horizontal_speed)
			{
				horizontal_speed = std::min<float>(0, STUDENT_ACCELERATION + horizontal_speed);
			}
		}

		// Ruch w poziomie
		x += horizontal_speed;

		// --- TYMCZASOWA FIZYKA I SKAKANIE ---
		// Symulujemy podłogę na dole ekranu na podstawie stałych z Global.hpp
		float temporary_ground_level = SCREEN_HEIGHT - 64 - CELL_SIZE;

		if (y >= temporary_ground_level)
		{
			y = temporary_ground_level;
			on_ground = 1;
		}
		else
		{
			on_ground = 0;
		}

		// Skok (Strzałka w górę lub Z)
		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
		{
			if (0 == vertical_speed && 1 == can_jump && 1 == on_ground)
			{
				vertical_speed = STUDENT_JUMP_SPEED;
				jump_timer = STUDENT_JUMP_TIMER;
				can_jump = 0;
			}
			else if (0 < jump_timer)
			{
				vertical_speed = STUDENT_JUMP_SPEED;
				jump_timer--;
			}
			else
			{
				vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			}
		}
		else
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			jump_timer = 0;
			can_jump = 1;
		}

		// Ruch w pionie (grawitacja / skok)
		y += vertical_speed;

		// Ponowne sprawdzenie podłogi po zmianie pozycji Y
		if (y >= temporary_ground_level)
		{
			y = temporary_ground_level;
			vertical_speed = 0;
			jump_timer = 0;
			on_ground = 1;
		}
		// ---------------------------------------------------------------------

		// Obracanie sprite'a (lustrzane odbicie) w zależności od kierunku ruchu
		if (0 == horizontal_speed)
		{
			if (1 == moving)
			{
				flipped = 1 - flipped;
			}
		}
		else if (0 < horizontal_speed)
		{
			flipped = 0;
		}
		else if (0 > horizontal_speed)
		{
			flipped = 1;
		}

		// Tymczasowe zbieranie energetyków (czysta kolizja hitboxów)
		for (EnergyDrink& energy_drink : energy_drinks)
		{
			if (get_hit_box().findIntersection(energy_drink.get_hit_box()).has_value())
			{
				energy_drink.set_dead(true);

				if (0 == powerup_state)
				{
					powerup_state = 1;
					growth_timer = STUDENT_GROWTH_DURATION;
				}
			}
		}

		if (0 < invincible_timer)
		{
			invincible_timer--;
		}

		// Odliczanie czasu działania energetyka
		if (0 < growth_timer)
		{
			growth_timer--;
			if (0 == growth_timer)
			{
				powerup_state = 0;
			}
		}

		// Spadek w przepaść (śmierć)
		if (y >= SCREEN_HEIGHT)
		{
			die(1);
		}

		// Aktualizacja animacji
		if (0 != horizontal_speed)
		{
			walk_animations[powerup_state].set_animation_speed(STUDENT_WALK_ANIMATION_SPEED * current_max_speed / std::abs(horizontal_speed));
			walk_animations[powerup_state].update();
		}
	}
	else
	{
		// Logika po śmierci (odskok w górę i spadanie poza ekran)
		if (0 == death_timer)
		{
			vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
			y += vertical_speed;
		}
		else if (1 == death_timer)
		{
			vertical_speed = STUDENT_JUMP_SPEED;
		}

		death_timer = std::max(0, death_timer - 1);
	}

	// Czyszczenie zebranych energetyków z pamięci
	energy_drinks.erase(std::remove_if(energy_drinks.begin(), energy_drinks.end(), [](const EnergyDrink& i_energy_drink)
	{
		return i_energy_drink.get_dead();
	}), energy_drinks.end());
}

sf::FloatRect Student::get_hit_box() const
{
	return sf::FloatRect(sf::Vector2f{x, y}, sf::Vector2f{CELL_SIZE, CELL_SIZE});
}

void Student::set_position(const float i_x, const float i_y)
{
	x = i_x;
	y = i_y;
	sprite.setPosition(sf::Vector2f{x, y});
}

void Student::draw(sf::RenderWindow& i_window)
{
	i_window.draw(sprite);
}

void Student::die(const bool i_instant_death)
{
	if (1 == i_instant_death)
	{
		dead = 1;
		death_timer = STUDENT_DEATH_DURATION;
	}
	// TUTAJ WRÓCIMY, GDY BĘDZIEMY PISAĆ WROGÓW:
	else if (0 == dead && 0 == invincible_timer)
	{
		if (0 < powerup_state)
		{
			// TODO: Obsługa odporności po energetyku!
			powerup_state = 0;
			growth_timer = 0; 
			invincible_timer = STUDENT_INVINCIBILITY_DURATION; 
		}
		else
		{
			dead = 1;
			death_timer = STUDENT_DEATH_DURATION;
		}
	}
}