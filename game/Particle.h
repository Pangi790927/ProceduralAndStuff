#ifndef PARTICLE_H
#define PARTICLE_H

#include "Options.h"

class Particle {
public:
	float age = 0;
	float food = 0;
	float disease = 0;
	
	float descomposed = 0;

	static float MAX_DECOMPOSED;
	static float MAX_AGE;
	static float MAX_FOOD;
	static float MAX_DISEASE;
	static float AGE_MED;
	static float AGE_SIG;
	static float DISEASE_KT;
	static float FOOD_KS;
	static float FOOD_KI;
	static float U_MAX;
	static float FOOD_CONSUME;
	static float Q_DISEASE;
	static float DISEASE_THRESHOLD;
	static float DISEASE_GAIN;
	static float DISEASE_LOSS;
	static float DISEASE_KT_DEATH;
	static float FOOD_CONSUME_ENERGY;
	static float FOOD_CHILD_COST;
	static float FOOD_START;
	static float DECIDE_CHANCE_DIE;
	static float DECIDE_CHANCE_DISEASE;
	static float DECIDE_CHANCE_DIVIDE;

	Particle () {
		food = FOOD_START;
	}

	Point3f pos;
	Point3f spd;
	
	/// status: x - feed, y - age, z - intoxication
	Point3f status = Point3f(0, 0, 0);
	bool dead = false;
	bool disapear = false;

	bool update (EnvNode& env, int time) {
		// std::cout << "+++++++++++++++++++ PART +++++++++++++++++++" << std::endl;
		bool divided = false;

		env.update(time);
		if (dead) {
			descomposed++;
			status = Point3f(descomposed / float(MAX_DECOMPOSED)
					, descomposed / float(MAX_DECOMPOSED)
					, descomposed / float(MAX_DECOMPOSED));			
			if (descomposed > MAX_DECOMPOSED)
			{
				disapear = true;
			}
		}
		else {
			age++;

			float gatheredFood = std::min (env.food, FOOD_CONSUME);   
			food += gatheredFood;
			env.food -= gatheredFood;
			food -= FOOD_CONSUME_ENERGY;
			
			if (diseaseWill()) {
				disease += Q_DISEASE * food;
				// std::cout << Q_DISEASE * food << std::endl;
			}
			// std::cout << "disease: " << disease << std::endl;
			// std::cout << "env disease2: " << env.disease << std::endl;
			
			if (food > FOOD_CHILD_COST && divideWill()) {
				food -= FOOD_CHILD_COST;
				divided = true;
			}

			float gain = env.disease * (DISEASE_GAIN / 100.0f);
			float envGain = disease * (DISEASE_LOSS / 100.0f);

			disease += gain - envGain;
			env.disease += -gain + envGain;

			// std::cout << "env disease2: " << env.disease << std::endl;
			// std::cout << "disease2: " << disease << std::endl;

			if (readyToDie()) {
				dead = true;
			}

			status.x = age / float(MAX_AGE);
			status.y = food / float(MAX_FOOD);
			status.z = disease / float(MAX_DISEASE);
		}

		// std::cout << "+++++++++++++++++++ PART +++++++++++++++++++" << std::endl;
		return divided;
	}

	float randNorm() {
		return rand() / float(RAND_MAX);
	}

	float diseaseProc() {
		return disease / MAX_DISEASE;
	}

	float ageProc() {
		return age / MAX_AGE;
	}

	float foodProc() {
		return food / MAX_FOOD;
	}

	bool divideWill() {
		if (rand() % (int)DECIDE_CHANCE_DIVIDE)
			return false;

		return randNorm() * 100.0f < foodFunc(food / MAX_FOOD * 100) &&
				randNorm() * 100.0f < diseaseFunc(disease / MAX_DISEASE * 100) &&
				randNorm() * 100.0f < ageFunc(age / MAX_AGE * 100);
	}

	bool diseaseWill() {
		if (rand() % (int)DECIDE_CHANCE_DISEASE)
			return false;

		return randNorm() * 100 < DISEASE_THRESHOLD;
	}

	bool readyToDie() {
		if (rand() % (int)DECIDE_CHANCE_DIE)
			return false;

		return randNorm() * 100 > ageDeathFunction(age / MAX_AGE * 100)
				|| randNorm() * 100 < (100 - diseaseDeathFunc(disease / MAX_DISEASE * 100))
				|| food < 0;
	}

	float foodFunc (float value) {
		return U_MAX * 100 * value / 100 / (FOOD_KS + value / 100 + value * value / 10000 / FOOD_KI);
	}

	float diseaseDeathFunc (float value) {
		return 100 * DISEASE_KT_DEATH / (DISEASE_KT_DEATH + value * value);
	}

	float diseaseFunc (float value) {
		return 100 * DISEASE_KT / (DISEASE_KT + value * value);
	}

	float ageDeathFunction (float value) {
		return atan(-value / 10 + 10) * 100 / atan(10);
	}

	float ageFunc (float value) {
		return 10000. / 2 * 1.0f / (AGE_SIG * std::sqrt(2 * 3.141492653589)) * std::pow(2.71, 
				1.0 / 2 * (value - AGE_MED) * (value - AGE_MED) / AGE_SIG / AGE_SIG);
	}

	bool isGone() {
		return disapear;
	}

	static void init() {
		MAX_DECOMPOSED = opts["MAX_DECOMPOSED"];
		MAX_AGE = opts["MAX_AGE"];
		MAX_FOOD = opts["MAX_FOOD"];
		MAX_DISEASE = opts["MAX_DISEASE"];
		DISEASE_KT = opts["DISEASE_KT"];
		AGE_MED = opts["AGE_MED"];
		FOOD_KI = opts["FOOD_KI"];
		FOOD_KS = opts["FOOD_KS"];
		U_MAX = opts["U_MAX"];
		FOOD_CONSUME = opts["FOOD_CONSUME"];
		Q_DISEASE = opts["Q_DISEASE"];
		DISEASE_THRESHOLD = opts["DISEASE_THRESHOLD"];
		DISEASE_LOSS = opts["DISEASE_LOSS"];
		DISEASE_GAIN = opts["DISEASE_GAIN"];
		DISEASE_KT_DEATH = opts["DISEASE_KT_DEATH"];
		FOOD_CONSUME_ENERGY = opts["FOOD_CONSUME_ENERGY"];
		FOOD_START = opts["FOOD_START"];
		FOOD_CHILD_COST = opts["FOOD_CHILD_COST"];
		DECIDE_CHANCE_DIE = opts["DECIDE_CHANCE_DIE"];
		DECIDE_CHANCE_DISEASE = opts["DECIDE_CHANCE_DISEASE"];
		DECIDE_CHANCE_DIVIDE = opts["DECIDE_CHANCE_DIVIDE"];

	}
};

float Particle::MAX_DECOMPOSED = 0;
float Particle::MAX_AGE = 0;
float Particle::MAX_FOOD = 0;
float Particle::MAX_DISEASE = 0;
float Particle::AGE_MED = 0;
float Particle::AGE_SIG = 0;
float Particle::DISEASE_KT = 0;
float Particle::FOOD_KI = 0;
float Particle::FOOD_KS = 0;
float Particle::U_MAX = 0;
float Particle::FOOD_CONSUME = 0;
float Particle::Q_DISEASE = 0;
float Particle::DISEASE_THRESHOLD = 0;
float Particle::DISEASE_GAIN = 0;
float Particle::DISEASE_LOSS = 0;
float Particle::DISEASE_KT_DEATH = 0;
float Particle::FOOD_CONSUME_ENERGY = 0;
float Particle::FOOD_START = 0;
float Particle::FOOD_CHILD_COST = 0;
float Particle::DECIDE_CHANCE_DIE = 0;
float Particle::DECIDE_CHANCE_DISEASE = 0;
float Particle::DECIDE_CHANCE_DIVIDE = 0;

#endif