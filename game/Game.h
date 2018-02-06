#ifndef GAME_H
#define GAME_H

#include "GameMesh.h"
#include "FPSCounter.h"
#include "TimeInterval.h"

#include "ProceduralTexture.h"

class Game {
public:
	GameMesh cursor;
	GameMesh square;
	GameMesh gameTestObjs;

	ShaderProgram gameMenuShader;
	ShaderProgram gameDefaultShader;
	ShaderProgram gameTexShader;

	Point3f lightDir = Point3f(1.0, 3.0, 2.0).normalize();

	int width;
	int height;

	Point3f mousePos;
	Point3f camPos;

	ProceduralTexture gameProcTex;

	FPSCounter fpsCount;

	Game() 
	: width(2)
	, gameProcTex(256, 256)
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
		gameDefaultShader = loadShader("Shaders/defaultShader");
		gameTexShader = loadShader("Shaders/textureShader");

		cursor.addLine(Point3f(0, 0.1, 0), Point3f(0, -0.1, 0), Point3f(1, 0, 0));
		cursor.addLine(Point3f(0.1, 0, 0), Point3f(-0.1, 0, 0), Point3f(1, 0, 0));
		
		gameTestObjs.addLine(Point3f(0, 0, 0), lightDir * -100);
		gameTestObjs.addCube(1, Point3f(1, 1, 0),
				Matrix4f::returnTranslationMatrix(lightDir * -100));

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				gameTestObjs.addSphere(0.5, 50, Point3f(0, 1 * ((i + j % 2) % 2), 0), 
						Matrix4f::returnTranslationMatrix(Point3f(i, 0, j)));
			}
		}

		square.addSquare(10, Point3f(0.5, 0.5, 1), Matrix4f::returnRotationMatrix(3.1415 / 2, 1, 0, 0));

		cursor.initGLMesh();
		gameTestObjs.initGLMesh();
		square.initGLMesh();
		
		gameProcTex.createRandTexture();
		
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
	}

	void draw (Matrix4f &proj, Matrix4f &view) {
		/// 2D Menu shader
		gameMenuShader.useProgram();
		gameMenuShader.setMatrix("projectionMatrix", Matrix4f::returnIdentityMatrix());
		gameMenuShader.setMatrix("viewMatrix", Matrix4f::returnIdentityMatrix());
		gameMenuShader.setMatrix("worldMatrix", Matrix4f::returnIdentityMatrix());
			// cursor.draw(gameMenuShader);
		gameMenuShader.disableProgram();

		/// 3D texture shader
		gameTexShader.useProgram();
		gameTexShader.setMatrix("projectionMatrix", proj);
		gameTexShader.setMatrix("viewMatrix", view);
		gameTexShader.setVector("lightDir", lightDir);
			gameProcTex.bind();
			square.draw(gameTexShader);
		gameTexShader.disableProgram();

		/// 3D default shader
		gameDefaultShader.useProgram();
		gameDefaultShader.setMatrix("projectionMatrix", proj);
		gameDefaultShader.setMatrix("viewMatrix", view);
		gameDefaultShader.setMatrix("worldMatrix", Matrix4f::returnIdentityMatrix());
		gameDefaultShader.setVector("lightDir", lightDir);
			GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(15, 0, 0));
				gameTestObjs.draw(gameDefaultShader);
			GameMesh::popMatrix();
		gameDefaultShader.disableProgram();

		/// counting fps
		if (fpsCount.hasUpdated()) {
			std::cout << "FPS: " << fpsCount.getCount() << std::endl;
			fpsCount.resetUpdate();
		}
		else {
			fpsCount.getCount();
		}
	}

	~Game() {
	}
};

#endif