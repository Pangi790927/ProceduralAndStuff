#ifndef MAIN_H
#define MAIN_H

#include "GameMesh.h"
#include "FPSCounter.h"
#include "TimeInterval.h"
#include "Collider.h"

#include "ProceduralTexture.h"

class Main {
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

	std::vector<Point3f> nodes;
	std::vector<Point3f> nodesSpeed;

	Point3f mousePos;
	Point3f camPos;

	ProceduralTexture gameProcTex;

	FPSCounter fpsCount;

	float randNorm() {
		return rand() / float(RAND_MAX);
	}

	Point3f randVec (float x, float y, float z) {
		return Point3f(
			(randNorm() * 2 - 1.0f) * x,
			(randNorm() * 2 - 1.0f) * y,
			(randNorm() * 2 - 1.0f) * z
		);
	}

	void drawLine (Point3f a, Point3f b, Point3f color = Point3f(0, 0, 0)) {
		glBegin(GL_LINES);
			glColor3fv(color.array);
			glVertex3fv(a.array);
			glVertex3fv(b.array);
		glEnd();
	}

	void update (Point3f pos = Point3f(0.3, 0.3, 0)) {
		static Point3f g = Point3f(0, -0.01, 0);
		static float friction = 1;

		static float retractSpeed = 0.05;
		static float maxLenght = 0.05;

		/// apply forces
		
		nodes[0] = pos;
		for (int i = 1; i < nodesSpeed.size(); i++) {
			nodesSpeed[i] = nodesSpeed[i] + g - nodesSpeed[i] * friction;
		}

		/// apply restraints
		auto nodesCopy = nodes;
		for (int i = 1; i < nodes.size(); i++) {
			Point3f dir = nodes[i] - nodes[i - 1];
			float dist = dir.abs();
			float toAdd = std::max(std::min(1.0f / dist * (dist - maxLenght), 0.3f), -0.3f);
			if (dist > maxLenght)
				nodesSpeed[i] = nodesSpeed[i] - toAdd * dir;
			else if (dist < maxLenght)
				nodesSpeed[i] = nodesSpeed[i] + toAdd * dir;
		}

		for (int i = 1; i < nodes.size(); i++) {
			

			Point3f A = nodes[i];
			Point3f B = nodes[i] + nodesSpeed[i];
			Point3f C = Point3f(0, 0, 0);

			if ((B - C).abs() > 0.1)
			{
				nodes[i] = nodes[i] + nodesSpeed[i];	
			}
		}
	}

	void drawNodes() {
		for (int i = 1; i < nodes.size(); i++) {
			float lenght = (nodes[i] - nodes[i - 1]).abs();
			std::cout << lenght << std::endl;
			drawLine(nodes[i], nodes[i - 1], Point3f(lenght / 100.0, 0, 0));
		}
	}

	Main() 
	: width(2)
	, gameProcTex(256, 256)
	{
		for (int i = 0; i < 20; ++i) {
			nodes.push_back(randVec(1, 1, 0));
			nodesSpeed.push_back(Point3f(0, 0, 0));
		}
	}

	~Main() {
	}

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
		update(cursor.pos);
	}

	void draw (Matrix4f &proj, Matrix4f &view) {
		/// 2D Menu shader
		gameMenuShader.useProgram();
		gameMenuShader.setMatrix("projectionMatrix", Matrix4f::returnIdentityMatrix());
		gameMenuShader.setMatrix("viewMatrix", Matrix4f::returnIdentityMatrix());
		gameMenuShader.setMatrix("worldMatrix", Matrix4f::returnIdentityMatrix());
			// cursor.draw(gameMenuShader);
			drawNodes();
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
};

#endif