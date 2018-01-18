#ifndef ENV_NODE_H
#define ENV_NODE_H

#include <algorithm>

class EnvNode {
public:
	const int MAX_FOOD = 1e4;
	const int MAX_DISEASE = 1e4;

	const int FOOD_INC = 1;
	const int DISEASE_INC = -1;

	int lastUpdate = 0;
	int food = MAX_FOOD;
	int disease = 0;

	Point3f dir;

	void update (int time) {
		if (time != lastUpdate)
		{
			food = std::min(food + time * FOOD_INC, MAX_FOOD);
			disease = std::min(disease + time * DISEASE_INC, MAX_DISEASE);
			lastUpdate = time;
		}
	}
};

#endif