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
	cout << "[LOG]: Запуск программы.." << endl;

	//srand(time(0));
	srand((unsigned)time(NULL));

	Clock clock;

//	float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
//	clock.restart(); //перезагружает время
//	time = time / 800; //скорость игры

	RenderWindow window(VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //Посмотреть настройки max и min размера окна
	
	cout << "[LOG]: Запуск меню" << endl;
	menu(window);
	
	cout << "[LOG]: Запуск игры" << endl;

	window.setFramerateLimit(60); //нужно ли?
	
	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text txtPlCards("", font, 40);
	Text txtBotCards("", font, 40);
	//text.setColor(Color::Red);
	txtPlCards.setStyle(sf::Text::Bold);

	txtPlCards.setString("Ваших карт: ");
	txtPlCards.setPosition(490, 900);
	txtBotCards.setString("Карт компьютера: ");
	txtBotCards.setPosition(420, 100);

	Texture tBackground;
	Texture tBackOfCard;

	tBackground.loadFromFile("images/GameBack.png");
	tBackOfCard.loadFromFile("images/BackOfCard.png");
	
	Sprite sBackground(tBackground);
	//Sprite sBackOfCard(tBackOfCard);

	Sprite cardSprites[CountOfCards];
	for (int i = 0; i < CountOfCards; i++) 
	{
		cardSprites[i].setTexture(tBackOfCard);
		cardSprites[i].setPosition(1600, 410);
	}

	//sBackOfCard.setPosition(860, 750);

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
	//for (int i = 0; i < CountOfCards; i++) window.draw(cardSprites[i]);
	window.display();

	int plCount = 0;
	int botCount = 0;

	int startX = 1600;
	int startY = 410;

	int changeX;
	int changeY;

	int step;

	int currentCard = CountOfCards - 1;

	int whoCard = 0;

	bool logic1;
	bool logic2;

	cout << "[LOG]: Окончание подготовки" << endl;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 1000; //скорость игры

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{	
				if (event.key.code == Mouse::Left && (botCount + plCount) == CountOfCards)
				{
					//window.draw(sBackground);
					//window.draw(sBackOfCard);
					//window.display();
					if (!PlCards.empty() && !BotCards.empty()) 
					{
						cout << endl;
						cout << "[LOG]: Карты = " << PlCards.front() << " " << BotCards.front() << endl;

						BufStorage.push_back(PlCards.front());
						BufStorage.push_back(BotCards.front());

						if (GetGroup(CardPrice, PlCards.front()) > GetGroup(CardPrice, BotCards.front()))
						{
							cout << "[LOG]: Игрок выиграл раунд" << endl;
							for (const auto &i : BufStorage)
								PlCards.push(i);

							BufStorage.clear();
						}
						else if (GetGroup(CardPrice, PlCards.front()) < GetGroup(CardPrice, BotCards.front()))
						{
							cout << "[LOG]: Бот выиграл раунд" << endl;
							for (const auto &i : BufStorage)
								BotCards.push(i);

							BufStorage.clear();
						}
						else
						{
							cout << "[LOG]: Ничья в этом раунде" << endl;
						}
						PlCards.pop();
						BotCards.pop();
					}
					else 
					{
						//Проверить на все одинаковые группы и вывести ничью
						if (PlCards.size() > BotCards.size())
							NameWinner = "Игрок";
						else if (PlCards.size() < BotCards.size())
							NameWinner = "Бот";
						else if (PlCards.size() == BotCards.size())
							NameWinner = "Ничья";
						else
							NameWinner = "Ошибка";

						cout << "==============================" << endl;
						cout << "[LOG]: Выиграл - " << NameWinner << endl;
					}
				}
				else 
				{
					int flag = 0;

					for (int i = 0; i < CountOfCards; i++) 
					{
						if (flag == 0) cardSprites[i].setPosition(856, 770);
						if (flag == 1) cardSprites[i].setPosition(856, 50);

						flag = flag == 0 ? 1 : 0;
					}
					plCount = CountOfCards / 2;
					botCount = CountOfCards / 2;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				menu(window);
			}
		}

		if (botCount < CountOfCards / 2)
		{
			if (whoCard == 0) 
			{
				changeY = startY + 360;
				changeX = startX - 740;
				step = 8;
				logic1 = cardSprites[currentCard].getPosition().y < changeY;
				logic2 = cardSprites[currentCard].getPosition().x > changeX;
			}
			else if (whoCard == 1) 
			{
				changeY = startY - 360;
				changeX = startX - 740;
				step = -8;
				logic1 = cardSprites[currentCard].getPosition().y > changeY;
				logic2 = cardSprites[currentCard].getPosition().x > changeX;
			}

			if (logic1)
			{
				cardSprites[currentCard].move(0, step);
				//cout << cardSprites[currentCard].getPosition().x << " " << cardSprites[currentCard].getPosition().y << endl;
			}
			else if (cardSprites[currentCard].getPosition().x > changeX)
			{
				cardSprites[currentCard].move(-8, 0);
				//cout << cardSprites[currentCard].getPosition().x << " " << cardSprites[currentCard].getPosition().y << endl;
			}
			else 
			{
				if (whoCard == 0)
				{
					plCount++;
					whoCard = 1;
					//cout << "PLAYER:" << cardSprites[currentCard].getPosition().x << " " << cardSprites[currentCard].getPosition().y << endl;
				}
				else if (whoCard == 1)
				{
					botCount++;
					whoCard = 0;
					//cout << "BOT:" << cardSprites[currentCard].getPosition().x << " " << cardSprites[currentCard].getPosition().y << endl;
				}
				currentCard--;
			}
		}

		std::ostringstream streamPl;
		streamPl << plCount;
		txtPlCards.setString("Ваших карт: " + streamPl.str());

		std::ostringstream streamBot;
		streamBot << botCount;
		txtBotCards.setString("Карт компьютера: " + streamBot.str());

		window.clear();
		window.draw(sBackground);
		window.draw(txtPlCards);
		window.draw(txtBotCards);
		//window.draw(sBackOfCard);
		for (int i = 0; i < CountOfCards; i++) window.draw(cardSprites[i]);

		window.display();
	}

	cout << "Выход из программы" << endl;
	system("pause");
	return 0;
}