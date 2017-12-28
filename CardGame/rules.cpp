#include "rules.h"

using namespace std;
using namespace sf;

void rules(RenderWindow & window)
{
	//�������� ��������� �������
	cout << "[LOG]: �������� ������" << endl;
	
	bool isRules = true;

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text txtRules("������� ���� - ����� ������", font, 60);
	txtRules.setStyle(sf::Text::Bold);
	txtRules.setPosition(100, 100);

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
		window.draw(txtRules);
		window.draw(menuBg);
		window.display();
	}
}