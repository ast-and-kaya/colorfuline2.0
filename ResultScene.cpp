#include "ResultScene.h"

float ResultScene::m_result_score;
int ResultScene::m_result_combo;
int ResultScene::m_result_great;

int ResultScene::m_result_note_number;

ResultScene::ResultScene()
{
}


ResultScene::~ResultScene()
{
}


void ResultScene::initialize()
{
	//背景
	m_bg.setTexture(tex.get("black"));

	m_shad.loadFromFile("data/shader/result_bg.frag", sf::Shader::Fragment);
	m_shad.setParameter("r", sf::Vector2f(windowManager.getWindow()->getSize().x, windowManager.getWindow()->getSize().y));
	m_state.shader = &m_shad;
	
	//フォント
	characterDisplay.setFont("Dosis", "data/font/Dosis-Light.ttf");
	//スコア等の文字
	//スコア
	characterDisplay.setCharacter("Iscore", "Dosis", "SCORE", sf::Vector2f(1000, 158), 176, sf::Color( 0, 0, 0, 50));
	string s = to_string(m_result_score);
	for (int i = 0; i < 4; i++) s.pop_back();
	characterDisplay.setCharacter("Sscore", "Dosis", s + "%", sf::Vector2f(1860, 224), 110, sf::Color(0, 0, 0, 255));
	characterDisplay.setOrigin("Sscore", CharacterDisplay::Align::Right);
	//コンボ
	characterDisplay.setCharacter("Icombo", "Dosis", "COMBO", sf::Vector2f(1000, 430), 176, sf::Color(0, 0, 0, 50));
	characterDisplay.setCharacter("Scombo_sub", "Dosis", "/" +to_string(m_result_note_number), sf::Vector2f(1860, 544), 60, sf::Color(0, 0, 0, 255));
	characterDisplay.setOrigin("Scombo_sub", CharacterDisplay::Align::Right);
	characterDisplay.setCharacter("Scombo", "Dosis", to_string(m_result_combo), sf::Vector2f(1830 - characterDisplay.getBox("Scombo_sub").x, 475), 130, sf::Color(0, 0, 0, 255));
	characterDisplay.setOrigin("Scombo", CharacterDisplay::Align::Right);
	//パーフェクト
	characterDisplay.setCharacter("Igreat", "Dosis", "GREAT", sf::Vector2f(1000, 698), 176, sf::Color(0, 0, 0, 50));
	characterDisplay.setCharacter("Sgreat_sub", "Dosis", "/" + to_string(m_result_note_number), sf::Vector2f(1860, 812), 60, sf::Color(0, 0, 0, 255));
	characterDisplay.setOrigin("Sgreat_sub", CharacterDisplay::Align::Right);
	characterDisplay.setCharacter("Sgreat", "Dosis", to_string(m_result_great), sf::Vector2f(1830 - characterDisplay.getBox("Sgreat_sub").x, 745), 130, sf::Color(0, 0, 0, 255));
	characterDisplay.setOrigin("Sgreat", CharacterDisplay::Align::Right);

	//メニュー
	characterDisplay.setCharacter("retry", "Dosis", "Retry", sf::Vector2f(120, 850), 120);
	characterDisplay.setCharacter("next", "Dosis", "Next", sf::Vector2f(520, 850), 120);

	//セレクトバー
	m_select.setTexture(tex.get("result_select"));
	m_select.setOrigin(350, 0);
	m_select_tf = true;
	m_select_posx[0] = 330;//retry
	m_select_posx[1] = 730;//next
	m_select.setPosition(m_select_posx[m_select_tf], 968);

	//ブラック
	sceneMovement.initialize();
	sceneMovement.Out();
}

Scene* ResultScene::update()
{
	Scene* next = this;
	sceneMovement.update();

	//セレクトバー
	if (sceneMovement.getAlpha() == 0)
	{
		if (keyManager.push_key(sf::Keyboard::Right) || keyManager.push_key(sf::Keyboard::Left)) {
			m_select_tf = !m_select_tf;
			m_select.setScale(0, 1);
		}
	}
	m_select.setPosition(m_select_posx[m_select_tf], 968);//位置変更
	if (m_select.getScale().x < 1) m_select.setScale(m_select.getScale().x + 0.15, 1);//セレクトバースケール調整

	if (keyManager.push_key(sf::Keyboard::Return))
	{
		sceneMovement.In();
	}
	if (sceneMovement.getAlpha() >= 255)
	{
		if (m_select_tf)
		{
			SelectScene s;
			s.setBlackZindex(true);
			next = new SelectScene;
		}
		else {
			next = new GameScene;
		}
	}

	return next;
}

void ResultScene::render()
{

	windowManager.getWindow()->draw( m_bg, m_state);

	characterDisplay.render("Iscore");
	characterDisplay.render("Icombo");
	characterDisplay.render("Igreat");

	characterDisplay.render("Sscore");
	characterDisplay.render("Scombo_sub");
	characterDisplay.render("Scombo");
	characterDisplay.render("Sgreat_sub");
	characterDisplay.render("Sgreat");

	characterDisplay.render("next");
	characterDisplay.render("retry");
	windowManager.getWindow()->draw(m_select);

	sceneMovement.render();

	musicGuide.render();
}


void ResultScene::setScereData(float score, int combo, int great, int note) {
	m_result_score = score;
	m_result_combo = combo;
	m_result_great = great;
	m_result_note_number = note;
	cout << m_result_note_number << endl;
}