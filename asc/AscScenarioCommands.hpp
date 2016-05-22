# pragma once
# include <Siv3D.hpp>
# include "AscScenarioCommand.hpp"
# include "AscMessageManager.hpp"

namespace asc
{
	using namespace s3d;

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
