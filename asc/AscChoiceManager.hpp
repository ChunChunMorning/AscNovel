# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class ChoiceManager
	{
	private:

		bool m_isUpdating;

	public:

		ChoiceManager() :
			m_isUpdating(false) {}

		virtual ~ChoiceManager() = default;

		void start()
		{
			
		}

		void update()
		{
			
		}

		void clear()
		{
			
		}

		void draw() const
		{
			// ToDo Configurable
			const Rect m_choiceBox(850, 330, 400, 160);
			const Point m_textPosition(870, 340);
			const String m_choiceBoxTexture = L"test_choice_box";
			const String m_textFont = L"test_text";
			const Color m_textColor = Palette::Black;
			const String m_text = L"Yes\nNo";



			m_choiceBox(TextureAsset(m_choiceBoxTexture)).draw();

			FontAsset(m_textFont).draw(m_text, m_textPosition, m_textColor);
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

	};
}
