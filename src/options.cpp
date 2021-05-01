#include "options.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include <memory>

using namespace glm;


std::unique_ptr<Option> Option::FromString(std::string s) {
    size_t index;
    size_t length;
    std::unique_ptr<Option> opt;
    if (s.find("float") != std::string::npos) {
        index = s.find("float");
        length = strlen("float");
        opt = std::make_unique<OptFloat>();
    } else if (s.find("color") != std::string::npos) {
        index = s.find("color");
        length = strlen("color");
        opt = std::make_unique<OptColor>();
    } else if (s.find("vec3") != std::string::npos) {
        index = s.find("vec3");
        length = strlen("vec3");
        opt = std::make_unique<OptVec>();
    } else {
        return nullptr;
    }
    
    size_t l = s.find("(", index+length);
    size_t r = s.rfind(")");
    if (l == std::string::npos || r == std::string::npos)
        return nullptr;

    std::stringstream sstr(s.substr(l+1, r));

    opt->deserialize(sstr);

    return std::move(opt);
}

Option::Option(std::string name, bool visible)
	: name(name),
	  visible(visible),
	  dirty(true) {}

void Option::reset() {}

OptVec::OptVec(std::string name, glm::vec3 def, float min, float max,
		       float m, bool norm, bool visible)
	: Option(name, visible),
	  val(def), def(def), min(min), max(max), m(m), norm(norm) {}
	
void OptVec::show() {
	if (visible) {
		if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(val),
		                      (max-min)/500.f, min, max)) {
			dirty = true;
		}
	}
}
void OptVec::upload(Shader& shader, std::string item) {
	shader.use();
	GLint uniformLocation = shader.getUniformLocation(item.c_str());
	vec3 realval = norm ? normalize(val) : val;
	realval *= m;
	glUniform3f(uniformLocation, realval.x, realval.y, realval.z);
}
void OptVec::reset() {
	val = def;
	dirty = true;
}
void OptVec::serialize(std::ostream& out) const {
	out << "uniform vec3 " << name << " = vec3("
        << def[0] << ", " << def[1] << ", " << def[2] << ");" << std::endl;
}
void OptVec::deserialize(std::istream& in) {
    in >> name;
    in >> min;
    in >> max;
    in >> def[0];
	in >> def[1];
    in >> def[2];
    m = 1.0;
    norm = false;
	reset();
}
OptVec2::OptVec2(std::string name, glm::vec2 def, float min, float max,
				 float m, bool norm, bool visible)
	: OptVec(name, vec3(def.x, def.y, 0.f), min, max, m, norm, visible) {}
	
void OptVec2::show() {
	if (visible) {
		if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(val),
		                      (max-min)/500.f, min, max)) {
			dirty = true;
		}
	}
}
void OptVec2::upload(Shader& shader, std::string item) {
	shader.use();
	GLint uniformLocation = shader.getUniformLocation(item.c_str());
	vec2 realval = norm ? normalize(vec2(val)) : vec2(val);
	realval *= m;
	glUniform2f(uniformLocation, realval.x, realval.y);
}

OptColor::OptColor(std::string name, glm::vec3 def, bool visible)
	: OptVec(name, def, 0.f, 1.f, 1.f, false, visible) {}

void OptColor::show() {
	if (visible) {
        if (ImGui::ColorEdit3(name.c_str(), glm::value_ptr(val))) {
			dirty = true;
		}
	}
}
void OptColor::serialize(std::ostream& out) const {
	out << "uniform vec3 " << name << " = vec3("
        << def[0] << ", " << def[1] << ", " << def[2] << ");" << std::endl;
}
void OptColor::deserialize(std::istream& in) {
    in >> name;
    in >> def[0];
	in >> def[1];
    in >> def[2];
	reset();
}


OptFloat::OptFloat(std::string name, float def, float min, float max,
		           bool visible)
	: Option(name, visible),
	  val(def), def(def), min(min), max(max) {}
	
void OptFloat::show() {
	if (visible) {
		if (ImGui::SliderFloat(name.c_str(), &val, min, max)) {
			dirty = true;
		}
	}
}
void OptFloat::upload(Shader& shader, std::string item) {
	shader.use();
	GLint uniformLocation = shader.getUniformLocation(item.c_str());
	glUniform1f(uniformLocation, val);
}
void OptFloat::reset() {
	val = def;
	dirty = true;
}
void OptFloat::serialize(std::ostream& out) const {
	out << "uniform float " << name << " = " << def << ";" << std::endl;
}
void OptFloat::deserialize(std::istream& in) {
    in >> name;
    in >> min;
	in >> def;
    in >> max;
	reset();
}

OptBool::OptBool(std::string name, float def, bool visible)
	: Option(name, visible), val(def), def(def) {}
	
void OptBool::show() {
	if (visible) {
		if (ImGui::Checkbox(name.c_str(), &val)) {
			dirty = true;
		}
	}
}
void OptBool::upload(Shader& shader, std::string item) {
	shader.use();
	GLint uniformLocation = shader.getUniformLocation(item.c_str());
	glUniform1i(uniformLocation, val);
}
void OptBool::reset() {
	val = def;
	dirty = true;
}
void OptBool::serialize(std::ostream& out) const {
	out << val << std::endl;
}
void OptBool::deserialize(std::istream& in) {
	in >> def;
	reset();
}

OptList::OptList(std::string name, int def, std::vector<std::string> options,
	             bool visible)
	: Option(name, visible), val(def), def(def), options(options) {}

void OptList::show() {
	if (visible) {
		ImGui::Text(name.c_str());
		for (int i = 0; i < options.size(); i++) {
			if( ImGui::RadioButton(options[i].c_str(), &val, i ) ) {
				dirty = true;
			}
		}
	}
}
void OptList::upload(Shader& shader, std::string item) {
	shader.use();
	GLint uniformLocation = shader.getUniformLocation(item.c_str());
	glUniform1i(uniformLocation, val);
}
void OptList::reset() {
	val = def;
	dirty = true;
}
void OptList::serialize(std::ostream& out) const {
	out << ((unsigned int)val) << std::endl;
}
void OptList::deserialize(std::istream& in) {
	unsigned int a;
	in >> a;
	def = a;
	reset();
}
