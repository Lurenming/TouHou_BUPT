#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//#include <windows.h>
#include "FO.h"
#include <list>
#include <utility> // for std::pair<>
using namespace std;
#define MAX_LIFE 8
#define MAX_BOMB 8


enum class PlayerCollisionResult {
	NoCollision, EnemyBullet, BluePoint, EnemyBody
};


class Game
{
public:
	Game();
	void NowLoading();
	void loadBackgrounds();
	void loadPrimeFrame();
	void loadPointsAndEffs();
	void loadBluePoints();
	void displayBluePoints();
	void setBluePointByEnemyType(list<FO>::iterator it);
	void loadEnemy();
	void loadMusicAndSounds();
	void loadOptionsUI();
	void run();
	void menu();
	void options();
	void Stage1();
	int S1E1();
	int S1E2();
	int S1E3();
	int S1E4();
	int S1E5();
	int S1E6();
	int S1E7();

	int S1E8();

	int S1E9();
	int S1E10();
	int S1E11();

	int S1E12();

	int S1E13();

	int S1E14();
	void enemiesPushToDraw(list<FO>::iterator it);
	void backEsPushToDraw(list<FO>::iterator it);
	void frameDisplay();
	void backgroundDisplay();
	void playerAmmoDisplay();
	void playerBombDisplay();
	void enemiesDisplay();
	//void enemyBulletsPreDisplay();
	void enemyBulletsDisplay();
	void playerDisplay();
	void effsDisplay();
	void boardDisplay();
	void enemyCollisionProcessing(list<FO>::iterator it);
	void enemyUnderAttack(list<FO>::iterator it, list<sf::Sprite>::iterator itAmmo);
	void enemyCrash(list<FO>::iterator it);
	void bluePointCollected(list<FO>::iterator it);
	void standardSButterflyFrame(list<FO>::iterator it, int temp);
	void standardMButterflyFrame(list<FO>::iterator it, int temp);

	void setSnipe(list<FO>::iterator it, double speed, int type, int color);
	//
	void setRoundSnipe(list<FO>::iterator it, double speed);
	//
	void setMultiRoundSnipe(list<FO>::iterator it, double speed, int color);

	void setGeneralMultiSnipe(list<FO>::iterator it, double speed, int type, int color, double angle);

	void setRandom(list<FO>::iterator it, double speed, int type, int color, double leftBoarder, double range);
	//
	void setRoundRandom(list<FO>::iterator it, double speed, int color, double leftBoarder, double range);
	//
	void setSharpRandom(list<FO>::iterator it, double speed);

	void setSharpLine(list<FO>::iterator it, double speed);

	void setSharpFlower1(list<FO>::iterator it, double speed, int type, int color);

	void nonSpellCard1(list<FO>::iterator it);
	void nonSpellCard2(list<FO>::iterator it);
	void nonSpellCard3(list<FO>::iterator it);
	void nonSpellCard4(list<FO>::iterator it);
	void nonSpellCard5(list<FO>::iterator it);
	void nonSpellCard6(list<FO>::iterator it);
	void spellCard1(list<FO>::iterator it);
	void spellCard2(list<FO>::iterator it);
	void spellCard3(list<FO>::iterator it);
	void spellCard4(list<FO>::iterator it);
	void spellCard5(list<FO>::iterator it);
	void spellCard6(list<FO>::iterator it);

	void processTaps();
	void playerInput(sf::Keyboard::Key key, bool isPressed);
	friend bool isOutOfBoard(sf::Sprite value);//wait for updating
	//friend bool isFOOutOfBoard(FO value);//wait for updating
	void mainProcessing();
	bool checkCollision(sf::Sprite obj1, sf::Sprite obj2);
	pair<PlayerCollisionResult, list<FO>::iterator> checkPlayerCollision();

	void GameOver();
	/*friend DWORD WINAPI BGMPlay(LPVOID lpParameter);*/
	~Game();

private:
	Game* self = this;
	bool mIsMovingUp, mIsMovingDown, mIsMovingLeft, mIsMovingRight, mIsGrazing, mIsFire, mIsUsingBomb;
	sf::RenderWindow mWindow;
	sf::Font font;
	sf::Text text, tempScore;
	sf::Texture loading, nowLoading, stageSelect, front00, julgePointArray, Title1, Title2, allBullets1, allBullets2, whiteSpark;
	sf::Texture bullets, buffetsEff, deathCircle, bg1, bgEff1, bg2, bgEff2, bg3, bgEff3, Enemy1, Enemy2, Enemy3, lifePieces, magicSquare;
	sf::Texture bombPieces;
	sf::Texture title,optionsBg,optionsTitle;
	sf::Texture bluePointTexture;
	sf::Sprite loadingUI, loadingUISub, back[6], backEff[6], front01, front02, front03, front04;
	sf::Sprite julgePoint, playerAmmo, AmmoEff, deathEff, lifeBoard;
	sf::Sprite bombBoard;
	sf::Sprite titleBackground,optionsBackground,optionsTitleUI;
	sf::Music menuMusic, stage1BGM, stage2BGM, stage3BGM;
	sf::SoundBuffer playerBulletSoundBuffer, playerBulletSoundBuffer1, enemyBulletSoundBuffer, collisionSoundBuffer, spellCardSoundBuffer, buttomSoundBuffer;
	sf::SoundBuffer breakSoundBuffer, playerDeadSoundBuffer, SCAnounceBuffer, cardGetBuffer;
	sf::SoundBuffer selectSoundBuffer;
	sf::SoundBuffer bluePointCollectedSoundBuffer;
	sf::SoundBuffer bombSoundBuffer;
	sf::Sound playerBulletSound, playerBulletSound1, enemyBulletSound, collisionSound, spellCardSound, buttomSound, SCAnounce, cardGet;
	sf::Sound breakSound, playerDeadSound;
	sf::Sound selectSound;
	sf::Sound bluePointCollectedSound;
	sf::Sound bombSound;
	sf::Clock clock;
	sf::Clock clockForInvulnerability;
	FO player;
	list<FO>  enemyBullets, enemyBulletsPre;
	list<sf::Sprite> playerBullets, deathEffs;
	list<FO> enemies;
	list<sf::Sprite> playerBulletsEffs, backgroundEffs;
	list<FO> bluePoints;
	long long remnant, score, bomb;
};

