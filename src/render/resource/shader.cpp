#include "pch.h"

#ifndef GLEW_STATIC
#  define GLEW_STATIC
#endif
#include "glew/glew.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32s")

#include <array>
#include <cstdio>

#include "shader.h"
#include "util/utils.h"


shader::manager_t shader::manager;


shader::manager_t::ptr shader::manager_t::Get(const std::string& name)
{
    bool existed = Exists(name);
    ptr result = resource_manager_named<shader>::Get(name);
    if (!existed) {
        result.lock()->Load(name);
    }
    return result;
}


void shader::Load(const std::string& path)
{
    std::array<UINT, NUM_SHADER_TYPES>
        shaders = {0},
        shTypes = {GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER};
    std::array<std::string, NUM_SHADER_TYPES> suffixes = {".vert", ".geom", ".tctrl", ".teval", ".frag"};
    bool isOk = true;
    static char errorLog[ERROR_LOG_LENGTH];

    for (int i = 0; i < NUM_SHADER_TYPES; ++i) {
        std::unique_ptr<char[]> source(utils::LoadText(path + suffixes[i]));
        if (!source) {
            // File not found
            continue;
        }

        if ((shaders[i] = glCreateShader(shTypes[i])) == 0) {
            isOk = false;
            SaveLog("Error creating shader \'" + path + suffixes[i] + "\'!");
            break;
        }
        auto p = source.get();
        glShaderSource(shaders[i], 1, &p, nullptr);

        int result;
        glCompileShader(shaders[i]);
        glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &result);
        if (result != 1) {
            glGetShaderInfoLog(shaders[i], sizeof(errorLog), &result, errorLog);
            SaveLog(errorLog);
            isOk = false;
            break;
        }
    }

    if (isOk) {
        if ((glProgram = glCreateProgram()) == 0) {
            isOk = false;
        } else {
            for (int i = 0; i < NUM_SHADER_TYPES; ++i) {
                if (shaders[i] != 0) {
                    glAttachShader(glProgram, shaders[i]);
                }
            }
            glLinkProgram(glProgram);

            int result;
            glGetProgramiv(glProgram, GL_LINK_STATUS, &result);
            if (result != 1) {
                glGetProgramInfoLog(glProgram, sizeof(errorLog), &result, errorLog);
                SaveLog(errorLog);
                isOk = false;
            }
        }
    }

    if (!isOk) {
        // Clean up
        for (int i = 0; i < NUM_SHADER_TYPES; ++i) {
            if (shaders[i] != 0) {
                if (glProgram != 0) {
                    glDetachShader(glProgram, shaders[i]);
                }
                glDeleteShader(shaders[i]);
            }
        }
        if (glProgram != 0) {
            glDeleteProgram(glProgram);
        }
        glProgram = 0;
        return;
    }
}


void shader::SaveLog(const std::string& text)
{
    FILE* file;
    errno_t err;

    if ((err = fopen_s(&file, LOG_PATH, "a")) == 0) {
        fprintf(file, "%s\n", text.c_str());
        fclose(file);
    }
}


void shader::SetLayout(vertex* vertx) {
    auto Layout = vertx->GetLayout();
    for (const auto& it : Layout) {
        glVertexAttribPointer(it.index, it.size, it.type, it.isNormalised, it.stride, it.pointer);
        glEnableVertexAttribArray(it.index);
    }
}


void shader::Enable()
{
    glUseProgram(glProgram);
}


void shader::Disable()
{
    glUseProgram(glProgram);
}


void shader::SetUniform(const std::string &name, float val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniform1f(loc, val);
    }
}


void shader::SetUniform(const std::string &name, int val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniform1i(loc, val);
    }
}


void shader::SetUniform(const std::string &name, const point& val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniform2i(loc, val.x, val.y);
    }
}


void shader::SetUniform(const std::string &name, const coord& val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniform2f(loc, val.x, val.y);
    }
}


/*
void shader::SetUniform(const std::string &name, matrix &val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, false, val.GetData());
    }
}
*/


void shader::SetUniform(const std::string &name, const vec &val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniform3f(loc, val.x, val.y, val.z);
    }
}


void shader::SetUniform(const std::string &name, const color &val) const
{
    int loc = glGetUniformLocation(glProgram, name.c_str());
    if (loc != -1) {
        glUniform4f(loc, val.r, val.g, val.b, val.a);
    }
}
