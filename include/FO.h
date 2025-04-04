#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
class FO
{
public:
	FO();
	FO(int flag);
	void setSButterfly(double x, double y);	//设置参数
	sf::Vector2f getJulgeArea();			//判定区
	~FO();
	

public:
	sf::Vector2f velocity;				//速度向量
	sf::Sprite hero, HSAmmo, LSAmmo;	//精灵
	sf::Texture Reimu, Marisa, Sanae, Enemy1, Enemy2;		//纹理
	int HealthPoint, Score, width, height, staticFrame, dynamicFrame, phase;
	int type, gap, born, score, bounds;
	double damage, speed, theta;
	bool isSym;
};

