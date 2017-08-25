#define TINYGLTF_IMPLEMENTATION
#include <gltf.hpp>

namespace Zion
{
	Gltf::Gltf(Shader &shader, const char *path)
	{
		loadFromFile(shader, path);
	}

	Gltf::Gltf(const Gltf &rhs)
	{
		*this = rhs;
	}

	Gltf& Gltf::operator=(const Gltf &rhs)
	{
		if (this != &rhs)
		{
			_vao = rhs._vao;
			_vbos = rhs._vbos;
			_ibo = rhs._ibo;
			_materials = rhs._materials;
			_locMat = rhs._locMat;
		}
		return *this;
	}

	Gltf::~Gltf()
	{
		for (GLuint& vbo : _vbos)
			glDeleteBuffers(1, &vbo);
		_vbos.clear();
		_materials.clear();
		glDeleteBuffers(1, &_ibo);
		glDeleteVertexArrays(1, &_vao);
	}

	bool Gltf::loadFromFile(Shader &shader, const char *path)
	{
		bool                ret;
		std::string         err;
		std::string         ext = Gltf::GetFilePathExtension(path);
		tinygltf::TinyGLTF  loader;

		_shader = shader;
		_path = std::string(path);
		if (ext.compare("glb") == 0)
			ret = loader.LoadBinaryFromFile(&_model, &err, path);
		else
			ret = loader.LoadASCIIFromFile(&_model, &err, path);
		if (!err.empty())
			std::cerr << "glTF parse error: " << err << std::endl;
		if (!ret)
		{
			std::cerr << "Failed to load glTF: " << path << std::endl;
			return false;
		}
		return _processModel();
	}

	bool Gltf::_processModel()
	{
		std::vector<tinygltf::Accessor>& acc = _model.accessors;
		std::vector<tinygltf::BufferView>&  bufViews = _model.bufferViews;
		std::vector<tinygltf::Buffer>& bufs = _model.buffers;

		for (tinygltf::Scene& scene : _model.scenes)
		{
			for (int i : scene.nodes)
			{
				tinygltf::Node& node = _model.nodes[i];
				if (node.mesh >= 0)
					_processModelMesh(_model.meshes[node.mesh], i);
			}
		}
		_loadDataToGpu();
		_loadMaterials();
		_clearVectors();
		_loaded = true;
		return true;
	}

	void Gltf::_processModelMesh(tinygltf::Mesh& mesh, int node)
	{
		std::vector<tinygltf::Accessor>&    acc = _model.accessors;
		std::vector<tinygltf::BufferView>&  bufViews = _model.bufferViews;
		std::vector<tinygltf::Buffer>&      bufs = _model.buffers;
		std::map<std::string, int>::const_iterator it;

		tinygltf::BufferView bufView;

		for (tinygltf::Primitive& prim : mesh.primitives)
		{
			tinygltf::Accessor& Acc = acc[prim.attributes["POSITION"]];
			size_t currVecSize = _vertex.size() / 3;

			int matVal = (prim.material == -1) ? 0 : prim.material;
			/// adding material indexes to _matIndexs
			Model::loadOneManyToVector(_matIndexs, (float)matVal, (int)Acc.count);
			/// adding nodes to _nodes for animation
			Model::loadOneManyToVector(_nodes, node, (int)Acc.count);
			/// adding position vertices to _vertex array
			bufView = bufViews[Acc.bufferView];
			auto *posData = (float *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			_vertex.insert(_vertex.end(), posData, posData + (Acc.count * 3));
			/// adding normals to _normals
			if ((it = prim.attributes.find("NORMAL")) != prim.attributes.end())
			{
				Acc = acc[prim.attributes["NORMAL"]];
				bufView = bufViews[Acc.bufferView];
				auto *data = (float *)&bufs[bufView.buffer].data[bufView.byteOffset];
				_normals.insert(_normals.end(), data, data + (Acc.count * 3));
			}
			/// adding uv coords to _uvs
			if ((it = prim.attributes.find("TEXCOORD_0")) != prim.attributes.end())
			{
				Acc = acc[prim.attributes["TEXCOORD_0"]];
				bufView = bufViews[Acc.bufferView];
				auto *data = (float *)&bufs[bufView.buffer].data[bufView.byteOffset];
				_uvs.insert(_uvs.end(), data, data + (Acc.count * 2));
			}
			/// adding indices of mesh to _indices
			Acc = acc[prim.indices];
			bufView = bufViews[Acc.bufferView];
			auto *indicesData = (GLushort *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			for (int i = 0; i < Acc.count; i++)
				_indices.push_back((GLushort)(indicesData[i] + currVecSize));
		}
	}

	void Gltf::_loadDataToGpu()
	{
		GLuint  vbo[5];
		GLint position = _shader.getAttribLocation((char *)"position");
		GLint matIndex = _shader.getAttribLocation((char *)"matIndex");
		GLint normal = _shader.getAttribLocation((char *)"normal");
		GLint uv = _shader.getAttribLocation((char *)"uv");
		GLint node = _shader.getAttribLocation((char *)"node");

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		glGenBuffers(5, vbo);
		_vbos.insert(_vbos.end(), vbo, vbo + 4);
		if (!_vertex.empty() && position != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, _vertex.size() * sizeof(GLfloat), _vertex.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray((GLuint)position);
			glVertexAttribPointer((GLuint)position, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
			Window::getError((char *)"after adding vertex in Gltf model");
		}
		if (!_normals.empty() && normal != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray((GLuint)normal);
			glVertexAttribPointer((GLuint)normal, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
			Window::getError((char *)"after adding normal in Gltf model");
		}
		if (!_matIndexs.empty() && matIndex != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
			glBufferData(GL_ARRAY_BUFFER, _matIndexs.size() * sizeof(GLfloat), _matIndexs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray((GLuint)matIndex);
			glVertexAttribPointer((GLuint)matIndex, 1, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
			Window::getError((char *)"after adding matIndex in Gltf model");
		}
		if (!_nodes.empty() && node != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
			glBufferData(GL_ARRAY_BUFFER, _nodes.size() * sizeof(int), _nodes.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray((GLuint)node);
			glVertexAttribPointer((GLuint)node, 1, GL_INT, GL_FALSE, 0, (void *)nullptr);
			Window::getError((char *)"after adding node in Gltf model");
		}
		if (!_uvs.empty() && uv != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
			glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(GLfloat), _uvs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray((GLuint)uv);
			glVertexAttribPointer((GLuint)uv, 2, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
			Window::getError((char *)"after adding uv in Gltf model");
		}
		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLushort)(_indices.size() * sizeof(GLushort)), &_indices[0],
			GL_STATIC_DRAW);
		Window::getError((char *)"after adding indices in Gltf model");
		glBindVertexArray(0);
	}

	void Gltf::_clearVectors()
	{
		_indicesCount = (GLushort)_indices.size();
		_indices.clear();
		_vertex.clear();
		_matIndexs.clear();
		_normals.clear();
		_uvs.clear();
		_nodes.clear();
	}

	void Gltf::_loadMaterials()
	{
		int     index = 0;

		for (tinygltf::Material& mat : _model.materials)
		{
			Material newMat;
			bool    foundMat = false;
			for (std::pair<std::string, tinygltf::Parameter> val : mat.values)
			{
				if (val.first == std::string("baseColorFactor") && val.second.number_array.size() == 4)
				{
					foundMat = true;
					newMat.base_color = glm::vec4(
							(float)val.second.number_array[0],
							(float)val.second.number_array[1],
							(float)val.second.number_array[2],
							(float)val.second.number_array[3]
					);
				}
				if (val.first == std::string("baseColorTexture") && !val.second.json_double_value.empty())
				{
					for (std::pair<std::string, double> ind : val.second.json_double_value)
					{
						foundMat = true;
						tinygltf::Image& image = _model.images[_model.textures[(int)ind.second].source];
						//tinygltf::Sampler& sampler = _model.samplers[_model.textures[(int)ind.second].sampler];

						if (image.uri.empty())
							newMat.texure.loadTextureFromData(image.image.data(),
						                                  _model.bufferViews[image.bufferView].byteLength);
						else
						{
							std::size_t found = _path.find_last_of("/");
							if (found == std::string::npos)
								newMat.texure.loadTextureFromPath(image.uri.c_str());
							else
								newMat.texure.loadTextureFromPath
										((_path.substr(0, found+1) + image.uri).c_str());
						}
						break;
					}
				}
			}
			if (foundMat)
				addMaterial(index++, newMat);
		}
	}

	void Gltf::render(glm::mat4 matrix)
	{
		_shader.enable();
		_shader.setUniformMat4((GLchar *)"model_matrix", matrix);
		for (std::pair<int, Material> material : _materials)
			Material::sendMaterialToShader(_shader, material.second, material.first);
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr);
		glBindVertexArray(0);
		for (std::pair<int, Material> material : _materials)
			material.second.texure.unbindTexture();
		_shader.disable();
	}
}