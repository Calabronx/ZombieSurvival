#pragma once
#ifndef SCORE_H
#define SCORE_H

struct Score
{
	enum Type {
		ZOMBIES_KILLED,
		PICKUP_ITEMS,
		BULLETS_FIRED,
		GUNS_COLLECTED,
		DAMAGE_MADE,
		DAMAGE_TAKEN,
		TOTAL_DEATHS,
		ScoreCount
	};
	Type score;
	std::string name;
	size_t value;
};
#endif // !SCORE_H
