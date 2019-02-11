#pragma once
class IndexBuffer
{
private:
	// this is called renderer ID, to keep things abstract as it could be used similar in e.g DX12, as opposed to OpenGL -PC
	unsigned int m_RendererID; 
	unsigned int m_Count; // how many indices this buffer has - PC
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	virtual ~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

