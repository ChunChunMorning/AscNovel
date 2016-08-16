# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	struct Choice
	{
		int32 seekPoint;
		String text;
		Point position;
	};

	class ChoiceManager
	{
	private:

		bool m_isUpdating;

		Array<Choice> m_choices;

	public:

		ChoiceManager() :
			m_isUpdating(false) {}

		virtual ~ChoiceManager() = default;

		void start(String text)
		{
			const auto args = text.split(L',');
			Array<std::pair<int32, String>> choices;

			for (auto i = 0u; i < args.size(); i += 2)
			{
				choices.push_back(std::make_pair(Parse<int32>(args[i]), args[i + 1]));
			}

			start(choices);
		}

		void start(const Array<std::pair<int32, String>>& choices)
		{
			// ToDo Configurable
			const Point m_textPosition(870, 340);
			const String m_textFont = L"test_text";

			Point position = m_textPosition;

			for (const auto& c : choices)
			{
				m_choices.push_back({c.first, c.second, position});
				position.y += FontAsset(m_textFont)(c.second).region().h;
			}

			m_isUpdating = true;
		}

		void update()
		{
			
		}

		void clear()
		{
			
		}

		void draw() const
		{
			if(!m_isUpdating)
				return;

			// ToDo Configurable
			const Rect m_choiceBox(850, 330, 400, 160);
			const String m_choiceBoxTexture = L"test_choice_box";
			const String m_textFont = L"test_text";
			const Color m_textColor = Palette::Black;


			m_choiceBox(TextureAsset(m_choiceBoxTexture)).draw();

			for (auto i = 0u; i < m_choices.size(); i++)
			{
				FontAsset(m_textFont)(m_choices[i].text).draw(m_choices[i].position);
			}
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

	};
}
