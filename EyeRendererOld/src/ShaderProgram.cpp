#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>



ShaderProgram::ShaderProgram(const std::string& vertexFile, const std::string& fragmentFile)
{
   prepareShader(vertexShader, &vertexShaderDataPtr, "data/basic.vert", GL_VERTEX_SHADER);
   prepareShader(fragmentShader, &fragmentShaderDataPtr, "data/basic.frag", GL_FRAGMENT_SHADER);
   initializeShaderProgram();
}

ShaderProgram::~ShaderProgram()
{
   free(vertexShaderDataPtr);
   vertexShaderDataPtr = nullptr;
   free(fragmentShaderDataPtr);
   fragmentShaderDataPtr = nullptr;
}

void ShaderProgram::use()
{
   glUseProgram(shaderProgram);

}

void ShaderProgram::prepareShader(unsigned int& shader, char** shaderData,
                               const std::string& shaderFile, GLenum shaderType)
{
   std::ifstream fileStream;
   fileStream.open(shaderFile);

   std::stringstream buffer;
   buffer << fileStream.rdbuf();
   std::string strBuffer = buffer.str();
   *shaderData = reinterpret_cast<char*>(malloc(strBuffer.length() * sizeof(char)));
   strncpy(*shaderData, strBuffer.c_str(), strBuffer.length());

   shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(shader, 1, shaderData, NULL);
   glCompileShader(shader);

   int  success;
   char infoLog[512];
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if(!success)
   {
       glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
       std::cout << "SHADER compilation of " << shaderFile << " failed : " << infoLog << std::endl;
   }

}

void ShaderProgram::initializeShaderProgram()
{
   shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   int  success;
   char infoLog[512];
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if(!success) {
       glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
       std::cout << "SHADER linking failed : " << infoLog << std::endl;
   }

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}
