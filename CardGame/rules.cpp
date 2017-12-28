#include "rules.h"

using namespace std;
using namespace sf;

void rules(RenderWindow & window)
{
	//Добавить текстовые правила
	cout << "[LOG]: Открытие правил" << endl;
	
	bool isRules = true;

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text txtHeadRules("Правила игры", font, 60);
	txtHeadRules.setStyle(sf::Text::Bold);
	txtHeadRules.setPosition(750, 200);

	Text txtRules("Все кидают карты на стол и вскрывают их. \nУ кого оказалась карта старше - забирает все карты со стола. \nПобедитель будет определен, если у остальных не останется карт или \nпройдет достаточное количество раундов.", font, 40);
	txtRules.setStyle(sf::Text::Bold);
	txtRules.setPosition(300, 400);

	Texture menuBackground;
	menuBackground.loadFromFile("images/DarkWall.jpg");
	Sprite menuBg(menuBackground);

	menuBg.setPosition(0, 0);

	while (isRules)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				isRules = false;
			}
		}
		window.clear();
		window.draw(menuBg);
		window.draw(txtHeadRules);
		window.draw(txtRules);
		window.display();
	}
}