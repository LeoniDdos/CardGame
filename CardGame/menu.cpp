#include "menu.h"
#include "rules.h"

using namespace std;
using namespace sf;

void menu(RenderWindow & window)
{
	cout << "[LOG]: Открытие меню" << endl;
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text txtTitle ("Пьяница", font, 130);
	txtTitle.setStyle(sf::Text::Bold);
	txtTitle.setPosition(710, 100);

	menuTexture1.loadFromFile("images/word_play.png");
	menuTexture2.loadFromFile("images/word_rules.png");
	menuTexture3.loadFromFile("images/word_quit.png");
	menuBackground.loadFromFile("images/MenuBack.png");

	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menuBg(menuBackground);

	bool isMenu = 1;
	int menuNum = 0;

	menu1.setPosition(785, 400);
	menu2.setPosition(750, 540);
	menu3.setPosition(790, 680);
	menuBg.setPosition(0, 0);

	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (IntRect(785, 400, 340, 100).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Red); menuNum = 1; }
			if (IntRect(750, 540, 410, 95).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Red); menuNum = 2; }
			if (IntRect(790, 680, 320, 90).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Red); menuNum = 3; }
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (menuNum == 1) isMenu = false;
				if (menuNum == 2) { rules(window); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
				if (menuNum == 3) { window.close(); isMenu = false; cout << "[LOG]: Запуск игры" << endl; }
			}
			window.draw(menuBg);
			window.draw(txtTitle);
			window.draw(menu1);
			window.draw(menu2);
			window.draw(menu3);
			window.display();
		}
	}
}