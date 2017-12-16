#ifndef GAME_H
#define GAME_H

#include "GameMesh.h"
#include "ProceduralMap.h"
#include "FPSCounter.h"
#include "TimeInterval.h"

class Game {
public:
	GameMesh cursor;
	GameMesh mySphere;
	GameMesh gameTestObjs;
	ShaderProgram gameMenuShader;
	ShaderProgram gameDefaultShader;
	ShaderProgram gameMapShader;

	Point3f lightDir = Point3f(-1.0, -3.0, -2.0).normalize();

	int width;
	int height;

	Point3f mousePos;
	Point3f camPos;
	ProceduralMap<> gameProcMap;

	FPSCounter fpsCount;

	Game() 
	: gameProcMap(30, 9, Point2f(0, 0))
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
			ERROR_PRINT(std::string(except.what()), 0, 0);
			return ShaderProgram();
		}
	}

	void init() {
		glEnable( GL_MULTISAMPLE );

		gameMenuShader = loadShader("gameMenuShader");
		gameMapShader = loadShader("gameMapShader");
		gameDefaultShader = loadShader("defaultShader");
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

		cursor.initGLMesh();
		gameTestObjs.initGLMesh();
		mySphere.initGLMesh();

		gameProcMap.initDraw();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
		gameProcMap.update(Point2f(camPos.x, camPos.z)); 
		// std::cout << QuadNode<DefaultProceduralChoser>::quadCount << std::endl;
	}

	void draw (Matrix4f &proj, Matrix4f &view) {
		gameMenuShader.useProgram();

//		we load identity for menu
		gameMenuShader.setMatrix("projectionMatrix", Matrix4f::returnIdentityMatrix());
		gameMenuShader.setMatrix("viewMatrix", Matrix4f::returnIdentityMatrix());

		cursor.draw(gameMenuShader);

		gameMapShader.useProgram();
		gameMapShader.setMatrix("projectionMatrix", proj);
		gameMapShader.setMatrix("viewMatrix", view);
		gameMapShader.setVector("lightDir", lightDir);

		gameMapShader.setMatrix("worldMatrix", GameMesh::topMatrixStack());
		gameProcMap.draw(gameMapShader);
		// gameProcMap.debugDraw();

		gameDefaultShader.useProgram();
		gameDefaultShader.setMatrix("projectionMatrix", proj);
		gameDefaultShader.setMatrix("viewMatrix", view);
		gameDefaultShader.setVector("lightDir", lightDir);

		GameMesh::pushMatrix(Matrix4f::returnTranslationMatrix(camPos.x, 0, camPos.z));
			// mySphere.draw(gameDefaultShader);
		GameMesh::popMatrix();

		// gameTestObjs.draw(gameDefaultShader);
 
		if (fpsCount.hasUpdated()) {
			std::cout << "FPS: " << fpsCount.getCount() << std::endl;
			fpsCount.resetUpdate();
		}
		else {
			fpsCount.getCount();
		}
	}
};

#endif