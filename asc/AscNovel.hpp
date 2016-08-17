# pragma once
# include <Siv3D.hpp>
# include "AscChoiceManager.hpp"
# include "AscMessageManager.hpp"
# include "AscSoundManager.hpp"
# include "AscSpriteManager.hpp"
# include "AscTimeManager.hpp"

namespace asc
{
	using namespace s3d;

	using Commnad = std::pair<int32, String>;

	class Novel
	{
	private:

		bool m_isUpdating;

		int32 m_currentLine;

		int32 m_lastSeekPoint;

		Array<Commnad> m_commands;

		ChoiceManager m_choiceManager;

		SoundManager m_soundManager;

		MessageManager m_messageManager;

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
			m_commands.push_back({ 8, L"bgm1,3000" });
			m_commands.push_back({ 1, L"Play BGM1" });
			m_commands.push_back({ 0, L"6" });
			m_commands.push_back({ 9, L"bgm1,3000" });
			m_commands.push_back({ 8, L"bgm2,3000" });
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
			m_soundManager.update();
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
	};
}