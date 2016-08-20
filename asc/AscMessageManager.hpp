# pragma once
# include <Siv3D.hpp>
# include "AscIMessgeButton.hpp"

namespace asc
{
	using namespace s3d;

	using TextureAssetName = String;
	using FontAssetName = String;

	class DefaultButton : public IMessageButton
	{
	public:

		void onShownAll() override {}

		void onSubmit() override {}

		void onClear() override {}

		void update() override {}

		void draw() const override {}

	};

	class MessageManager
	{
	private:

		std::unique_ptr<IMessageButton> m_button;

		bool m_isAutomatic;

		bool m_callOnShonwAll;

		Stopwatch m_stopwatch;

		uint32 m_charCount;

		String m_text;

		String m_name;

		KeyCombination m_submit;

		int32 m_speed;
		
		int32 m_time;

		TextureAssetName m_texture;

		FontAssetName m_textFont;

		FontAssetName m_nameFont;

		Color m_color;

		Rect m_region;

		Point m_textPosition;

		Point m_namePosition;

		Array<wchar> m_silentChars;

		std::function<void()> m_onCountChar;

		bool isSilentChar(wchar character) const
		{
			return std::find(m_silentChars.begin(), m_silentChars.end(), character) != m_silentChars.end();
		}

	public:

		MessageManager(std::function<void()> onCountChar) :
			m_button(std::make_unique<DefaultButton>()),
			m_charCount(0U),
			m_speed(100),
			m_time(100),
			m_silentChars({ L' ', L'Å@' }),
			m_onCountChar(onCountChar) {}

		void setName(const String& name)
		{
			m_name = name;
		}

		void setText(const String& text)
		{
			m_text = text;
		}

		void setKey(const KeyCombination& submit)
		{
			m_submit = submit;
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

		void setSilentChars(const Array<wchar> silentChars)
		{
			m_silentChars = silentChars;
		}

		void setButton(std::unique_ptr<IMessageButton>&& button)
		{
			m_button.swap(button);
		}

		void start(bool isAutomatic = false)
		{
			m_stopwatch.start();
			m_charCount = 0U;
			m_isAutomatic = isAutomatic;
			m_callOnShonwAll = false;
		}

		void update()
		{
			if(!isUpdating())
				return;

			const auto typingTime = static_cast<int32>(m_text.length) * m_speed;

			if (m_stopwatch.ms() >= typingTime + m_time)
			{
				if (m_isAutomatic)
				{
					m_stopwatch.reset();
					return;
				}
				
				if(!m_callOnShonwAll)
				{
					m_button->onShownAll();
					m_callOnShonwAll = true;
				}

				if (m_submit.clicked || m_submit.pressed)
				{
					m_stopwatch.reset();
					m_button->onSubmit();
				}

				return;
			}
			
			if (m_stopwatch.ms() < typingTime && m_submit.clicked)
			{
				m_stopwatch.set(static_cast<Milliseconds>(typingTime));

				return;
			}

			const auto charCount = Min<uint32>(m_stopwatch.ms() / m_speed, m_text.length);

			if (charCount > m_charCount)
			{
				m_charCount = charCount;

				if (!isSilentChar(m_text[charCount - 1]))
				{
					m_onCountChar();
				}
			}
		}

		void clear()
		{
			m_name.clear();
			m_text.clear();
			m_stopwatch.reset();
			m_button->onClear();
		}

		void skip()
		{
			m_stopwatch.reset();
			m_charCount = m_text.length;
		}

		bool isUpdating() const
		{
			return m_stopwatch.isActive();
		}

		void draw() const
		{
			m_region(TextureAsset(m_texture)).draw();
			FontAsset(m_nameFont).draw(m_name, m_namePosition, m_color);
			FontAsset(m_textFont).draw(m_text.substr(0U, m_charCount), m_textPosition, m_color);
			m_button->draw();
		}

	};
}
