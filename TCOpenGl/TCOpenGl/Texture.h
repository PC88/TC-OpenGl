#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer; // for stb_image to clear its image data
	int m_Width, m_Height, m_BPP; // BPP: bytes per pixel, which for a RGBA image is 4 bytes -PC
public:
	Texture(const std::string& path);
	virtual ~Texture();

	// TODO: texture slot API call, to find out exactly - openGL dependent PC
	void Bind(unsigned int slot = 0) const; // the texture "slot" depends on the GPU, typical values are 32 for desktop, 8 for mobile - PC
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

