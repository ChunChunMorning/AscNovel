# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	using TextureAssetName = String;
	using FontAssetName = String;

	class MessageManager
	{
	private:

		bool m_isAutomatic;

		Stopwatch m_stopwatch;

		uint32 m_charCount;

		String m_text;

		String m_name;

		KeyCombination m_submit;

		KeyCombination m_skip;

		int32 m_speed;
		
		int32 m_time;

		TextureAssetName m_texture;

		FontAssetName m_textFont;

		FontAssetName m_nameFont;

		Color m_color;

		Rect m_region;

		Point m_textPosition;

		Point m_namePosition;

		std::function<void()> m_onCountChar;

	public:

		MessageManager(std::function<void()> onCountChar) :
			m_charCount(0U),
			m_speed(100),
			m_time(100),
			m_onCountChar(onCountChar)
		{
			m_stopwatch.start();
			m_stopwatch.pause();
		}

		void setName(const String& name)
		{
			m_name = name;
		}

		void setText(const String& text)
		{
			m_text = text;
		}

		void setKey(const KeyCombination& submit, const KeyCombination& skip)
		{
			m_submit = submit;
			m_skip = skip;
		}

		void setSpeed(int32 speed)
		{
			m_speed = speed;
		}

		void setTime(int32 time)
		{
			m_time = time;
		}

		void setTexture(const TextureAssetName& texture, const Rect& region)
		{
			m_texture = texture;
			m_region = region;
		}

		void setFont(const FontAssetName& text, const FontAssetName& name)
		{
			m_textFont = text;
			m_nameFont = name;
		}

		void setColor(const Color& color)
		{
			m_color = color;
		}

		void setPosition(const Point& text, const Point& name)
		{
			m_textPosition = text;
			m_namePosition = name;
		}

		void start(bool isAutomatic = false)
		{
			m_stopwatch.restart();
			m_charCount = 0U;
			m_isAutomatic = isAutomatic;
		}

		void update()
		{
			if(m_stopwatch.isPaused())
				return;

			const auto typingTime = static_cast<int32>(m_text.length) * m_speed;

			if (m_stopwatch.ms() >= typingTime)
			{
				if (m_stopwatch.ms() >= typingTime + m_time && (m_submit.clicked || m_skip.pressed || m_isAutomatic))
				{
					m_stopwatch.pause();
				}
			}
			else if(m_submit.clicked || m_skip.pressed)
			{
				m_stopwatch.set(static_cast<Milliseconds>(m_text.length * m_speed));
			}

			const auto charCount = Min<uint32>(m_stopwatch.ms() / m_speed, m_text.length);

			if (charCount > m_charCount)
			{
				m_charCount = charCount;

				if (m_text[charCount - 1] != L' ')
				{
					m_onCountChar();
				}
			}
		}

		void clear()
		{
			m_name.clear();
			m_text.clear();
			m_stopwatch.pause();
		}

		bool isUpdating() const
		{
			return !m_stopwatch.isPaused();
		}

		void draw() const
		{
			m_region(TextureAsset(m_texture)).draw();

			FontAsset(m_nameFont).draw(m_name, m_namePosition, m_color);
			FontAsset(m_textFont).draw(m_text.substr(0U, m_charCount), m_textPosition, m_color);
		}

	};
}
