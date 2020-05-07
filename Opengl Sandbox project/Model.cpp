#include "Model.h"

Model::Model(const std::vector<Vertex>& v, const std::vector<unsigned>& i) {


	count = i.size();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * v.size(), &v[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * i.size(), &i[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

Model Model::loadObj(const std::string& path) {

	std::vector<Vertex> v;
	std::vector<unsigned> i;

	std::ifstream file;
	try {
		file.open(path);
	}
	catch (const std::exception& e) {
		std::cout << "failed to load model" << std::endl;
	}

	std::string buff;
	while (std::getline(file, buff)) {
		std::stringstream ss;
		ss << buff;
		std::string type;
		ss >> type;




		if (type.compare("v") == 0) {
			float cord[3];
			for (int i = 0; i < 3; i++) {
				ss >> cord[i];
			}

			v.push_back(Vertex(cord));
		}
		if (type.compare("f") == 0) {
			std::stringstream indicesStream;
			indicesStream << ss.rdbuf();

			std::vector<unsigned> t;
			std::string stringBuff;
			unsigned unBuff;
			while (indicesStream >> stringBuff) {
				auto inStream = std::stringstream(stringBuff);
				while (inStream >> unBuff) {
					t.push_back(--unBuff);
					char del;
					inStream >> del;

				}
			}

			if (t.size() == 12) {
				i.push_back(t[0]);
				i.push_back(t[3]);
				i.push_back(t[6]);

				i.push_back(t[0]);
				i.push_back(t[6]);
				i.push_back(t[9]);

			}
			if (t.size() == 9) {
				i.push_back(t[0]);
				i.push_back(t[3]);
				i.push_back(t[6]);
			}

		}
	}


	return Model(v, i);
}

void Model::draw(const Shader& shad) const {
	shad.bind();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
