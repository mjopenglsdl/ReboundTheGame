//
// Copyright (c) 2016-2018 Jo�o Baptista de Paula e Silva.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//


#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include "glad/glad.h"

#include "../TexturedVertexArray.hpp"

#include "rendering/Drawable.hpp"
#include "rendering/Texture.hpp"

class GlTexturedVertexArray : public Drawable
{
	GLuint vao, vertexBuffer, elementBuffer;
	std::shared_ptr<Texture> texture;

protected:
	virtual void draw(const glm::mat3& projTransform) override;

public:
	GlTexturedVertexArray();
	~GlTexturedVertexArray();

	auto getTexture() const { return texture; }
	void setTexture(const std::shared_ptr<Texture>& texture) { this->texture = texture; }

	void setVertices(const TVertex* vertices, size_t numVertices);
	void setIndices(const uint16_t* indices, size_t numIndices);
};