# pragma once
# include <Siv3D.hpp>
# include "AscMessageManager.hpp"
# include "AscScenarioCommand.hpp"
# include "AscScenarioController.hpp"
# include "AscSpriteManager.hpp"

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

	//////////////////////////////////////////////////////
	//
	//	SpriteManager
	//

	class AddSprite : public ScenarioCommand
	{
	private:

		SpriteManager* m_spriteManager;

		Sprite m_sprite;

	public:

		AddSprite(SpriteManager* spriteManager, int id, const String& texture, const RectF& region) :
			m_spriteManager(spriteManager),
			m_sprite(id, texture, region) {}

		void execute() override
		{
			m_spriteManager->addSprite(m_sprite);
		};

	};

	class AddFixedSprite : public ScenarioCommand
	{
	private:

		SpriteManager* m_spriteManager;

		FixedSprite m_fixedSprite;

	public:

		AddFixedSprite(SpriteManager* spriteManager, int id, const String& texture, const RectF& region) :
			m_spriteManager(spriteManager),
			m_fixedSprite(id, texture, region) {}

		void execute() override
		{
			m_spriteManager->addSprite(m_fixedSprite);
		};

	};
}
