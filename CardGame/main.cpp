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

const int countOfCards = 16; //Количество карт
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

int currentCard = countOfCards - 1;

int whoCard = 0;
int flagSide = 0;

bool isFight = false;
bool expression;
bool flagRight = false;

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

void firstGive()
{
	if (whoCard == 0)
	{
		plCards.push(cardStorage.front());
		cardStorage.erase(cardStorage.begin());
		whoCard = 1;
	}
	else if (whoCard == 1)
	{
		botCards.push(cardStorage.front());
		cardStorage.erase(cardStorage.begin());
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
		isFight = true;
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

	for (const auto &i : cardStorage)
	{
		if (whoCard == 0)
		{
			plCards.push(i);
			whoCard = 1;
			continue;
		}
		else if (whoCard == 1)
		{
			botCards.push(i);
			whoCard = 0;
		}
		else std::cout << "Error" << endl;
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

	//ЗДЕСЬ ДОЛЖА БЫТЬ СОРТИРОВКА ПО CARTSTORAGE

	for (const auto &i : cardStorage) cout << "Card Storage: " << i << endl;

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
				if (event.key.code == Mouse::Left && (plCards.size() + botCards.size()) == countOfCards || isFight)
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

		if (!(plCards.size() + botCards.size() == countOfCards) && !isFight)
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
		streamPl << plCards.size();
		txtPlCards.setString("Ваших карт: " + streamPl.str());

		std::ostringstream streamBot;
		streamBot << botCards.size();
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