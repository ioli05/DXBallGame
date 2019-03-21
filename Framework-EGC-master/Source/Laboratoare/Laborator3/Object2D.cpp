#include "Object2D.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(height, 0, 0), color),
		VertexFormat(corner + glm::vec3(height, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 center, float radius, glm::vec3 color, bool fill) {
	float x, y;
	float px = radius, py = 0;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	vertices.push_back(VertexFormat(center, color));
	vertices.push_back(VertexFormat(glm::vec3(center.x + px, center.y + py, 0), color));

	for (int i = 1; i <= 360; i++) {
		x = center.x + radius * cos(3.14f / 180 * i);
		y = center.y + radius * sin(3.14f / 180 * i);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		indices.push_back(0);
		indices.push_back(i - 1);
		indices.push_back(i);
		px = x;
		py = y;
	}

	indices.push_back(0);
	indices.push_back(360);
	indices.push_back(1);

	Mesh* circle = new Mesh(name);
	if (!fill) {
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}
