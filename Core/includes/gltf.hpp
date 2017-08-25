#pragma once

#include <tiny_gltf.h>
#include <model.hpp>

namespace Zion
{
	class   Gltf : public Model
	{
	private:
		tinygltf::Model     _model;
		std::vector<int>    _nodes;
		std::string         _path;
	private:
		bool    _processModel();
		void    _processModelMesh(tinygltf::Mesh& mesh, int node);
		void    _loadDataToGpu();
		void    _loadMaterials();
		void    _clearVectors();
	public:
		Gltf() = default;
		Gltf(Shader& shader, const char *path);
		Gltf(const Gltf & rhs);
		Gltf&   operator=(const Gltf & rhs);
		~Gltf();

		bool    loadFromFile(Shader& shader, const char *path);
		void    render(glm::mat4 matrix);
	};
}