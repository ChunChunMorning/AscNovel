# pragma once
# include <Siv3D.hpp>
# include "AscSprite.hpp"

namespace asc
{
	using namespace s3d;

	class SpriteManager
	{
	private:

		Array<Sprite> m_sprites;

	public:

		SpriteManager()
		{
			m_sprites.push_back(Sprite(1, L"character1", RectF(0, 0, 640, 720)));
			m_sprites.push_back(Sprite(2, L"character2", RectF(640, 0, 640, 720)));
			m_sprites.push_back(Sprite(3, L"character3", RectF(480, 180, 320, 360)));
			m_sprites[0].turnOn();
		}

		virtual ~SpriteManager() = default;

		void draw() const
		{
			for (const auto& sprite : m_sprites)
			{
				sprite.draw();
			}
		}
	};
}
