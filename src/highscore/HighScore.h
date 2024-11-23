#pragma once
#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>
#include "../entity/data/DataTables.h"

class HighScore
{
	public:
		enum Type {
			ZOMBIES_KILLED,
			PICKUP_ITEMS,
			BULLETS_FIRED,
			GUNS_COLLECTED,
			DAMAGE_MADE,
			DAMAGE_TAKEN,
			TOTAL_DEATHS
		};

	public:
		HighScore();

		void addScore(size_t scoreID, size_t score);

		std::unique_ptr<ScoreData> getScore(size_t scoreId);

		void printScores() const;

	private:
		std::vector<std::unique_ptr<ScoreData>> mScores;
};
#endif // !HIGHSCORE_H

