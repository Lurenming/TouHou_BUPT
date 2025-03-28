#include "Game.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <list>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.1415926
#define EPS 0.00001
using namespace std;


bool isOutOfBoard(sf::Sprite value)
{
	if (value.getPosition().y >= 0 && value.getPosition().y <= 960 && value.getPosition().x >= 0 && value.getPosition().x <= 960)
	{
		return false;
	}
	return true;
}

bool isFOOutOfBoard(const FO value)
{
	if (value.hero.getPosition().y >= -100 && value.hero.getPosition().y <= 960 && value.hero.getPosition().x >= 0 && value.hero.getPosition().x <= 960)
	{
		return false;
	}
	return true;
}

Game::Game()
	:mWindow(sf::VideoMode(1280, 960), "TouHou114.0-chs")			//生成窗口
	, font()
	, player(1)
{
	loadMusicAndSounds();
	NowLoading();

	loadBackgrounds();

	srand((unsigned)time(NULL));
	remnant = 3;
	score = 0;
	clockForInvulnerability.restart();
	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingLeft = false;
	mIsMovingRight = false;
	mIsGrazing = false;
	mIsFire = false;

	loadPrimeFrame();

	loadPointsAndEffs();

	loadBluePoints();

	loadEnemy();


	if (!font.loadFromFile("./res/RobotoCondensed-Regular.ttf"))	//加载字体
	{
		puts("Error: Load RobotoCondensed-Regular.ttf failed!");
	}

	//text.setString("v0.1");
	//text.setFont(font);
	//text.setCharacterSize(50);

	

}
/*		version1   无固定加载过程，已废弃

void Game::NowLoading()
{
	//design by dthylacetate
	// Create the loading window
	if (!loading.loadFromFile("./res/sig.png"))
	{
		puts("Error: Load loading failed!");
	}
	if (!nowLoading.loadFromFile("./res/loading.png", sf::IntRect(0, 0, 128, 64)))
	{
		puts("Error: Load nowloading failed!");
	}
	loadingUI.setTexture(loading);
	loadingUISub.setTexture(nowLoading);
	loadingUISub.setScale(1.5, 1.5);
	loadingUISub.setPosition(sf::Vector2f(1000, 800));
	mWindow.clear();
	mWindow.draw(loadingUI);
	mWindow.draw(loadingUISub);
	mWindow.display();
}

*/

void Game::NowLoading()
{
    // 创建加载窗口和加载纹理
    if (!loading.loadFromFile("./res/sig.png"))
    {
        puts("Error: Load loading failed!");
    }
    if (!nowLoading.loadFromFile("./res/loading.png", sf::IntRect(0, 0, 128, 64)))
    {
        puts("Error: Load nowloading failed!");
    }
    //menuMusic.play();
    loadingUI.setTexture(loading);
    loadingUISub.setTexture(nowLoading);
    loadingUISub.setScale(1.5f, 1.5f);
    loadingUISub.setPosition(sf::Vector2f(1000, 800));
    
    // 设置渐变闪烁相关变量
    sf::Clock clock;
    float duration = 4.0f; // 加载持续时间
    float elapsedTime = 0.0f;
    
    // 渐变闪烁效果
    while (elapsedTime < duration)
    {
        elapsedTime = clock.getElapsedTime().asSeconds();
        
        // 计算透明度
        float alpha = (sin(elapsedTime * 6.0f) + 1.0f) / 2.0f * 255; // 0到255之间的值
        loadingUISub.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha))); // 设置透明度
        
        mWindow.clear();
        mWindow.draw(loadingUI);
        mWindow.draw(loadingUISub);
        mWindow.display();
        
        // 控制帧率
        sf::sleep(sf::milliseconds(16)); // 大约60FPS
    }

	 //menuMusic.stop();
}

void Game::loadBackgrounds()		//加载背景纹理
{
	if (!bg1.loadFromFile("./stage02a.png"))
	{
		puts("Error: Load stage02a failed!");
	}
	if (!bgEff1.loadFromFile("./res/stage02b7.png"))
	{
		puts("Error: Load stage02b failed!");
	}
	if (!bg2.loadFromFile("./res/stage02a.png"))
	{
		puts("Error: Load stage3bg failed!");
	}												//bg2仅仅实现了函数，未实装
	if (!bgEff2.loadFromFile("./res/stage02b.png"))
	{
		puts("Error: Load stage3bg failed!");
	}
	for (int i = 0; i < 6; i++)
	{
		back[i].setTexture(bg1);
		back[i].setScale(sf::Vector2f(1.5, 1.5));
		back[i].setPosition((float)65.0, (float)(i - 1)*192.0 + 35.0);
		backEff[i].setTexture(bgEff1);
		backEff[i].setScale(sf::Vector2f(1.5, 1.5));			//模拟滚动背景
		backEff[i].setPosition(65.0, (i - 1)*384.0 + 35.0);
	}

	/*for (int i = 0; i < 6; i++)
	{
	back[i].setTexture(bg2);
	back[i].setScale(sf::Vector2f(3.0, 3.0));
	back[i].setPosition(65.0, (i - 1)*4*192.0 + 35.0);
	backEff[i].setTexture(bgEff2);
	backEff[i].setScale(sf::Vector2f(6.0, 6.0));
	backEff[i].setPosition(65.0, (i - 1)*384.0/2.0 + 35.0);
	}*/

	if (!lifePieces.loadFromFile("./res/lifePieces.png"))
	{
		puts("Error: Load lifePieces failed!");			//生命条加载
	}
	lifeBoard.setTexture(lifePieces);

	if (!Title1.loadFromFile("./res/stg01logo.png"))
	{
		puts("Error: Load stg01logo failed!");			//logo加载
	}
	if (!Title2.loadFromFile("./res/front01.png"))
	{
		puts("Error: Load clear failed!");				//clear图片加载
	}
	if (!whiteSpark.loadFromFile("./res/White.png"))
	{
		puts("Error: Load White failed!");		
	}
}
//
void Game::loadPrimeFrame()					//把主界面的图片切割展示
{	
	if (!front00.loadFromFile("./res/front00.png"))
	{
		puts("Error: Load front failed!");
	}
	front01.setTexture(front00);
	front01.setTextureRect(sf::IntRect(65, 0, 490, 1280));
	front01.setPosition(sf::Vector2f(795, 0));
	front02.setTexture(front00);
	front02.setTextureRect(sf::IntRect(69, 1030, 730, 39));
	front02.setPosition(sf::Vector2f(65, 0));
	front03.setTexture(front00);
	front03.setTextureRect(sf::IntRect(69, 1062, 739, 38));
	front03.setPosition(sf::Vector2f(65, 922));
	front04.setTexture(front00);
	front04.setTextureRect(sf::IntRect(0, 0, 70, 1280));
	front04.setPosition(sf::Vector2f(0, 0));
}
//
void Game::loadPointsAndEffs()		//加载子弹和判定点
{
	if (!julgePointArray.loadFromFile("./res/etama2.png"))	//判定点加载
	{
		puts("Error: Load julgePointArray failed!");
	}

	julgePoint.setTexture(julgePointArray);
	julgePoint.setTextureRect(sf::IntRect(0, 16, 64, 64));
	julgePoint.setScale(1.5, 1.5);

	if (!bullets.loadFromFile("./res/etama.png"))		//弹幕加载
	{
		puts("Error: Load bullets failed!");
	}
	if (!buffetsEff.loadFromFile("./res/enemy_aura.png"))
	{
		puts("Error: Load buffetsEff failed!");
	}
	if (!deathCircle.loadFromFile("./res/eff_deadcircle.png"))		//死亡特效加载
	{
		puts("Error: Load deathCircle failed!");
	}
	if (!allBullets1.loadFromFile("./res/bullet1.png"))
	{
		puts("Error: Load bullet1 failed!");
	}
	if (!allBullets2.loadFromFile("./res/bullet2.png"))
	{
		puts("Error: Load bullet2 failed!");
	}
	if (!magicSquare.loadFromFile("./res/eff_magicsquare.png"))
	{
		puts("Error: Load magicsquare failed!");
	}
}

void Game::loadBluePoints()
{
	if (!bluePointTexture.loadFromFile("./res/ls_bluePoint.png"))
	{
		puts("Error: Load ls_bluePoint.png failed!");
	}
}

void Game::displayBluePoints()			//处理蓝点的显示和移除超出边界的蓝点
{
	//printf("len of bluePoints: %llu before remove_if\n", (unsigned long long) bluePoints.size());
	bluePoints.remove_if(isFOOutOfBoard);
	//printf("len of bluePoints: %llu after remove_if\n", (unsigned long long) bluePoints.size());

	for (list<FO>::iterator it = bluePoints.begin(); it != bluePoints.end(); it++)
	{
	    //puts("enemiesPushToDraw(it) for blue point");
		enemiesPushToDraw(it);
	}
}

void Game::setBluePointByEnemyType(list<FO>::iterator it)	//用于创建蓝点并使其下落
{
	// 不用播放音效
	FO bluePoint;
	bluePoint.speed = 4;
	bluePoint.width = 16;
	bluePoint.height = 16;
	bluePoint.hero.setTexture(bluePointTexture);
	bluePoint.hero.setTextureRect(sf::IntRect(0, 0, 16, 16));
	bluePoint.hero.setOrigin(8, 8);
	bluePoint.hero.setScale(1, 1);

	bluePoint.theta = PI / 2;

	// printf("dead enemy position: (%f, %f)\n", it->hero.getPosition().x, it->hero.getPosition().y);
	// ^^^^^^ 据此成功发现 bug 原因。
	// 原先在敌机死亡后再调用 setBluePointByEnemyType()，但死亡代码过程中会把敌机的位置移到屏幕外边去，导致蓝点也生在屏幕外面了，所以看不见蓝点。
	// 在敌机死亡前调用 setBluePoint() 即可。
	// bluePoint.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	// printf("new blue point position: (%f, %f)\n", bluePoint.hero.getPosition().x, bluePoint.hero.getPosition().y);


	switch (it->type)
	{
	case 0://title
		puts("setBluePointByEnemyType(): unexpected enemy type: title");
		break;
	case 1://player
		puts("setBluePointByEnemyType(): unexpected enemy type: player");
		break;
	case 2://sButterfly
		bluePoint.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
		bluePoints.push_back(bluePoint);
		break;
	case 3://sFairy
		bluePoint.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
		bluePoints.push_back(bluePoint);
		break;
	case 4://mFairy
		bluePoint.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
		bluePoints.push_back(bluePoint);
		break;
	case 5://mButterfly
		// 生成 3 个蓝点，位置分散一些
		for (int i = 0; i < 3; i++)
		{
			bluePoint.hero.setPosition(it->hero.getPosition().x + (rand() % 64 - 32), it->hero.getPosition().y + it->height + (rand() % 64 - 32));
			bluePoints.push_back(bluePoint);
		}
		break;
	case 6://Ghost
		// 生成 15 个蓝点，位置分散一些
		for (int i = 0; i < 15; i++)
		{
			bluePoint.hero.setPosition(it->hero.getPosition().x + (rand() % 128 - 64), it->hero.getPosition().y + it->height + (rand() % 128 - 64));
			bluePoints.push_back(bluePoint);
		}
		break;
	}

    // printf("len of bluePoints: %llu\n", (unsigned long long) bluePoints.size());
}


//
void Game::loadEnemy()
{							//加载敌人突破
	// Load a enemy to display
	if (!Enemy1.loadFromFile("./res/enemy.png"))
	{
		puts("Error: Load enemy1 failed!");
	}
	if (!Enemy2.loadFromFile("./res/enemy2.png"))
	{
		puts("Error: Load enemy2 failed!");
	}
	if (!Enemy3.loadFromFile("./res/enemy5.png"))
	{
		puts("Error: Load enemy3 failed!");
	}
}
//
void Game::loadMusicAndSounds()		//加载背景音乐和音效
{
	if (!menuMusic.openFromFile("./res/menu.wav"))
	{
		puts("Error: Open menu.wav failed!");
	}
	if (!stage1BGM.openFromFile("./res/stage1.wav"))
	{
		puts("Error: Open stage1.wav failed!");
	}
	if (!stage2BGM.openFromFile("./res/stage2.wav"))
	{
		puts("Error: Open stage2.wav failed!");
	}
	if (!stage3BGM.openFromFile("./res/stage3.wav"))
	{
		puts("Error: Open stage3.wav failed!");
	}
	menuMusic.setVolume(100);
	stage1BGM.setVolume(100);
	stage2BGM.setVolume(100);
	stage3BGM.setVolume(100);

	if (!selectSoundBuffer.loadFromFile("./res/se_select00.wav"))
	{
		puts("Error: Open se_select00.wav failed!");
	}
	selectSound.setBuffer(selectSoundBuffer);
	selectSound.setVolume(50);
	if (!playerBulletSoundBuffer.loadFromFile("./res/se_damage00.wav"))		//加载子弹音效
	{
		puts("Error: Open se_damage00.wav failed!");
	}
	playerBulletSound.setBuffer(playerBulletSoundBuffer);
	playerBulletSound.setVolume(50);
	if (!enemyBulletSoundBuffer.loadFromFile("./res/se_tan00.wav"))		//加载弹幕音效
	{
		puts("Error: Open se_tan00.wav failed!");
	}
	enemyBulletSound.setBuffer(enemyBulletSoundBuffer);
	enemyBulletSound.setVolume(15);
	if (!breakSoundBuffer.loadFromFile("./res/se_enep00.wav"))			//加载中弹音效
	{
		puts("Error: Open se_enep00.wav failed!");
	}

	bluePointCollectedSound.setBuffer(bluePointCollectedSoundBuffer);
	bluePointCollectedSound.setVolume(50);
	if (!bluePointCollectedSoundBuffer.loadFromFile("./res/se_item00.wav"))			//加载蓝点收集音效
	{
		puts("Error: Open se_item00.wav failed!");
	}


	breakSound.setBuffer(breakSoundBuffer);							//加载死亡音效
	breakSound.setVolume(50);
	if (!playerDeadSoundBuffer.loadFromFile("./res/se_pldead00.wav"))
	{
		puts("Error: Open se_pldead00.wav failed!");
	}
	playerDeadSound.setBuffer(playerDeadSoundBuffer);
	playerDeadSound.setVolume(50);
	if (!SCAnounceBuffer.loadFromFile("./res/se_cat00.wav"))		//加载SC音效
	{
		puts("Error: Open se_cat00.wav failed!");
	}
	SCAnounce.setBuffer(SCAnounceBuffer);
	SCAnounce.setVolume(50);
	if (!cardGetBuffer.loadFromFile("./res/se_cardget.wav"))		//加载收取音效
	{
		puts("Error: Open se_cardget.wav failed!");
	}
	cardGet.setBuffer(cardGetBuffer);
	cardGet.setVolume(50);
}

void Game::run()
{
	//播放主菜单界面，展示主菜单
	menu();
	int level = 1;			//关卡选择，目前仅仅实装了sg1
	switch (level)
	{
	case 1:
		stage1BGM.play();
		stage1BGM.setLoop(true);
		break;
	}
	//游戏进行的主循环
	mWindow.setFramerateLimit(60);
	mWindow.draw(player.hero);			//更新人物位置
	frameDisplay();
	/*HANDLE hThread_1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BGMPlay, self, 0, NULL);*/
	while (mWindow.isOpen())
	{
		mWindow.clear();
		
		switch (level)
		{
		case 1:
			Stage1();
			break;
		}
		processTaps(); 
		mainProcessing();
		
		frameDisplay();
	}
}

void Game::menu()
{
	sf::Text textStart("start", font, 50);
	sf::Text textExStart("extra start", font, 50);
	sf::Text textOptions("options", font, 50);
	sf::Text textQuit("Quit", font, 50);
	sf::Text text5G("Use 5G", font, 50);
	if (!title.loadFromFile("./res/title.png"))
	{
		puts("Error: Load title failed!");
	}
	titleBackground.setTexture(title);
	menuMusic.play();
	menuMusic.setLoop(true);
	// 设置选项的位置
	textStart.setPosition(100, 560);
	textExStart.setPosition(110, 630);
	textOptions.setPosition(120, 700);
	textQuit.setPosition(130, 770);
	text5G.setPosition(140, 840);
	int selectedItem = 0;		// 记录当前选项，默认为textStart
	// 主菜单主循环
	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mWindow.close();
				return;
			}
			// 读取上下方向键切换选项
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					selectSound.play();
					selectedItem = (selectedItem - 1 + 4) % 4;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					selectSound.play();
					selectedItem = (selectedItem + 1) % 4;
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					selectSound.play();
					//Quit
					if (selectedItem == 2)
					{
						mWindow.close();
						return;
					}
					//Start
					else if (selectedItem == 0)
					{
						menuMusic.stop();
						return;
					}
					//Options
					else if (selectedItem == 1)
					{
						printf("Options\n");
						options();
					}
					else if (selectedItem == 3)
					{
						printf("5G\n");
					}
				}
				else if (event.key.code == sf::Keyboard::X)
				{
					// 模拟官作主菜单中的“按X跳转到Quit”操作
					selectSound.play();
					if (selectedItem == 2)
					{
						mWindow.close();
						return;
					}
					else
					{
						selectedItem = 2;
					}
				}
			}
		}
		// 选中时高亮
		textStart.setFillColor(sf::Color(166, 166, 166));
		textExStart.setFillColor(selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);
		textOptions.setFillColor(selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
		textQuit.setFillColor(selectedItem == 2 ? sf::Color::Yellow : sf::Color::White);
		text5G.setFillColor(selectedItem == 3 ? sf::Color::Yellow : sf::Color::White);
		mWindow.draw(titleBackground);
		mWindow.draw(textStart);
		mWindow.draw(textExStart);
		mWindow.draw(textOptions);
		mWindow.draw(textQuit);
		mWindow.draw(text5G);

		mWindow.display();
	}
}

void Game::loadOptionsUI()
{
	if (!optionsBg.loadFromFile("./res/options.png"))
	{
		puts("Error: Load options background failed.");
	}
	if (!optionsTitle.loadFromFile("./res/optionsTitle.png"))
	{
		puts("Error: Load options title failed.");
	}
	optionsBackground.setTexture(optionsBg);
	optionsTitleUI.setTexture(optionsTitle);
}

void Game::options()
{
	// 音量和音效的值
	static int volume = 10;
	static int sfx = 10;
	// 调整残机和雷（雷没有实现）
	static int lifeDisplay = remnant;
	static int bombDisplay = 2;

	int selectedItem = 0;
	loadOptionsUI();
	optionsTitleUI.setScale(4, 4);
	optionsTitleUI.setPosition(240, 50);

	// UI（文本）
	sf::Text textLife("", font, 50);
	textLife.setPosition(200, 400);

	sf::Text textBomb("", font, 50);
	textBomb.setPosition(210, 460);

	sf::Text textVolume("", font, 50);
	textVolume.setPosition(220, 520);

	sf::Text textSFX("", font, 50);
	textSFX.setPosition(230, 580);

	sf::Text textBack("Return to main menu", font, 50);
	textBack.setPosition(240, 640);
	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mWindow.close();
				return;
			}
			if(event.type == sf::Event::KeyPressed)
			{
				// 切换选项
				if (event.key.code == sf::Keyboard::Up)
				{
					selectSound.play();
					selectedItem = (selectedItem - 1 + 5) % 5;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					selectSound.play();
					selectedItem = (selectedItem + 1) % 5;
				}
				// 调整音量和音效
				else if (event.key.code == sf::Keyboard::Left)
				{
					if (selectedItem == 2)  // 音量调小
					{
						if (volume > 0)
							volume--;
						selectSound.play();
						menuMusic.setVolume(volume * 10);
						stage1BGM.setVolume(volume * 10);
						stage2BGM.setVolume(volume * 10);
						stage3BGM.setVolume(volume * 10);
					}
					else if (selectedItem == 3)  // 音效调小
					{
						if (sfx > 0)
							sfx--;
						selectSound.setVolume(sfx * 5);
						selectSound.play();
						playerBulletSound.setVolume(sfx * 5);
						enemyBulletSound.setVolume(sfx * 1.5);
						bluePointCollectedSound.setVolume(sfx * 5);
						breakSound.setVolume(sfx * 5);
						playerDeadSound.setVolume(sfx * 5);
						SCAnounce.setVolume(sfx * 5);
						cardGet.setVolume(sfx * 5);
					}
					else if (selectedItem == 0)  // 调整残机
					{
						if (lifeDisplay > 0)
						{
							lifeDisplay--;
							remnant = lifeDisplay;
							printf("%lld\n", remnant);
						}
					}
					else if (selectedItem == 1)  // 调整雷
					{
						if (bombDisplay > 0)
						{
							bombDisplay--;
						}
					}
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					if (selectedItem == 2)  // 音量调大
					{
						if (volume < 10)
							volume++;
						selectSound.play();
						menuMusic.setVolume(volume * 10);
						stage1BGM.setVolume(volume * 10);
						stage2BGM.setVolume(volume * 10);
						stage3BGM.setVolume(volume * 10);
					}
					else if (selectedItem == 3)  // 音效调大
					{
						if (sfx < 10)
							sfx++;
						selectSound.setVolume(sfx * 5);
						selectSound.play();
						playerBulletSound.setVolume(sfx * 5);
						enemyBulletSound.setVolume(sfx * 1.5);
						bluePointCollectedSound.setVolume(sfx * 5);
						breakSound.setVolume(sfx * 5);
						playerDeadSound.setVolume(sfx * 5);
						SCAnounce.setVolume(sfx * 5);
						cardGet.setVolume(sfx * 5);
					}
					else if (selectedItem == 0)  // 调整残机
					{
						if (lifeDisplay < MAX_LIFE)
						{
							lifeDisplay++;
							remnant = lifeDisplay;
							printf("%lld\n", remnant);
						}
					}
					else if (selectedItem == 1)  // 调整雷
					{
						if (bombDisplay < MAX_BOMB)
						{
							bombDisplay++;
						}
					}
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					if (selectedItem == 4)
					{
						selectSound.play();
						return;
					}
				}
				// 按X退出 
				else if (event.key.code == sf::Keyboard::X)
				{
					if (selectedItem == 4)
					{
						selectSound.play();
						return;
					}
					else
					{
						selectedItem = 4;
					}
				}
			}
		}
		// 更新显示文本
		textVolume.setString("Music: " + std::to_string(volume));
		textSFX.setString("SFX: " + std::to_string(sfx));
		textLife.setString("Life: " + std::to_string(lifeDisplay));
		textBomb.setString("Bomb: " + std::to_string(bombDisplay));

		// 选中时高亮
		textLife.setFillColor(selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);
		textBomb.setFillColor(selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
		textVolume.setFillColor(selectedItem == 2 ? sf::Color::Yellow : sf::Color::White);
		textSFX.setFillColor(selectedItem == 3 ? sf::Color::Yellow : sf::Color::White);
		textBack.setFillColor(selectedItem == 4 ? sf::Color::Yellow : sf::Color::White);

		mWindow.clear();
		mWindow.draw(optionsBackground);
		mWindow.draw(optionsTitleUI);
		mWindow.draw(textVolume);
		mWindow.draw(textSFX);
		mWindow.draw(textBack);
		mWindow.draw(textLife);
		mWindow.draw(textBomb);
		mWindow.display();
	}
}

void Game::Stage1()
{
	static sf::Time elapsed1 = clock.restart();		//游戏帧重置
	elapsed1 = clock.getElapsedTime();
	
	static int evts[20] = { 0 };

	static int curTime = 1;
	if (curTime < elapsed1.asSeconds())
	{
		printf("%.0f\n", elapsed1.asSeconds());
		curTime++;
	}

	switch ((int)elapsed1.asSeconds() + 0)//test用，所有敌人生成都是以时间来判断
	{
	case 1:
		//pre
		evts[1] = 1;
		break;
	case 12:
		//title
		evts[2] = 1;
		break;
	case 17:
		evts[3] = 1;
		break;
	case 23:
		evts[4] = 1;
		break;
	case 29:
		evts[5] = 1;
		break;
	case 36:
		//wave
		evts[6] = 1;
		break;
	case 42:
		evts[7] = 1;
		break;
	case 49:
		//middle
		evts[8] = 1;
		break;
	case 75:
		//reinforce
		evts[9] = 1;
		break;
	case 81:
		evts[10] = 1;
		break;
	case 90:
		evts[11] = 1;
		break;
	case 100:
		//切换场景至测试2面
		evts[12] = 1;
		break;
	case 105:
		//boss
		evts[13] = 1;
		break;
	}

	if (evts[1])
	{
		if (S1E1())
		{
			evts[1] = 0;
		}
	}
	if (evts[2])
	{
		if (S1E2())
		{
			evts[2] = 0;
		}
	}
	if (evts[3])
	{
		if (S1E3())
		{
			evts[3] = 0;
		}
	}
	if (evts[4])
	{
		if (S1E4())
		{
			evts[4] = 0;
		}
	}
	if (evts[5])
	{
		if (S1E5())
		{
			evts[5] = 0;
		}
	}
	if (evts[6])
	{
		if (S1E6())
		{
			evts[6] = 0;
		}
	}
	if (evts[7])
	{
		if (S1E7())
		{
			evts[7] = 0;
		}
	}
	if (evts[8])
	{
		if (S1E8())
		{
			evts[8] = 0;
		}
	}
	if (evts[9] && !evts[8])
	{
		if (S1E9())
		{
			evts[9] = 0;
		}
	}
	if (evts[10])
	{
		if (S1E10())
		{
			evts[10] = 0;
		}
	}
	if (evts[11])
	{
		if (S1E11())
		{
			evts[11] = 0;
		}
	}
	if (evts[12])
	{
		if (S1E12())
		{
			evts[12] = 0;
		}
	}
	if (evts[13])
	{
		if (S1E13())
		{
			evts[13] = 0;
			evts[14] = 1;
		}
	}
	if (evts[14])
	{
		if (S1E14())
		{
			evts[14] = 0;
		}
	}
}
//生成敌人的函数   S表示关卡，E指代波数，比较重复的代码不作注释，任何行为都是基于游戏帧来执行
int Game::S1E1()
{
	static int i1 = 0;			//记录帧
	i1++;
	static list<FO> wave1, wave2;		//用于存放敌机对象的链表
	double gapTime = 0.4;				//生成时间间隔
	int gapFrame = gapTime * 60;		//生成帧间隔
	static int gap = 0, temp = 0;		//gap用来区分每个敌人生成的时间，确保分开生成

	if (i1 % gapFrame == 1 && i1 < 15 * gapFrame)
	{
		FO sButterfly(2);						//生成第一类小怪
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(400 + pow(-1.0, i1 / gapFrame) * 0.8 * i1, 20.0));		//根据当前时间帧计算敌人的位置（左右交替）
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);
		wave1.push_back(sButterfly);
	}
	if (i1 == 270)
	{								//生成第二类敌人，大蝴蝶
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.hero.setPosition(sf::Vector2f(400 + 200, -20.0));
		mButterfly.born = i1;
		wave2.push_back(mButterfly);
		mButterfly.hero.setPosition(sf::Vector2f(400 - 200, -20.0));
		wave2.push_back(mButterfly);
	}

	wave1.remove_if(isFOOutOfBoard);		//第一类敌人的行为
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		
		temp = i1 - it->gap * gapFrame;
		if (temp < 200)//phase1			//正常是向下移动，偶尔发射子弹
		{
			if (rand() % 20 == 0)
			{
				setMultiRoundSnipe(it, 5.0, 5);
			}
			it->speed = 50.0 / (temp + 1.0);
			it->theta = 0.5*PI;
			it->hero.setTextureRect(sf::IntRect(i1 % 35 / 7 * it->width, 320, it->width, it->height));
		}
		else//phase2				//时间过了，小怪准备离场
		{
			it->speed = (temp - 200) / 10.0;
			it->theta = 0.5 * PI + pow(-1.0, it->gap + 1.0) * 10.0*PI / 360.0;

			if (it->speed <= 11)
			{
				it->hero.setTextureRect(sf::IntRect((int)it->speed * it->width, 320, it->width, it->height));
			}
			else
			{
				it->hero.setTextureRect(sf::IntRect((int)11 * it->width, 320, it->width, it->height));
			}
			if (it->theta > PI / 2.0)
			{
				it->hero.setScale(-1.5, 1.5);

				if (!it->isSym)
				{
					it->hero.move(1.5*it->width, 0);
					it->isSym = true;
				}
			}
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	wave2.remove_if(isFOOutOfBoard);		//第二类敌人的行为
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)
	{					//向下随机角度发射子弹
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 400)							
		{
			it->speed = (650.0 - i1) / 240;
			it->theta = 0.5 * PI;		
		}
		else if (i1 >= 400 && i1 < 500)
		{
			it->speed = 0.0;
			
			setSharpRandom(it, 3.0);
			setSharpRandom(it, 3.0);
			
		}
		else
		{
			it->speed = (i1 - 500) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 15 * 60)			//关卡结束判断
	{
		wave1.clear();//Final clear for accident
		wave2.clear();
		return 1;
	}				
	return 0;
}

int Game::S1E2()			//E2比较特殊，这里涉及一个小过场
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1;

	if (i1 == 1)
	{
		FO mainTitle(0);
		mainTitle.hero.setTexture(Title1);		//加载过场logo
		mainTitle.hero.setTextureRect(sf::IntRect(0, 0, 512, 256));
		//mainTitle.hero.setScale(sf::Vector2f(1.5, 1.5));
		mainTitle.speed = 0.0;
		mainTitle.hero.setPosition(sf::Vector2f(200.0, 150.0));
		mainTitle.hero.setColor(sf::Color(255, 255, 255, 0));

		wave1.push_back(mainTitle);
	}

	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		it->hero.setColor(sf::Color(255, 255, 255, -i1*(i1-301) / (151.0*150.0) * 255));
		enemiesPushToDraw(it);			//实现logo的渐变出现
	}
	if (i1 > 5 * 60)
	{
		wave1.clear();			//动画结束检测的
		return 1;
	}
	return 0;
}

int Game::S1E3()
{				//第一串小怪
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2;
	double gapTime = 0.2;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;
	if (i1 % gapFrame == 1 && i1 < 15 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(600.0, 20.0));
		sButterfly.speed = 3.0;
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;

		wave1.push_back(sButterfly);
	}
	if (i1 == 21)
	{				//大蝴蝶
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.born = i1;
		mButterfly.hero.setPosition(sf::Vector2f(200.0, -20.0));
		wave2.push_back(mButterfly);
	}

	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;

		if (rand() % 5 == 0)
		{
			setRoundRandom(it, 4.0, 5, 60, 90);//setMultiRoundSnipe(it, 8.0, 5);
		}
		it->theta = 0.5*PI + temp * temp / 20000.0 * 0.2 * PI;
		if (it->theta >= 0.9 * PI)
		{
			it->theta = 0.9 * PI;
		}
		if (it->theta <= 0.7 * PI)
		{
			it->hero.setTextureRect(sf::IntRect((int)(it->theta * 10.0 / PI) * it->width, 320, it->width, it->height));
		}
		else
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp % 4 + 8) * it->width, 320, it->width, it->height));
		}

		if (it->theta > PI / 2.0)
		{
			it->hero.setScale(-1.5, 1.5);
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	wave2.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)
	{
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 150)
		{
			it->speed = (400.0 - i1) / 240;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 150 && i1 < 250)
		{
			it->speed = 0.0;
			for (int i = 0; i < 4; i++)
			{
				setSharpRandom(it, 3.0);
			}
		}
		else
		{
			it->speed = (i1 - 250) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 10 * 60)
	{
		wave1.clear();//Final clear for accident
		wave2.clear();
		return 1;
	}
	return 0;
}

int Game::S1E4()
{				//三波串怪
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2, wave3;
	double gapTime = 0.2;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;

	if (i1 % gapFrame == 1 && i1 < 10 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(700.0, 960.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave1.push_back(sButterfly);
	}
	if (i1 % gapFrame == 1 && (i1 - 3 * 60) < 10 * gapFrame && (i1 - 3 * 60) > 0)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320 + 3 * sButterfly.height, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(200.0, 20.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave2.push_back(sButterfly);
	}
	if (i1 % gapFrame == 1 && (i1 - 3 * 60) < 10 * gapFrame && (i1 - 3 * 60) > 0)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(200.0, 960.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave3.push_back(sButterfly);
	}

	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;

		if (rand() % 10 == 0 && it->hero.getPosition().y < 400)
		{
			setRandom(it, 4.0, 1, 5, 60, 90);  //setRoundRandom(it, 4.0, 5, 60, 90);//setMultiRoundSnipe(it, 8.0, 5);
		}

		if (it->hero.getPosition().y >= 400)
		{
			it->theta = -0.5*PI;
		}
		else if (it->hero.getPosition().y < 400)
		{
			it->theta -= (0.5 * PI / 180.0);
		}
		
		if (it->theta >= -0.7 * PI && it->hero.getPosition().y >= 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4) * it->width, 320, it->width, it->height));
		}
		else if (it->theta >= -0.7 * PI && it->hero.getPosition().y < 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 32 % 4 + 3) * it->width, 320, it->width, it->height));
		}
		else
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4 + 8) * it->width, 320, it->width, it->height));
		}

		if (it->theta < PI / 2.0)
		{
			it->hero.setScale(-1.5, 1.5);
		}
		//it->theta = -it->theta;

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	wave2.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;

		if (rand() % 20 == 0)
		{
			setRandom(it, 4.0, 1, 5, 60, 90); //setRoundRandom(it, 4.0, 5, 60, 90);//setMultiRoundSnipe(it, 8.0, 5);
		}
		it->theta = 0.5*PI - temp * temp / 20000.0 * 0.2 * PI;
		if (it->theta <= 0.1 * PI)
		{
			it->theta = 0.1 * PI;
		}
		if (it->theta >= 0.3 * PI)
		{
			it->hero.setTextureRect(sf::IntRect((int)(it->theta * 10.0 / PI) * it->width, 320 + 3 * it->height, it->width, it->height));
		}
		else
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp % 4 + 8) * it->width, 320 + 3 * it->height, it->width, it->height));
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	wave3.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave3.begin(); it != wave3.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;

		if (rand() % 10 == 0 && it->hero.getPosition().y < 400)
		{
			setRandom(it, 4.0, 1, 5, 60, 90); //setRoundRandom(it, 4.0, 5, 60, 90);//setMultiRoundSnipe(it, 8.0, 5);
		}

		if (it->hero.getPosition().y >= 400)
		{
			it->theta = -0.5*PI;
		}
		else if (it->hero.getPosition().y < 400)
		{
			it->theta += (0.5 * PI / 180.0);
		}

		if (it->theta >= -0.3 * PI && it->hero.getPosition().y >= 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4) * it->width, 320 + 2 * it->height, it->width, it->height));
		}
		else if (it->theta >= -0.3 * PI && it->hero.getPosition().y < 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4 + 8) * it->width, 320 + 2 * it->height, it->width, it->height));
		}
		else
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 32 % 4 + 3) * it->width, 320 + 2 * it->height, it->width, it->height));
		}



		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 15 * 60)
	{
		wave1.clear();		//时间到了小怪跑路，到屏幕外然后清除
		wave2.clear();
		wave3.clear();
		return 1;
	}
	return 0;
}

int Game::S1E5()//mButter quit anime dierction
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2, wave3, wave4;
	double gapTime = 0.2;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;

	if (i1 == 1)
	{
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.born = i1;
		mButterfly.hero.setPosition(sf::Vector2f(250.0, -20.0));
		wave1.push_back(mButterfly);
	}
	if (i1 == 41)
	{
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.born = i1;
		mButterfly.hero.setPosition(sf::Vector2f(600.0, -20.0));
		mButterfly.HealthPoint = 60;
		wave2.push_back(mButterfly);
	}
	if (i1 % gapFrame == 1 && i1 < 10 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(700.0, 960.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave3.push_back(sButterfly);
	}
	if (i1 % gapFrame == 1 && i1 < 10 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320 + sButterfly.height, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(150.0, 960.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave4.push_back(sButterfly);
	}

	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 80)
		{
			it->speed = (80 - i1) / 8.0;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 80 && i1 < 250)
		{
			it->speed = 0.0;
			for (int i = 0; i < 1; i++)
			{
				setSharpRandom(it, 3.0);
			}
		}
		else
		{
			it->speed = (i1 - 250) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	wave2.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)
	{
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 120)
		{
			it->speed = (120 - i1) / 8.0;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 120 && i1 < 290)
		{
			it->speed = 0.0;
			for (int i = 0; i < 1; i++)
			{
				setSharpRandom(it, 3.0);
			}
		}
		else
		{
			it->speed = (i1 - 290) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	wave3.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave3.begin(); it != wave3.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;

		if (rand() % 10 == 0 && it->hero.getPosition().y < 400)
		{
			setRandom(it, 4.0, 1, 5, 60, 90);  //setRoundRandom(it, 4.0, 5, 60, 90);//setMultiRoundSnipe(it, 8.0, 5);
		}

		if (it->hero.getPosition().y >= 400)
		{
			it->theta = -0.5*PI;
		}
		else if (it->hero.getPosition().y < 400)
		{
			it->theta -= (0.5 * PI / 180.0);
		}

		if (it->theta >= -0.7 * PI && it->hero.getPosition().y >= 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)fabs(temp / 8 % 4 - 1) * it->width, 320, it->width, it->height));
		}
		else if (it->theta >= -0.7 * PI && it->hero.getPosition().y < 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 32 % 4 + 3) * it->width, 320, it->width, it->height));
		}
		else
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4 + 8) * it->width, 320, it->width, it->height));
		}

		if (it->theta < PI / 2.0)
		{
			it->hero.setScale(-1.5, 1.5);
		}
		//it->theta = -it->theta;

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	wave4.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave4.begin(); it != wave4.end(); it++)
	{

		temp = i1 - it->gap * gapFrame + 32;

		if (rand() % 30 == 0 && it->hero.getPosition().y < 400)
		{
			setRandom(it, 8.0, 1, 5, 60, 90); //setRoundRandom(it, 4.0, 5, 60, 90);//setMultiRoundSnipe(it, 8.0, 5);
		}

		if (it->hero.getPosition().y >= 400)
		{
			it->theta = -0.5*PI;
		}
		else if (it->hero.getPosition().y < 400)
		{
			it->theta += (0.5 * PI / 180.0);
		}

		if (it->theta >= -0.3 * PI && it->hero.getPosition().y >= 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4) * it->width, 320 + 2 * it->height, it->width, it->height));
		}
		else if (it->theta >= -0.3 * PI && it->hero.getPosition().y < 400)
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4 + 8) * it->width, 320 + 2 * it->height, it->width, it->height));
		}
		else
		{
			it->hero.setTextureRect(sf::IntRect((int)(temp / 32 % 4 + 3) * it->width, 320 + 2 * it->height, it->width, it->height));
		}
		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 15 * 60)
	{
		wave1.clear();//Final clear for accident
		wave2.clear();
		return 1;
	}
	return 0;
}

int Game::S1E6()
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2, wave3, wave4;
	double gapTime = 0.2;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;
	
	if (i1 == 41)
	{
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.born = i1;
		mButterfly.hero.setPosition(sf::Vector2f(250.0, -20.0));
		wave1.push_back(mButterfly);
	}
	if (i1 == 1)
	{
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.born = i1;
		mButterfly.hero.setPosition(sf::Vector2f(600.0, -20.0));
		wave2.push_back(mButterfly);
	}
	if (i1 % gapFrame == 1 && i1 < 7 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setOrigin(sButterfly.width / 2, sButterfly.height / 2);
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(800.0, 660.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave3.push_back(sButterfly);
	}
	if (i1 % gapFrame == 1 && i1 < 7 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320 + sButterfly.height, sButterfly.width, sButterfly.height));
		sButterfly.hero.setOrigin(sButterfly.width / 2, sButterfly.height / 2);
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(0.0, 660.0));
		sButterfly.speed = 3.0;
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave4.push_back(sButterfly);
	}

	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 120)
		{
			it->speed = (120 - i1) / 8.0;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 120 && i1 < 290)
		{
			it->speed = 0.0;
			for (int i = 0; i < 1; i++)
			{
				setSharpRandom(it, 3.0);
			}
		}
		else
		{
			it->speed = (i1 - 290) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	wave2.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)
	{
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 80)
		{
			it->speed = (80 - i1) / 8.0;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 80 && i1 < 250)
		{
			it->speed = 0.0;
			for (int i = 0; i < 1; i++)
			{
				setSharpRandom(it, 3.0);
			}
		}
		else
		{
			it->speed = (i1 - 250) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	wave3.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave3.begin(); it != wave3.end(); it++)
	{
		temp = i1 - it->gap * gapFrame;

		if (rand() % 25 == 0 && it->hero.getPosition().y < 400)
		{
			setMultiRoundSnipe(it, 4.0, 5);
		}

		if (it->hero.getPosition().x >= 650 && it->hero.getPosition().y < 500)
		{
			it->theta = 0;
		}
		else if (it->hero.getPosition().x >= 650 && it->hero.getPosition().y > 500)
		{
			it->theta = PI;
		}
		else
		{
			it->theta += (0.5 * PI / 100.0);
		}

		standardSButterflyFrame(it, temp);

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	wave4.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave4.begin(); it != wave4.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;

		if (rand() % 25 == 0 && it->hero.getPosition().y < 400)
		{
			setMultiRoundSnipe(it, 4.0, 5);
		}

		if (it->hero.getPosition().x <= 150 && it->hero.getPosition().y > 500)
		{
			it->theta = 0;
		}
		else if (it->hero.getPosition().x <= 150 && it->hero.getPosition().y < 500)
		{
			it->theta = PI;
		}
		else
		{
			it->theta -= (0.5 * PI / 100.0);
		}

		standardSButterflyFrame(it, temp);

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 15 * 60)
	{
		wave1.clear();//Final clear for accident
		wave2.clear();
		wave3.clear();
		wave4.clear();
		return 1;
	}
	return 0;
}

int Game::S1E7()
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1;
	
	if (i1 == 1)
	{
		FO mButterflt(5);
		mButterflt.hero.setTexture(Enemy1);
		mButterflt.hero.setTextureRect(sf::IntRect(0, 448, mButterflt.width, mButterflt.height));
		mButterflt.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterflt.hero.setOrigin(mButterflt.width*0.5, mButterflt.height*0.5 - 24);
		mButterflt.born = i1;
		mButterflt.hero.setPosition(sf::Vector2f(450.0, -20.0));
		mButterflt.HealthPoint *= 4;
		mButterflt.score *= 4;
		wave1.push_back(mButterflt);
	}
	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		it->hero.setTextureRect(sf::IntRect(i1 % 50 / 10 * it->width, 448, it->width, it->height));
		if (i1 < 80)
		{
			it->speed = (80 - i1) / 16.0;			//非常炫酷的对称小开花弹幕
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 80 && i1 < 400)
		{
			it->speed = 0.0;
			
			setSharpFlower1(it, 4.0, 4, 0);
			
		}
		else
		{
			it->speed = (i1 - 400) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	if (i1 > 10 * 60)
	{
		wave1.clear();
		return 1;
	}
	return 0;
}

int Game::S1E8()				//这一波是第一面的结束，生成一个幽灵作为boss
{
	static int i1 = 0, stp = 0;
	i1++;
	static list<FO> wave1, wave2;	//wave1存储幽灵本体，wave2存储一个绕着转的背景魔法阵

	if (i1 == 1)
	{						//生成
		FO ghost(6);
		ghost.hero.setTexture(Enemy2);
		ghost.hero.setTextureRect(sf::IntRect(0, 64, ghost.width, ghost.height));
		ghost.hero.setScale(sf::Vector2f(1.5, 1.5));
		ghost.hero.setOrigin(ghost.width*0.5, ghost.height*0.5 - 24);
		ghost.born = i1;
		ghost.hero.setPosition(sf::Vector2f(450.0, -20.0));
		FO spellBoard;
		spellBoard.hero.setTexture(magicSquare);
		spellBoard.hero.setOrigin(128, 128);
		spellBoard.hero.setPosition(ghost.hero.getPosition());
		spellBoard.theta = 0;
		spellBoard.hero.setColor(sf::Color(255, 255, 255, 127));
		wave1.push_back(ghost);
		wave2.push_back(spellBoard);
	}
	if (wave1.size() == 0)
	{				//判断是否击破，击破之后魔法阵清除
		wave2.clear();
		return 1;
	}
	wave2.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)		//控制魔法阵的旋转效果
	{
		it->hero.setPosition(wave1.begin()->hero.getPosition().x, wave1.begin()->hero.getPosition().y + 32);
		it->theta += PI / 100.0;
		it->hero.setRotation(it->theta / PI * 180.0 + 90);
		backEsPushToDraw(it);
	}
	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		if (it->phase == 1 && stp == 0)
		{
			stp = 1;
			i1 = 80;
		}
		it->hero.setTextureRect(sf::IntRect(i1 % 80 / 10 * it->width, 64, it->width, it->height));
		if (i1 < 80)
		{
			it->speed = (80 - i1) / 16.0;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 80 && i1 < 1200)				//处理boss的攻击行为处于哪个阶段
		{
			it->speed = 0.0;
			switch (it->phase)
			{
			case 2:
				nonSpellCard1(it);		//1非
				break;
			case 1:
				if (i1 == 141)
				{
					SCAnounce.play();
				}
				if (i1 > 3 * 60)
				{
					spellCard1(it);		//1符
				}
				break;
			}
			

		}
		else
		{
			it->speed = (i1 - 1200) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}
	if (i1 > 60 * 60)
	{
		wave1.clear();			//到点跑路
		return 1;
	}
	return 0;
}

int Game::S1E9()				//随机大蝴蝶，类似春终米粒弹，实现封位效果
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2;
	double gapTime = 0.2;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;

	if (i1 % gapFrame == 1 && i1 < 30 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(rand() * 30 % 650 + 100, 20.0));
		//sButterfly.setSButterfly(400 + pow(-1.0, i1 / gapFrame) * i1, 20.0);
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;
		//printf("Now %x\n", sButterfly.hero);

		wave1.push_back(sButterfly);
	}
	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;
		if (temp < 200)					//phase1
		{
			if (rand() % 45 == 0)
			{
				setMultiRoundSnipe(it, 5.0, 5);
			}
			it->speed = 50.0 / (temp + 1.0);
			it->theta = 0.5*PI;
			it->hero.setTextureRect(sf::IntRect(i1 % 35 / 7 * it->width, 320, it->width, it->height));
		}
		else					//phase2
		{
			it->speed = (temp - 200) / 10.0;
			it->theta = 0.5 * PI + pow(-1.0, it->gap + 1.0) * 10.0*PI / 360.0;

			if (it->speed <= 11)
			{
				it->hero.setTextureRect(sf::IntRect((int)it->speed * it->width, 320, it->width, it->height));
			}
			else
			{
				it->hero.setTextureRect(sf::IntRect((int)11 * it->width, 320, it->width, it->height));
			}
			if (it->theta > PI / 2.0)
			{
				it->hero.setScale(-1.5, 1.5);

				if (!it->isSym)
				{
					it->hero.move(1.5*it->width, 0);
					it->isSym = true;
				}
			}
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 10 * 60)
	{
		wave1.clear();
		wave2.clear();
		return 1;
	}
	return 0;
}

int Game::S1E10()					
{						
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2;
	double gapTime = 1.8;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;

	if (i1 % gapFrame == 1 && i1 < 5 * gapFrame)
	{
		FO mButterfly(5);
		mButterfly.hero.setTexture(Enemy1);
		mButterfly.hero.setTextureRect(sf::IntRect(0, 448, mButterfly.width, mButterfly.height));
		mButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		mButterfly.hero.setOrigin(mButterfly.width*0.5, mButterfly.height*0.5 - 24);
		mButterfly.hero.setPosition(sf::Vector2f(450 + pow(-1, gap)*200, -20.0));
		mButterfly.born = i1;
		mButterfly.gap = gap;
		wave1.push_back(mButterfly);
		gap++;
	}
	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;
		if (temp < 120)			//phase1
		{
			it->speed = 70.0 / (temp + 1.0);
			it->theta = 0.5*PI;
		}
		else				//phase2
		{
			if (temp < 220)
			{
				it->speed = (temp - 120) / 20.0;
			}
			else
			{
				it->speed = 5;
			}
			if (it->gap % 2 == 0)
			{
				it->theta = PI;
			}
			else
			{
				it->theta = 0;
			}
			if (temp % 6 == 0)
			{
				setSharpLine(it, 5.0);
			}
		}
		standardMButterflyFrame(it, temp);

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 20 * 60)
	{
		wave1.clear();
		wave2.clear();
		return 1;
	}
	return 0;
}

int Game::S1E11()			//顶端大量生成小怪，一阶段下落，二阶段随机跑路
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1, wave2;
	double gapTime = 0.1;
	int gapFrame = gapTime * 60;
	static int gap = 0, temp = 0;

	if (i1 % gapFrame == 1 && i1 < 60 * gapFrame)
	{
		FO sButterfly(2);
		sButterfly.hero.setTexture(Enemy1);
		sButterfly.hero.setTextureRect(sf::IntRect(0, 320, sButterfly.width, sButterfly.height));
		sButterfly.hero.setScale(sf::Vector2f(1.5, 1.5));
		sButterfly.hero.setPosition(sf::Vector2f(rand() * 30 % 650 + 100, 20.0));
		sButterfly.born = i1;
		sButterfly.gap = gap;
		gap++;

		wave1.push_back(sButterfly);
	}
	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{

		temp = i1 - it->gap * gapFrame;
		if (temp < 200)//phase1
		{
			if (rand() % 90 == 0)
			{
				setRoundRandom(it, 6.0, 5, 0, PI);
			}
			it->speed = 50.0 / (temp + 1.0);
			it->theta = 0.5*PI;
			it->hero.setTextureRect(sf::IntRect(i1 % 35 / 7 * it->width, 320, it->width, it->height));
		}
		else//phase2
		{
			it->speed = (temp - 200) / 10.0;
			it->theta = 0.5 * PI + pow(-1.0, it->gap + 1.0) * 10.0*PI / 360.0;

			if (it->speed <= 11)
			{
				it->hero.setTextureRect(sf::IntRect((int)it->speed * it->width, 320, it->width, it->height));
			}
			else
			{
				it->hero.setTextureRect(sf::IntRect((int)11 * it->width, 320, it->width, it->height));
			}
			if (it->theta > PI / 2.0)
			{
				it->hero.setScale(-1.5, 1.5);

				if (!it->isSym)
				{
					it->hero.move(1.5*it->width, 0);
					it->isSym = true;
				}
			}
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);
	}

	if (i1 > 15 * 60)
	{
		wave1.clear();//Final clear for accident
		wave2.clear();
		return 1;
	}
	return 0;
}

int Game::S1E12()			//第一面结束，进入第二面，但方便测试可以看作都是第一面
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1;

	if (i1 == 1)
	{
		FO mainTitle(0);
		mainTitle.hero.setTexture(whiteSpark);
		mainTitle.hero.setTextureRect(sf::IntRect(0, 0, 1280, 960));
		mainTitle.speed = 0.0;
		mainTitle.hero.setPosition(sf::Vector2f(0, 0));
		mainTitle.hero.setColor(sf::Color(255, 255, 255, 0));

		wave1.push_back(mainTitle);
	}

	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		double alpha = -i1*(i1 - 301) / (151.0*150.0) * 255;
		it->hero.setColor(sf::Color(255, 255, 255, alpha));
		if (i1 < 151)
		{
			stage1BGM.setVolume((255 - alpha) / 255.0*100.0);			//第一面的bgm渐变降低
			if (alpha > 240)
			{
				stage1BGM.pause();
				for (int i = 0; i < 6; i++)
				{
					back[i].setTexture(bg2);					//切换背景至第二面
					back[i].setScale(sf::Vector2f(1.5, 1.5));
					back[i].setPosition((float)65.0, (float)(i - 1)*192.0 + 35.0);
					backEff[i].setTexture(bgEff2);
					backEff[i].setScale(sf::Vector2f(1.5, 1.5));
					backEff[i].setPosition(65.0, (i - 1)*384.0 + 35.0);
				}
			}
		}
		enemiesPushToDraw(it);
	}
	if (i1 == 200)
	{
		stage2BGM.play();
		stage2BGM.setLoop(true);		//第二面的bgm
	}
	if (i1 > 5 * 60)
	{
		wave1.clear();
		return 1;
	}
	return 0;
}

int Game::S1E13()			//测试用，第二面仅一个幽灵boss
{
	static int i1 = 0, stp = 0;
	i1++;
	static list<FO> wave1, wave2;

	if (i1 == 1)
	{
		FO ghost(6);
		ghost.hero.setTexture(Enemy2);
		ghost.hero.setTextureRect(sf::IntRect(0, 64, ghost.width, ghost.height));
		ghost.hero.setScale(sf::Vector2f(1.5, 1.5));
		ghost.hero.setOrigin(ghost.width*0.5, ghost.height*0.5 - 24);
		ghost.born = i1;
		ghost.hero.setPosition(sf::Vector2f(450.0, -20.0));
		FO spellBoard;
		spellBoard.hero.setTexture(magicSquare);
		spellBoard.hero.setOrigin(128, 128);
		spellBoard.hero.setPosition(ghost.hero.getPosition());
		spellBoard.theta = 0;
		spellBoard.hero.setColor(sf::Color(255, 255, 255, 127));
		ghost.phase = 10;			//初始阶段定义，目前是2非
		wave1.push_back(ghost);
		wave2.push_back(spellBoard);
	}
	if (wave1.size() == 0)
	{
		wave2.clear();
		return 1;
	}
	wave2.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave2.begin(); it != wave2.end(); it++)
	{
		it->hero.setPosition(wave1.begin()->hero.getPosition().x, wave1.begin()->hero.getPosition().y + 32);
		it->theta += PI / 100.0;
		it->hero.setScale(1.0 + 0.1*sin(i1 / 11.0), 1.0 + 0.1*sin(i1 / 11.0));
		it->hero.setRotation(it->theta / PI * 180.0 + 90);
		backEsPushToDraw(it);
	}
	wave1.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		if (it->phase + stp == 9)				//判断目前的阶段并执行
		{
			stp++;
			i1 = 80;
		}
		it->hero.setTextureRect(sf::IntRect(i1 % 80 / 10 * it->width, 64, it->width, it->height));
		if (i1 < 80)
		{
			it->speed = (80 - i1) / 16.0;
			it->theta = 0.5 * PI;
		}
		else if (i1 >= 80 && i1 < 12000)
		{
			it->speed = 0.0;
			switch (it->phase)
			{
			case 10:
				nonSpellCard2(it);
				break;
			case 9:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					spellCard2(it);
				}
				break;
			case 8:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					nonSpellCard3(it);
				}
				break;
			case 7:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					spellCard3(it);
				}
				break;
			case 6:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					nonSpellCard4(it);
				}
				break;
			case 5:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					spellCard4(it);
				}
				break;
			case 4:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					nonSpellCard5(it);
				}
				break;
			case 3:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					spellCard5(it);
				}
				break;
			case 2:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					nonSpellCard6(it);
				}
				break;
			case 1:
				if (i1 == 241)
				{
					SCAnounce.play();
				}
				if (i1 > 6 * 60)
				{
					spellCard6(it);
				}
				break;
			}
		}
		else
		{
			it->speed = (i1 - 12000) / 60.0;
			it->theta = 1.5 * PI;
		}

		enemyCollisionProcessing(it);
		
		enemiesPushToDraw(it);

		if (i1 > 120 * 60)
		{
			if (it->phase >= 2)
			{
				cardGet.play();
				breakSound.play();
				score += it->score;
				//死亡效果
				deathEff.setTexture(deathCircle);
				deathEff.setTextureRect(sf::IntRect(64, 0, 64, 64));
				deathEff.setOrigin(32, 32);
				deathEff.setPosition(it->hero.getPosition().x + it->width * 0.25, it->hero.getPosition().y + it->height * 0.25);
				deathEff.setScale(0.1, 0.1);
				deathEffs.push_back(deathEff);
				deathEff.setScale(0.3, 0.06);
				deathEff.setRotation(rand() % 360);
				deathEffs.push_back(deathEff);
				it->HealthPoint += 1500;
			}
			else
			{
				wave1.clear();
			}
				
			return 1;
		}
	}
	return 0;
}



int Game::S1E14()			//未实装
{
	static int i1 = 0;
	i1++;
	static list<FO> wave1;

	if (i1 == 1)
	{
		FO mainTitle(0);
		mainTitle.hero.setTexture(Title2);
		mainTitle.hero.setTextureRect(sf::IntRect(0, 0, 512, 256));
		mainTitle.speed = 0.0;
		mainTitle.hero.setPosition(sf::Vector2f(200.0, 150.0));
		mainTitle.hero.setColor(sf::Color(255, 255, 255, 0));

		wave1.push_back(mainTitle);
	}

	for (list<FO>::iterator it = wave1.begin(); it != wave1.end(); it++)
	{
		it->hero.setColor(sf::Color(255, 255, 255, -i1*(i1 - 301) / (151.0*150.0) * 255));
		enemiesPushToDraw(it);
	}
	if (i1 > 5 * 60)
	{
		wave1.clear();
		return 1;
	}
	return 0;
}

void Game::enemiesPushToDraw(list<FO>::iterator it)			//处理不同类型的敌人的逻辑和弹幕发射
{
	switch (it->type)
	{
	case 101:				//边界反弹类
		if (it->hero.getPosition().y < 60 || it->hero.getPosition().y > 900)
		{	
			if (it->bounds < 3)
			{
				it->theta += PI;
				it->bounds++;
			}
		}
		break;
	case 103:			//发射弹幕类
		it->HealthPoint++;
		if (it->HealthPoint > 60 && it->phase > 0)
		{
			enemyBulletSound.play();
			FO nonSpell1;
			nonSpell1.speed = it->speed;
			nonSpell1.width = 16;
			nonSpell1.height = 16;
			nonSpell1.hero.setTexture(allBullets1);
			switch (it->phase)
			{
			case 4:
				nonSpell1.hero.setTextureRect(sf::IntRect(32, 16, 16, 16));
				break;
			case 3:
				nonSpell1.hero.setTextureRect(sf::IntRect(32, 16, 16, 16));
				break;
			case 2:
				nonSpell1.hero.setTextureRect(sf::IntRect(240, 16, 16, 16));
				break;
			case 1:
				nonSpell1.hero.setTextureRect(sf::IntRect(160, 16, 16, 16));
				break;
			}
			nonSpell1.hero.setOrigin(8, 8);
			nonSpell1.hero.setScale(1.5, 1.5);
			nonSpell1.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y);
			nonSpell1.type = 103;
			nonSpell1.phase = it->phase-1;
			for (int i = 0; i < 5; i++)
			{
				nonSpell1.speed += i / 10.0;
				nonSpell1.theta = it->theta + PI - 18 / 180.0*PI + i*18.0 / 180.0*PI;
				nonSpell1.hero.setRotation(nonSpell1.theta / PI * 180.0 + 90);
				enemyBullets.push_back(nonSpell1);
			}
			it->hero.setPosition(-100, -100);
			return;
		}
		break;
	case 104:			//抛物线运动类
		it->HealthPoint++;
		if (it->speed > EPS)
		{
			it->velocity.x = it->speed * cos(it->theta);
			it->velocity.y = it->speed * sin(it->theta) + 4.9*it->HealthPoint*it->HealthPoint/7200.0;
			it->hero.setRotation(atan2(it->velocity.y, it->velocity.x) / PI * 180.0 + 90);
			if (it->velocity.y >= 8)
			{
				it->velocity.y = 8;
			}
			it->hero.move(it->velocity);
		}
		enemies.push_back(*it);
		return;
		break;
	}
			//通用类
	if (it->speed > EPS)
	{
		it->velocity.x = it->speed * cos(it->theta);
		it->velocity.y = it->speed * sin(it->theta);
		it->hero.move(it->velocity);
	}

	enemies.push_back(*it);
}

void Game::backEsPushToDraw(list<FO>::iterator it)			//管理更新背景特效
{
	if (it->speed > EPS)
	{
		it->velocity.x = it->speed * cos(it->theta);
		it->velocity.y = it->speed * sin(it->theta);
		it->hero.move(it->velocity);
	}

	backgroundEffs.push_back(it->hero);
}

void Game::frameDisplay()//ammo->front->player->jpoint
{
								//用来渲染每一帧的显示效果
	player.staticFrame = player.staticFrame % 56;
	player.staticFrame++;
	mWindow.clear();
	
	backgroundDisplay();

	playerAmmoDisplay();

	effsDisplay();
	
	playerDisplay();

	enemiesDisplay();
	
	enemyBulletsDisplay();

	displayBluePoints();

	boardDisplay();

	
	mWindow.draw(text);		//demo
	
	mWindow.display();		//刷新窗口
}
//
void Game::backgroundDisplay()				//实现背景的滚动
{
	for (int i = 0; i < 6; i++)					//滚动背景
	{
		if (back[i].getPosition().y >= 5 * 192 + 35)
		{
			back[i].setPosition(65.0, 35 - 192);
		}
		back[i].move(0.0, 3.0);
		mWindow.draw(back[i]);

	}
	for (int i = 0; i < 4; i++)					//滚动背景特效
	{
		if (backEff[i].getPosition().y >= 3 * 384 + 35)
		{
			backEff[i].setPosition(65.0, 35 - 384);
		}
		backEff[i].move(0.0, 5.0);
		mWindow.draw(backEff[i]);
	}
	for (list<sf::Sprite>::iterator it = backgroundEffs.begin(); it != backgroundEffs.end(); it++)
	{
		mWindow.draw(*it);
	}
	backgroundEffs.clear();
}
//
void Game::playerAmmoDisplay()			//处理自机的子弹
{
	if (mIsFire)
	{
		//playerAmmo = (mIsGrazing) ? player.LSAmmo : player.HSAmmo;
		if (player.staticFrame % 2 == 1)		//每帧射击
		{
			player.LSAmmo.setPosition(sf::Vector2f(player.hero.getPosition().x + 4, player.hero.getPosition().y + 80));
			playerBullets.push_back(player.LSAmmo);
			player.LSAmmo.setPosition(sf::Vector2f(player.hero.getPosition().x + 20, player.hero.getPosition().y + 80));
			playerBullets.push_back(player.LSAmmo);
			player.HSAmmo.setPosition(sf::Vector2f(player.hero.getPosition().x + 4, player.hero.getPosition().y + 110));
			playerBullets.push_back(player.HSAmmo);
			player.HSAmmo.setPosition(sf::Vector2f(player.hero.getPosition().x + 20, player.hero.getPosition().y + 110));
			playerBullets.push_back(player.HSAmmo);
		}
	}
	playerBullets.remove_if(isOutOfBoard);
	for (list<sf::Sprite>::iterator it = playerBullets.begin(); it != playerBullets.end(); it++)
	{
		it->move(0.0, - 60.0);	//子弹的移动
		mWindow.draw(*it);
	}
}
//
void Game::enemiesDisplay()		//敌人精灵绘制
{
	for (list<FO>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		mWindow.draw(it->hero);
	}
	enemies.clear();
}
//
/*void Game::enemyBulletsPreDisplay()
{
	enemyBulletsPre.remove_if([](FO obj) { if (obj.hero.getScale().x <= 1.0) return true; else return false; });
	for (list<FO>::iterator it = enemyBulletsPre.begin(); it != enemyBulletsPre.end(); it++)
	{
		it->hero.setScale(it->hero.getScale().x - 0.2, it->hero.getScale().y - 0.2);
		it->hero.setColor(sf::Color(255, 255, 255, it->hero.getColor().White + 255 / 5));
		if (obj.hero.getScale().x <= 1.0)
		{
			enemyBullets.push_back(*it);
		}
	}
}*/
//
void Game::enemyBulletsDisplay()			//处理敌方子弹的显示和移除超出边界的子弹
{
	enemyBullets.remove_if(isFOOutOfBoard);
	for (list<FO>::iterator it = enemyBullets.begin(); it != enemyBullets.end(); it++)
	{
		enemiesPushToDraw(it);
	}
}
//
void Game::playerDisplay()
{
	pair<PlayerCollisionResult, list<FO>::iterator> collision = checkPlayerCollision();

	if ((collision.first == PlayerCollisionResult::EnemyBullet          // 被弹
		|| collision.first == PlayerCollisionResult::EnemyBody)         // 或者被体术
		&& clockForInvulnerability.getElapsedTime().asSeconds() > 1.0)  // 而且不在无敌时间内
	{
		// 清空场上所有来自敌机的子弹
		for (list<FO>::iterator it = enemyBullets.begin(); it != enemyBullets.end(); it++)
		{
			enemyCrash(it);
		}

		clockForInvulnerability.restart();
		playerDeadSound.play();

		// 扣除残机
		if (remnant > 1)
		{
			remnant--;
			printf("remnant: %lld\n", remnant);
		}
		else
		{
			GameOver();
			//printf("Game Over!\n");
		}
	}

	// 这是吃到蓝点。
	if (collision.first == PlayerCollisionResult::BluePoint)
	{
		bluePointCollected(collision.second);
	}

	if (collision.first == PlayerCollisionResult::NoCollision)
	{
		;  // noop。
	}

	if (mIsMovingLeft)			//检测左移
	{
		if (player.dynamicFrame < 14)
		{
			player.dynamicFrame++;
		}
		player.hero.setTextureRect(sf::IntRect(player.width * (player.dynamicFrame / 2), player.height, player.width, player.height));
	}
	else if (mIsMovingRight)		//检测右移
	{
		if (player.dynamicFrame < 14)
		{
			player.dynamicFrame++;
		}
		player.hero.setTextureRect(sf::IntRect(player.width * (player.dynamicFrame / 2), 2 * player.height, player.width, player.height));
	}
	else
	{
		player.hero.setTextureRect(sf::IntRect(player.width * (player.staticFrame / 8), 0, player.width, player.height));
		player.dynamicFrame = 0;
	}

	mWindow.draw(player.hero);		//更新人物位置

	static int julgeRotate = 0;
	julgeRotate++;
	julgeRotate %= 360;
	julgePoint.setOrigin(32, 32);
	julgePoint.setRotation(julgeRotate);
	julgePoint.setPosition(sf::Vector2f(player.hero.getPosition().x - 24 + 48, player.hero.getPosition().y - 8 + 48));
	if (mIsGrazing)			//检测低速
	{	
		mWindow.draw(julgePoint);
	}
}
//
void Game::effsDisplay()		//处理子弹效果和死亡效果
{
	for (list<sf::Sprite>::iterator it = playerBulletsEffs.begin(); it != playerBulletsEffs.end(); it++)
	{
		mWindow.draw(*it);
	}
	playerBulletsEffs.clear();
	for (list<sf::Sprite>::iterator it = deathEffs.begin(); it != deathEffs.end(); it++)
	{
		double i = it->getScale().x, j = it->getScale().y / it->getScale().x;
		i += 0.1;
		it->setScale(i, i*j);
		it->setColor(sf::Color(255, 255, 255, 255 * (1.2 - 0.5*i)));
		
		mWindow.draw(*it);
	}
	deathEffs.remove_if([](sf::Sprite obj) { if (obj.getScale().x > 2.3 || (obj.getRotation() < EPS && obj.getScale().x > 2.0)) return true; else return false; });
}
//
void Game::boardDisplay()			//显示血量和分数
{
	mWindow.draw(front01);
	mWindow.draw(front02);
	mWindow.draw(front03);
	mWindow.draw(front04);

	switch (remnant)
	{
	case 8:
		lifeBoard.setTextureRect(sf::IntRect(0, 0, 278, 36));
		break;
	case 7:
		lifeBoard.setTextureRect(sf::IntRect(0, 44, 278, 36));
		break;
	case 6:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 2, 278, 36));
		break;
	case 5:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 3, 278, 36));
		break;
	case 4:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 4, 278, 36));
		break;
	case 3:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 5, 278, 36));
		break;
	case 2:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 6, 278, 36));
		break;
	case 1:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 7, 278, 36));
		break;
	case 0:
		lifeBoard.setTextureRect(sf::IntRect(0, 44 * 8, 278, 36));
	default:
		;
	}
	
	lifeBoard.setScale(1.5, 1.5);
	lifeBoard.setPosition(830, 300);
	mWindow.draw(lifeBoard);

	static string scoreStr;
	scoreStr = "Score:             ";
	scoreStr += to_string(score);
	tempScore.setString(scoreStr);
	tempScore.setStyle(sf::Text::Italic);
	tempScore.setFont(font);
	tempScore.setCharacterSize(50);
	tempScore.setPosition(840, 50);
	mWindow.draw(tempScore);
}

void Game::enemyCollisionProcessing(list<FO>::iterator it)		//处理敌人被子弹击中的实现
{
	for (list<sf::Sprite>::iterator itAmmo = playerBullets.begin(); itAmmo != playerBullets.end(); itAmmo++)
	{
		if (checkCollision(it->hero, *itAmmo))
		{
			enemyUnderAttack(it, itAmmo);

			if (it->HealthPoint <= 0)
			{
				it->phase--;
				// 爆蓝点喽
				//puts("setBluePointByEnemyType(it);");
				setBluePointByEnemyType(it);
				if (it->phase <= 0)
				{	// 敌机击毁？
					enemyCrash(it);
				}
				else
				{	// 敌机是 boss，进入了下一阶段？
					cardGet.play();
					breakSound.play();
					score += it->score;
					deathEff.setTexture(deathCircle);
					deathEff.setTextureRect(sf::IntRect(64, 0, 64, 64));
					deathEff.setOrigin(32, 32);
					deathEff.setPosition(it->hero.getPosition().x + it->width * 0.25, it->hero.getPosition().y + it->height * 0.25);
					deathEff.setScale(0.1, 0.1);
					deathEffs.push_back(deathEff);
					deathEff.setScale(0.3, 0.06);
					deathEff.setRotation(rand() % 360);
					deathEffs.push_back(deathEff);
					it->HealthPoint += 1500;
				}
			}
		}
	}
}

void Game::enemyUnderAttack(list<FO>::iterator it, list<sf::Sprite>::iterator itAmmo)		//处理敌人被击中后的效果
{
	score++;
	it->HealthPoint -= player.damage;
	AmmoEff.setTexture(buffetsEff);
	AmmoEff.setTextureRect(sf::IntRect(player.staticFrame % 7 * 48, 0, 48, 48));
	AmmoEff.setPosition(itAmmo->getPosition().x - 10, it->hero.getPosition().y + it->height - 10);
	playerBulletsEffs.push_back(AmmoEff);
	itAmmo->setPosition(-100, -100);
}

void Game::enemyCrash(list<FO>::iterator it)		//处理敌人被击毁
{
	breakSound.play();
	score += it->score;
	deathEff.setTexture(deathCircle);
	deathEff.setTextureRect(sf::IntRect(64, 0, 64, 64));
	deathEff.setOrigin(32, 32);
	deathEff.setPosition(it->hero.getPosition().x + it->width * 0.25, it->hero.getPosition().y + it->height * 0.25);
	deathEff.setScale(0.1, 0.1);
	deathEffs.push_back(deathEff);
	deathEff.setScale(0.3, 0.06);
	deathEff.setRotation(rand() % 360);
	deathEffs.push_back(deathEff);
	it->hero.setPosition(-100, -100);
}

void Game::bluePointCollected(list<FO>::iterator it)		//处理蓝点被吃掉
{
	bluePointCollectedSound.play();
	// 玩家越高，分数越大
	score += 50 + static_cast<int>(100 * (1 - static_cast<double>(player.hero.getPosition().y - 40) / (850 - 40)));

	it->hero.setPosition(-100, -100);
}

void Game::standardSButterflyFrame(list<FO>::iterator it, int temp)	//设置蝴蝶的动画帧和方向
{
	double t1 = it->theta - 0.5 * PI;
	double t2 = it->theta - 1.5 * PI;			//计算角度
	int he = 320;
	if (fabs(t1) < EPS || fabs(t2) * PI < EPS)			//判断蝴蝶方向并设置对应的纹理
	{
		it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 5) * it->width, he, it->width, it->height));
	}
	else if ((fabs(t1) > EPS &&  fabs(t1) < PI / 8.0) || (fabs(t2) > EPS &&  fabs(t2) < PI / 8.0))
	{
		it->hero.setTextureRect(sf::IntRect(5 * it->width, he, it->width, it->height));
	}
	else if ((fabs(t1) > PI / 8.0 &&  fabs(t1) < PI / 4.0) || (fabs(t2) > PI / 8.0 &&  fabs(t2) < PI / 4.0))
	{
		it->hero.setTextureRect(sf::IntRect(6 * it->width, he, it->width, it->height));
	}
	else if ((fabs(t1) > PI / 4.0 &&  fabs(t1) < PI / 2.0) || (fabs(t2) > PI / 4.0 &&  fabs(t2) < PI / 2.0))
	{
		it->hero.setTextureRect(sf::IntRect(7 * it->width, he, it->width, it->height));
	}
	else
	{
		it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 4 + 8) * it->width, he, it->width, it->height));
	}

	if ((it->theta > 0.5 * PI && it->theta < 1.5 * PI) || (it->theta < -0.5 * PI && it->theta > -1.5 * PI))
	{
		it->hero.setScale(-1.5, 1.5);
	}
	else
	{
		it->hero.setScale(1.5, 1.5);
	}
}

void Game::standardMButterflyFrame(list<FO>::iterator it, int temp)	//和上一个函数类似，但处理的是中型蝴蝶
{
	double t1 = it->theta - 0.5 * PI;
	double t2 = it->theta - 1.5 * PI;
	int he = 448;
	if (fabs(t1) < EPS || fabs(t2) * PI < EPS)
	{
		it->hero.setTextureRect(sf::IntRect((int)(temp / 8 % 5) * it->width, he, it->width, it->height));
	}
	else if ((fabs(t1) > EPS &&  fabs(t1) < PI / 8.0) || (fabs(t2) > EPS &&  fabs(t2) < PI / 8.0))
	{
		it->hero.setTextureRect(sf::IntRect(5 * it->width, he, it->width, it->height));
	}
	else if ((fabs(t1) > PI / 8.0 &&  fabs(t1) < PI / 4.0) || (fabs(t2) > PI / 8.0 &&  fabs(t2) < PI / 4.0))
	{
		it->hero.setTextureRect(sf::IntRect(6 * it->width, he, it->width, it->height));
	}
	else
	{
		it->hero.setTextureRect(sf::IntRect(7 * it->width, he + (temp / 8 % 3 - 2) * it->height, it->width, it->height));
	}
	
	if ((it->theta > 0.5 * PI && it->theta < 1.5 * PI) || (it->theta < -0.5 * PI && it->theta > -1.5 * PI))
	{
		it->hero.setScale(-1.5, 1.5);
	}
	else
	{
		it->hero.setScale(1.5, 1.5);
	}
}

void Game::setSnipe(list<FO>::iterator it, double speed, int type, int color)	//用于创建和发射敌人的子弹
{
	enemyBulletSound.play();
	FO Snipe;
	Snipe.speed = speed;
	Snipe.width = 16;
	Snipe.height = 16;
	Snipe.hero.setTexture(allBullets1);
	Snipe.hero.setTextureRect(sf::IntRect(color * 16, type * 16, 16, 16));
	Snipe.hero.setOrigin(8, 8);
	Snipe.hero.setScale(1.5, 1.5);
	Snipe.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	Snipe.theta = atan2(julgePoint.getPosition().y - Snipe.hero.getPosition().y, julgePoint.getPosition().x - Snipe.hero.getPosition().x);

	enemyBullets.push_back(Snipe);
}

void Game::setRoundSnipe(list<FO>::iterator it, double speed)			//圆形弹参数设置
{
	enemyBulletSound.play();
	FO RoundSnipe;
	RoundSnipe.speed = speed;
	RoundSnipe.width = 16;
	RoundSnipe.height = 16;
	RoundSnipe.hero.setTexture(allBullets1);
	RoundSnipe.hero.setTextureRect(sf::IntRect(80, 32, 16, 16));
	RoundSnipe.hero.setOrigin(8, 8);
	RoundSnipe.hero.setScale(1.5, 1.5);
	RoundSnipe.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	RoundSnipe.theta = atan2(julgePoint.getPosition().y - RoundSnipe.hero.getPosition().y, julgePoint.getPosition().x - RoundSnipe.hero.getPosition().x);
	
	enemyBullets.push_back(RoundSnipe);
}

void Game::setMultiRoundSnipe(list<FO>::iterator it, double speed, int color)	
{
	enemyBulletSound.play();
	FO MultiRoundSnipe;
	MultiRoundSnipe.speed = speed;
	MultiRoundSnipe.width = 16;
	MultiRoundSnipe.height = 16;
	MultiRoundSnipe.hero.setTexture(allBullets1);
	MultiRoundSnipe.hero.setTextureRect(sf::IntRect(color * 16, 48, 16, 16));
	MultiRoundSnipe.hero.setOrigin(8, 8);
	MultiRoundSnipe.hero.setScale(1.5, 1.5);
	MultiRoundSnipe.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	MultiRoundSnipe.theta = atan2(julgePoint.getPosition().y - MultiRoundSnipe.hero.getPosition().y, julgePoint.getPosition().x - MultiRoundSnipe.hero.getPosition().x);
	enemyBullets.push_back(MultiRoundSnipe);
	MultiRoundSnipe.theta += 0.4 * PI;
	enemyBullets.push_back(MultiRoundSnipe);
	MultiRoundSnipe.theta -= 0.8 * PI;
	enemyBullets.push_back(MultiRoundSnipe);
}

void Game::setGeneralMultiSnipe(list<FO>::iterator it, double speed, int type, int color, double angle)
{
	enemyBulletSound.play();
	FO GeneralMultiSnipe;
	GeneralMultiSnipe.speed = speed;
	GeneralMultiSnipe.width = 16;
	GeneralMultiSnipe.height = 16;
	GeneralMultiSnipe.hero.setTexture(allBullets1);
	GeneralMultiSnipe.hero.setTextureRect(sf::IntRect(color * 16, type * 16, 16, 16));
	GeneralMultiSnipe.hero.setOrigin(8, 8);
	GeneralMultiSnipe.hero.setScale(1.5, 1.5);
	GeneralMultiSnipe.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	GeneralMultiSnipe.theta = atan2(julgePoint.getPosition().y - GeneralMultiSnipe.hero.getPosition().y, julgePoint.getPosition().x - GeneralMultiSnipe.hero.getPosition().x) + (rand() % 5 * 0.01 - 0.02) * PI;
	enemyBullets.push_back(GeneralMultiSnipe);
	GeneralMultiSnipe.theta += angle * PI;
	enemyBullets.push_back(GeneralMultiSnipe);
	GeneralMultiSnipe.theta -= angle * 2 * PI;
	enemyBullets.push_back(GeneralMultiSnipe);
}

void Game::setRandom(list<FO>::iterator it, double speed, int type, int color, double leftBoarder, double range)	//随机弹设置
{
	enemyBulletSound.play();
	FO Random;
	Random.speed = speed;
	Random.theta = rand() % (int)range + leftBoarder;
	Random.width = 16;
	Random.height = 16;
	Random.hero.setTexture(allBullets1);
	Random.hero.setTextureRect(sf::IntRect(color * 16, type * 16, 16, 16));
	Random.hero.setOrigin(8, 8);
	Random.hero.setScale(1.5, 1.5);
	Random.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	Random.hero.setRotation(Random.theta / PI * 180.0 + 90);
	enemyBullets.push_back(Random);
}

void Game::setRoundRandom(list<FO>::iterator it, double speed, int color, double leftBoarder, double range)	//随机弹
{
	enemyBulletSound.play();
	FO RoundRandom;
	RoundRandom.speed = speed;
	RoundRandom.theta = rand() % (int)range + leftBoarder;
	RoundRandom.width = 16;
	RoundRandom.height = 16;
	RoundRandom.hero.setTexture(allBullets1);
	RoundRandom.hero.setTextureRect(sf::IntRect(color * 16, 48, 16, 16));
	RoundRandom.hero.setOrigin(8, 8);
	RoundRandom.hero.setScale(1.5, 1.5);
	RoundRandom.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	RoundRandom.hero.setRotation(RoundRandom.theta / PI * 180.0 + 90);
	enemyBullets.push_back(RoundRandom);
}

void Game::setSharpRandom(list<FO>::iterator it, double speed)
{
	enemyBulletSound.play();
	FO SharpRandom;
	SharpRandom.speed = speed;
	SharpRandom.theta = rand()%360;
	SharpRandom.width = 16;
	SharpRandom.height = 16;
	SharpRandom.hero.setTexture(allBullets1);
	SharpRandom.hero.setTextureRect(sf::IntRect(64, 64, 16, 16));//80 96
	SharpRandom.hero.setOrigin(8, 8);
	SharpRandom.hero.setScale(1.5, 1.5);
	SharpRandom.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	SharpRandom.hero.setRotation(SharpRandom.theta  / PI * 180.0 + 90);
	enemyBullets.push_back(SharpRandom);
}

void Game::setSharpLine(list<FO>::iterator it, double speed)
{
	enemyBulletSound.play();
	FO SharpLine;
	SharpLine.speed = speed;
	SharpLine.theta = 0.5 * PI;
	if (it->theta > 0.5 * PI)
	{
		SharpLine.theta += it->speed * 0.04 * PI;
	}
	else
	{
		SharpLine.theta -= it->speed * 0.04 * PI;
	}
	SharpLine.width = 16;
	SharpLine.height = 16;
	SharpLine.hero.setTexture(allBullets1);
	SharpLine.hero.setTextureRect(sf::IntRect(64, 64, 16, 16));//80 96
	SharpLine.hero.setOrigin(8, 8);
	SharpLine.hero.setScale(1.5, 1.5);
	SharpLine.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	SharpLine.hero.setRotation(SharpLine.theta / PI * 180.0 + 90);
	enemyBullets.push_back(SharpLine);
}

void Game::setSharpFlower1(list<FO>::iterator it, double speed, int type, int color)		//开花弹
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp -= 2 * PI;
	}
	enemyBulletSound.play();
	FO SharpFlower1;
	SharpFlower1.speed = speed;
	SharpFlower1.width = 16;
	SharpFlower1.height = 16;
	SharpFlower1.hero.setTexture(allBullets1);
	SharpFlower1.hero.setTextureRect(sf::IntRect(0, 64, 16, 16));
	SharpFlower1.hero.setOrigin(8, 8);
	SharpFlower1.hero.setScale(1.5, 1.5);
	SharpFlower1.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (cts % 2 == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			SharpFlower1.theta = theta1 + PI * 2 * i / 3.0;
			SharpFlower1.hero.setRotation(SharpFlower1.theta / PI * 180.0 + 90);
			enemyBullets.push_back(SharpFlower1);
		}
	}
	SharpFlower1.hero.setTextureRect(sf::IntRect(176, 64, 16, 16));
	SharpFlower1.hero.setOrigin(8, 8);
	SharpFlower1.hero.setScale(1.5, 1.5);
	SharpFlower1.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (temp >= 0 && temp <= PI)
	{
		SharpFlower1.speed /= 2.0;
		SharpFlower1.speed += 9.0 / (temp + 1.5);
		for (int i = 0; i < 3; i++)
		{
			SharpFlower1.theta = theta2 + PI * 2 * i / 3.0 + temp;
			SharpFlower1.hero.setRotation(SharpFlower1.theta / PI * 180.0 + 90);
			enemyBullets.push_back(SharpFlower1);
			SharpFlower1.theta = theta2 + PI * 2 * i / 3.0 - temp;
			SharpFlower1.hero.setRotation(SharpFlower1.theta / PI * 180.0 + 90);
			enemyBullets.push_back(SharpFlower1);
		}
	}

	theta1 += 0.02 * PI;
	temp += PI / 24.0;
	if (sin(temp) < EPS)
	{
		theta2 = theta1 + PI / 6.0;
	}
}

void Game::nonSpellCard1(list<FO>::iterator it)			//一面旋转弹，非符，具体原理参考THB wiki/b站
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp -= 2 * PI;
	}
	enemyBulletSound.play();
	FO nonSpell1;
	nonSpell1.speed = 8.0;
	nonSpell1.width = 16;
	nonSpell1.height = 16;
	nonSpell1.hero.setTexture(allBullets1);
	nonSpell1.hero.setTextureRect(sf::IntRect(80, 64, 16, 16));
	nonSpell1.hero.setOrigin(8, 8);
	nonSpell1.hero.setScale(1.5, 1.5);
	nonSpell1.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	
	for (int i = 0; i < 5; i++)
	{
		nonSpell1.theta = theta1 + PI * 2 * i / 5.0;
		nonSpell1.hero.setRotation(nonSpell1.theta / PI * 180.0 + 90);
		enemyBullets.push_back(nonSpell1);
	}

	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 120.0;
}

void Game::nonSpellCard2(list<FO>::iterator it)		//2非
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}
	
	FO nonSpell2;
	nonSpell2.speed = 4.0;
	nonSpell2.width = 16;
	nonSpell2.height = 16;
	nonSpell2.hero.setTexture(allBullets1);
	nonSpell2.hero.setTextureRect(sf::IntRect(80, 128, 16, 16));
	nonSpell2.hero.setOrigin(8, 8);
	nonSpell2.hero.setScale(1.5, 1.5);
	nonSpell2.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (cts % 360 <= 120)
	{
		if (cts % 24 == 0)
		{
			enemyBulletSound.play();
			nonSpell2.hero.setPosition((rand() % 300) * 2 + 100, (rand() % 100) + 60);
			for (int i = 0; i < 72; i++)
			{
				nonSpell2.theta = theta1 + PI * 2 * i / 72.0;
				nonSpell2.hero.setRotation(nonSpell2.theta / PI * 180.0 + 90);
				enemyBullets.push_back(nonSpell2);
			}
		}
	}
	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 240.0;
}

void Game::nonSpellCard3(list<FO>::iterator it)		//3非
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}
	enemyBulletSound.play();
	FO nonSpell3;
	nonSpell3.speed = 6.0;
	nonSpell3.width = 16;
	nonSpell3.height = 16;
	nonSpell3.hero.setTexture(allBullets1);
	nonSpell3.hero.setTextureRect(sf::IntRect(80, 128, 16, 16));
	nonSpell3.hero.setOrigin(8, 8);
	nonSpell3.hero.setScale(1.5, 1.5);
	nonSpell3.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (cts % 2 == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			nonSpell3.theta = theta1 + PI * 2 * i / 10.0;
			nonSpell3.hero.setRotation(nonSpell3.theta / PI * 180.0 + 90);
			enemyBullets.push_back(nonSpell3);
		}
	}
	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 240.0;
}


void Game::nonSpellCard4(list<FO>::iterator it)				//4非
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}
	enemyBulletSound.play();
	FO nonSpell4;
	nonSpell4.speed = 6.0;
	nonSpell4.width = 16;
	nonSpell4.height = 16;
	nonSpell4.hero.setTexture(allBullets1);
	nonSpell4.hero.setTextureRect(sf::IntRect(80, 128, 16, 16));
	nonSpell4.hero.setOrigin(8, 8);
	nonSpell4.hero.setScale(1.5, 1.5);
	if (cts % 2 == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			nonSpell4.theta = theta1 + PI * 2 * i / 3.0;
			nonSpell4.hero.setRotation(nonSpell4.theta / PI * 180.0 + 90);
			nonSpell4.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
			enemyBullets.push_back(nonSpell4);
			if (cts % 7 == 0)
			{
				nonSpell4.hero.setTextureRect(sf::IntRect(160, 128, 16, 16));
				nonSpell4.hero.setPosition(it->hero.getPosition().x + 200, it->hero.getPosition().y + it->height + 100);
				enemyBullets.push_back(nonSpell4);
				nonSpell4.hero.setPosition(it->hero.getPosition().x - 200, it->hero.getPosition().y + it->height + 100);
				enemyBullets.push_back(nonSpell4);
			}
		}
	}
	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 240.0;
}

void Game::nonSpellCard5(list<FO>::iterator it)		//5非
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}

	FO nonSpell5;
	nonSpell5.speed = 4.0;
	nonSpell5.width = 16;
	nonSpell5.height = 16;
	nonSpell5.hero.setTexture(allBullets1);
	nonSpell5.hero.setTextureRect(sf::IntRect(80, 128, 16, 16));
	nonSpell5.hero.setOrigin(8, 8);
	nonSpell5.hero.setScale(1.5, 1.5);
	nonSpell5.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (cts % 360 <= 240)
	{
		if (cts % 8 == 0)
		{
			enemyBulletSound.play();
			nonSpell5.hero.setPosition(it->hero.getPosition().x + 100*sin(cts/20.0), it->hero.getPosition().y + it->height);
			for (int i = 0; i < 36; i++)
			{
				nonSpell5.theta = theta1 + PI * 2 * i / 36.0;
				nonSpell5.hero.setRotation(nonSpell5.theta / PI * 180.0 + 90);

				enemyBullets.push_back(nonSpell5);
			}
		}
	}
	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 240.0;
}

void Game::nonSpellCard6(list<FO>::iterator it)			//6非
{
	static double theta1 = PI / 6.0;
	static double theta2 = -PI / 6.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}

	FO nonSpell6;
	nonSpell6.speed = 4.0;
	nonSpell6.width = 16;
	nonSpell6.height = 16;
	nonSpell6.hero.setTexture(allBullets1);
	nonSpell6.hero.setTextureRect(sf::IntRect(32, 16, 16, 16));
	nonSpell6.hero.setOrigin(8, 8);
	nonSpell6.hero.setScale(1.5, 1.5);
	nonSpell6.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (cts % 48 == 0)
	{
		enemyBulletSound.play();
		for (int i = 0; i < 48; i++)
		{
			nonSpell6.theta = theta1 + PI * 2 * i / 48.0;
			nonSpell6.hero.setRotation(nonSpell6.theta / PI * 180.0 + 90);
			enemyBullets.push_back(nonSpell6);
		}
	}
	if (cts % 2 == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			nonSpell6.hero.setTextureRect(sf::IntRect(128, 16, 16, 16));
			nonSpell6.theta = theta2 + PI * 2 * i / 5.0;
			nonSpell6.hero.setRotation(nonSpell6.theta / PI * 180.0 + 90);
			enemyBullets.push_back(nonSpell6);
		}
		for (int i = 0; i < 5; i++)
		{
			nonSpell6.hero.setTextureRect(sf::IntRect(208, 16, 16, 16));
			nonSpell6.theta = theta1 + PI * 2 * i / 5.0;
			nonSpell6.hero.setRotation(nonSpell6.theta / PI * 180.0 + 90);
			enemyBullets.push_back(nonSpell6);
		}
	}
	theta1 += 0.02 * PI;
	theta2 -= 0.02 * PI;
	temp += PI / 240.0;
}

void Game::spellCard1(list<FO>::iterator it)		//1符
{
	static double range = 0, x1 = rand() % 200 + 250;
	static int ct = 0, temp = 0;

	FO Card1;
	Card1.speed = 8.0;
	Card1.width = 16;
	Card1.height = 16;
	Card1.hero.setTexture(allBullets1);
	Card1.hero.setTextureRect(sf::IntRect(16, 7 * 16, 16, 16));
	Card1.hero.setOrigin(8, 8);
	Card1.hero.setScale(1.5, 1.5);
	Card1.theta = 1.5 * PI;
	Card1.type = 101;
	if (temp % 10 == 0)
	{
		enemyBulletSound.play();
		Card1.hero.setPosition(it->hero.getPosition().x + range, it->hero.getPosition().y + it->height);
		enemyBullets.push_back(Card1);
		Card1.hero.setPosition(it->hero.getPosition().x - range, it->hero.getPosition().y + it->height);
		enemyBullets.push_back(Card1);
		Card1.hero.setPosition(x1 + range, it->hero.getPosition().y + it->height);
		enemyBullets.push_back(Card1);
		Card1.hero.setPosition(x1 - range, it->hero.getPosition().y + it->height);
		enemyBullets.push_back(Card1);
	}

	(ct % 2) ? range -= 16 : range += 16;			//判断是否过低，实现弹反的效果
	temp++;
	if (range >= 350 || range <= 0)
	{
		ct++;
	}
}

void Game::spellCard2(list<FO>::iterator it)			//2符
{
	double speed = 4;
	int type = 4, color = 0;
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}
	enemyBulletSound.play();
	FO Card2;
	Card2.speed = speed;
	Card2.width = 16;
	Card2.height = 16;
	Card2.hero.setTexture(allBullets1);
	Card2.hero.setTextureRect(sf::IntRect(0, 64, 16, 16));
	Card2.hero.setOrigin(8, 8);
	Card2.hero.setScale(1.5, 1.5);
	Card2.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);

	for (int i = 0; i < 3; i++)
	{
		Card2.theta = theta1 + PI * 2 * i / 3.0;
		Card2.hero.setRotation(Card2.theta / PI * 180.0 + 90);
		enemyBullets.push_back(Card2);
	}

	Card2.hero.setTextureRect(sf::IntRect(176, 64, 16, 16));
	Card2.hero.setOrigin(8, 8);
	Card2.hero.setScale(1.5, 1.5);
	Card2.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (temp >= 0 && temp <= PI)
	{
		Card2.speed /= 2.0;
		Card2.speed += 9.0 / (temp + 1.5);
		for (int i = 0; i < 3; i++)
		{
			Card2.theta = theta2 + PI * 2 * i / 3.0 + temp;
			Card2.hero.setRotation(Card2.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card2);
			Card2.theta = theta2 + PI * 2 * i / 3.0 - temp;
			Card2.hero.setRotation(Card2.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card2);
		}
	}

	theta1 += 0.02 * PI;
	temp += PI / 24.0;
	if (sin(temp) < EPS)
	{
		theta2 = theta1 + PI / 6.0;
	}
}

void Game::spellCard3(list<FO>::iterator it)			//3符
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}
	
	FO Card3;
	Card3.speed = 2.0;
	Card3.width = 16;
	Card3.height = 16;
	Card3.hero.setTexture(allBullets1);
	Card3.hero.setTextureRect(sf::IntRect(80, 16, 16, 16));
	Card3.hero.setOrigin(8, 8);
	Card3.hero.setScale(1.5, 1.5);
	Card3.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	Card3.type = 103;
	Card3.phase = 3;
	if (cts % 360 == 1)
	{
		enemyBulletSound.play();
		for (int i = 0; i < 6; i++)
		{
			Card3.theta = theta1 + PI * 2 * i / 6.0;
			Card3.hero.setRotation(Card3.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card3);
		}
	}
	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 240.0;
}

void Game::spellCard4(list<FO>::iterator it)			//4符
{
	enemyBulletSound.play();
	FO Card4;
	Card4.speed = 4.0;
	Card4.theta = rand() % 360;
	Card4.width = 16;
	Card4.height = 16;
	Card4.hero.setTexture(allBullets1);
	Card4.hero.setTextureRect(sf::IntRect(rand()%15 * 16, 4 * 16, 16, 16));
	Card4.hero.setOrigin(8, 8);
	Card4.hero.setScale(1.5, 1.5);
	Card4.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	Card4.hero.setRotation(Card4.theta / PI * 180.0 + 90);
	Card4.type = 104;
	enemyBullets.push_back(Card4);
	Card4.theta = rand() % 360;
	enemyBullets.push_back(Card4);
}

void Game::spellCard5(list<FO>::iterator it)				//5符
{
	static double theta1 = PI / 2.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}

	FO Card5;
	Card5.speed = 4.0;
	Card5.width = 16;
	Card5.height = 16;
	Card5.hero.setTexture(allBullets1);
	Card5.hero.setTextureRect(sf::IntRect(32, 16, 16, 16));
	Card5.hero.setOrigin(8, 8);
	Card5.hero.setScale(1.5, 1.5);
	Card5.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);
	if (cts % 72 == 0)
	{
		enemyBulletSound.play();
		for (int i = 0; i < 72; i++)
		{
			Card5.theta = theta1 + PI * 2 * i / 72.0;
			Card5.hero.setRotation(Card5.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card5);
		}
	}
	if (cts % 8 == 0)
	{
		Card5.speed = 7.0;
		for (int i = 0; i < 8; i++)
		{
			Card5.hero.setTextureRect(sf::IntRect(128, 16, 16, 16));
			Card5.theta = theta2 + PI * 2 * i / 8.0;
			Card5.hero.setRotation(Card5.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card5);
		}
		for (int i = 0; i < 8; i++)
		{
			Card5.hero.setTextureRect(sf::IntRect(208, 16, 16, 16));
			Card5.theta = theta1 + PI * 2 * i / 8.0;
			Card5.hero.setRotation(Card5.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card5);
		}
	}
	theta1 += 0.003 * PI;
	theta2 -= 0.003 * PI;
	temp += PI / 240.0;
}

void Game::spellCard6(list<FO>::iterator it)				//6符
{
	static double theta1 = PI / 6.0;
	static double theta2 = PI / 2.0;
	static double temp = 0.0;
	static int cts = 0, cc = 40;
	cts++;
	if (fabs(temp - 2 * PI) < EPS)
	{
		temp = 0.0;
	}

	FO Card6;
	Card6.speed = 4.0;
	Card6.width = 32;
	Card6.height = 32;
	Card6.hero.setTexture(allBullets2);
	Card6.hero.setTextureRect(sf::IntRect(64, 96, 32, 32));
	Card6.hero.setOrigin(16, 16);
	Card6.hero.setScale(1.5, 1.5);
	Card6.hero.setPosition(it->hero.getPosition().x, it->hero.getPosition().y + it->height);

	if (cts % 60 == 0)
	{
		enemyBulletSound.play();
		for (int i = 0; i < 36; i++)//(rand() % 500)+ 100
		{
			Card6.theta = theta1 + PI * 2 * i / 36.0;
			Card6.hero.setRotation(Card6.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card6);
		}
	}
	if ((cts % (2*cc)) < cc)
	{
		if (cts % 19 == 0)
		{
			Card6.hero.setTextureRect(sf::IntRect(224, 96, 32, 32));
			Card6.speed = 8.0;
			enemyBulletSound.play();
			Card6.hero.setPosition(it->hero.getPosition().x+100, it->hero.getPosition().y + it->height);
			Card6.theta = atan2(julgePoint.getPosition().y - Card6.hero.getPosition().y, julgePoint.getPosition().x - Card6.hero.getPosition().x);
			Card6.hero.setRotation(Card6.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card6);
			Card6.hero.setPosition(it->hero.getPosition().x-100, it->hero.getPosition().y + it->height);
			Card6.theta = atan2(julgePoint.getPosition().y - Card6.hero.getPosition().y, julgePoint.getPosition().x - Card6.hero.getPosition().x);
			Card6.hero.setRotation(Card6.theta / PI * 180.0 + 90);
			enemyBullets.push_back(Card6);
			if (cc <= 300)
			{
				cc++;
			}	
		}
	}
	
	theta1 += 0.04 * PI * sin(temp);
	temp += PI / 240.0;
}

void Game::processTaps()		//玩家输入处理
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			playerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			playerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		default:
			break;
		}
	}
}

void Game::playerInput(sf::Keyboard::Key key, bool isPressed)		//读取输入
{
	if (key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;
	else if (key == sf::Keyboard::Z)
		mIsFire = isPressed;
	else if (key == sf::Keyboard::LShift)
		mIsGrazing = isPressed;
	player.speed = (mIsGrazing) ? 3.0 : 10.0;
}
/*
bool isOutOfBoard(sf::Sprite value)
{
	if (value.getPosition().y >= 0 && value.getPosition().y <= 960 && value.getPosition().x >= 0 && value.getPosition().x <= 960)
	{
		return false;
	}
	return true;
}
*/

/*
bool isFOOutOfBoard(const FO value)
{
	if (value.hero.getPosition().y >= -100 && value.hero.getPosition().y <= 960 && value.hero.getPosition().x >= 0 && value.hero.getPosition().x <= 960)
	{
		return false;
	}
	return true;
}
*/

void Game::mainProcessing()			//处理移动
{
	if (mIsMovingUp == true && player.hero.getPosition().y > 40)
	{
		//player.theta = PI*0.5;
		player.hero.move(0.0, float(-player.speed));
	}
	if (mIsMovingDown == true && player.hero.getPosition().y < 850)
	{
		//player.theta = PI*1.5;
		player.hero.move(0.0, float(player.speed));
	}
	if (mIsMovingLeft == true && player.hero.getPosition().x > 69)
	{
		//player.theta = PI;
		player.hero.move(float(-player.speed), 0.0);
	}
	if (mIsMovingRight == true && player.hero.getPosition().x < 751)
	{
		//player.theta = 0.0;
		player.hero.move(float(player.speed), 0.0);
	}

	/*if ((mIsMovingDown || mIsMovingLeft || mIsMovingRight || mIsMovingUp) && !isOutOfBoard(player.hero))
	{
		player.velocity.x = player.speed * cos(player.theta);
		player.velocity.y = player.speed * sin(player.theta);
		player.hero.move(player.velocity);
	}*/
	
	if (mIsFire)			//处理是否开火，设置状态
	{
		if (playerBulletSound.getStatus() != playerBulletSound.Playing)
		{
			playerBulletSound.play();
		}
	}
}

bool Game::checkCollision(sf::Sprite obj1, sf::Sprite obj2)		//检测精灵碰撞
{								
	sf::FloatRect f1 = obj1.getGlobalBounds();
	sf::FloatRect f2 = obj2.getGlobalBounds();
	if (f1.intersects(f2))
	{
		return true;
	}
	return false;
}

pair<PlayerCollisionResult, list<FO>::iterator> Game::checkPlayerCollision()		// 检测玩家是否与子弹/敌机/蓝点碰撞
{
	sf::Vector2f JP = julgePoint.getPosition();
	JP.x -= 8;
	JP.y -= 8;

	// 敌机子弹
	for (list<FO>::iterator it = enemyBullets.begin(); it != enemyBullets.end(); it++)
	{
		sf::FloatRect f = it->hero.getGlobalBounds();

		f.width /= 2.0;
		f.height /= 2.0;
		if (f.contains(JP))
		{
			return {PlayerCollisionResult::EnemyBullet, it};
		}
	}

	// 敌机体术
	for (list<FO>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		if (it->type == 0)
		{
			continue;  // 孩子们，我不想被过场动画体术
		}

		sf::FloatRect f = it->hero.getGlobalBounds();

		f.width /= 2.0;
		f.height /= 2.0;
		if (f.contains(JP))
		{
			return {PlayerCollisionResult::EnemyBody, it};
		}
	}

	// 蓝点
	for (list<FO>::iterator it = bluePoints.begin(); it != bluePoints.end(); it++)
	{
		sf::FloatRect f1 = player.hero.getGlobalBounds();
		sf::FloatRect f2 = it->hero.getGlobalBounds();

		f1.height -= 12;
		f1.top += 12;
		f2.width = f2.height = 1;

		if (f1.intersects(f2))
		{
			return {PlayerCollisionResult::BluePoint, it};
		}
	}

	return {PlayerCollisionResult::NoCollision, enemyBullets.end()}; // 没有撞到, 这时用 enemyBullets.end() 来充当“撞上的物品”
}
/*
void Game::GameOver()
{
	exit(0);
}
*/
void Game::GameOver()
{
    // 暂停游戏状态
    sf::Clock pauseClock;
    sf::Time pauseDuration = sf::seconds(5); // 设置暂停时间为 10 秒

    // 显示分数的弹窗
    sf::RenderWindow gameOverWindow(sf::VideoMode(400, 300), "Game Over");

    // 创建显示分数的文本
    sf::Text scoreText;
    scoreText.setFont(font); // 使用已加载的字体
    scoreText.setString("Score: " + std::to_string(score)); // 将得分转化为字符串
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(50, 50);

	/*
	sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./res/sl_pl01.png")) // 替换为图片路径
    {
        // 处理加载失败情况
        puts("Failed to load background image");
        return;
    }

	sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(-50, 0); // 设置图片位置
    backgroundSprite.setScale(
        gameOverWindow.getSize().x / backgroundSprite.getLocalBounds().width,
        gameOverWindow.getSize().y / backgroundSprite.getLocalBounds().height
    ); // 适应窗口大小

	*/

    // 停止游戏更新循环
    while (pauseClock.getElapsedTime() < pauseDuration)
    {
        sf::Event event;
        while (gameOverWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameOverWindow.close();
            }
        }

        // 渲染暂停窗口内容
        gameOverWindow.clear(sf::Color::White);
		//gameOverWindow.draw(backgroundSprite);
        gameOverWindow.draw(scoreText);
        gameOverWindow.display();
    }

    // 关闭弹窗后继续运行游戏窗口
    exit(0);
}


/*DWORD WINAPI BGMPlay(LPVOID lpParameter)
{
	Game* game = (Game*)lpParameter;
	int i = 0;
	while (1)
	{
		i %= 10;
		i++;
		if (game->mIsFire && i == 1)
		{
			if (game->playerBulletSound.getStatus() != game->playerBulletSound.Playing)
			{
				game->playerBulletSound.play();
			}

			//
		}
	}
	return 0;
}*/

Game::~Game()
{
	;
}
