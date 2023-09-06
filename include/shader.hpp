#ifndef SHADER_HPP
#define SHADER_HPP

#include "glinclude.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

class Shader {
public:
    uint id;
    Shader() {};
    Shader(const char *vPath, const char *fPath) {
        string vCode, fCode;
        std::ifstream vf, ff;
        vf.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        ff.exceptions(std::ifstream::badbit | std::ifstream::failbit);

        try {
            vf.open(vPath); ff.open(fPath);

            stringstream v, f;
            v << vf.rdbuf(); f << ff.rdbuf();

            vf.close(); ff.close();
            vCode = v.str(); fCode = f.str();
        } catch (std::ifstream::failure &e) {
            std::cerr << "ERROR::SHADER::FAILED_TO_READ " << e.what() << std::endl;
        }
        compile(vCode.c_str(), fCode.c_str());
    }
    void compile(const char *vCode, const char *fCode) {
        uint v, f;
        
        v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, 1, &vCode, nullptr);
        glCompileShader(v);
        checkCompilation(v, "VERTEX");

        f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &fCode, nullptr);
        glCompileShader(f);
        checkCompilation(f, "FRAGMENT");

        id = glCreateProgram();
        glAttachShader(id, v);
        glAttachShader(id, f);
        glLinkProgram(id);
        checkCompilation(id, "PROGRAM");
        
        glDeleteShader(v);
        glDeleteShader(f);
    }
    void enable() {
        glUseProgram(id);
    }
    void setInt(const string &name, int val) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), val);
    }
    void setFloat(const string &name, float val) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), val);
    }
    void setVec2(const string &name, float x, float y) {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }
private:
    void checkCompilation(uint shader, string type) {
        int ok = 1;
        char log[1024];
        if (type == "PROGRAMM") {
            glGetProgramiv(id, GL_LINK_STATUS, &ok);
            if (!ok) {
                glGetProgramInfoLog(shader, 1024, nullptr, log);
                std::cerr << "ERROR::SHADER::FAILED_LINKING " << log << '\n'; 
            }
        } else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
            if (!ok) {
                glGetShaderInfoLog(shader, 1024, nullptr, log);
                std::cerr << "ERROR::SHADER::FAILED_COMPILING::" << type << ' ' << log << '\n'; 
            }
        }
    }
};

#endif