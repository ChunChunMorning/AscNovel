# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	using FontAssetName = String;

	class Novel
	{
	public:
		Novel() :
			m_nameFont(L"test_name"),
			m_textFont(L"test_text"),
			m_textBoxRegion(6, 440, 1268, 285),
			m_namePosition(40, 525),
			m_textPosition(60, 575),
			m_name(L"名前"),
			m_text(L"テキスト") {}

		virtual ~Novel() = default;

		void start(uint32 seekPoint) {}

		void update() {}

		void draw() const
		{
			m_textBoxRegion(TextureAsset(L"test_text_box")).draw();

			FontAsset(m_nameFont).draw(m_name, m_namePosition, Palette::Black);
			FontAsset(m_textFont).draw(m_text, m_textPosition, Palette::Black);
		}

	private:

		FontAssetName m_nameFont;

		FontAssetName m_textFont;

		Rect m_textBoxRegion;

		Point m_namePosition;

		Point m_textPosition;

		String m_name;

		String m_text;

	};
}