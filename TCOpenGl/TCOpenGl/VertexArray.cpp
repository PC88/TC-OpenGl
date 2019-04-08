#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	//GLCall(glBindVertexArray(m_RendererID)); we dont have to bind it in this case  it is done in "AddBuffer()"-PC
}


VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); //this->Bind(); same as this -PC
	vb.Bind();
	const auto& elements = layout.GetElements(); // returns vector of structs for atribptr method below -PC
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i)); // enable the VAP - PC
		GLCall(glVertexAttribPointer(i, element.count , element.type, element.normalized, layout.GetStride(), (const void*)offset));// detail on this composed in notes for future ref
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type); // after each iteration modify the offset -PC
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
