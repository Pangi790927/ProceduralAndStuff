#ifndef ENV_NODE_H
#define ENV_NODE_H

#include <algorithm>

class EnvNode {
public:
	float lastUpdate = 0;
	float food = 0;
	float disease = 0;

	static float MAX_NODE_DISEASE;
	static float MAX_NODE_FOOD;
	static float DISEASE_INC;
	static float FOOD_INC;

	Point3f dir;

	void update (int time) {
		if (time != lastUpdate)
		{
			food = std::min(food + (time - lastUpdate) * FOOD_INC, MAX_NODE_FOOD);
			disease = std::max(std::min(disease + (time - lastUpdate) * DISEASE_INC,
					MAX_NODE_DISEASE), 0.0f);
			lastUpdate = time;
		}
	}

	static void init() {
		MAX_NODE_DISEASE = opts["MAX_NODE_DISEASE"];
		MAX_NODE_FOOD = opts["MAX_NODE_FOOD"];
		DISEASE_INC = opts["DISEASE_INC"];
		FOOD_INC = opts["FOOD_INC"];
	}
};

float EnvNode::MAX_NODE_DISEASE = 0;
float EnvNode::MAX_NODE_FOOD = 0;
float EnvNode::DISEASE_INC = 0;
float EnvNode::FOOD_INC = 0;

#endif