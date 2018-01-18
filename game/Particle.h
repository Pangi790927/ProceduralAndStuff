#ifndef PARTICLE_H
#define PARTICLE_H

class Particle {
public:
	static const int MAX_AGE = 1e4;
	static const int MAX_FOOD = 1e4;
	static const int MAX_DISEASE = 1e4;
	static const int MAX_DESCOMPOSED = 1e4;

	int age = 0;
	int food = MAX_FOOD / 10;
	int disease = 0;
	int descomposed = 0;

	Point3f pos;
	Point3f spd;
	
	/// status: x - feed, y - age, z - intoxication
	Point3f status = Point3f(0, 0, 0);
	bool dead = false;
	bool disapear = false;

	void update (EnvNode& env, int time) {
		env.update(time);
		age++;
		
		if (age > MAX_AGE) {
			dead = true;
		}

		if (dead) {
			descomposed++;
			status = Point3f(descomposed / float(MAX_DESCOMPOSED), descomposed / float(MAX_DESCOMPOSED), 0);			
			if (descomposed > MAX_DESCOMPOSED)
			{
				disapear = true;
			}
		}
		else {
			status.x = age / float(MAX_AGE);
			status.y = food / float(MAX_FOOD);
			status.z = disease / float(MAX_DISEASE);
		}
		
	}

	bool isGone() {
		return disapear;
	}
};

#endif