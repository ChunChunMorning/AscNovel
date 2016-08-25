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

		template <class Type>
		void add(int32 id, const TextureAssetName& texture, double x, double y, double w, double h)
		{
			m_sprites.push_back(std::make_unique<Type>(id, texture, x, y, w, h));
		}

		void lightUp(int32 id, bool light)
		{
			for (const auto& sprite : m_sprites)
			{
				if (sprite->getID() == id)
				{
					sprite->lightUp(light);
					return;
				}
			}
		}

		void lightUpSpot(int32 id)
		{
			for (const auto& sprite : m_sprites)
			{
				sprite->lightUp(sprite->getID() == id);
			}
		}

		void bring(int32 id)
		{
			std::sort(
				m_sprites.begin(),
				m_sprites.end(),
				[&](const std::unique_ptr<Sprite>& e1, const std::unique_ptr<Sprite>& e2)
			{
				if (e1->getID() == id)
					return false;
				return e2->getID() == id;
			}
			);
		}

		void erase(int32 id)
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
