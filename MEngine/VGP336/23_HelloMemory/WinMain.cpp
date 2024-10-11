#include <MEngine/Inc/MEngine.h>

using namespace MEngine;
using namespace MEngine::Core;

static int sUniqueId = 0;
class Student
{
public:
	Student(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{
	
	}
	Student(const std::string& name, int id)
		: mName(name)
		, mId(id)
	{

	}
private:
	int mId = 0;
	std::string mName;
};


class Node
{
public:
	Node(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{

	}
	Node(const std::string& name, int id)
		: mName(name)
		, mId(id)
	{

	}
private:
	int mId = 0;
	std::string mName;
};

class Entity
{
public:
	Entity(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{

	}
	Entity(const std::string& name, int id)
		: mName(name)
		, mId(id)
	{

	}
private:
	int mId = 0;
	std::string mName;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 200);
	TypedAllocator nodePool = TypedAllocator<Node>("NodePool", 1000);
	TypedAllocator entityPool = TypedAllocator<Entity>("EntityPool", 100);

	std::vector<Student*> students;
	std::vector<Node*> nodes;
	std::vector<Entity*> entities;
	//test new
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i), i);
		students.push_back(newStudent);
	}

	for (uint32_t i = 0; i < 500; ++i)
	{
		Node* newNode = nodePool.New("Node_" + std::to_string(i), i);
		nodes.push_back(newNode);
	}

	for (uint32_t i = 0; i < 50; ++i)
	{
		Entity* newEntity = entityPool.New("Entity_" + std::to_string(i), i);
		entities.push_back(newEntity);
	}

	//test deletion
	for (uint32_t i = 0; i < 20; ++i)
	{
		auto& student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	for (uint32_t i = 0; i < 200; ++i)
	{
		auto& node = nodes.back();
		nodePool.Delete(node);
		nodes.pop_back();
	}

	for (uint32_t i = 0; i < 10; ++i)
	{
		auto& entity = entities.back();
		entityPool.Delete(entity);
		entities.pop_back();
	}

	//add new students
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* student = studentPool.New("Students_New" + std::to_string(i), i);
		students.push_back(student);
	}

	for (uint32_t i = 0; i < 200; ++i)
	{
		Node* node = nodePool.New("Nodes_New" + std::to_string(i), i);
		nodes.push_back(node);
	}

	for (uint32_t i = 0; i < 50; ++i)
	{
		Entity* entity = entityPool.New("Entities_New" + std::to_string(i), i);
		entities.push_back(entity);
	}


	for (auto& student : students)
	{
		studentPool.Delete(student);
	}

	for (auto& node : nodes)
	{
		nodePool.Delete(node);
	}

	for (auto& entity : entities)
	{
		entityPool.Delete(entity);
	}
	students.clear();
	nodes.clear();
	entities.clear();

	return 0;
}