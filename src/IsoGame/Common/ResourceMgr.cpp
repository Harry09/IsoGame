#include "ResourceMgr.h"

#include "Directions.h"
#include "Common.h"

#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Font.hpp>

ResourceMgr *ResourceMgr::s_pInst;

char TEXTURE_DIR[] = "data\\sprites";

ResourceMgr::ResourceMgr()
{
	s_pInst = this;

	m_pFont = std::make_shared<sf::Font>();
	m_pFont->loadFromFile("data\\upheavtt.ttf");
}

ResourceMgr::Texture *ResourceMgr::GetTexture(const std::string &textureName, Directions::Direction direction)
{
	//printf("Getting texture '%s_%s'... ", textureName.c_str(), Directions::getDirectionName(direction));

	for (auto& texture : m_textures)
	{
		if (texture && texture->m_textureName == textureName && texture->m_direction == direction)
		{
			//printf("[OK]\n");

			return texture.get();
		}
	}

	//printf("[MISSING]\n");

	std::string texturePath = std::string(TEXTURE_DIR) + "\\" + textureName + "_" + Directions::getDirectionName(direction) + ".png";

	printf("Loading texture: '%s'... ", texturePath.c_str());

	if (IsFileExist(texturePath.c_str()))
	{
		auto texture = std::make_shared<Texture>();
		texture->m_pTexture = std::make_shared<sf::Texture>();

		if (texture->m_pTexture->loadFromFile(texturePath))
		{
			texture->m_direction = direction;
			texture->m_textureName = textureName;

			m_textures.push_back(texture);

			printf("[OK]\n");

			return texture.get();
		}
		else
		{
			printf("[ERROR]\n");
			return nullptr;
		}
	}
	else
	{
		printf("[FILE MISSING]\n");
		return nullptr;
	}
}
