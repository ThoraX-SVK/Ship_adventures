#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <string>
#include <thread>
#include <forward_list>
#include "Point.h"


sf::Vector2f compue_new_move(float angle, float forward_speed)
{
	const double PI = 3.14159265;

	double angle_sin = sin(angle*PI / 180);
	double angle_cos = cos(angle*PI / 180);

	if (angle_sin >= 0) 
	{
		if (angle_cos >= 0) 
		{
			return sf::Vector2f(forward_speed*sqrt(1 - pow(angle_sin, 2)),
								forward_speed*sqrt(1 - pow(angle_cos, 2)));
		}
		else
		{
			return sf::Vector2f(-forward_speed*sqrt(1 - pow(angle_sin, 2)),
							    forward_speed*sqrt(1 - pow(angle_cos, 2)));
		}
	}
	else
	{
		if (angle_cos >= 0)
		{
			return sf::Vector2f(forward_speed*sqrt(1 - pow(angle_sin, 2)),
								-forward_speed*sqrt(1 - pow(angle_cos, 2)));
		}
		else
		{
			return sf::Vector2f(-forward_speed*sqrt(1 - pow(angle_sin, 2)),
								-forward_speed*sqrt(1 - pow(angle_cos, 2)));
		}
	}
}

void draw_points(std::forward_list<Point*>& points, sf::RenderWindow& app)
{
	for (Point *p : points)
	{
		app.draw(p->get_sprite());
	}
}

void add_new_random_point(std::forward_list<Point*> *points, sf::Texture& tex, sf::RenderWindow& app)
{
	sf::Vector2f pos(rand() % app.getSize().x, rand() % app.getSize().y);
	Point *p = new Point(pos, tex);

	points->push_front(p);
}

void check_colisions(std::forward_list<Point*> *points, sf::Sprite& sprite)
{
	Point *to_remove;

	for (Point *p : *points)
	{
		if (p->overlaps_sprite(sprite))
		{
			to_remove = p;
		}
	}
	points->remove(to_remove);
}

int main()
{
	srand(time(0));


	sf::RenderWindow app(sf::VideoMode::getDesktopMode(), "Doooo", sf::Style::Fullscreen);
	app.setFramerateLimit(60);

	sf::Clock deltaClock;
	sf::Clock gameTime;

	sf::Texture stop;
	stop.loadFromFile("Img/play.png");
	stop.setSmooth(true);

	sf::Texture point_tex;
	point_tex.loadFromFile("Img/point.png");
	point_tex.setSmooth(true);

	sf::Sprite player;
	player.setTexture(stop);
	player.setPosition(sf::Vector2f(100, 100));
	player.setOrigin(sf::Vector2f(15, 15));

	sf::Sprite point;
	point.setTexture(point_tex);
	player.setPosition(400, 400);

	sf::Font font;
	font.loadFromFile("BebasNeue.otf");

	sf::Text t_speed;
	t_speed.setFont(font);
	t_speed.setFillColor(sf::Color::White);

	float speed = 0.0f;
	float rotation = 0.0f;
	sf::Vector2f to_move(0, 0);
	const float rotation_speed = 200.0f;
	const float max_speed = 6.0f;
	const float acceleration_rate = 12.0f;
	const float natural_decceleration = 1.5f;
	const float brakeing_speed = 15.0f;
	const float new_point_respawn_time = 1.0f;

	std::forward_list<Point*> points;
	

	Point p(sf::Vector2f(300,300), point_tex);
	points.push_front(&p);

	while (app.isOpen())
	{
		sf::Time deltaTime = deltaClock.restart();
		sf::Event e;
		while (app.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) {
				app.close();
			}
		}

		if (gameTime.getElapsedTime().asSeconds() > new_point_respawn_time)
		{
			gameTime.restart();
			add_new_random_point(&points, point_tex, app);
		}

		to_move = compue_new_move(rotation, speed);
		player.setPosition(player.getPosition().x + to_move.x,
			player.getPosition().y - to_move.y);

		check_colisions(&points, player);

		if (rotation >= 360)
		{
			rotation -= 360;
		}
		if (rotation <= -360)
		{
			rotation += 360;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			app.close();
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (speed < max_speed)
			{
				speed += acceleration_rate*deltaTime.asSeconds();
			}
			if (speed >= max_speed)
			{
				speed = max_speed;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (speed > 0)
			{
				speed -= brakeing_speed*deltaTime.asSeconds();
			}
			if (speed <= 0) 
			{
				speed = 0;
			}
		}
		else
		{
			if (speed > 0)
			{
				speed -= natural_decceleration*deltaTime.asSeconds();
			}
			if (speed <= 0)
			{
				speed = 0;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			rotation -= rotation_speed * deltaTime.asSeconds();
			player.rotate(rotation_speed * deltaTime.asSeconds());	
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			rotation += rotation_speed * deltaTime.asSeconds();
			player.rotate(-rotation_speed * deltaTime.asSeconds());
		}

		t_speed.setString(std::to_string(speed));
		
		app.clear();
		draw_points(points, app);
		app.draw(t_speed);
		app.draw(player);
		app.display();
	}




	return 0;
}
