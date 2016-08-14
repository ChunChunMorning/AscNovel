# pragma once
# include <Siv3D.hpp>
# include "AscMessageManager.hpp"
# include "AscSpriteManager.hpp"

namespace asc
{
	using namespace s3d;

	class Novel
	{
	private:

		bool m_isUpdating;

		int32 m_currentLine;

		Array<Array<String>> m_scenario;

		std::unique_ptr<MessageManager> m_messageManager;

		std::unique_ptr<SpriteManager> m_spriteManager;

	public:

		Novel() :
			m_isUpdating(false),
			m_messageManager(std::make_unique<MessageManager>()),
			m_spriteManager(std::make_unique<SpriteManager>())
		{
			m_scenario.push_back({ L"Point", L"0"});
			m_scenario.push_back({ L"Text", L"0: Write Text"});
			m_scenario.push_back({ L"Point", L"1"});
			m_scenario.push_back({ L"Sprite", L"1", L"character1", L"0", L"0", L"640", L"720" });
			m_scenario.push_back({ L"Sprite", L"3", L"character3", L"480", L"180", L"320", L"360" });
			m_scenario.push_back({ L"Text", L"1: Write Text" });
			m_scenario.push_back({ L"Text", L"1: Write Text2" });
			m_scenario.push_back({ L"Point", L"2" });
		}

		virtual ~Novel() = default;

		bool start(int32 seekPoint)
		{
			m_spriteManager->clearSprite();
			
			const auto size = m_scenario.size() - 1;
			for (auto i = 0u; i < size; i++)
			{
				const auto index = (m_currentLine + i) % m_scenario.size();

				const auto command = m_scenario[index][0];
				if (command == L"Point" && Parse<int32>(m_scenario[index][1]) == seekPoint)
				{
					m_isUpdating = true;
					m_currentLine = index + 1;

					return true;
				}
			}

			return false;
		}

		void update()
		{
			while (
				m_isUpdating &&
				!m_messageManager->isUpdating()
			)
			{
				const auto command = m_scenario[m_currentLine][0];

				if (command == L"Point")
				{
					m_isUpdating = false;
				}
				else
				{
					if (command == L"Text")
					{
						m_messageManager->setText(m_scenario[m_currentLine][1]);
						m_messageManager->start();
					}
					else if (command == L"Sprite")
					{
						m_spriteManager->addSprite(Sprite(
							Parse<int>(m_scenario[m_currentLine][1]),
							m_scenario[m_currentLine][2],
							RectF(0, 0, 640, 720)
							));
					}

					m_currentLine++;
				}
			}

			m_messageManager->update();
		}

		bool isUpdating()
		{
			return m_isUpdating;
		}

		void draw() const
		{
			m_spriteManager->draw();
			m_messageManager->draw();
		}
	};
}