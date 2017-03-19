#pragma once

struct Geometry
{
	const std::vector<Vertex> trianglevertices =
	{
		{ { 0.0f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
	};

	// ������ �Ͼ�� ������ �ﰢ��
	//{ {0.0f, -0.5f}, { 1.0f, 1.0f, 1.0f }},
	//{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
	//{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }

	Geometry()
	{
	}

	void SetVertexBuffer();
	
}; 