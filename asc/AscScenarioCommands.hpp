# pragma once
# include <Siv3D.hpp>
# include "AscScenarioCommand.hpp"
# include "AscScenarioController.hpp"
# include "AscMessageManager.hpp"

namespace asc
{
	using namespace s3d;

	//////////////////////////////////////////////////////
	//
	//	ScenarioController
	//

	class SeekPoint : public ScenarioCommand
	{
	private:

		ScenarioController* m_scenarioController;

	public:

		SeekPoint(ScenarioController* scenarioController, int32 seekPoint) :
			ScenarioCommand(seekPoint),
			m_scenarioController(scenarioController) {}

		void execute() override
		{
			m_scenarioController->currentLine--;
			m_scenarioController->finish();
		};

	};



	//////////////////////////////////////////////////////
	//
	//	MessageManager
	//

	class WriteText : public ScenarioCommand
	{
	private:

		MessageManager* m_messageManager;

		String m_text;

	public:

		WriteText(MessageManager* messageManager, String text) :
			m_messageManager(messageManager),
			m_text(text) {}

		void execute() override
		{
			m_messageManager->setText(m_text);
			m_messageManager->start();
		};

	};
}
