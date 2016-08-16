# pragma once
# include <Siv3D.hpp>
# include "AscChoiceManager.hpp"
# include "AscMessageManager.hpp"
# include "AscSpriteManager.hpp"

namespace asc
{
	using namespace s3d;

	using Commnad = std::pair<int32, String>;

	class Novel
	{
	private:

		bool m_isUpdating;

		int32 m_currentLine;

		Array<Commnad> m_commands;

		ChoiceManager m_choiceManager;

		MessageManager m_messageManager;

		SpriteManager m_spriteManager;

		void clearManager()
		{
			m_messageManager.clear();
			m_spriteManager.clear();
			m_choiceManager.clear();
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

			default:
				break;
			}

			m_currentLine++;
		}

	public:

		Novel() :
			m_isUpdating(false),
			m_currentLine(0)
		{
			m_commands.push_back({ 0, L"0"});
			m_commands.push_back({ 1, L"0: Write Text"});
			m_commands.push_back({ 0, L"1"});
			m_commands.push_back({ 3, L"1,character1,0,0,640,720" });
			m_commands.push_back({ 3, L"3,character3,480,180,320,360" });
			m_commands.push_back({ 1, L"1: Write Text" });
			m_commands.push_back({ 1, L"1: Write Text2" });
			m_commands.push_back({ 5, L"0,Yes,1,No" });
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
				!m_choiceManager.isUpdating()
			)
			{
				execute();
			}

			m_messageManager.update();
			m_choiceManager.update();
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

		void draw() const
		{
			m_spriteManager.draw();
			m_messageManager.draw();
			m_choiceManager.draw();
		}
	};
}