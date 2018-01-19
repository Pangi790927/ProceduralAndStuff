#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Options.h"
Options opts;

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

	std::vector<int> partTimeline;
	std::vector<float> foodTimeline;
	std::vector<float> ageTimeline;
	std::vector<float> diseaseTimeline;

	Simulator() {}

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

		if (active) {
			// std::cout << "-------------------------------------------" << std::endl;
			// std::cout << "food: " << particles[24].food << std::endl;
			// std::cout << "disease: " << particles[24].disease << std::endl;
			// std::cout << "age: " << particles[24].age << std::endl;
			// std::cout << "dead: " << particles[24].dead << std::endl;
			// std::cout << "-------------------------------------------" << std::endl;
			
			time++;
			gatherStatistics();
			for (auto&& part : particles) {
				
				int x = part.pos.x;
				int y = part.pos.y;
				int z = part.pos.z;
				
				if (isInisde(x, y, z)) {
					if (part.update(env[x][y][z], time)) {
						auto newPart = Particle();
						newPart.pos = part.pos + vecRand() * 2;
						particles.push_back(newPart);
					}
					part.spd = part.spd + env[x][y][z].dir * speed + vecRand() * speed * randSpeed;
				}
				else {
					part.spd = (Point3f(containerSize / 2, containerSize / 2, containerSize / 2)
								- part.pos).normalize() * speed;
				}
				
				part.spd = part.spd * friction;
				part.pos = part.pos + part.spd;
			}
		}
		auto it = std::begin(particles);
		
		std::vector<Particle> newParticles;
		for (auto&& part : particles) {
			if (!part.isGone()) {
				newParticles.push_back(part);
			}
		}
		particles = newParticles;
	}

	void gatherStatistics() {
		int aliveCount = 0;
		float foodProc;
		float diseaseProc;
		float ageProc;
		for (auto&& part : particles) {
			aliveCount += !part.dead;
			if (!part.dead) {
				foodProc += part.foodProc();
				diseaseProc += part.diseaseProc();
				ageProc += part.ageProc();
			}
		}
		partTimeline.push_back(aliveCount);
		foodTimeline.push_back(foodProc / float(aliveCount));
		diseaseTimeline.push_back(diseaseProc / float(aliveCount));
		ageTimeline.push_back(ageProc / float(aliveCount));
	}

	void init() {
		opts.loadOptions("config.cfg");
		Particle::init();
		EnvNode::init();
		particles = std::vector<Particle> (opts["INITIAL_COUNT"]);

		initField();
		createTorusField();

		for (auto&& part : particles) {
			part.pos = Point3f(containerSize * 0.5
					, containerSize * 0.5
					, containerSize * 0.5);
			part.spd = Point3f(0, 0, 0);
		}
		
		initFieldGraphics();
		sphere.addSphere(0.1, 10, Point3f(0, 0, 0));

		sphere.initGLMesh();
		containerFieldGraphic.initGLMesh();
		torus.initGLMesh();
		
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


	void createOtherField() {

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
						(cos(u)) * sin(2 * v) * b,
						(sin(u)) * sin(2 * v) * b,
						sin(v) * b
					);
					Point3f vec = (
						Point3f(
							(cos(u)) * sin(2 * (v + div)) * b,
							(sin(u)) * sin(2 * (v + div)) * b,
							sin(v + div) * b
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

	template <typename Type>
	void plotAt (float x, float y, float scale, Point3f color, std::vector<Type> &toPlot, float detail = 400) {
		if (toPlot.size() > detail) {
			for (int i = 1; i < detail; i++) {
				int indexLast = (toPlot.size() - 1) * ((i - 1) / detail);
				int index = (toPlot.size() - 1) * (i / detail);

				glBegin(GL_LINES);
					glColor3f(color.x, color.y, color.z);
					glVertex3f(x + (i - 1) / detail, y + toPlot[indexLast] / scale, 0);
					glVertex3f(x + (i) / detail, y + toPlot[index] / scale, 0);
				glEnd();
			}
		}
	}

	void drawTimeLine (ShaderProgram& prog) {
		prog.setVector("uColor", Point3f());
		GameMesh::pushMatrix(Matrix4f::returnIdentityMatrix());
			float maxTime = 1;
			
			for (auto&& timeLoc : partTimeline) {
				maxTime = std::max(maxTime, (float)timeLoc);
			}

			plotAt(0, 0, maxTime,	Point3f(0, 1, 1), partTimeline);
			plotAt(0, 0, 1,			Point3f(1, 0, 0), ageTimeline);
			plotAt(0, 0, 1,			Point3f(0, 1, 0), foodTimeline);
			plotAt(0, 0, 1,			Point3f(0, 0, 1), diseaseTimeline);

			glBegin(GL_LINES);
				glColor3f(0, 0, 0);
				glVertex3f(0, 0, 0);
				glVertex3f(1, 0, 0);
			glEnd();
		GameMesh::popMatrix();
	}

	void drawParticles (ShaderProgram& prog) {
		for (auto&& part : particles) {
			prog.setVector("uColor", part.status);
			GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(part.pos));
				sphere.draw(prog);
			GameMesh::popMatrix();
		}
	}

	void drawTorus (ShaderProgram& prog) {
		prog.setVector("uColor", Point3f());
		torus.draw(prog);
	}

	void drawNodes (ShaderProgram& prog) {
		prog.setVector("uColor", Point3f());
		containerFieldGraphic.draw(prog);
	}
};

#endif