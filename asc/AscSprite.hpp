# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using TextureAssetName = String;

	const Color ShadeColor = Color(120);

	class Sprite
	{
	private:

		int32 m_id;

	protected:

		TextureAssetName m_texture;

		RectF m_region;

		bool m_light;

	public:

		Sprite() = default;

		Sprite(const String& string)
		{
			const auto args = string.split(L',');

			m_id = Parse<int32>(args[0]);
			m_texture = args[1];
			m_region.x = Parse<double>(args[2]);
			m_region.y = Parse<double>(args[3]);
			m_region.w = Parse<double>(args[4]);
			m_region.h = Parse<double>(args[5]);
			m_light = false;
		}

		virtual ~Sprite() = default;

		bool lightUp(bool light)
		{
			return m_light = light;
		}

		int32 getID() const
		{
			return m_id;
		}

		virtual void draw() const
		{
			m_region(TextureAsset(m_texture)).draw(m_light ? Palette::White : ShadeColor);
		}

	};

	class FixedSprite : public Sprite
	{
	public:

		FixedSprite() = default;

		FixedSprite(const String& string) : Sprite(string) {}

		virtual ~FixedSprite() = default;

		virtual void draw() const override
		{
			m_region(TextureAsset(m_texture)).draw();
		}

	};
}