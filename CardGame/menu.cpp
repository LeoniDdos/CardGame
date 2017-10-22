#include "menu.h"

using namespace std;
using namespace sf;

void menu(RenderWindow & window)
{
	//Добавить текстовые правила
	cout << "[LOG]: Открытие меню" << endl;
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;

	menuTexture1.loadFromFile("images/word_play.png");
	menuTexture2.loadFromFile("images/word_rules.png");
	menuTexture3.loadFromFile("images/word_quit.png");
	menuBackground.loadFromFile("images/MenuBack.png");

	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menuBg(menuBackground);

	bool isMenu = 1;
	int menuNum = 0;

	menu1.setPosition(750, 280); //Поправить зоны нажатий
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
				if (menuNum == 1) isMenu = false;
				//	if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
				if (menuNum == 3) { window.close(); isMenu = false; cout << "[LOG]: Запуск игры" << endl; }
			}
			window.draw(menuBg);
			window.draw(menu1);
			window.draw(menu2);
			window.draw(menu3);
			window.display();
		}
	}
}