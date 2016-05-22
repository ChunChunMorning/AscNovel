# pragma once
# include <Siv3D.hpp>
# include "AscMessageManager.hpp"
# include "AscScenarioCommand.hpp"
# include "AscScenarioCommands.hpp"

namespace asc
{
	using namespace s3d;

	class Novel
	{
	private:

		std::unique_ptr<MessageManager> m_messageManager;

		Array<std::unique_ptr<ScenarioCommand>> m_scenarioCommands;

	public:

		Novel() :
			m_messageManager(std::make_unique<MessageManager>())
		{
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"0: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"1: Write Text"));
			m_scenarioCommands.push_back(std::make_unique<WriteText>(m_messageManager.get(), L"2: Write Text"));
		}

		virtual ~Novel() = default;

		void start(int32 seekPoint)
		{
			m_scenarioCommands[seekPoint]->execute();
		}

		void update()
		{
			m_messageManager->update();
		}

		void draw() const
		{
			m_messageManager->draw();
		}
	};
}