#pragma once

struct OpenGLBuffer
{
	OpenGLBuffer()
	{
		glCreateVertexArrays(1, &vao);
		glCreateBuffers(1, &vbo);
	}

	~OpenGLBuffer()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	GLuint vao{}, vbo{};
};