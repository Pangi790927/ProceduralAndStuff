#ifndef GAME_H
#define GAME_H

#include "GameMesh.h"
#include "FPSCounter.h"
#include "TimeInterval.h"

#include "ProceduralMap.h"
#include "ProceduralTexture.h"
#include "ProceduralSound.h"
#include "Sound.h"

#define TESTING_PROCEDURAL_MAP 1
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

	SoundSource soundSource;

	Point3f lightDir = Point3f(-1.0, -3.0, -2.0).normalize();

	int width;
	int height;

	Point3f mousePos;
	Point3f camPos;

#if TESTING_PROCEDURAL_MAP
	ProceduralMap<> gameProcMap;
#endif
	ProceduralTexture gameProcTex;
	ProceduralSound gameProcSound;

	FPSCounter fpsCount;

	Game() 
#if TESTING_PROCEDURAL_MAP
	: gameProcMap(100, 9, Point2f(0, 0)), gameProcTex(256, 256)
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

		// 				// do re mi fa so la si do 
		// 				// A  B  C  D  E  F  G  A
		
		// auto notes = [](int note) {
		// 	return pow(2.0, (note - 49) / 12.0) * 440;
		// };

		// int DO = 37;
		// int RE = 39;
		// int MI = 40;
		// int FA = 42;
		// int SO = 44;
		// int LA = 45;
		// int SI = 47;
		// int SILENT = -1;

		// int note[] = {DO, RE, MI, FA, SO, LA, SI};
		// unsigned char s = 255;	// silent

		// unsigned char doremiSong[] = {
		// 	1, 2, 3, 4, 5, 6, 7, 1
		// };

		unsigned char s = 255;
		unsigned char yesterday[] = {
			2, 1, 1, s, s, s, s, s,		s, s, 3, 4, 5, 6, 7, 8,
			7, 6, 6, s, s, s, s, s,		s, s, 6, 6, 5, 4, 3, 2,
			4, s, 3, 3, s, s, 2, s,		1, s, 3, 2, s, s, 6, s,
			1, s, 3, 3, s, s, s, s,		3, s, s, s, 3, s, s, s,
			6, s, 7, s, 8, s, 7, 6,		7, s, s, 6, 5, s, 6, 3,
			s, s, s, s, s, s, s, s,		3, s, s, s, 3, s, s, s 	
		};

		// char S = -1;
		// char keyStrokes1[] = {
		// 	30, S, S, S, S, 31, S, S, S, S, 32, S, S, S, S, 33,
		// 	34, S, S, S, S, 35, S, S, S, S, 36, S, S, S, S, 37,
		// 	38, S, S, S, 39, S, S, S, 40, S, S, S, 41,
		// 	42, S, S, S, 43, S, S, S, 44, S, S, S, 45,
		// 	46, S, S, 47, S, S, 48, S, S, 49,
		// 	50, S, S, 51, S, S, 52, S, S, 53
		// };

		// auto playSongDoToSi = [&](unsigned char* song, int size) {
		// 	for (int i = 0; i < size; i++) {
		// 		if (song[i] != 255)
		// 			Sound::playNote(notes(note[song[i]]), 1.5);
				
		// 		usleep(125 * 1000);
		// 	}
		// };

		// auto playSong = [&](char* song, int size) {
		// 	for (int i = 0; i < size; i++) {
		// 		if (song[i] != -1)
		// 			Sound::playNote(notes(song[i]), 1.5);
				
		// 		usleep(125 * 1000);
		// 	}
		// };

		Sound::init();
			
		// gameProcSound.addNote(0, 440, 1.5);
		gameProcSound.addSong(yesterday, sizeof(yesterday));
		gameProcSound.createBuffer();

		float pos[] = {0, 0, 0};
		float vel[] = {0, 0, 0};
		float orient[] = {0, 0, -1, 0, 1, 0};
		soundSource.createSource(gameProcSound, pos, vel, orient);
		
		soundSource.play();

		// // playSongDoToSi(doremiSong, sizeof(doremiSong));
		// playSongDoToSi(yesterday, sizeof(yesterday));
		// // playSong(keyStrokes1, sizeof(keyStrokes1));

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

		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++) {
				gameTestObjs.addSphere(0.5, 50, Point3f(0, 1 * ((i + j % 2) % 2), 0), 
						Matrix4f::returnTranslationMatrix(Point3f(i, 0, j)));
			}

		mySphere.addSphere(0.1, 30, Point3f(1, 0, 0));

		square.addSquare(10, Point3f(0.5, 0.5, 1), Matrix4f::returnRotationMatrix(3.1415 / 2, 1, 0, 0));

		cursor.initGLMesh();
		gameTestObjs.initGLMesh();
		mySphere.initGLMesh();
		square.initGLMesh();

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

		#if TESTING_PROCEDURAL_MAP
			gameMapShader.useProgram();
			gameMapShader.setMatrix("projectionMatrix", proj);
			gameMapShader.setMatrix("viewMatrix", view);
			gameMapShader.setVector("lightDir", lightDir);

			gameMapShader.setMatrix("worldMatrix", GameMesh::topMatrixStack());
			gameProcTex.bind();
			gameProcMap.draw(gameMapShader);
			
			gameMapShader.disableProgram();
		#endif

		#if TESTING_PROCEDURAL_TEXTURE
			gameTexShader.useProgram();
			gameTexShader.setMatrix("projectionMatrix", proj);
			gameTexShader.setMatrix("viewMatrix", view);
			gameTexShader.setVector("lightDir", lightDir);

			// gameProcTex.bind();
			gameProcTex.bind();
			square.draw(gameTexShader);
			gameTexShader.disableProgram();
		#endif

		gameDefaultShader.useProgram();
		gameDefaultShader.setMatrix("projectionMatrix", proj);
		gameDefaultShader.setMatrix("viewMatrix", view);
		gameDefaultShader.setVector("lightDir", lightDir);

		GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(camPos.x, 0, camPos.z));
			mySphere.draw(gameDefaultShader);	// walks with player
		GameMesh::popMatrix();
		GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(15, 0, 0));
			gameTestObjs.draw(gameDefaultShader);
		GameMesh::popMatrix();
		// GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(0, 10, 0));
		// 	gameProcSound.draw(gameDefaultShader);
		// 	GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(soundSource.getStatef(AL_SEC_OFFSET), 0, 0));
		// 		std::cout << soundSource.getStatef(AL_SEC_OFFSET) << std::endl;
		// 		mySphere.draw(gameDefaultShader);
		// 	GameMesh::popMatrix();
		// GameMesh::popMatrix();

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