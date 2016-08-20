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

		Sprite(int32 id, const TextureAssetName& texture, double x, double y, double w, double h) :
			m_id(id),
			m_texture(texture),
			m_region(x, y, w, h),
			m_light(false) {}

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

		FixedSprite(int32 id, const TextureAssetName& texture, double x, double y, double w, double h) :
			Sprite(id, texture, x, y, w, h) {}

		virtual void draw() const override
		{
			m_region(TextureAsset(m_texture)).draw();
		}

	};
}