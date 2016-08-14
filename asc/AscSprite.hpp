# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using TextureAssetName = String;

	const std::array<Color, 2> light =
	{
		Color(255, 255, 255),
		Color(120, 120, 120)
	};

	class Sprite
	{
	private:

		int m_id;

	protected:

		TextureAssetName m_texture;

		RectF m_region;

		bool m_isHighlight;

	public:

		Sprite() = default;

		Sprite(int id, TextureAssetName texture, RectF region) :
			m_id(id),
			m_texture(texture),
			m_region(region),
			m_isHighlight(false) {}

		virtual ~Sprite() = default;

		virtual void turnOn()
		{
			m_isHighlight = true;
		}

		virtual void turnOff()
		{
			m_isHighlight = false;
		}

		int getID() const
		{
			return m_id;
		}

		virtual void draw() const
		{
			m_region(TextureAsset(m_texture)).draw(m_isHighlight ? light[0] : light[1]);
		}

	};

	class FixedSprite : public Sprite
	{
	public:

		FixedSprite() = default;

		FixedSprite(int id, TextureAssetName texture, RectF region) :
			Sprite(id, texture, region)
		{
			turnOn();
		}

		virtual ~FixedSprite() = default;

		virtual void turnOn() override {}

		virtual void turnOff() override {}

		virtual void draw() const override
		{
			m_region(TextureAsset(m_texture)).draw();
		}

	};
}