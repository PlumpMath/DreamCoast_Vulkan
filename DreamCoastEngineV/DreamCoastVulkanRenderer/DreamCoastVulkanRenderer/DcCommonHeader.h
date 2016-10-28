//Common Header For DreamCoast Vulkan Rendering Learning Example

// OpenGL�� Vulkan���� ����ϴ� ��Ƽ�÷��� ������ �ڵ鷯, GLFW_INCLUDE_VULKAN�� Ȱ��ȭ �ҽ� vulkan.h�� ��Ŭ��� ó���˴ϴ�.
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
// Vulkan�� ���̻� ���ο��� ��Ʈ�������� �����Լ��� ������ ���� �����Ƿ� ����մϴ�.
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

// std c, c++
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>