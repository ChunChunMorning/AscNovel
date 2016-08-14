# pragma once
# include <Siv3D.hpp>
# include "AscMessageManager.hpp"
# include "AscScenarioCommand.hpp"
# include "AscScenarioCommands.hpp"
# include "AscScenarioController.hpp"
# include "AscSpriteManager.hpp"

namespace asc
{
	using namespace s3d;

	class Novel
	{
	private:

		std::unique_ptr<ScenarioController> m_scenarioController;

		std::unique_ptr<MessageManager> m_messageManager;

		std::unique_ptr<SpriteManager> m_spriteManager;

		Array<std::unique_ptr<ScenarioCommand>> m_scenarioCommands;

	public:

		Novel() :
			m_scenarioController(std::make_unique<ScenarioController>()),
			m_messageManager(std::make_unique<MessageManager>()),
			m_spriteManager(std::make_unique<SpriteManager>())
		{
			m_scenarioCommands.push_back(std::make_unique<SeekPoint>(m_scenarioController.get(), 0));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"0: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<SeekPoint>(m_scenarioController.get(), 1));
			m_scenarioCommands.push_back(std::make_unique<AddSprite>(m_spriteManager.get(), 1, L"character1", RectF(0, 0, 640, 720)));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"0: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"1: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<SeekPoint>(m_scenarioController.get(), 2));
			m_scenarioCommands.push_back(std::make_unique<AddSprite>(m_spriteManager.get(), 1, L"character1", RectF(0, 0, 640, 720)));
			m_scenarioCommands.push_back(std::make_unique<AddSprite>(m_spriteManager.get(), 2, L"character2", RectF(640, 0, 640, 720)));
			m_scenarioCommands.push_back(std::make_unique<AddSprite>(m_spriteManager.get(), 3, L"character3", RectF(480, 180, 320, 360)));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"0: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"1: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"2: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<SeekPoint>(m_scenarioController.get(), 3));
		}

		virtual ~Novel() = default;

		bool start(int32 seekPoint)
		{
			m_spriteManager->clearSprite();
			return m_scenarioController->start(seekPoint, m_scenarioCommands);
		}

		void update()
		{
			while (
				m_scenarioController->isUpdating() &&
				!m_messageManager->isUpdating()
			)
			{
				m_scenarioCommands[m_scenarioController->currentLine++]->execute();
			}

			m_messageManager->update();
		}

		bool isUpdating()
		{
			return m_scenarioController->isUpdating();
		}

		void draw() const
		{
			m_spriteManager->draw();
			m_messageManager->draw();
		}
	};
}