#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
private:
	unsigned int m_renderer_id;
	// save filepaths for debugging purposes ----
	std::string m_v_filepath;
	std::string m_f_filepath;
	// ------------------------------------------
	mutable std::unordered_map<std::string, int> m_uniform_location_cache;
public:
	Shader(const std::string& vpath, const std::string& fpath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(unsigned int id, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1i(unsigned int id, int value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat4f(unsigned int id, const glm::mat4& matrix);
private:
	std::string parse_shader(const std::string& path);
	unsigned int compile_shader(unsigned int type, const std::string& source);
	unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	int get_uniform_location(const std::string& name) const;
};