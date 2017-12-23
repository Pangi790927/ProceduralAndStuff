#ifndef GAME_MESH_H_INCLUDED
#define GAME_MESH_H_INCLUDED

#include <stack>

#include "DeprecatedVBOMeshDraw.h"

class GameMesh {
public: 
	typedef Vertex <
		VertexAttribute <
			int, 		VertexTexID				, VertexAttribute <
			Point2f, 	VertexTexCoord			, VertexAttribute <
			Point3f, 	VertexNormal			, VertexAttribute <
			Point3f, 	VertexPosition			, VertexAttribute <
			Point4f, 	VertexColor				, VertexAttributeEnd	
	> > > > > >
	GameVertexType;

	Mesh <GameVertexType> mesh;
	DeprecatedVBOMeshDraw glMesh;
	Point3f pos;
	Matrix4f rotation = Matrix4f::returnIdentityMatrix();

	static std::stack<Matrix4f> matrixStack;

	static void pushMatrix(Matrix4f transf) {
		matrixStack.push(matrixStack.top() * transf);
	}

	static void popMatrix() {
		matrixStack.pop();
	}

	static Matrix4f& topMatrixStack() {
		static std::once_flag flag;
		std::call_once(flag, [&](){ 
			matrixStack.push(Matrix4f::returnIdentityMatrix()); 
		});

		return matrixStack.top();
	}

	void initGLMesh() {
		glMesh.init(mesh);
	}

	void draw (ShaderProgram& shader) {
		Matrix4f transform = Matrix4f::returnTranslationMatrix(pos) * rotation;

		shader.setMatrix("worldMatrix", topMatrixStack() * transform);
		glMesh.draw(shader);
	}

	void addVert (Point3f pos, Point3f color, Point3f normal = Point3f(0, 0, 0)
			, Point2f tex = Point2f(0, 0)) {
		GameVertexType newVert;

		newVert.get<VertexPosition>() = pos;
		newVert.get<VertexColor>() = color;
		newVert.get<VertexNormal>() = normal;
		newVert.get<VertexTexCoord>() = tex;
		mesh.addVertex(newVert);
	}

	/// transf is additional transformation to the object to be added
	void addLine (Point3f A, Point3f B, 
			Point3f color = Point3f(1, 1, 1), Matrix4f transf = Matrix4f::returnIdentityMatrix()) 
	{
		int index = mesh.getVertCount();

		addVert(transf * A, color);
		addVert(transf * B, color);

		mesh.elementIndex.push_back(std::vector<int>{index + 0, index + 1});
	}

	void addTriangle (Point3f A, Point3f B, Point3f C, 
			Point3f color = Point3f(1, 1, 1), Matrix4f transf = Matrix4f::returnIdentityMatrix(),
			Point3f normA = Point3f(0, 0, 1), Point3f normB = Point3f(0, 0, 1), Point3f normC = Point3f(0, 0, 1))
	{
		int index = mesh.getVertCount();
	
		addVert(transf * A, color, transf.mult(normA));
		addVert(transf * B, color, transf.mult(normB));
		addVert(transf * C, color, transf.mult(normC));
	
		mesh.elementIndex.push_back(std::vector<int>{index + 0, index + 1, index + 2});
	}

	// 2d surfaces will all be on xy, z will be perpendicular on them
	void addSquare (float side,
			Point3f color = Point3f(1, 1, 1), Matrix4f transf = Matrix4f::returnIdentityMatrix(), 
			Point3f normal = Point3f(0, 0, 1)) 
	{
		int index = mesh.getVertCount();
	
		addVert(transf * Point3f(-side, -side, 0), color, transf.mult(normal), Point2f(0, 0));
		addVert(transf * Point3f( side, -side, 0), color, transf.mult(normal), Point2f(0, 1));
		addVert(transf * Point3f( side,  side, 0), color, transf.mult(normal), Point2f(1, 1));
		addVert(transf * Point3f(-side,  side, 0), color, transf.mult(normal), Point2f(1, 0));

		mesh.elementIndex.push_back(std::vector<int>{index + 0, index + 1, index + 2, index + 3});
	}

	void addCircle ( float radius, int complex,
			Point3f color = Point3f(1, 1, 1), Matrix4f transf = Matrix4f::returnIdentityMatrix()) 
	{
		float pi = 3.141592653;
		float div = 2 * pi / complex;
		for (float alpha = 0; alpha < pi * 2 - div / 2; alpha += div) {
			addLine(
				Point3f(cos(alpha), sin(alpha), 0),
				Point3f(cos(alpha + div), sin(alpha + div), 0),
				color, 
				transf
			);
		}
	}
	
	void addCube (float side, 
			Point3f color = Point3f(1, 1, 1), Matrix4f transf = Matrix4f::returnIdentityMatrix()) 
	{
		Matrix4f transform = Matrix4f::returnTranslationMatrix(0, 0, side);
		
		addSquare(side, color, transf * transform);
		addSquare(side, color, transf * Matrix4f::returnRotationMatrix( 3.1415926	, 1, 0, 0) * transform);
		addSquare(side, color, transf * Matrix4f::returnRotationMatrix( 3.1415926 / 2, 1, 0, 0) * transform);
		addSquare(side, color, transf * Matrix4f::returnRotationMatrix(-3.1415926 / 2, 1, 0, 0) * transform);
		addSquare(side, color, transf * Matrix4f::returnRotationMatrix( 3.1415926 / 2, 0, 1, 0) * transform);
		addSquare(side, color, transf * Matrix4f::returnRotationMatrix(-3.1415926 / 2, 0, 1, 0) * transform);
	}

	void addSphere (float radius, int complex, 
			Point3f color = Point3f(1, 1, 1), Matrix4f transf = Matrix4f::returnIdentityMatrix()) 
	{
		float pi = 3.141592653;
		float div = 2 * pi / complex;
		for (float alpha = 0; alpha < pi - div / 2; alpha += div) {
 			for (float beta = 0; beta < pi * 2 - div / 2; beta += div) {
				addTriangle(
					Point3f(	cos(alpha) * cos(beta), 
								sin(alpha) * cos(beta), 
								sin(beta)) * radius,
					Point3f(	cos(alpha + div) * cos(beta), 
								sin(alpha + div) * cos(beta), 
								sin(beta)) * radius,
					Point3f(	cos(alpha + div) * cos(beta + div), 
								sin(alpha + div) * cos(beta + div), 
								sin(beta + div)) * radius,
					color, 
					transf, 
					Point3f(	cos(alpha) * cos(beta), 
								sin(alpha) * cos(beta), 
								sin(beta)),
					Point3f(	cos(alpha + div) * cos(beta), 
								sin(alpha + div) * cos(beta), 
								sin(beta)),
					Point3f(	cos(alpha + div) * cos(beta + div), 
								sin(alpha + div) * cos(beta + div), 
								sin(beta + div))
				);

				addTriangle(
					Point3f(	cos(alpha + div) * cos(beta + div), 
								sin(alpha + div) * cos(beta + div), 
								sin(beta + div)) * radius,
					Point3f(	cos(alpha) * cos(beta + div), 
								sin(alpha) * cos(beta + div), 
								sin(beta + div)) * radius,
					Point3f(	cos(alpha) * cos(beta), 
								sin(alpha) * cos(beta), 
								sin(beta)) * radius,
					color, 
					transf,
					Point3f(	cos(alpha + div) * cos(beta + div), 
								sin(alpha + div) * cos(beta + div), 
								sin(beta + div)),
					Point3f(	cos(alpha) * cos(beta + div), 
								sin(alpha) * cos(beta + div), 
								sin(beta + div)),
					Point3f(	cos(alpha) * cos(beta), 
								sin(alpha) * cos(beta), 
								sin(beta))
				);
			}
		}
	}
};

std::stack<Matrix4f> GameMesh::matrixStack;

#endif