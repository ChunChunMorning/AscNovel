# pragma once
# include <Siv3D.hpp>
# include "AscChoiceManager.hpp"
# include "AscMessageManager.hpp"
# include "AscIMessgeButton.hpp"
# include "AscSoundManager.hpp"
# include "AscSpriteManager.hpp"
# include "AscTimeManager.hpp"

namespace asc
{
	using namespace s3d;

	class Novel
	{
	private:

		using Commnad = std::pair<int32, String>;

		bool m_isUpdating;

		int32 m_currentLine;

		int32 m_lastSeekPoint;

		KeyCombination m_skip;

		Array<Commnad> m_commands;

		ChoiceManager m_choiceManager;

		MessageManager m_messageManager;

		SoundManager m_soundManager;

		SpriteManager m_spriteManager;

		TimeManager m_timeManager;

		void clearManager()
		{
			m_messageManager.clear();
			m_spriteManager.clear();
			m_choiceManager.clear();
			m_timeManager.clear();
		}

		void execute()
		{
			switch (m_commands[m_currentLine].first)
			{
			// Point
			case 0:
				m_isUpdating = false;
				return;

			// Text
			case 1:
				m_messageManager.setText(m_commands[m_currentLine].second);
				m_messageManager.start();
				break;

			// Name
			case 2:
				m_messageManager.setName(m_commands[m_currentLine].second);
				break;

			// AddSprite
			case 3:
				m_spriteManager.add<Sprite>(m_commands[m_currentLine].second);
				break;

			// AddFixedSprite
			case 4:
				m_spriteManager.add<FixedSprite>(m_commands[m_currentLine].second);
				break;

			// Choice
			case 5:
				m_choiceManager.start(m_commands[m_currentLine].second);
				break;

			// Jump
			case 6:
				start(Parse<int32>(m_commands[m_currentLine].second));
				return;

			// AutomaticText
			case 7:
				m_messageManager.setText(m_commands[m_currentLine].second);
				m_messageManager.start(true);
				break;

			// Play BGM
			case 8:
				m_soundManager.playBGM(m_commands[m_currentLine].second);
				break;

			// Stop BGM
			case 9:
				m_soundManager.stopBGM(m_commands[m_currentLine].second);
				break;

			// Lihgt Up
			case 10:
				m_spriteManager.lightUp(m_commands[m_currentLine].second);
				break;

			// Lihgt Up Spot
			case 11:
				m_spriteManager.lightUpSpot(m_commands[m_currentLine].second);
				break;

			// Bring
			case 12:
				m_spriteManager.bring(m_commands[m_currentLine].second);
				break;

			// Erase
			case 13:
				m_spriteManager.erase(m_commands[m_currentLine].second);
				break;

			// Wait
			case 14:
				m_timeManager.wait(m_commands[m_currentLine].second);
				break;

			default:
				break;
			}

			m_currentLine++;
		}

		void skip()
		{
			while (m_isUpdating && !m_choiceManager.isUpdating())
			{
				execute();
			}

			m_timeManager.clear();
			m_messageManager.skip();
		}

	public:

		Novel() :
			m_isUpdating(false),
			m_currentLine(0),
			m_lastSeekPoint(-1),
			m_choiceManager(
				[&] { m_soundManager.playMoveSound(); },
				[&] { m_soundManager.playSubmitSound(); }
			),
			m_messageManager(
				[&] { m_soundManager.playCharSound(); }
			)
		{
			m_commands.push_back({ 0, L"1"});
			m_commands.push_back({ 3, L"1,character1,0,0,640,720" });
			m_commands.push_back({ 3, L"3,character3,480,180,320,360" });
			m_commands.push_back({ 1, L"Characters" });
			m_commands.push_back({ 0, L"2" });
			m_commands.push_back({ 1, L"Only Text" });
			m_commands.push_back({ 0, L"3" });
			m_commands.push_back({ 7, L"Show Character?" });
			m_commands.push_back({ 5, L"1,Yes,2,No" });
			m_commands.push_back({ 0, L"4" });
			m_commands.push_back({ 1, L"Jump 2" });
			m_commands.push_back({ 6, L"2" });
			m_commands.push_back({ 0, L"5" });
			m_commands.push_back({ 8, L"bgm1,3" });
			m_commands.push_back({ 1, L"Play BGM1" });
			m_commands.push_back({ 0, L"6" });
			m_commands.push_back({ 9, L"bgm1,3" });
			m_commands.push_back({ 8, L"bgm2,3" });
			m_commands.push_back({ 1, L"Play BGM2" });
			m_commands.push_back({ 9, L"bgm2,0" });
			m_commands.push_back({ 1, L"Stop BGM2" });
			m_commands.push_back({ 0, L"7" });
			m_commands.push_back({ 3, L"1,character1,0,0,640,720" });
			m_commands.push_back({ 3, L"3,character3,0,180,320,360" });
			m_commands.push_back({ 10, L"1,true" });
			m_commands.push_back({ 10, L"3,true" });
			m_commands.push_back({ 1, L"Light Up" });
			m_commands.push_back({ 10, L"1,false" });
			m_commands.push_back({ 1, L"Light Down" });
			m_commands.push_back({ 11, L"1" });
			m_commands.push_back({ 1, L"Spot Light" });
			m_commands.push_back({ 12, L"1" });
			m_commands.push_back({ 1, L"Bring 1" });
			m_commands.push_back({ 13, L"3" });
			m_commands.push_back({ 1, L"Erase 3" });
			m_commands.push_back({ 0, L"8" });
			m_commands.push_back({ 7, L"Wait 3 second" });
			m_commands.push_back({ 14, L"3000" });
			m_commands.push_back({ 1, L"fin" });
			m_commands.push_back({ 0, L"-1" });
		}

		virtual ~Novel() = default;

		bool start(int32 seekPoint)
		{
			const auto size = m_commands.size() - 1;
			for (auto i = 0u; i < size; i++)
			{
				const auto index = (m_currentLine + i) % m_commands.size();

				const auto command = m_commands[index];
				if (
					command.first == 0 &&
					Parse<int32>(command.second) == seekPoint
				)
				{
					clearManager();
					m_currentLine = index + 1;
					m_lastSeekPoint = seekPoint;
					m_isUpdating = true;

					return true;
				}
			}

			return false;
		}

		void update()
		{
			if (m_skip.clicked && !m_choiceManager.isUpdating())
			{
				skip();
			}

			while (
				m_isUpdating &&
				!m_messageManager.isUpdating() &&
				!m_choiceManager.isUpdating() &&
				!m_timeManager.isUpdating()
			)
			{
				m_choiceManager.lastSelectedSeekPoint().then([&](int32 seekPoint){ start(seekPoint); });
				execute();
			}

			m_messageManager.update();
			m_choiceManager.update();
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

		int32 seekPoint() const
		{
			return m_lastSeekPoint;
		}

		void draw() const
		{
			m_spriteManager.draw();
			m_messageManager.draw();
			m_choiceManager.draw();
		}

		Novel& setSpeed(int32 speed)
		{
			m_messageManager.setSpeed(speed);

			return *this;
		}

		Novel& setWaitingTime(int32 time)
		{
			m_messageManager.setTime(time);

			return *this;
		}

		Novel& setKey(const KeyCombination& submit, const KeyCombination& skip)
		{
			m_messageManager.setKey(submit);
			m_skip = skip;

			return *this;
		}

		Novel& setKey(const KeyCombination& submit, const KeyCombination& skip, const KeyCombination& up, const KeyCombination& down)
		{
			m_messageManager.setKey(submit);
			m_choiceManager.setKey(submit, up, down);
			m_skip = skip;

			return *this;
		}

		Novel& setButton(std::unique_ptr<IMessageButton>&& button)
		{
			m_messageManager.setButton(std::move(button));

			return *this;
		}

		template <class Type>
		Novel& setButton(const Type& button)
		{
			return setButton(static_cast<std::unique_ptr<IMessageButton>>(std::make_unique<Type>(button)));
		}

		Novel& setFont(const FontAssetName& text)
		{
			return setFont(text, text);
		}

		Novel& setFont(const FontAssetName& text, const FontAssetName& name = L"")
		{
			m_messageManager.setFont(text, name);
			m_choiceManager.setFont(text);

			return *this;
		}

		Novel& setColor(const Color& color, const Color& selectedColor = Palette::Red)
		{
			m_messageManager.setColor(color);
			m_choiceManager.setColor(color, selectedColor);

			return *this;
		}

		Novel& setMessageTexture(const TextureAssetName& texture, const Rect& region)
		{
			m_messageManager.setTexture(texture, region);

			return *this;
		}

		Novel& setMessagePosition(const Point& text, const Point& name = Point::Zero)
		{
			m_messageManager.setPosition(text, name);

			return *this;
		}

		Novel& setChoiceTexture(const TextureAssetName texture, const Rect& region)
		{
			m_choiceManager.setTexture(texture, region);

			return *this;
		}

		Novel& setChoicePosition(const Point& position)
		{
			m_choiceManager.setPosition(position);

			return *this;
		}

		Novel& setBGMVolume(double volume)
		{
			m_soundManager.setBGMVolume(volume);

			return *this;
		}

		Novel& setSEVolume(double volume)
		{
			m_soundManager.setSEVolume(volume);

			return *this;
		}

		Novel& setSound(const SoundAssetName& charCount)
		{
			m_soundManager.setSE(charCount);

			return *this;
		}

		Novel& setSound(const SoundAssetName& charCount, const SoundAssetName& move, const SoundAssetName& submit)
		{
			m_soundManager.setSE(charCount, move, submit);

			return *this;
		}

		Novel& setSilentChars(const Array<wchar> silentChars)
		{
			m_messageManager.setSilentChars(silentChars);

			return *this;
		}

	};
}