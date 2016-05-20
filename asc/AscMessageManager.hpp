# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class MessageManager
	{
	public:

		MessageManager() :
			m_name(L"名前"),
			m_text(L"テキスト"),
			m_charCount(0U) {}

		virtual ~MessageManager() = default;

		void start()
		{
			m_stopwatch.restart();
			m_charCount = 0U;
		}

		void update()
		{
			// ToDo Configurable
			const int32 m_textSpeed = 100;



			m_charCount = static_cast<int>(m_stopwatch.ms() / m_textSpeed);
		}

		void draw() const
		{
			// ToDo Configurable
			const Rect m_messageBox(6, 440, 1268, 285);
			const Point m_namePosition(40, 525);
			const Point m_textPosition(60, 575);
			const String m_messageBoxTexture = L"test_message_box";
			const String m_nameFont = L"test_name";
			const String m_textFont = L"test_text";
			const Color m_messageColor = Palette::Black;



			m_messageBox(TextureAsset(m_messageBoxTexture)).draw();

			FontAsset(m_nameFont).draw(m_name, m_namePosition, m_messageColor);
			FontAsset(m_textFont).draw(m_text.substr(0U, m_charCount), m_textPosition, m_messageColor);
		}

	private:

		Stopwatch m_stopwatch;

		String m_name;

		String m_text;

		uint32 m_charCount;

	};
}
