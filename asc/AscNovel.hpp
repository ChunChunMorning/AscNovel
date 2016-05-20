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
				m_messageManager->setName(L"0: ���O");
				m_messageManager->setText(L"���͈�s�ځB\n���͓�s��");
				break;

			case 1:
				m_messageManager->setName(L"1: ���O");
				m_messageManager->setText(L"�P��s�e�L�X�g");
				break;

			default:
				m_messageManager->setText(L"�e�L�X�g�̂ݍX�V");
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