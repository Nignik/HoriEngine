#pragma once

class OpenGLBuffer
{
public:
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

	void copyFrom(const OpenGLBuffer& other, int dataSize)
	{
		glBindBuffer(GL_COPY_READ_BUFFER, other.vbo);
		glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);

		glBufferData(GL_COPY_WRITE_BUFFER, dataSize, nullptr, GL_STATIC_DRAW);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, dataSize);

		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}

	OpenGLBuffer(OpenGLBuffer&& other) noexcept
		: vao(other.vao), vbo(other.vbo)
	{
		other.vao = 0;
		other.vbo = 0;
	}

	OpenGLBuffer& operator=(OpenGLBuffer&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);

			vao = other.vao;
			vbo = other.vbo;

			other.vao = 0;
			other.vbo = 0;
		}
		return *this;
	}

	OpenGLBuffer(const OpenGLBuffer&) = delete;
	OpenGLBuffer& operator=(const OpenGLBuffer&) = delete;

	GLuint vao{}, vbo{};
};