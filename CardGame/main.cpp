#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

using namespace std;
using namespace sf;

void menu(RenderWindow & window) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/word_play.png");
	menuTexture2.loadFromFile("images/word_rules.png");
	menuTexture3.loadFromFile("images/word_quit.png");
	//aboutTexture.loadFromFile("images/about.png");
	menuBackground.loadFromFile("images/Green.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menuBg(menuBackground);//, about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(750, 280); //поправить зоны нажатий
	menu2.setPosition(750, 410);
	menu3.setPosition(750, 540);
	menuBg.setPosition(0, 0);

	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		//window.clear(Color(100, 67, 0));

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (IntRect(750, 280, 300, 100).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Red); menuNum = 1; }
			if (IntRect(750, 410, 300, 100).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Red); menuNum = 2; }
			if (IntRect(750, 540, 300, 100).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Red); menuNum = 3; }
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			//	if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
				if (menuNum == 3) { window.close(); isMenu = false; }
			}
			window.draw(menuBg);
			window.draw(menu1);
			window.draw(menu2);
			window.draw(menu3);
			window.display();
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "=========[LOG SYSTEM]=========" << endl;
	cout << "[LOG]: Запуск программы.." << endl;

	int w = 32;

	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //Посмотреть конструкторы на макс мин размеры
	
	cout << "[LOG]: Запуск меню" << endl;
	menu(window);
	
	cout << "[LOG]: Запуск игры" << endl;

	window.setFramerateLimit(60); //нужно ли?
	
	Texture tBackground;
	Texture tBackOfCard;

	tBackground.loadFromFile("images/background.png");
	tBackOfCard.loadFromFile("images/BackOfCard.png");
	
	Sprite sBackground(tBackground);
	Sprite sBackOfCard(tBackOfCard);

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	cout << "[LOG]: Окончание подготовки" << endl;

	window.clear();
	window.draw(sBackground);
	window.display();

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);
		//int x = pos.x / w;
		//int y = pos.y / w;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{	
				if (event.key.code == Mouse::Left) {
					//window.draw(sBackground);
					//window.draw(sBackOfCard);
					//window.display();
				}
			}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					menu(window);
				}
		}


		//window.draw(shape);
		window.draw(sBackground);
		window.draw(sBackOfCard);
		//window.draw(sBackOfCard);
		window.display();
	}
	system("pause");
	return 0;
}