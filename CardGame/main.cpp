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

RenderWindow window(VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //���������� ��������� max � min ������� ����

const int countOfCards = 16; //���������� ���� � ������
const int countOfPlayers = 2; //���������� �������

//����� �� �����
queue<int> plCards;
queue<int> botCards;

vector<int> cardStorage; //��������� ������
vector<int> roundStorage; //��������� ���� ������

//������� �������������� ������
int startX = 1600;
int startY = 410;

//���� ��� ������������ �����
int stepX = 0;
int stepY = 0;

int curCard; //������� �����

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

Texture tBackground;
Texture tBackOfCard;

Texture txtrBotCard;
Texture txtrPlCard;

Sprite sBackground;

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

int getGroup(int Array[countOfCards/4][4], int num)
{
	for (int i = 0; i < countOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
			if (Array[i][j] == num)
				return i;

	return 0;
}

void displayCards()
{
	queue<int> bufPl = plCards;
	queue<int> bufBot = botCards;

	//��������� ���� ������
	while (!bufPl.empty())
	{
		window.draw(cardSprites[bufPl.front()]);
		bufPl.pop();
	}

	//��������� ���� ����
	while (!bufBot.empty())
	{
		window.draw(cardSprites[bufBot.front()]);
		bufBot.pop();
	}

	//��������� ������
	for (const auto &i : cardStorage)
	{
		window.draw(cardSprites[i]);
	}
}

void drawAndDisplay() {
	std::ostringstream streamPl;
	streamPl << plCards.size();
	txtPlCards.setString("����� ����: " + streamPl.str());

	std::ostringstream streamBot;
	streamBot << botCards.size();
	txtBotCards.setString("���� ����������: " + streamBot.str());

	window.clear();

	window.draw(sBackground);

	displayCards();

	window.draw(txtPlCards);
	window.draw(txtBotCards);
	if (!isFight) window.draw(txtRound);
	window.draw(txtBattle);

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

template <typename T>
std::string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

void getTextures() 
{
	txtrPlCard.loadFromFile("images/" + NumberToString(plCards.front()) + ".png");
	txtrBotCard.loadFromFile("images/" + NumberToString(botCards.front()) + ".png");

	cardSprites[plCards.front()].setTexture(txtrPlCard);
	cardSprites[botCards.front()].setTexture(txtrBotCard);
}

void returnBackOfCards() 
{
	for (int i = 0; i < countOfCards; i++) 
	{
		cardSprites[i].setTexture(tBackOfCard);
	}
}

void battleCheck()
{
	if (!plCards.empty() && !botCards.empty() && roundsCount <= 10)
	{
		String nameRound;

		getTextures();
		drawAndDisplay();

		cout << endl;
		cout << "[LOG]: ����� = " << plCards.front() << " " << botCards.front() << endl;

		roundStorage.push_back(plCards.front());
		roundStorage.push_back(botCards.front());

		//cardSprites[plCards.front()].setTextureRect(IntRect(20, 320, 206, 306));
		//cardSprites[botCards.front()].setTextureRect(IntRect(20, 320, 206, 306));

		if (getGroup(cardPrice, plCards.front()) > getGroup(cardPrice, botCards.front()))
		{
			cout << "[LOG]: ����� ������� �����" << endl;
			nameRound = "�����";
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
			cout << "[LOG]: ��� ������� �����" << endl;
			nameRound = "���";
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
			cout << "[LOG]: ����� � ���� ������" << endl;
			nameRound = "�����";
			plCards.pop();
			botCards.pop();
		}

		txtRound.setString("�����: " + nameRound);
		roundsCount++;
	}
	else
	{
		isFight = false;

		string NameWinner;

		if (plCards.size() > botCards.size())
			NameWinner = "�����";
		else if (plCards.size() < botCards.size())
			NameWinner = "���";
		else if (plCards.size() == botCards.size())
			NameWinner = "�����";

		cout << "==============================" << endl;
		cout << "[LOG]: ������� - " << NameWinner << endl;
		txtBattle.setString("����������: " + NameWinner);
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

	if (expression && roundsCount <=10) 
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
				cout << curCard << endl;
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
	cout << "[LOG]: ������ ����" << endl;

	resetGame();

	window.setFramerateLimit(60); //����� ��?

	font.loadFromFile("CyrilicOld.ttf");
	
	txtPlCards.setStyle(sf::Text::Bold);
	txtBotCards.setStyle(sf::Text::Bold);
	txtRound.setStyle(sf::Text::Bold);
	txtBattle.setStyle(sf::Text::Bold);

	txtPlCards.setString("����� ����: ");
	txtPlCards.setPosition(490, 900);
	txtBotCards.setString("���� ����������: ");
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

	cout << "[LOG]: ��������� ����������" << endl;
}

int main()
{
	setlocale(LC_ALL, "rus");

	cout << "=========[LOG SYSTEM]=========" << endl;
	cout << "[LOG]: ������ ���������.." << endl;

	srand((unsigned)time(NULL));

	cout << "[LOG]: ������ ����" << endl;
	menu(window);

	Clock clock;

	tBackground.loadFromFile("images/GameBack.png");
	sBackground.setPosition(0, 0);
	sBackground.setTexture(tBackground);
	tBackOfCard.loadFromFile("images/BackOfCard.png");

	firstStart();

	window.clear();

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		float time = float(clock.getElapsedTime().asMicroseconds()); //���� ��������� ����� � �������������
		clock.restart(); //������������� �����
		time = time / 1000; //�������� ����

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
			startAnim(handLoc);
		}

		if (isFight)
		{
			startAnim(tableLoc);
		}

		drawAndDisplay();
	}

	cout << "����� �� ���������" << endl;
	system("pause");
	return 0;
}