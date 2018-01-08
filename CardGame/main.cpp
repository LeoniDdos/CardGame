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

RenderWindow window(VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //Посмотреть настройки max и min размера окна

const int countOfCards = 16; //Количество карт в колоде
const int countOfPlayers = 2; //Количество игроков

//Карты на руках
queue<int> plCards;
queue<int> botCards;

vector<int> cardStorage; //Хранилище колоды
vector<int> roundStorage; //Хранилище карт раунда

//Позиция первоначальной колоды
int startX = 1600;
int startY = 410;

//Шаги для передвижения карты
int stepX = 0;
int stepY = 0;

int curCard; //Текущая карта

int whoCard = 0;
int flagSide = 0;

bool isFight = false;
bool expression;

int cardPrice[countOfCards / 4][4];

int roundsCount = 0;

Sprite cardSprites[countOfCards];

Font font;
Text txtPlCards("", font, 40);
Text txtBotCards("", font, 40);
Text txtRound("", font, 40);
Text txtBattle("", font, 40);

Texture tBackOfCard;

enum PutLocation
{
	handLocation,
	centerLocation
};

enum Side
{
	upSide,
	downSide,
	leftSide,
	rightSide
};

int GetGroup(int Array[countOfCards/4][4], int num)
{
	for (int i = 0; i < countOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
			if (Array[i][j] == num)
				return i;

	return 0;
}

void putInHand()
{
	if (!isFight)
	{
		for (const auto &i : cardStorage)
		{
			if (whoCard == 0)
			{
				plCards.push(i);
				whoCard = 1;
				//continue;
			}
			else if (whoCard == 1)
			{
				botCards.push(i);
				whoCard = 0;
			}
		}
	}

	queue<int> bufPl = plCards;
	queue<int> bufBot = botCards;

	while (!bufPl.empty())
	{
		cardSprites[bufPl.front()].setPosition(856, 770);
		bufPl.pop();
	}
	while (!bufBot.empty())
	{
		cardSprites[bufBot.front()].setPosition(856, 50);
		bufBot.pop();
	}
}

void firstGive()
{
	if (whoCard == 0)
	{
		plCards.push(curCard);
		cardStorage.pop_back();
		whoCard = 1;
	}
	else if (whoCard == 1)
	{
		botCards.push(curCard);
		cardStorage.pop_back();
		whoCard = 0;
	}
}

void battleCheck()
{
	if (!plCards.empty() && !botCards.empty() && roundsCount <= 10)
	{
		String nameRound;

		cout << endl;
		cout << "[LOG]: Карты = " << plCards.front() << " " << botCards.front() << endl;

		roundStorage.push_back(plCards.front());
		roundStorage.push_back(botCards.front());

		if (GetGroup(cardPrice, plCards.front()) > GetGroup(cardPrice, botCards.front()))
		{
			cout << "[LOG]: Игрок выиграл раунд" << endl;
			nameRound = "Игрок";
			plCards.pop();
			botCards.pop();
			for (const auto &i : roundStorage)
				plCards.push(i);

			cout << "PlCards: " << plCards.size() << endl;
			cout << "BotCards: " << botCards.size() << endl;
			roundStorage.clear();

			sleep(sf::milliseconds(1000));

			putInHand();
			isFight = false;
		}
		else if (GetGroup(cardPrice, plCards.front()) < GetGroup(cardPrice, botCards.front()))
		{
			cout << "[LOG]: Бот выиграл раунд" << endl;
			nameRound = "Бот";
			plCards.pop();
			botCards.pop();
			for (const auto &i : roundStorage)
				botCards.push(i);

			cout << "PlCards: " << plCards.size() << endl;
			cout << "BotCards: " << botCards.size() << endl;
			roundStorage.clear();

			sleep(sf::milliseconds(1000));

			putInHand();
			isFight = false;
		}
		else
		{
			cout << "[LOG]: Ничья в этом раунде" << endl;
			nameRound = "Ничья";
			plCards.pop();
			botCards.pop();
		}

		txtRound.setString("Раунд: " + nameRound);
		roundsCount++;
	}
	else
	{
		isFight = false;

		string NameWinner;

		if (plCards.size() > botCards.size())
			NameWinner = "Игрок";
		else if (plCards.size() < botCards.size())
			NameWinner = "Бот";
		else if (plCards.size() == botCards.size())
			NameWinner = "Ничья";

		cout << "==============================" << endl;
		cout << "[LOG]: Выиграл - " << NameWinner << endl;
		txtBattle.setString("Победитель: " + NameWinner);
	}
}

void moveCard(PutLocation putLocation, int cardNum, Side side, int endX, int endY)
{
	curCard = cardNum;

	switch (side)
	{
	case upSide:
		stepX = 0;
		stepY = -8;
		expression = cardSprites[curCard].getPosition().y > endY;
		break;
	case downSide:
		stepX = 0;
		stepY = 8;
		expression = cardSprites[curCard].getPosition().y < endY;
		break;
	case leftSide:
		stepX = -8;
		stepY = 0;
		expression = cardSprites[curCard].getPosition().x > endX;
		break;
	case rightSide:
		stepX = 8;
		stepY = 0;
		expression = cardSprites[curCard].getPosition().x < endX;
		break;
	default:
		break;
	}

	if (expression) 
	{
		cardSprites[curCard].move(stepX, stepY);
	}
	else 
	{
		if (flagSide == 0) flagSide = 1;
		else
		{
			if (putLocation == handLocation)
			{
				firstGive();
				cout << curCard << endl;
			}
			else if (putLocation == centerLocation) 
			{
				if (whoCard == 1) battleCheck();
				whoCard = whoCard == 0 ? 1 : 0;
			}
			flagSide = 0;
		}
	}
}

void moveCardMajor(PutLocation putLocation, int cardNum, float endX, float endY, Side side1, Side side2)
{
	if (flagSide == 0) 
	{
		moveCard(putLocation, cardNum, side1, endX, endY);
	}
	else if (flagSide == 1) 
	{
		moveCard(putLocation, cardNum, side2, endX, endY);
	}
}

void startAnim(PutLocation putLocation) 
{
	switch (putLocation)
	{
	case handLocation: 
	{
		if (whoCard == 0) moveCardMajor(putLocation, cardStorage.back(), 840, 770, downSide, leftSide);
		else moveCardMajor(putLocation, cardStorage.back(), 860, 50, upSide, leftSide);
		break;
	}
	case centerLocation:
	{
		if (whoCard == 0) moveCardMajor(putLocation, plCards.front(), 980, 410, rightSide, upSide);
		else moveCardMajor(putLocation, botCards.front(), 760, 410, leftSide, downSide);
		break;
	}
	default:
		break;
	}
}

void resetGame() 
{
	while (!plCards.empty())
		plCards.pop();
	
	while (!botCards.empty())
		botCards.pop();

	roundStorage.clear();
	cardStorage.clear();

	isFight = false;

	flagSide = 0;
	whoCard = 0;

	roundsCount = 0;
}

void firstStart()
{
	cout << "[LOG]: Запуск игры" << endl;

	resetGame();

	window.setFramerateLimit(60); //нужно ли?

	font.loadFromFile("CyrilicOld.ttf");
	
	txtPlCards.setStyle(sf::Text::Bold);
	txtBotCards.setStyle(sf::Text::Bold);
	txtRound.setStyle(sf::Text::Bold);
	txtBattle.setStyle(sf::Text::Bold);

	txtPlCards.setString("Ваших карт: ");
	txtPlCards.setPosition(490, 900);
	txtBotCards.setString("Карт компьютера: ");
	txtBotCards.setPosition(420, 100);

	txtRound.setString("");
	txtRound.setPosition(250, 500);
	txtBattle.setString("");
	txtBattle.setPosition(800, 500);

	for (int i = 0; i < countOfCards; i++)
		cardStorage.push_back(i);

	int k = 0;
	for (int i = 0; i < countOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
		{
			cardPrice[i][j] = k;
			k++;
		}

	random_device rd;
	mt19937 g(rd());

	std::shuffle(cardStorage.begin(), cardStorage.end(), g);

	for (const auto &i : cardStorage) 
	{
		cout << "Card Storage: " << i << endl;

		cardSprites[i].setTexture(tBackOfCard);
		cardSprites[i].setPosition(1600, 410);
	}

	cout << "[LOG]: Окончание подготовки" << endl;
}

void displayCards() 
{
	queue<int> bufPl = plCards;
	queue<int> bufBot = botCards;

	//Отрисовка карт игрока
	while (!bufPl.empty())
	{
		window.draw(cardSprites[bufPl.front()]);
		bufPl.pop();
	}

	//Отрисовка карт бота
	while (!bufBot.empty())
	{
		window.draw(cardSprites[bufBot.front()]);
		bufBot.pop();
	}

	//Отрисовка колоды
	for (const auto &i : cardStorage) 
	{
		window.draw(cardSprites[i]);
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "=========[LOG SYSTEM]=========" << endl;
	cout << "[LOG]: Запуск программы.." << endl;

	srand((unsigned)time(NULL));

	cout << "[LOG]: Запуск меню" << endl;
	menu(window);

	Clock clock;
	Texture tBackground;
	tBackground.loadFromFile("images/GameBack.png");
	tBackOfCard.loadFromFile("images/BackOfCard.png");
	Sprite sBackground(tBackground);

	firstStart();
	window.clear();

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		float time = float(clock.getElapsedTime().asMicroseconds()); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 1000; //скорость игры

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{	
				if (event.key.code == Mouse::Left && (plCards.size() + botCards.size()) == countOfCards || isFight)
				{
					isFight = true;
				}
				else 
				{
					putInHand();
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				menu(window);
				firstStart();
			}
		}

		if (!(plCards.size() + botCards.size() == countOfCards) && !isFight)
		{
			startAnim(handLocation);
		}

		if (isFight)
		{
			startAnim(centerLocation);
		}

		std::ostringstream streamPl;
		streamPl << plCards.size();
		txtPlCards.setString("Ваших карт: " + streamPl.str());

		std::ostringstream streamBot;
		streamBot << botCards.size();
		txtBotCards.setString("Карт компьютера: " + streamBot.str());

		window.clear();

		window.draw(sBackground);

		displayCards();

		window.draw(txtPlCards);
		window.draw(txtBotCards);
		if (!isFight) window.draw(txtRound);
		window.draw(txtBattle);

		window.display();
	}

	cout << "Выход из программы" << endl;
	system("pause");
	return 0;
}