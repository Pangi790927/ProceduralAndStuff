#ifndef GAME_H
#define GAME_H

#include "GameMesh.h"
#include "FPSCounter.h"
#include "TimeInterval.h"

#include "ProceduralMap.h"
#include "ProceduralTexture.h"
#include "ProceduralSound.h"
#include "Sound.h"

#include "Simulator.h"

#define TESTING_PROCEDURAL_MAP 0
#define TESTING_PROCEDURAL_TEXTURE 1

class Game {
public:
	GameMesh cursor;
	GameMesh mySphere;
	GameMesh gameTestObjs;
	GameMesh square;

	ShaderProgram gameMenuShader;
	ShaderProgram gameDefaultShader;
	ShaderProgram gameMapShader;
	ShaderProgram gameTexShader;

	Simulator simulator;

	SoundSource soundSource;
	Point3f lightDir = Point3f(-1.0, -3.0, -2.0).normalize();

	int width;
	int height;

	Point3f mousePos;
	Point3f camPos;

#if TESTING_PROCEDURAL_MAP
	ProceduralMap<> gameProcMap;
#endif
#if TESTING_PROCEDURAL_TEXTURE
	ProceduralTexture gameProcTex;
#endif
	ProceduralSound gameProcSound;

	FPSCounter fpsCount;

	Game() 
	: width(2)
#if TESTING_PROCEDURAL_MAP
	, gameProcMap(100, 9, Point2f(0, 0))
#endif
#if TESTING_PROCEDURAL_TEXTURE
	, gameProcTex(256, 256)
#endif
	{}

	ShaderProgram loadShader(std::string shaderName) {
		try {
			ShaderProgram prog = ShaderProgram({
				std::make_pair(GL_VERTEX_SHADER, shaderName + ".vs"),
				std::make_pair(GL_FRAGMENT_SHADER, shaderName + ".fs")
			});

			prog.linkProgram();	/// linking must be separated
			return prog;
		}
		catch (const std::exception& except) {
			std::cout << "Exception occured " << except.what() << std::endl;
			return ShaderProgram();
		}
	}

	void init() {
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_TEXTURE_2D);
 
		gameMenuShader = loadShader("Shaders/gameMenuShader");
		gameMapShader = loadShader("Shaders/gameMapShader");
		gameDefaultShader = loadShader("Shaders/defaultShader");
		gameTexShader = loadShader("Shaders/textureShader");
		// cursor.addSquare(0.01, Point3f(1, 0, 1), Matrix4f::returnScaleMatrix(20, 1, 1));
		// cursor.addSquare(0.01, Point3f(1, 0, 1), Matrix4f::returnScaleMatrix(1, 20, 1));
		
		cursor.addLine(Point3f(0, 0.1, 0), Point3f(0, -0.1, 0), Point3f(1, 0, 0));
		cursor.addLine(Point3f(0.1, 0, 0), Point3f(-0.1, 0, 0), Point3f(1, 0, 0));
		
		// gameTestObjs.addSphere(10, 300, Point3f(1, 0, 1));

		gameTestObjs.addLine(Point3f(0, 0, 0), lightDir * -100);
		gameTestObjs.addCube(1, Point3f(1, 1, 0),
				Matrix4f::returnTranslationMatrix(lightDir * -100));

		// gameTestObjs.addCube(1, Point3f(0, 0.5, 0.5));

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				gameTestObjs.addSphere(0.5, 50, Point3f(0, 1 * ((i + j % 2) % 2), 0), 
						Matrix4f::returnTranslationMatrix(Point3f(i, 0, j)));
			}
		}

		simulator.init();

		mySphere.addSphere(0.1, 10, Point3f(1, 0, 0));
		square.addSquare(10, Point3f(0.5, 0.5, 1), Matrix4f::returnRotationMatrix(3.1415 / 2, 1, 0, 0));

		cursor.initGLMesh();
		gameTestObjs.initGLMesh();
		square.initGLMesh();
		mySphere.initGLMesh();

		#if TESTING_PROCEDURAL_MAP
			gameProcMap.initDraw();
		#endif

		#if TESTING_PROCEDURAL_TEXTURE
 			gameProcTex.createRandTexture();
		#endif

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void input (Camera &camera, Keyboard &keyboard, Mouse &mouse
			, bool hiddenCursor, int width, int height)
	{
		this->width = width;
		this->height = height;

		mousePos = Mouse::transformeMousePos(mouse.getPosition(), width, height);
		camPos = camera.pos;

		if (keyboard.wasPressedOnce('k') | keyboard.wasPressedOnce('K')) {
			simulator.active = true;
		}

		if (keyboard.wasPressedOnce('l') | keyboard.wasPressedOnce('L')) {
			simulator.active = false;
		}

		cursor.pos = mousePos;
	}

	void gameLogic() {
		#if TESTING_PROCEDURAL_MAP
			gameProcMap.update(Point2f(camPos.x, camPos.z)); 
		#endif
		// std::cout << QuadNode<DefaultProceduralChoser>::quadCount << std::endl;
	}

	void draw (Matrix4f &proj, Matrix4f &view) {
		gameMenuShader.useProgram();

//		we load identity for menu
		gameMenuShader.setMatrix("projectionMatrix", Matrix4f::returnIdentityMatrix());
		gameMenuShader.setMatrix("viewMatrix", Matrix4f::returnIdentityMatrix());

		// cursor.draw(gameMenuShader);

		gameMenuShader.disableProgram();

		gameDefaultShader.useProgram();
		gameDefaultShader.setMatrix("projectionMatrix", proj);
		gameDefaultShader.setMatrix("viewMatrix", view);
		gameDefaultShader.setMatrix("worldMatrix", Matrix4f::returnIdentityMatrix());
		gameDefaultShader.setVector("lightDir", lightDir);

		for (int i = 0; i < 10; i++)
			simulator.update();

		simulator.drawParticles(gameDefaultShader);
		// simulator.drawTorus(gameDefaultShader);
		// simulator.drawNodes(gameDefaultShader);

		gameDefaultShader.disableProgram();

		if (fpsCount.hasUpdated()) {
			std::cout << "FPS: " << fpsCount.getCount() << std::endl;
			fpsCount.resetUpdate();
		}
		else {
			fpsCount.getCount();
		}
	}

	~Game() {
		Sound::free();
	}
};

#endif