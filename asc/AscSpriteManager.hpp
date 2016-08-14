# pragma once
# include <Siv3D.hpp>
# include "AscSprite.hpp"

namespace asc
{
	using namespace s3d;

	class SpriteManager
	{
	private:

		Array<std::unique_ptr<Sprite>> m_sprites;

	public:

		SpriteManager()
		{
			m_sprites.push_back(std::make_unique<Sprite>(1, L"character1", RectF(0, 0, 640, 720)));
			m_sprites.push_back(std::make_unique<Sprite>(2, L"character2", RectF(640, 0, 640, 720)));
			m_sprites.push_back(std::make_unique<FixedSprite>(3, L"character3", RectF(480, 180, 320, 360)));
			m_sprites[0]->turnOn();
			deleteSprite(3);
		}

		virtual ~SpriteManager() = default;

		void addSprite(int id, const String& texture, const RectF& region)
		{
			m_sprites.push_back(std::make_unique<Sprite>(id, texture, region));
		}

		void addFixedSprite(int id, const String& texture, const RectF& region)
		{
			m_sprites.push_back(std::make_unique<FixedSprite>(id, texture, region));
		}

		void deleteSprite(int id)
		{
			Erase_if(m_sprites, [&](const std::unique_ptr<Sprite>& sprite){ return id == sprite->getID(); });
		}

		void clearSprite()
		{
			m_sprites.clear();
		}

		void draw() const
		{
			for (const auto& sprite : m_sprites)
			{
				sprite->draw();
			}
		}

	};
}
