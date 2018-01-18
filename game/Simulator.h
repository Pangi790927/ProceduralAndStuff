#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "EnvNode.h"
#include "Particle.h"

class Simulator {
public:
	GameMesh torus;
	GameMesh containerFieldGraphic;
	GameMesh sphere;

	const static int containerSize = 30;
	EnvNode env[containerSize][containerSize][containerSize];
	std::vector<Particle> particles;
	bool active = false;

	Simulator() : particles(1000) {}

	void update() {
		auto isInisde = [this](int x, int y, int z) {
			return x > 0 && y > 0 && z > 0 && x < containerSize && y < containerSize && z < containerSize;
		};

		auto normRand = [](){
			return (rand() / float(RAND_MAX) - 0.5) * 2;
		};

		auto vecRand = [&](){
			return Point3f(normRand(), normRand(), normRand()).normalize();
		};

		float speed = 0.001;
		float randSpeed = 0.4;
		float friction = 0.99;

		static int time = 0;

		for (auto&& part : particles) {
			if (active) {
				time++;
				
				int x = part.pos.x;
				int y = part.pos.y;
				int z = part.pos.z;
				
				if (isInisde(x, y, z)) {
					part.update(env[x][y][z], time);
					part.spd = part.spd + env[x][y][z].dir * speed + vecRand() * speed * randSpeed;
				}
				else {
					part.spd = (Point3f(containerSize / 2, containerSize / 2, containerSize / 2) - part.pos).normalize() 
							* speed;
				}
				
				part.spd = part.spd * friction;
				part.pos = part.pos + part.spd;
			}
		}
		auto it = std::begin(particles);
		
		while (it != std::end(particles)) {
		    if ((*it).isGone())
		        it = particles.erase(it);
		    else
		        ++it;
		}
	}

	void initField() {
		for (int i = 0; i < containerSize; i++) {
			for (int j = 0; j < containerSize; j++) {
				for (int k = 0; k < containerSize; k++) {
					float center = containerSize / 2;

					Point3f pos = Point3f(i - center, j - center, k - center);

					env[i][j][k].dir = (pos * -1).normalize();
				}
			}
		}
	}

	bool isInisdeField (int x, int y, int z) {
		return x > 0 && y > 0 && z > 0 && x < containerSize && y < containerSize && z < containerSize;
	}

	void createTorusField() {

		auto isInisde = [this](int x, int y, int z) {
			return x > 0 && y > 0 && z > 0 && x < containerSize && y < containerSize && z < containerSize;
		};

		float pi = 3.141592653589;
		float n = 100;
		float div = pi * 2 / n;
		float a = 3;
		for (float a = 1; a < 7; a += 1) {
			float b = 6;
			float c = 6;
			for (float u = 0; u < 2 * pi; u += div) {
				for (float v = 0; v < 2 * pi; v += div) {
					float center = containerSize / 2;
					Point3f center3f(center, center, center);

					Point3f origin = Point3f(
						(c + a * cos(v)) * cos(u),
						(c + a * cos(v)) * sin(u),
						b * sin(v)
					);
					Point3f vec = (
						Point3f(
							(c + a * cos(v + div)) * cos(u),
							(c + a * cos(v + div)) * sin(u),
							b * sin(v + div)
						)
						- origin
					).normalize();
					torus.addPoint(origin + center3f);
					torus.addLine(origin + center3f, origin + center3f + vec, 
							(vec + Point3f(1, 1, 1)) / 2);

					int x = (origin + center3f).x;
					int y = (origin + center3f).y;
					int z = (origin + center3f).z;

					if (isInisdeField(x, y, z)) {
						env[x][y][z].dir = vec;	
					}
				}
			}
		}
	}

	void initFieldGraphics() {
		for (int i = 0; i < containerSize; i++) {
			for (int j = 0; j < containerSize; j++) {
				for (int k = 0; k < containerSize; k++) {
					containerFieldGraphic.addSphere(0.1, 4, env[i][j][k].dir,
							Matrix4f::returnTranslationMatrix(Point3f(i, j, k)));
					
					containerFieldGraphic.addLine( Point3f(0, 0, 0), env[i][j][k].dir, Point3f(1, 1, 0),
							Matrix4f::returnTranslationMatrix(Point3f(i, j, k)));
				}
			}
		}
	}

	void init() {
		initField();
		createTorusField();

		for (auto&& part : particles) {
			part.pos = Point3f(containerSize / 2, containerSize / 2, containerSize / 2);
			part.spd = Point3f(0, 0, 0);
		}
		
		initFieldGraphics();
		sphere.addSphere(0.1, 10, Point3f(0, 0, 0));

		sphere.initGLMesh();
		containerFieldGraphic.initGLMesh();
		torus.initGLMesh();
		
	}

	void drawParticles(ShaderProgram& prog) {
		for (auto&& part : particles) {
			prog.setVector("uColor", part.status);
			GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(part.pos));
				sphere.draw(prog);
			GameMesh::popMatrix();
		}
	}

	void drawTorus(ShaderProgram& prog) {
		prog.setVector("uColor", Point3f());
		torus.draw(prog);
	}

	void drawNodes(ShaderProgram& prog) {
		prog.setVector("uColor", Point3f());
		containerFieldGraphic.draw(prog);
	}
};

#endif