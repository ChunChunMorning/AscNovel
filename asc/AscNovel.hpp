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

		void start(int32 seekPoint)
		{
			switch (seekPoint)
			{
			case 0:
				m_messageManager->setName(L"0: 名前");
				m_messageManager->setText(L"文章一行目。\n文章二行目");
				break;

			case 1:
				m_messageManager->setName(L"1: 名前");
				m_messageManager->setText(L"単一行テキスト");
				break;

			default:
				m_messageManager->setText(L"テキストのみ更新");
				break;
			}
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