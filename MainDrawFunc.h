#ifndef MAINDRAWFUNC_H_INCLUDED
#define MAINDRAWFUNC_H_INCLUDED 

#include <GL/glx.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "DrawFunction.h"
#include "MathLib.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "OBJLoader.h"
#include "FixedFunctionMeshDraw.h"
#include "Game.h"

typedef Vertex <
	VertexAttribute <
		int, 		VertexTexID				, VertexAttribute <
		Point2f, 	VertexTexCoord			, VertexAttribute <
		Point3f, 	VertexNormal			, VertexAttribute <
		Point3f, 	VertexPosition			, VertexAttributeEnd
		// Point4f, 	VertexColor				, VertexAttributeEnd	
> > > > >
ObjLoaderVertexType;

class MainDrawFunc : public DrawFunction {
public:
	MainDrawFunc(OpenGLWindow &window) : DrawFunction(window) {}

	Game game;

	Camera camera;
	float cammeraSpeed = 0.5;
	float cammeraSpeedBoost = 0.6;
	float cammeraAngSpeed = 1;

	// ShaderProgram shaderProgram;
	// Mesh<ObjLoaderVertexType> myMesh;

	// OBJLoader<ObjLoaderVertexType> objLoader;
	
	void rescale() {
		glViewport(0, 0, window.width, window.height);
	}

	/// definding what goes into init(); and will be called only once, after all the initializing is 
	virtual void initScene() override {
		// glEnable(GL_TEXTURING);
		glEnable(GL_TEXTURE_2D);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			ERROR_PRINT(std::string((char*)glewGetErrorString(err)), 0, 0);
		}
		debugPrint << glGetString(GL_VERSION) << std::endl;

		/// creating the shader
		game.init();

		// try {
		// 	shaderProgram = ShaderProgram({
		// 		std::make_pair(GL_VERTEX_SHADER, "defaultShader.vs"),
		// 		std::make_pair(GL_FRAGMENT_SHADER, "defaultShader.fs")
		// 	});

		// 	shaderProgram.linkProgram();	/// linking must be separated
		// }
		// catch (const std::exception& except) {
		// 	ERROR_PRINT(std::string(except.what()), 0, 0);
		// }

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POLYGON_SMOOTH );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0, 1.0, 1.0, 1.0);

		// must be reverse for camera
		camera.initialTransformation = Matrix4f::returnTranslationMatrix(0, -10, 0);

		// window.makeFullscreen();

		rescale();

		// myMesh = objLoader.loadMesh("3D objects/carObj/", "sls_amg_obj.obj");
		// myMesh = objLoader.loadMesh("3D objects/Ivy/", "IVY_OBJ.obj");	 Unclear texture in mtl
		// myMesh = objLoader.loadMesh("3D objects/Bush/", "Bush1.obj");
		// myMesh = objLoader.loadMesh("3D objects/Moskvitch/", "Moskvitch.obj");
		// myMesh = objLoader.loadMesh("3D objects/Grumman F-14 Tomcat/", "f14d.obj"); // -- unclear texture mapping
		// myMesh = objLoader.loadMesh("3D objects/Blender Nature Asset/", "BlenderNatureAsset.obj");Grass_01
		// myMesh = objLoader.loadMesh("3D objects/Grass pack/", "Grass_02.obj");
	}

	virtual void onRescale() override {
		rescale();
	}

	void inputHandle() {
		/// exit button
		if (window.keyboard.wasPressedOnce(Keyboard::ESC)) {
			/// maybe prompt for are you sure ?
			window.closeAnounce();
		}

		camera.speed = cammeraSpeed;
		camera.rotationSpeed = cammeraAngSpeed;

		static bool blockCursorMovement = false;
		if (window.mouse.mouseLeftOnce()) {
			if (!window.isCursorHidden()){
				window.hideCursor();
				blockCursorMovement = true;
			}
			else {
				window.showCursor();
				blockCursorMovement = false;
			}
		}

		static Mouse fakeMause;
		fakeMause.position = Point2i(window.width / 2, window.height / 2);

		if (blockCursorMovement)
			window.moveMouseTo(window.width / 2, window.height / 2);

		/// will only use this camera type if the cursor is hidden (in center of the screen)
		if (window.isCursorHidden()){
			camera.setInput(window.keyboard, window.mouse, window.width, window.height);
		}
		else {
			camera.setInput(window.keyboard, fakeMause, window.width, window.height);
		}

		game.input(camera, window.keyboard, window.mouse, window.isCursorHidden()
				, window.width, window.height);
	}

	virtual void mainLoop() override {
		/// will call initScene only once inside the main loop in the first iteration of the loop
		init();

		/// will handle input
		inputHandle();

		game.gameLogic();

		/// compute matrix transforms
		Matrix4f projectionMatrix =
				Matrix4f::returnProjectionMatrix(55.0f, 1.0f * window.width / window.height, 0.1, 100000);

		Matrix4f viewMatrix = camera.getViewTransform();
		Matrix4f worldMatrix = Matrix4f::returnScaleMatrix(0.1);	/// this should be per object

		// shaderProgram.useProgram();
		// shaderProgram.setMatrix("projectionMatrix", projectionMatrix);
		// shaderProgram.setMatrix("viewMatrix", viewMatrix);
		// shaderProgram.setMatrix("worldMatrix", Matrix4f::returnIdentityMatrix());

		// shaderProgram.setVector("secondColor", Vector4f(0.5, 0.5, 0.5, 1.0f));

		/// starting the rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// float z_coord = -2;

		// Matrix4f transformMatrix = Matrix4f::returnIdentityMatrix();

		// Point3f A = transformMatrix * Point3f(-1, -1, z_coord);
		// Point3f B = transformMatrix * Point3f( 1, -1, z_coord);
		// Point3f C = transformMatrix * Point3f( 1,  1, z_coord);
		// Point3f D = transformMatrix * Point3f(-1,  1, z_coord);

		// FixedFunctionMeshDraw::draw<ObjLoaderVertexType>(myMesh, shaderProgram);

		game.draw(projectionMatrix, viewMatrix);
		// std::cout << Util::getOpenGLError() << std::endl;

		// glBegin(GL_QUADS);
		// 	glColor3f(1., 0., 0.);
		// 		glVertex3fv(A.array);
			
		// 	glColor3f(0., 1., 0.);
		// 		glVertex3fv(B.array);

		// 	glColor3f(0., 0., 1.);
		// 		glVertex3fv(C.array);

		// 	glColor3f(1., 1., 0.);
		// 		glVertex3fv(D.array);
		// glEnd();

		// glColor3f(0.5, 0.5, 0.5);
		// float size = 100;
		// glBegin(GL_QUADS);
		// 	glVertex3fv((transformMatrix * Point3f(-size, 0, -size)).array);
		// 	glVertex3fv((transformMatrix * Point3f( size, 0, -size)).array);
		// 	glVertex3fv((transformMatrix * Point3f( size, 0,  size)).array);
		// 	glVertex3fv((transformMatrix * Point3f(-size, 0,  size)).array);
		// glEnd();
	}
};

#endif