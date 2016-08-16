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

		SpriteManager() = default;

		virtual ~SpriteManager() = default;

		template <class Type>
		void add(const String& string)
		{
			m_sprites.push_back(std::make_unique<Type>(string));
		}

		void erase(int id)
		{
			Erase_if(m_sprites, [&](const std::unique_ptr<Sprite>& sprite){ return id == sprite->getID(); });
		}

		void clear()
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
