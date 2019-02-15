#pragma once
class VertexBuffer
{
private:
	// this is called renderer ID, to keep things abstract as it could be used similar in e.g DX12, as opposed to OpenGL -PC
	unsigned int m_RendererID; 

public:
	VertexBuffer(const void* data, unsigned int size);
	virtual ~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};

