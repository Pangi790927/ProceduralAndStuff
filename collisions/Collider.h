#ifndef COLLIDER_H
#define COLLIDER_H

/// Point
/// Ray, Line(2xRay), Segment(2xRay)
/// Plane, Triangle, Disc
/// Sphere, Cylinder, Cone, Convex shape(made from multiple triangles)

/// Must still fix precission problems bellow
/// To add: Cone, Elipse, 3d Elipse 

/// Point - Point => Distance < Eps
/// Point - Ray => Projection on line? => proj else origin of Ray
/// Point - Line => prj on line 
/// Point - Segment => point such that max(Dist(ray1), Dist(ray2))
/// Point - Plane => proj of point to plane
/// Point - Triangle => is Proj Point in Triangle ? Point : min(edge1, edge2, edge3)
/// Point - Disc => proj is under radius from origin ? get the closest point 
/// Point - Sphere => in radius ? else closest
/// Point - Cylinder => proj on main line? get closest, projection on discs ? get closest get minimum
/// Point - Convex => min of closest to all triangles with check if inside convex(pass rand line in point)
/// Ray - Ray => Some algorithm on google giving I1 and I2 (two points)
/// Ray - Line => Same algortihm as above 
/// Ray - Segment =??> above for support segment + the point to segment alg
/// Ray - Plane => ?? (if line is paralel closest to origin plane is chosen to find distance)
/// Ray - Triangle => intersection with point alg (if in same plane check rays intersection with segments)
/// Ray - Disc => intersection with point alg (if in same plane check rays intersection with segments)
/// Ray - Sphere => sphere center with ray (check if proj in sphere)
/// Ray - Cylinder => Ray lateral discs, check of ray origin with Cylinder + closest point of ray to Cylinder
/// Ray - Convex => origin inside? + ray on all faces
/// Line - All => 2xRay (at least one ray must intersect)
/// Segment - All => 2xRay (both rays must intersect)
/// Plane - Plane => solve system ? (normals in same dir => paralel) result is a line
/// Plane - Triangle => check line with the triangle 
/// Plane - Disc => check line with the triangle 
/// Plane - Sphere => project center on plane and chech projection in sphere
/// Plane - Cylinder => Chech with Discs and check core on plane 
/// Plane - Convex => check with all triangles
/// Triangle - Triangle => result segment on both
/// Triangle - Disc => result segment on both discs
/// Triangle - Sphere => Center To triangle (Check radius)
/// Triangle - Cylinder => 2xDisc check + check 3 segments of triangle + check core with triangle 
/// Triangle - Convex => NxTriangle + check center in Convex
/// Disc - Disc => Line Disc intersect 
/// Disc - Sphere => Center to disc
/// Disc - Cylinder => 2xDisk check + prj on segment of closest point to core segment + disc core to seg
/// Disc - Convex => NxTriangle + check center in Convex
/// Sphere - Sphere => distance between centers 
/// Sphere - Cylinder => 2xDisc to Sphere + sphere center to segment sum of distances
/// Sphere - Convex => NxSphere to Triangle + sphere center in Convex or convex in sphere
/// Cylinder - Cylinder => discs to Cylinder for both, min points of segments projected with normal check
/// Cylinder - Convex => NxCylinder + Cylinder in Convex or Convex in Cylinder
/// Convex - Convex => NxNxTriangle + convex1 in convex2 or convex2 in convex1  

namespace Collider
{
	/// Point (it is Point3<Type>)
	/// Ray, Line(2xRay), Segment(2xRay)

	float EPSILON = 1e-6;

	template <typename Type>
	class Ray {
	public:
		Point3<Type> o;
		Point3<Type> dir;

		Ray() {}
		Ray (Point3<Type> o, Point3<Type> dir) : o(o), dir(dir) {}
	};

	template <typename Type>
	class Line {
	public:
		Point3<Type> o;
		Point3<Type> dir;

		Line() {}
		Line (Point3<Type> o, Point3<Type> dir) : o(o), dir(dir) {}
	};

	template <typename Type>
	class Segment {
	public:
		Point3<Type> first;
		Point3<Type> second;

		Segment() {}
		Segment (Point3<Type> first, Point3<Type> second) : first(first), second(second) {}
	};

	/// Plane, Triangle, Disc
	template <typename Type>
	class Plane {
	public:
		Point3<Type> origin;
		Point3<Type> normal;

		Plane() {}
		Plane (Point3<Type> origin, Point3<Type> normal) : origin(origin), normal(normal) {}
	};

	template <typename Type>
	class Triangle {
	public:
		Point3<Type> first;
		Point3<Type> second;
		Point3<Type> third;

		Triangle() {}
		Triangle (Point3<Type> first, Point3<Type> second, Point3<Type> third) 
				: first(first), second(second), third(third) {}
	};

	template <typename Type>
	class Disc {
	public:
		Point3<Type> origin;
		Point3<Type> normal;
		Type radius;

		Disc() {}
		Disc (Point3<Type> origin, Point3<Type> normal, Type radius) 
				: origin(origin), normal(normal), radius(radius) {}
	};

	/// Sphere, Cylinder, Convex shape(made from multiple triangles)
	template <typename Type>
	class Sphere {
	public:
		Point3<Type> origin;
		Type radius;

		Sphere() {}
		Sphere (Point3<Type> origin, Type radius) : origin(origin), radius(radius) {}
	};

	template <typename Type>
	class Cylinder {
	public:
		Segment<Type> core;
		Type radius;

		Cylinder() {}
		Cylinder (Segment<Type> core, Type radius) : core(core), radius(radius) {}
	};

	template <typename Type>
	class ConvexShape {
	public:
		std::vector<Triangle<Type>> triangles;

		ConvexShape() {}
		ConvexShape (std::vector<Triangle<Type>>& triangles) : triangles(triangles) {}
	};
	/// we always collide first with second

	/// Point - Point => Distance < Eps
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> first, Point3<Type> second) {
		return std::pair<bool, Point3<Type>>((first - second).abs() < EPSILON, second);
	}

	/// Point - Ray => Projection on line? => proj else origin of Ray
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Ray<Type> ray) {
		auto alpha = ray.dir.dot(point - ray.o);
		auto closestToRay = ray.o + ray.dir * alpha;
		
		if (alpha < 0)
			closestToRay = ray.o;

		return std::pair<bool, Point3<Type>>((closestToRay - point).absSquared() < EPSILON * EPSILON && 
				alpha > 0, closestToRay);
	}

	/// Point - Line => prj on line 
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Line<Type> line) {
		auto alpha = line.direction.dot(point - line.origin);
		auto closestToLine = line.o + line.dir * alpha;

		return std::pair<bool, Point3<Type>>((closestToLine - point).absSquared() < EPSILON * EPSILON,
				closestToLine);
	}

	/// Point - Segment => point such that max(Dist(ray1), Dist(ray2))
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Segment<Type> segment) {
		auto direction = segment.second - segment.first;
		auto alpha = direction.dot(point - segment.first);
		auto closestToSegment = segment.first + direction * alpha;

		if (alpha < 0)
			closestToSegment = segment.first;

		if (alpha > 1)
			closestToSegment = segment.second;

		return std::pair<bool, Point3<Type>>((closestToSegment - point).absSquared() < EPSILON * EPSILON &&
				alpha >= 0 && alpha <= 1, closestToSegment);
	}

	/// Point - Plane => proj of point to plane
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Plane<Type> plane) {
		// plane.normal.dot(point + alpha * plane.normal - origin) == 0
	
		// normal should allways be normalized		
		auto alpha = plane.normal.dot(plane.origin - point); 
		auto closestToPlane = point + alpha * plane.normal - plane.origin;

		return std::pair<bool, Point3<Type>>(abs(alpha) < EPSILON, closestToPlane);
	}

	/// Point - Triangle => is Proj Point in Triangle ? Point : min(edge1, edge2, edge3)
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Triangle<Type> triangle) {
		// tringle.first will be the origin
		auto firstSegment = triangle.second - triangle.first;
		auto secondSegment = triangle.third - triangle.first;
		auto normal = firstSegment.cross(secondSegment).normalize();

		// normal should allways be normalized		
		auto alpha = normal.dot(triangle.first - point); 
		auto closestToPlane = point + alpha * normal - triangle.first;

		// check in triangle
		auto beta = firstSegment.dot(point - triangle.first);
		auto gamma = secondSegment.dot(point - triangle.first);

		if (abs(alpha) < EPSILON && beta >= 0 && gamma >= 0 && beta + gamma < 1) {
			// in the triangle
			return std::pair<bool, Point3<Type>>(true, point);
		}
		else {
			// not in the triangle 
			auto firstCollision = collide(point, Segment<Type>(triangle.first, triangle.second));
			auto secondCollision = collide(point, Segment<Type>(triangle.first, triangle.third));
			auto thirdCollision = collide(point, Segment<Type>(triangle.second, triangle.third));

			if ((firstCollision.second - point).absSquared() < 
					(secondCollision.second - point).absSquared())
			{
				if ((firstCollision.second - point).absSquared() < 
						(thirdCollision.second - point).absSquared())
				{
					return std::pair<bool, Point3<Type>>(false, firstCollision.second);
				}
				else {
					return std::pair<bool, Point3<Type>>(false, thirdCollision.second);
				}
			}
			else {
				if ((secondCollision.second - point).absSquared() < 
						(thirdCollision.second - point).absSquared())
				{
					return std::pair<bool, Point3<Type>>(false, secondCollision.second);
				}
				else {
					return std::pair<bool, Point3<Type>>(false, thirdCollision.second);
				}
			}
		}
	}

	/// Point - Disc => proj is under radius from origin ? get the closest point 
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Disc<Type> disc) {
		// normal should allways be normalized		
		auto alpha = disc.normal.dot(disc.origin - point); 
		auto closestToPlane = point + alpha * disc.normal - disc.origin;

		if (alpha < EPSILON && 
				(closestToPlane - disc.origin).absSquared() < disc.radius * disc.radius)
		{
			// in disc 
			return std::pair<bool, Point3<Type>>(true, closestToPlane);			
		}
		else {
			if ((closestToPlane - disc.origin).absSquared() < disc.radius * disc.radius) {
				return std::pair<bool, Point3<Type>>(false, closestToPlane);
			}
			else {
				return std::pair<bool, Point3<Type>>(false, 
						(closestToPlane - disc.origin).normalize() * disc.radius + disc.origin);	
			}
		}
	}

	/// Point - Sphere => in radius ? else closest
	template<typename Type>
	std::pair<bool, Point3<Type>> collide (Point3<Type> point, Sphere<Type> sphere) {
		if (abs(sphere.origin - point).absSquared() < EPSILON * EPSILON) {
			return std::pair<bool, Point3<Type>>(true, point);
		}
		else {
			return std::pair<bool, Point3<Type>>(false, 
					(point - sphere.origin).normalize() * sphere.radius + sphere.origin);
		}
	}

	/// Point - Cylinder => proj on main line? get closest, projection on discs ? get closest get minimum
	/// Point - Convex => min of closest to all triangles with check if inside convex(pass rand line in point)
	/// Ray - Ray => Some algorithm on google giving I1 and I2 (two points)
	template <typename Type>
	std::pair<bool, Point3<Type>> collide (Ray<Type> A, Ray<Type> B) {
		float intersect = cross(
			cross(A.dir, A.o - B.o),
			cross(B.dir, A.o - B.o)
		).absSquared();

		if (intersect < EPSILON * EPSILON) {
			// p - A.o
			// r - A.dir
			// q - B.o
			// s - B.dir

			// p + t r == q + u s 

			// t = (q - p) x s / (r x s).abs();
			// u = (q - p) x r / (r x s).abs();

			auto rs = A.dir.cross(B.dir).abs();
			auto qp = B.o - A.o;

			// if (r x s) == 0 and (q - p) x r == 0
			if (rs < EPSILON && abs(qp.cross(A.dir)) < EPSILON) {
				auto t0 = qp.dot(A.dir / A.dir.dot(A.dir));
				auto t1 = (qp + B.dir).dot(A.dir / A.dir.dot(A.dir));

				if (t0 < 0 && 1 < t1 || t1 < 0 && 1 < t0 || t0 < 1 && t0 > 0
						|| t1 < 1 && t1 > 1)
				{
					return std::pair<bool, Point3<Type>>(true, t0 * A.dir + A.o);
				}
				else 
					return std::pair<bool, Point3<Type>>(false, A.o);
			}
			// else if (r x s) != 0
			else if (rs >= EPSILON) {
				auto t = qp.cross(B.dir) / rs;
				auto u = qp.cross(A.dir) / rs;

				/// for ray: 
				if (t >= 0 && u >= 0)
					return std::pair<bool, Point3<Type>>(true, A.o + u * A.dir);
				else
					return std::pair<bool, Point3<Type>>(false, A.o);
			}
			// else if (r x s) == 0 and (q - p) x r != 0 => no intersection
			// else => no intersection 

			return std::pair<bool, Point3<Type>>(false, A.o);
		}
		else {
			return std::pair<bool, Point3<Type>>(false, A.o);
		}
	}

	/// Ray - Line => Same algortihm as above 
	/// Ray - Segment =??> above for support segment + the point to segment alg
	/// Ray - Plane => ?? (if line is paralel closest to origin plane is chosen to find distance)
	/// Ray - Triangle => intersection with point alg (if in same plane check rays intersection with segments)
	/// Ray - Disc => intersection with point alg (if in same plane check rays intersection with segments)
	/// Ray - Sphere => sphere center with ray (check if proj in sphere)
	/// Ray - Cylinder => Ray lateral discs, check of ray origin with Cylinder + closest point of ray to Cylinder
	/// Ray - Convex => origin inside? + ray on all faces
	/// Line - All => 2xRay (at least one ray must intersect)
	/// Segment - All => 2xRay (both rays must intersect)
	/// Plane - Plane => solve system ? (normals in same dir => paralel) result is a line
	/// Plane - Triangle => check line with the triangle 
	/// Plane - Disc => check line with the triangle 
	/// Plane - Sphere => project center on plane and chech projection in sphere
	/// Plane - Cylinder => Chech with Discs and check core on plane 
	/// Plane - Convex => check with all triangles
	/// Triangle - Triangle => result segment on both
	/// Triangle - Disc => result segment on both discs
	/// Triangle - Sphere => Center To triangle (Check radius)
	/// Triangle - Cylinder => 2xDisc check + check 3 segments of triangle + check core with triangle 
	/// Triangle - Convex => NxTriangle + check center in Convex
	/// Disc - Disc => Line Disc intersect 
	/// Disc - Sphere => Center to disc
	/// Disc - Cylinder => 2xDisk check + prj on segment of closest point to core segment + disc core to seg
	/// Disc - Convex => NxTriangle + check center in Convex
	/// Sphere - Sphere => distance between centers 
	/// Sphere - Cylinder => 2xDisc to Sphere + sphere center to segment sum of distances
	/// Sphere - Convex => NxSphere to Triangle + sphere center in Convex or convex in sphere
	/// Cylinder - Cylinder => discs to Cylinder for both, min points of segments projected with normal check
	/// Cylinder - Convex => NxCylinder + Cylinder in Convex or Convex in Cylinder
	/// Convex - Convex => NxNxTriangle + convex1 in convex2 or convex2 in convex1  
}

#endif