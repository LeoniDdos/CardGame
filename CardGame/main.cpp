#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "menu.h"
#include <queue>
#include <random>
#include <algorithm>

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

	RenderWindow window(VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //Посмотреть настройки max и min размера окна
	
	cout << "[LOG]: Запуск меню" << endl;
	menu(window);
	
	cout << "[LOG]: Запуск игры" << endl;

	window.setFramerateLimit(60); //нужно ли?
	
	Texture tBackground;
	Texture tBackOfCard;

	tBackground.loadFromFile("images/GameBack.png");
	tBackOfCard.loadFromFile("images/BackOfCard.png");
	
	Sprite sBackground(tBackground);
	Sprite sBackOfCard(tBackOfCard);

	//int ValCards[CountOfCards];
	//int VisCards[CountOfCards];

	string NameWinner;

	vector<int> CardStorage;
	vector<int> BufStorage;

	for (int i = 0; i < CountOfCards; i++)
		CardStorage.push_back(i);

	int CardPrice[CountOfCards/4][4];
	int k=0;
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
	window.display();

	cout << "[LOG]: Окончание подготовки" << endl;

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
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				menu(window);
			}
		}
		window.clear();
		window.draw(sBackground);

		//window.draw(sBackOfCard);
		window.display();
	}

	cout << "Выход из программы" << endl;
	system("pause");
	return 0;
}