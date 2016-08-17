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

		void lightUp(const String& string)
		{
			const auto args = string.split(L',');
			lightUp(Parse<int32>(args[0]), Parse<bool>(args[1]));
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

		void lightUpSpot(const String& string)
		{
			lightUpSpot(Parse<int32>(string));
		}

		void lightUpSpot(int32 id)
		{
			for (const auto& sprite : m_sprites)
			{
				sprite->lightUp(sprite->getID() == id);
			}
		}

		void bring(const String& string)
		{
			bring(Parse<int32>(string));
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
				else if (e2->getID() == id)
					return true;
				return true;
			}
			);
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
