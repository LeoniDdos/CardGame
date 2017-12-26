#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "menu.h"
#include <queue>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace sf;


const int CountOfCards = 16;

int GetGroup(int Array[CountOfCards/4][4], int num)
{
	for (int i = 0; i < CountOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
			if (Array[i][j] == num)
				return i;

	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");
	
	cout << "=========[LOG SYSTEM]=========" << endl;
	cout << "[LOG]: ������ ���������.." << endl;

	//srand(time(0));
	srand((unsigned)time(NULL));

	RenderWindow window(VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //���������� ��������� max � min ������� ����
	
	cout << "[LOG]: ������ ����" << endl;
	menu(window);
	
	cout << "[LOG]: ������ ����" << endl;

	window.setFramerateLimit(60); //����� ��?
	
	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text txtPlCards("", font, 40);
	Text txtBotCards("", font, 40);
	//text.setColor(Color::Red);
	txtPlCards.setStyle(sf::Text::Bold);

	txtPlCards.setString("����� ����: ");
	txtPlCards.setPosition(490, 900);
	txtBotCards.setString("���� ����������: ");
	txtBotCards.setPosition(420, 100);

	Texture tBackground;
	Texture tBackOfCard;

	tBackground.loadFromFile("images/GameBack.png");
	tBackOfCard.loadFromFile("images/BackOfCard.png");
	
	Sprite sBackground(tBackground);
	Sprite sBackOfCard(tBackOfCard);

	sBackOfCard.setPosition(860, 750);

	//int ValCards[CountOfCards];
	//int VisCards[CountOfCards];

	string NameWinner;

	vector<int> CardStorage;
	vector<int> BufStorage;

	for (int i = 0; i < CountOfCards; i++)
		CardStorage.push_back(i);

	int CardPrice[CountOfCards/4][4];
	int k = 0;
	for (int i = 0; i < CountOfCards / 4;i++)
		for (int j = 0; j < 4; j++)
		{
			CardPrice[i][j] = k;
			k++;
		}

	queue<int> PlCards;
	queue<int> BotCards;

	random_device rd;
	mt19937 g(rd());

	std::shuffle(CardStorage.begin(), CardStorage.end(), g);

	bool flag = 0;
	for (const auto &i : CardStorage)
	{
		if (flag == 0)
		{
			PlCards.push(i);
			flag = 1;
			continue;
		}
		else if (flag == 1)
		{
			BotCards.push(i);
			flag = 0;
		}
		else std::cout << "Error" << endl;
	}

	window.clear();
	window.draw(sBackground);
	window.draw(txtPlCards);
	window.draw(txtBotCards);
	window.display();

	cout << "[LOG]: ��������� ����������" << endl;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{	
				if (event.key.code == Mouse::Left) 
				{
					//window.draw(sBackground);
					//window.draw(sBackOfCard);
					//window.display();
					if (!PlCards.empty() && !BotCards.empty()) 
					{
						cout << endl;
						cout << "[LOG]: ����� = " << PlCards.front() << " " << BotCards.front() << endl;

						BufStorage.push_back(PlCards.front());
						BufStorage.push_back(BotCards.front());

						if (GetGroup(CardPrice, PlCards.front()) > GetGroup(CardPrice, BotCards.front()))
						{
							cout << "[LOG]: ����� ������� �����" << endl;
							for (const auto &i : BufStorage)
								PlCards.push(i);

							BufStorage.clear();
						}
						else if (GetGroup(CardPrice, PlCards.front()) < GetGroup(CardPrice, BotCards.front()))
						{
							cout << "[LOG]: ��� ������� �����" << endl;
							for (const auto &i : BufStorage)
								BotCards.push(i);

							BufStorage.clear();
						}
						else
						{
							cout << "[LOG]: ����� � ���� ������" << endl;
						}
						PlCards.pop();
						BotCards.pop();
					}
					else 
					{
						//��������� �� ��� ���������� ������ � ������� �����
						if (PlCards.size() > BotCards.size())
							NameWinner = "�����";
						else if (PlCards.size() < BotCards.size())
							NameWinner = "���";
						else if (PlCards.size() == BotCards.size())
							NameWinner = "�����";
						else
							NameWinner = "������";

						cout << "==============================" << endl;
						cout << "[LOG]: ������� - " << NameWinner << endl;
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				menu(window);
			}
		}

		std::ostringstream playerCount;
		playerCount << PlCards.size();
		txtPlCards.setString("����� ����: " + playerCount.str());

		std::ostringstream botCount;
		botCount << BotCards.size();
		txtBotCards.setString("���� ����������: " + botCount.str());

		window.clear();
		window.draw(sBackground);
		window.draw(txtPlCards);
		window.draw(txtBotCards);
		window.draw(sBackOfCard);

		window.display();
	}

	cout << "����� �� ���������" << endl;
	system("pause");
	return 0;
}