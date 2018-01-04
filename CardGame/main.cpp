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

//Количество карт
const int countOfCards = 16;
const int countOfPlayers = 2;

//Колоды
queue<int> plCards;
queue<int> botCards;

vector<int> bufStorage; //Буфферное хранилище карт раунда

//Количество карт на руках
int plCount = 0;
int botCount = 0;

//Позиция первоначальной колоды
int startX = 1600;
int startY = 410;

int changeX;
int changeY;

float step;

int currentCard = countOfCards - 1;

int whoCard = 0;

bool logic1;
//bool logic2;

int CardPrice[countOfCards / 4][4];

Sprite cardSprites[countOfCards];

Font font;
Text txtPlCards("", font, 40);
Text txtBotCards("", font, 40);
Text txtRound("", font, 40);
Text txtBattle("", font, 40);

bool flagRight = false;

int roundsCount = 0;

Texture tBackOfCard;

int GetGroup(int Array[countOfCards/4][4], int num)
{
	for (int i = 0; i < countOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
			if (Array[i][j] == num)
				return i;

	return 0;
}

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

int flagSide = 0;

bool expression;

int stepX = 0;
int stepY = 0;

void firstGive()
{
	if (whoCard == 0)
	{
		plCount++;
		whoCard = 1;
	}
	else if (whoCard == 1)
	{
		botCount++;
		whoCard = 0;
	}
	currentCard--;
}

void moveCard(PutLocation putLocation, Side side, int endX, int endY)
{
	switch (side)
	{
	case upSide:
		stepX = 0;
		stepY = -8;
		expression = cardSprites[currentCard].getPosition().y > endY;
		break;
	case downSide:
		stepX = 0;
		stepY = 8;
		expression = cardSprites[currentCard].getPosition().y < endY;
		break;
	case leftSide:
		stepX = -8;
		stepY = 0;
		expression = cardSprites[currentCard].getPosition().x > endX;
		break;
	case rightSide:
		stepX = 8;
		stepY = 0;
		expression = cardSprites[currentCard].getPosition().x < endX;
		break;
	default:
		break;
	}

	if (expression) 
	{
		cardSprites[currentCard].move(stepX, stepY);
	}
	else 
	{
		if (flagSide == 0) flagSide = 1;
		else
		{
			if (putLocation == handLocation)
			{
				firstGive();
			}
			else if (putLocation == centerLocation) 
			{

			}
			flagSide = 0;
		}
	}
}

void moveCardMajor(PutLocation putLocation, float endX, float endY, Side side1, Side side2)
{
	if (flagSide == 0) 
	{
		moveCard(putLocation, side1, endX, endY);
	}
	else if (flagSide == 1) 
	{
		moveCard(putLocation, side2, endX, endY);
	}
}

void startAnim(PutLocation putLocation) 
{
	switch (putLocation)
	{
	case handLocation: 
	{
		if (whoCard == 0) moveCardMajor(putLocation, 860, 770, downSide, leftSide);
		else moveCardMajor(putLocation, 860, 50, upSide, leftSide);
		break;
	}
	case centerLocation:
	{
		if (whoCard == 0) moveCardMajor(putLocation, 880, 410, rightSide, upSide);
		else moveCardMajor(putLocation, 840, 410, leftSide, downSide);
		break;
	}
	default:
		break;
	}
}

void battleCheck()
{
	if (!plCards.empty() && !botCards.empty() && roundsCount <= 10)
	{
		String nameRound;

		cout << endl;
		cout << "[LOG]: Карты = " << plCards.front() << " " << botCards.front() << endl;

		bufStorage.push_back(plCards.front());
		bufStorage.push_back(botCards.front());

		if (GetGroup(CardPrice, plCards.front()) > GetGroup(CardPrice, botCards.front()))
		{
			cout << "[LOG]: Игрок выиграл раунд" << endl;
			nameRound = "Игрок";
			plCards.pop();
			botCards.pop();
			for (const auto &i : bufStorage)
				plCards.push(i);

			plCount += bufStorage.size()/2;
			botCount -= bufStorage.size()/2;
			cout << "PlCards: " << plCards.size() << endl;
			cout << "BotCards: " << botCards.size() << endl;
			bufStorage.clear();
		}
		else if (GetGroup(CardPrice, plCards.front()) < GetGroup(CardPrice, botCards.front()))
		{
			cout << "[LOG]: Бот выиграл раунд" << endl;
			nameRound = "Бот";
			plCards.pop();
			botCards.pop();
			for (const auto &i : bufStorage)
				botCards.push(i);

			plCount -= bufStorage.size()/2;
			botCount += bufStorage.size()/2;
			cout << "PlCards: " << plCards.size() << endl;
			cout << "BotCards: " << botCards.size() << endl;
			bufStorage.clear();
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
		string NameWinner;

		//Проверить на все одинаковые группы и вывести ничью
		if (plCards.size() > botCards.size())
			NameWinner = "Игрок";
		else if (plCards.size() < botCards.size())
			NameWinner = "Бот";
		else if (plCards.size() == botCards.size())
			NameWinner = "Ничья";
		else
			NameWinner = "Ошибка";

		cout << "==============================" << endl;
		cout << "[LOG]: Выиграл - " << NameWinner << endl;
		txtBattle.setString("Победитель: " + NameWinner);
	}
}

void skipAnim() 
{
	int flag = 0;

	for (int i = 0; i < countOfCards; i++)
	{
		if (flag == 0) cardSprites[i].setPosition(856, 770);
		if (flag == 1) cardSprites[i].setPosition(856, 50);

		flag = flag == 0 ? 1 : 0;
	}
	plCount = countOfCards / 2;
	botCount = countOfCards / 2;
}

void resetGame() 
{
	while (!plCards.empty())
		plCards.pop();
	
	while (!botCards.empty())
		botCards.pop();

	bufStorage.clear();

	plCount = 0;
	botCount = 0;

	changeX = 0;
	changeY = 0;

	step = 0;

	currentCard = countOfCards - 1;

	whoCard = 0;

	flagRight = false;

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

	vector<int> CardStorage;

	for (int i = 0; i < countOfCards; i++)
		CardStorage.push_back(i);

	int k = 0;
	for (int i = 0; i < countOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
		{
			CardPrice[i][j] = k;
			k++;
		}

	random_device rd;
	mt19937 g(rd());

	std::shuffle(CardStorage.begin(), CardStorage.end(), g);

	bool flag = 0;
	for (const auto &i : CardStorage)
	{
		if (flag == 0)
		{
			plCards.push(i);
			flag = 1;
			continue;
		}
		else if (flag == 1)
		{
			botCards.push(i);
			flag = 0;
		}
		else std::cout << "Error" << endl;
	}

	for (int i = 0; i < countOfCards; i++)
	{
		cardSprites[i].setTexture(tBackOfCard);
		cardSprites[i].setPosition(1600, 410);
	}

	cout << "[LOG]: Окончание подготовки" << endl;
}

void setRightPlace() 
{
	queue<int> bufPl = plCards;
	queue<int> bufBot = botCards;

	int who = 0;

	for (int i = 0; i < countOfCards; i++) 
	{
		if (who == 0) 
		{
			cardSprites[bufPl.front()].setPosition(856, 770);
			bufPl.pop();
		}
		if (who == 1)
		{
			cardSprites[bufBot.front()].setPosition(856, 50);
			bufBot.pop();
		}

		who = who == 0 ? 1 : 0;
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
				if (event.key.code == Mouse::Left && (botCount + plCount) == countOfCards)
				{
					battleCheck();
				}
				else 
				{
					skipAnim();
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				menu(window);
				firstStart();
			}
		}

		if (!(botCount + plCount == countOfCards))
		{
			startAnim(handLocation);
			flagRight = true;
		}
		else if(flagRight == true) 
		{
			setRightPlace(); //Постоянно?
			flagRight = false;
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
		window.draw(txtRound);
		window.draw(txtBattle);
		for (int i = 0; i < countOfCards; i++) window.draw(cardSprites[i]);

		window.display();
	}

	cout << "Выход из программы" << endl;
	system("pause");
	return 0;
}