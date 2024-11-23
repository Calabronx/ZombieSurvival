#include "HighScoreState.h"
#include "../../util/Utility.h"
#include "../../gui/Button.h"

HighScoreState::HighScoreState(StateStack& stack, Context context)
	: State(stack, context)
	, mTitleTxt()
	, mZombiesKilledTxt()
	, mBulletsFiredTxt()
	, mItemsCollectedTxt()
	, mDamageMadeTxt()
	, mDamageTakenTxt()
	, mGunsCollectedTxt()
	, mTotalDeathsTxt()
	, mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mTitleTxt.setFont(font);
	mZombiesKilledTxt.setFont(font);
	mBulletsFiredTxt.setFont(font);
	mItemsCollectedTxt.setFont(font);
	mDamageMadeTxt.setFont(font);
	mDamageTakenTxt.setFont(font);
	mGunsCollectedTxt.setFont(font);
	mTotalDeathsTxt.setFont(font);

	mTitleTxt.setString("HIGH SCORES");
	std::unique_ptr<ScoreData> zombiesKillScore(new ScoreData());
	zombiesKillScore = context.scores->getScore(HighScore::ZOMBIES_KILLED);
	mZombiesKilledTxt.setString("ZOMBIES KILLED: " + toString(zombiesKillScore->value));
	
	std::unique_ptr<ScoreData> bulletsScore(new ScoreData());
	bulletsScore = context.scores->getScore(HighScore::BULLETS_FIRED);
	mBulletsFiredTxt.setString("BULLETS FIRED: " + toString(bulletsScore->value));
	
	std::unique_ptr<ScoreData> itemsCollected(new ScoreData());
	itemsCollected = context.scores->getScore(HighScore::PICKUP_ITEMS);
	mItemsCollectedTxt.setString("ITEMS COLLECTED: " + toString(itemsCollected->value));
	
	std::unique_ptr<ScoreData> damageMade(new ScoreData());
	damageMade = context.scores->getScore(HighScore::DAMAGE_MADE);
	mDamageMadeTxt.setString("DAMAGE MADE: " + toString(damageMade->value));

	std::unique_ptr<ScoreData> damageTaken(new ScoreData());
	damageTaken = context.scores->getScore(HighScore::DAMAGE_TAKEN);
	mDamageTakenTxt.setString("DAMAGE TAKEN: " + toString(damageTaken->value));
	
	std::unique_ptr<ScoreData> gunsCollect(new ScoreData());
	gunsCollect = context.scores->getScore(HighScore::GUNS_COLLECTED);
	mGunsCollectedTxt.setString("GUNS COLLECTED: " + toString(gunsCollect->value));

	std::unique_ptr<ScoreData> totalDeaths(new ScoreData());
	totalDeaths = context.scores->getScore(HighScore::TOTAL_DEATHS);
	mTotalDeathsTxt.setString("TOTAL DEATHS: " + toString(totalDeaths->value));

	mTitleTxt.setCharacterSize(30);
	mZombiesKilledTxt.setCharacterSize(20);
	mBulletsFiredTxt.setCharacterSize(20);
	mItemsCollectedTxt.setCharacterSize(20);
	mDamageMadeTxt.setCharacterSize(20);
	mDamageTakenTxt.setCharacterSize(20);
	mGunsCollectedTxt.setCharacterSize(20);
	mTotalDeathsTxt.setCharacterSize(20);

	centerOrigin(mTitleTxt);
	centerOrigin(mZombiesKilledTxt);
	centerOrigin(mBulletsFiredTxt);
	centerOrigin(mItemsCollectedTxt);
	centerOrigin(mDamageMadeTxt);
	centerOrigin(mDamageTakenTxt);
	centerOrigin(mGunsCollectedTxt);
	centerOrigin(mTotalDeathsTxt);

	mTitleTxt.setPosition(0.5f * windowSize.x, 0.1f * windowSize.y);
	mZombiesKilledTxt.setPosition(0.2f * windowSize.x, 0.2f * windowSize.y);
	mBulletsFiredTxt.setPosition(0.2f * windowSize.x, 0.3f * windowSize.y);
	mItemsCollectedTxt.setPosition(0.2f * windowSize.x, 0.4f * windowSize.y);
	mGunsCollectedTxt.setPosition(0.2f * windowSize.x, 0.5f * windowSize.y);
	mDamageMadeTxt.setPosition(0.2f * windowSize.x, 0.6f * windowSize.y);
	mDamageTakenTxt.setPosition(0.2f * windowSize.x, 0.7f * windowSize.y);
	mTotalDeathsTxt.setPosition(0.2f * windowSize.x, 0.8f * windowSize.y);

	auto retryButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	retryButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	retryButton->setText("Play again");
	retryButton->setCallback([this]()
		{
			requestStateClear();
			requestStackPush(States::Game);
		});

	auto backToMenuButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backToMenuButton->setText("Back to menu");
	backToMenuButton->setCallback([this]()
		{
			requestStateClear();
			//requestStackPush(States::Menu);
			requestStackPush(States::Title);
		});

	//mGUIContainer.pack(retryButton);
	mGUIContainer.pack(backToMenuButton);
}

void HighScoreState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mTitleTxt);
	window.draw(mZombiesKilledTxt);
	window.draw(mBulletsFiredTxt);
	window.draw(mItemsCollectedTxt);
	window.draw(mGunsCollectedTxt);
	window.draw(mDamageMadeTxt);
	window.draw(mDamageTakenTxt);
	window.draw(mTotalDeathsTxt);
	window.draw(mGUIContainer);
}

bool HighScoreState::update(sf::Time dt)
{
	return false;
}

bool HighScoreState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
