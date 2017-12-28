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

//Количество карт
const int CountOfCards = 16;

//Колоды
queue<int> PlCards;
queue<int> BotCards;

vector<int> CardStorage;
vector<int> BufStorage; //Буфферное хранилище карт

//Количество карт на руках
int plCount = 0;
int botCount = 0;

//Позиция первоначальной колоды
int startX = 1600;
int startY = 410;

int changeX;
int changeY;

float step;

int currentCard = CountOfCards - 1;

int whoCard = 0;

bool logic1;
//bool logic2;

int CardPrice[CountOfCards / 4][4];

Sprite cardSprites[CountOfCards];

Font font;
Text txtPlCards("", font, 40);
Text txtBotCards("", font, 40);
Text txtRound("", font, 40);

int GetGroup(int Array[CountOfCards/4][4], int num)
{
	for (int i = 0; i < CountOfCards / 4; i++)
		for (int j = 0; j < 4; j++)
			if (Array[i][j] == num)
				return i;

	return 0;
}

void battleCheck()
{
	if (!PlCards.empty() && !BotCards.empty())
	{
		String nameRound;

		cout << endl;
		cout << "[LOG]: Карты = " << PlCards.front() << " " << BotCards.front() << endl;

		BufStorage.push_back(PlCards.front());
		BufStorage.push_back(BotCards.front());

		if (GetGroup(CardPrice, PlCards.front()) > GetGroup(CardPrice, BotCards.front()))
		{
			cout << "[LOG]: Игрок выиграл раунд" << endl;
			nameRound = "Игрок";
			PlCards.pop();
			BotCards.pop();
			for (const auto &i : BufStorage)
				PlCards.push(i);

			plCount += BufStorage.size()/2;
			botCount -= BufStorage.size()/2;
			cout << "PlCards: " << PlCards.size() << endl;
			cout << "BotCards: " << BotCards.size() << endl;
			BufStorage.clear();
		}
		else if (GetGroup(CardPrice, PlCards.front()) < GetGroup(CardPrice, BotCards.front()))
		{
			cout << "[LOG]: Бот выиграл раунд" << endl;
			nameRound = "Бот";
			PlCards.pop();
			BotCards.pop();
			for (const auto &i : BufStorage)
				BotCards.push(i);

			plCount -= BufStorage.size()/2;
			botCount += BufStorage.size()/2;
			cout << "PlCards: " << PlCards.size() << endl;
			cout << "BotCards: " << BotCards.size() << endl;
			BufStorage.clear();
		}
		else
		{
			cout << "[LOG]: Ничья в этом раунде" << endl;
			nameRound = "Ничья";
			PlCards.pop();
			BotCards.pop();
		}

		txtRound.setString("Раунд: " + nameRound);
	}
	else
	{
		string NameWinner;

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

void skipAnim() 
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

void firstAnim() 
{
	if (whoCard == 0)
	{
		changeY = startY + 360;
		changeX = startX - 740;
		step = 8;
		logic1 = cardSprites[currentCard].getPosition().y < changeY;
		//logic2 = cardSprites[currentCard].getPosition().x > changeX;
	}
	else if (whoCard == 1)
	{
		changeY = startY - 360;
		changeX = startX - 740;
		step = -8;
		logic1 = cardSprites[currentCard].getPosition().y > changeY;
		//logic2 = cardSprites[currentCard].getPosition().x > changeX;
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

void putCardsOnTable() 
{		
}

void setRightPlace() 
{
	queue<int> bufPl = PlCards;
	queue<int> bufBot = BotCards;

	int who = 0;

	for (int i = 0; i < CountOfCards; i++) 
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

	Clock clock;

	RenderWindow window(VideoMode(1920, 1080), "CardGame", Style::Fullscreen); //Посмотреть настройки max и min размера окна
	
	cout << "[LOG]: Запуск меню" << endl;
	menu(window);
	
	cout << "[LOG]: Запуск игры" << endl;

	window.setFramerateLimit(60); //нужно ли?
	
	font.loadFromFile("CyrilicOld.ttf");
	//text.setColor(Color::Red);
	txtPlCards.setStyle(sf::Text::Bold);
	txtBotCards.setStyle(sf::Text::Bold);
	txtRound.setStyle(sf::Text::Bold);

	txtPlCards.setString("Ваших карт: ");
	txtPlCards.setPosition(490, 900);
	txtBotCards.setString("Карт компьютера: ");
	txtBotCards.setPosition(420, 100);
	txtRound.setString("");
	txtRound.setPosition(250, 500);

	Texture tBackground;
	Texture tBackOfCard;

	tBackground.loadFromFile("images/GameBack.png");
	tBackOfCard.loadFromFile("images/BackOfCard.png");
	
	Sprite sBackground(tBackground);

	for (int i = 0; i < CountOfCards; i++) 
	{
		cardSprites[i].setTexture(tBackOfCard);
		cardSprites[i].setPosition(1600, 410);
	}

	for (int i = 0; i < CountOfCards; i++)
		CardStorage.push_back(i);

	int k = 0;
	for (int i = 0; i < CountOfCards / 4;i++)
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

	bool flagRight = false;

	cout << "[LOG]: Окончание подготовки" << endl;

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
				if (event.key.code == Mouse::Left && (botCount + plCount) == CountOfCards)
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
			}
		}

		if (!(botCount + plCount == CountOfCards))
		{
			firstAnim();
			flagRight = true;
		}
		else if(flagRight == true) 
		{
			setRightPlace();
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
		for (int i = 0; i < CountOfCards; i++) window.draw(cardSprites[i]);

		window.display();
	}

	cout << "Выход из программы" << endl;
	system("pause");
	return 0;
}