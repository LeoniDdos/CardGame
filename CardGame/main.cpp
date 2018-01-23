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

const int COUNT_OF_CARDS = 16; //Количество карт в колоде
const int COUNT_OF_PLAYERS = 2; //Количество игроков
const int COUNT_OF_ROUNDS = 10; //Количество раундов

//Карты на руках
queue<int> plCards;
queue<int> botCards;

vector<int> cardStorage; //Хранилище колоды
vector<int> roundStorage; //Хранилище карт раунда

//Позиция первоначальной колоды
int startX = 1600;
int startY = 410;

//Шаги для передвижения карты
float stepX = 0;
float stepY = 0;

int curCard; //Текущая карта

int whoCard = 0;
int flagSide = 0;

bool isFight = false;
bool expression;

int cardPrice[COUNT_OF_CARDS / 4][4];

int roundsCount = 0;

Sprite cardSprites[COUNT_OF_CARDS];

Font font;
Text txtPlCards("", font, 40);
Text txtBotCards("", font, 40);
Text txtRoundInfo("", font, 40);
Text txtBattleInfo("", font, 40);
Text txtRounds("", font, 40);

Texture tBackground;
Texture tBackOfCard;

Texture txtrBotCard;
Texture txtrPlCard;

Texture txtrMove;

Sprite sBackground;

Sprite sMove;

enum PutLocation
{
	handLoc,
	tableLoc
};

enum Side
{
	upSide,
	downSide,
	leftSide,
	rightSide
};

int getGroup(int numArray[COUNT_OF_CARDS / 4][4], int num)
{
	for (int i = 0; i < COUNT_OF_CARDS / 4; i++)
		for (int j = 0; j < 4; j++)
			if (numArray[i][j] == num)
				return i;

	return 0;
}

template <typename T>
std::string numberToString(T num)
{
	std::ostringstream ss;
	ss << num;
	return ss.str();
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

void drawAndDisplay() 
{
	txtPlCards.setString("Ваших карт: " + numberToString(plCards.size()));
	txtBotCards.setString("Карт компьютера: " + numberToString(botCards.size()));
	txtRounds.setString("Раунд: " + numberToString(roundsCount) + "/" + numberToString(COUNT_OF_ROUNDS));

	window.clear();

	window.draw(sBackground);

	displayCards();

	window.draw(txtPlCards);
	window.draw(txtBotCards);
	if ((plCards.size() + botCards.size()) == COUNT_OF_CARDS || isFight)
	{
		window.draw(txtRounds);
	}
	if (!isFight) 
	{ 
		window.draw(txtRoundInfo);
	}
	window.draw(txtBattleInfo);

	if ((plCards.size() + botCards.size()) == COUNT_OF_CARDS && !isFight && roundsCount != COUNT_OF_ROUNDS)
	{
		window.draw(sMove);
	}

	window.display();
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

void getTextures() 
{
	txtrPlCard.loadFromFile("images/" + numberToString(plCards.front()) + ".png");
	txtrBotCard.loadFromFile("images/" + numberToString(botCards.front()) + ".png");

	cardSprites[plCards.front()].setTexture(txtrPlCard);
	cardSprites[botCards.front()].setTexture(txtrBotCard);
}

void returnBackOfCards() 
{
	for (int i = 0; i < COUNT_OF_CARDS; i++)
	{
		cardSprites[i].setTexture(tBackOfCard);
	}
}

void battleCheck()
{
	if (!plCards.empty() && !botCards.empty() && roundsCount < COUNT_OF_ROUNDS)
	{
		String nameRound;

		getTextures();
		drawAndDisplay();

		cout << endl;
		cout << "[LOG]: Карты = " << plCards.front() << " " << botCards.front() << endl;

		roundStorage.push_back(plCards.front());
		roundStorage.push_back(botCards.front());

		if (getGroup(cardPrice, plCards.front()) > getGroup(cardPrice, botCards.front()))
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

			sleep(sf::milliseconds(1500));

			putInHand();
			isFight = false;

			returnBackOfCards();
		}
		else if (getGroup(cardPrice, plCards.front()) < getGroup(cardPrice, botCards.front()))
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

			sleep(sf::milliseconds(1500));

			putInHand();
			isFight = false;

			returnBackOfCards();
		}
		else
		{
			cout << "[LOG]: Ничья в этом раунде" << endl;
			nameRound = "Ничья";
			plCards.pop();
			botCards.pop();
		}

		txtRoundInfo.setString("Раунд: " + nameRound);
		roundsCount++;
	}
	
	if (plCards.empty() || botCards.empty() || roundsCount >= COUNT_OF_ROUNDS)
	{
		isFight = false;

		string winnerName;

		if (plCards.size() > botCards.size())
			winnerName = "Игрок";
		else if (plCards.size() < botCards.size())
			winnerName = "Бот";
		else if (plCards.size() == botCards.size())
			winnerName = "Ничья";

		cout << "==============================" << endl;
		cout << "[LOG]: Выиграл - " << winnerName << endl;
		txtBattleInfo.setString("Победитель: " + winnerName);
	}
}

void moveCard(PutLocation putLocation, int cardNum, Side side, float endX, float endY)
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

	if (expression && roundsCount < COUNT_OF_ROUNDS)
	{
		cardSprites[curCard].move(stepX, stepY);
	}
	else 
	{
		if (flagSide == 0) flagSide = 1;
		else
		{
			if (putLocation == handLoc)
			{
				firstGive();
				//cout << curCard << endl;
			}
			else if (putLocation == tableLoc) 
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
	case handLoc: 
	{
		if (whoCard == 0) moveCardMajor(putLocation, cardStorage.back(), 840, 770, downSide, leftSide);
		else moveCardMajor(putLocation, cardStorage.back(), 860, 50, upSide, leftSide);
		break;
	}
	case tableLoc:
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

	window.setFramerateLimit(60);

	font.loadFromFile("CyrilicOld.ttf");
	
	txtPlCards.setStyle(sf::Text::Bold);
	txtBotCards.setStyle(sf::Text::Bold);
	txtRoundInfo.setStyle(sf::Text::Bold);
	txtBattleInfo.setStyle(sf::Text::Bold);
	txtRounds.setStyle(sf::Text::Bold);

	txtPlCards.setString("Ваших карт: ");
	txtPlCards.setPosition(490, 900);
	txtBotCards.setString("Карт компьютера: ");
	txtBotCards.setPosition(420, 100);

	txtRoundInfo.setString("");
	txtRoundInfo.setPosition(250, 500);
	txtBattleInfo.setString("");
	txtBattleInfo.setPosition(800, 500);

	txtRounds.setPosition(1600, 70);

	for (int i = 0; i < COUNT_OF_CARDS; i++)
		cardStorage.push_back(i);

	int k = 0;
	for (int i = 0; i < COUNT_OF_CARDS / 4; i++)
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
		//cout << "Card Storage: " << i << endl;
		cardSprites[i].setTexture(tBackOfCard);
		cardSprites[i].setPosition(1600, 410);
	}

	cout << "[LOG]: Окончание подготовки" << endl;
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

	tBackground.loadFromFile("images/GameBack.png");
	sBackground.setPosition(0, 0);
	sBackground.setTexture(tBackground);
	tBackOfCard.loadFromFile("images/BackOfCard.png");

	txtrMove.loadFromFile("images/word_move.png");
	sMove.setTexture(txtrMove);
	sMove.setPosition(1400, 510);

	firstStart();

	window.clear();

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		float time = float(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time = time / 1000;

		sMove.setColor(Color::White);

		sf::Event event;
		
		if (IntRect(1400, 510, 190, 85).contains(Mouse::getPosition(window))) 
		{ 
			sMove.setColor(sf::Color(169, 169, 169, 255)); 
		}
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{	
				if (event.key.code == Mouse::Left && IntRect(1400, 510, 190, 85).contains(Mouse::getPosition(window)) && (plCards.size() + botCards.size()) == COUNT_OF_CARDS || isFight)
				{
					isFight = true;
				}
				else if (event.key.code == Mouse::Left && !((plCards.size() + botCards.size()) == COUNT_OF_CARDS || isFight))
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

		if (!(plCards.size() + botCards.size() == COUNT_OF_CARDS) && !isFight)
		{
			startAnim(handLoc);
		}
		if (isFight)
		{
			startAnim(tableLoc);
		}

		drawAndDisplay();
	}

	cout << "Выход из программы" << endl;
	system("pause");
	return 0;
}