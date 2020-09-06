#pragma once

#include "gl_inc.h"
#include <string>
#include <map>

class Program {
public:
    Program() = default;

    //void load(const char*, const char*);
    void bind() const;
    GLint uniformLocation(std::string uniform_name);

    static Program* getProgram(std::string name);

private:
    static std::map<std::string, Program> program_cache;
    static GLuint loadShader(std::string path, GLuint type);

    GLint id_;
    std::map<std::string, GLint> uniform_locations;
};
