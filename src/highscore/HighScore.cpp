#include "HighScore.h"
#include "Score.h"
#include <memory>

namespace {
	const std::vector<ScoreData> Table = initializeScoreData();
}

HighScore::HighScore()
	: mScores(Table.size())
{
	std::unique_ptr<ScoreData> zombiesKilledScore(new ScoreData());
	mScores[ZOMBIES_KILLED] = std::move(zombiesKilledScore);

	mScores[ZOMBIES_KILLED]->id = Table[ZOMBIES_KILLED].id;
	mScores[ZOMBIES_KILLED]->name = Table[ZOMBIES_KILLED].name;
	mScores[ZOMBIES_KILLED]->value = Table[ZOMBIES_KILLED].value;
	
	std::unique_ptr<ScoreData> bulletsShootScore(new ScoreData());
	mScores[BULLETS_FIRED] = std::move(bulletsShootScore);
	mScores[BULLETS_FIRED]->id = Table[BULLETS_FIRED].id;
	mScores[BULLETS_FIRED]->name = Table[BULLETS_FIRED].name;
	mScores[BULLETS_FIRED]->value = Table[BULLETS_FIRED].value;

	std::unique_ptr<ScoreData> pickupItemsScore(new ScoreData());
	mScores[PICKUP_ITEMS] = std::move(pickupItemsScore);
	mScores[PICKUP_ITEMS]->id = Table[PICKUP_ITEMS].id;
	mScores[PICKUP_ITEMS]->name = Table[PICKUP_ITEMS].name;
	mScores[PICKUP_ITEMS]->value = Table[PICKUP_ITEMS].value;

	std::unique_ptr<ScoreData> damageMadeScore(new ScoreData());
	mScores[DAMAGE_MADE] = std::move(damageMadeScore);
	mScores[DAMAGE_MADE]->id = Table[DAMAGE_MADE].id;
	mScores[DAMAGE_MADE]->name = Table[DAMAGE_MADE].name;
	mScores[DAMAGE_MADE]->value = Table[DAMAGE_MADE].value;

	std::unique_ptr<ScoreData> damageTakenScore(new ScoreData());
	mScores[DAMAGE_TAKEN] = std::move(damageTakenScore);
	mScores[DAMAGE_TAKEN]->id = Table[DAMAGE_TAKEN].id;
	mScores[DAMAGE_TAKEN]->name = Table[DAMAGE_TAKEN].name;
	mScores[DAMAGE_TAKEN]->value = Table[DAMAGE_TAKEN].value;

	std::unique_ptr<ScoreData> gunsCollectedScore(new ScoreData());
	mScores[GUNS_COLLECTED] = std::move(gunsCollectedScore);
	mScores[GUNS_COLLECTED]->id = Table[GUNS_COLLECTED].id;
	mScores[GUNS_COLLECTED]->name = Table[GUNS_COLLECTED].name;
	mScores[GUNS_COLLECTED]->value = Table[GUNS_COLLECTED].value;

	std::unique_ptr<ScoreData> totalDeathsScore(new ScoreData());
	mScores[TOTAL_DEATHS] = std::move(totalDeathsScore);
	mScores[TOTAL_DEATHS]->id = Table[TOTAL_DEATHS].id;
	mScores[TOTAL_DEATHS]->name = Table[TOTAL_DEATHS].name;
	mScores[TOTAL_DEATHS]->value = Table[TOTAL_DEATHS].value;
}

void HighScore::addScore(size_t scoreID, size_t score)
{
	mScores[scoreID]->value += score;
}

std::unique_ptr<ScoreData> HighScore::getScore(size_t scoreId)
{
	std::unique_ptr<ScoreData> score(new ScoreData());
	score = std::move(mScores[scoreId]);
	return score;
}

void HighScore::printScores() const
{
	std::cout << "*****************************" << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "HIGHSCORE" << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "ZOMBIES KILLED: " <<  mScores[ZOMBIES_KILLED]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "BULLETS FIRED: " <<  mScores[BULLETS_FIRED]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "ITEMS COLLECTED: " <<  mScores[PICKUP_ITEMS]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "DAMAGE MADE: " <<  mScores[DAMAGE_MADE]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "DAMAGE TAKEN: " <<  mScores[DAMAGE_TAKEN]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "GUNS COLLECTED: " <<  mScores[GUNS_COLLECTED]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "TOTAL DEATHS: " <<  mScores[TOTAL_DEATHS]->value << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "*****************************" << std::endl;
}
