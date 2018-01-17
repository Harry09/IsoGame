#pragma once

#include <vector>
#include <memory>
#include <string>

namespace Directions
{
	enum Direction;
}

namespace sf
{
	class Texture;
	class Font;
}



class ResourceMgr
{
public:
	class Texture
	{
	public:
		std::shared_ptr<sf::Texture>				m_pTexture;
		std::string									m_textureName;
		Directions::Direction						m_direction;
	};

private:
	std::shared_ptr<sf::Font>				m_pFont;

	std::vector<std::shared_ptr<Texture>>	m_textures;

	static ResourceMgr *s_pInst;

public:
	ResourceMgr();
	~ResourceMgr() = default;

	sf::Font *GetFont() { return m_pFont.get(); }
	Texture *GetTexture(const std::string &textureName, Directions::Direction direction);

	static ResourceMgr *Get() { return s_pInst; }
};

