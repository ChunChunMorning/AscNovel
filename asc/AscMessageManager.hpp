# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class MessageManager
	{
	private:

		Stopwatch m_stopwatch;

		String m_name;

		String m_text;

		uint32 m_charCount;

	public:

		MessageManager() :
			m_charCount(0U)
		{
			m_stopwatch.start();
			m_stopwatch.pause();
		}

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
			const int32 m_textWait = 100;



			if(m_stopwatch.isPaused())
				return;

			// This MessageManager is shown all.
			if (m_stopwatch.ms() >= static_cast<int>(m_text.length) * m_textSpeed + m_textWait)
			{
				if (Input::KeyEnter.clicked || Input::KeyQ.pressed)
				{
					m_stopwatch.pause();
				}

				return;
			}

			// Skip MessageManager.
			if (Input::KeyEnter.clicked || Input::KeyQ.pressed)
			{
				m_charCount = m_text.length;
				m_stopwatch.pause();

				return;
			}

			m_charCount = static_cast<int>(m_stopwatch.ms() / m_textSpeed);
		}

		void setName(const String& name)
		{
			m_name = name;
		}

		void setText(const String& text)
		{
			m_text = text;
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

		bool isUpdating() const
		{
			return !m_stopwatch.isPaused();
		}

	};
}
