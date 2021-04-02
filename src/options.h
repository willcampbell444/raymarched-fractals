#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

#include "shader.h"

// we will keep a list of options indexed by OptionIds
class Option {
public:
	Option(std::string name, bool visible=true);
	Option() : visible(true), dirty(false) {}
	virtual ~Option() {}

	virtual void show() = 0; // imgui
	virtual void upload(Shader& shader, std::string item) = 0;
	virtual void reset();
	virtual void serialize(std::ostream& out) const = 0;
	virtual void deserialize(std::istream& in)  = 0;

	static std::unique_ptr<Option> FromString(std::string s);

	std::string name;
	bool visible;
	bool dirty;
};

class OptVec : public Option {
public:
	OptVec(): m(1.f), norm(false) {}
	OptVec(std::string name, glm::vec3 def, float min, float max,
		   float m=1.f, bool norm=false, bool visible=true);
	virtual ~OptVec() {}
	
	virtual void show() override;
	virtual void upload(Shader& shader, std::string item) override;
	virtual void reset() override;
	virtual void serialize(std::ostream& out) const override;
	virtual void deserialize(std::istream& in) override;

	glm::vec3 val, def;
	float min, max, m;
	bool norm;
};
class OptColor : public OptVec {
public:
	OptColor() {}
	OptColor(std::string name, glm::vec3 def, bool visible=true);
	virtual void show() override;
	virtual void serialize(std::ostream& out) const override;
	virtual void deserialize(std::istream& in) override;
};
class OptVec2 : public OptVec {
public:
	OptVec2(std::string name, glm::vec2 def, float min, float max,
		    float m=1.f, bool norm=false, bool visible=true);
	
	virtual void show() override;
	virtual void upload(Shader& shader, std::string item) override;
};

class OptFloat : public Option {
public:
	OptFloat() {}
	OptFloat(std::string name, float def, float min, float max,
		     bool visible=true);
	virtual ~OptFloat() {}
	
	virtual void show() override;
	virtual void upload(Shader& shader, std::string item) override;
	virtual void reset() override;
	virtual void serialize(std::ostream& out) const override;
	virtual void deserialize(std::istream& in) override;

	float val, def, min, max;
};

class OptBool : public Option {
public:
	OptBool(std::string name, float def, bool visible=true);
	virtual ~OptBool() {}
	
	virtual void show() override;
	virtual void upload(Shader& shader, std::string item) override;
	virtual void reset() override;
	virtual void serialize(std::ostream& out) const override;
	virtual void deserialize(std::istream& in) override;

	bool val, def;
};

class OptList : public Option {
public:
	OptList(std::string name, int def, std::vector<std::string> options,
	        bool visible=true);
	virtual ~OptList() {}
	
	virtual void show() override;
	virtual void upload(Shader& shader, std::string item) override;
	virtual void reset() override;
	virtual void serialize(std::ostream& out) const override;
	virtual void deserialize(std::istream& in) override;

	int val, def;
	std::vector<std::string> options;
};
