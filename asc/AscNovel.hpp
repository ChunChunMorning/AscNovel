# pragma once
# include <Siv3D.hpp>
# include "AscMessageManager.hpp"

namespace asc
{
	using namespace s3d;

	class Novel
	{
	private:

		std::unique_ptr<MessageManager> m_messageManager;

	public:

		Novel() :
			m_messageManager(std::make_unique<MessageManager>()) {}

		virtual ~Novel() = default;

		void start()
		{
			m_messageManager->start();
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