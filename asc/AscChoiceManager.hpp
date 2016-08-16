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

		int32 m_lastSelectedSeekPoint;

		int32 m_selected;

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
			if(!m_isUpdating)
				return;

			const KeyCombination m_submit = Input::KeyEnter | Gamepad(0).button(0);
			const KeyCombination m_up = Input::KeyUp | Gamepad(0).povForward;
			const KeyCombination m_down = Input::KeyDown | Gamepad(0).povBackward;

			if (m_submit.clicked)
			{
				m_lastSelectedSeekPoint = m_choices[m_selected].seekPoint;
				m_isUpdating = false;
				return;
			}

			if (m_up.clicked)
			{
				m_selected = Max<int32>(m_selected - 1, 0);
			}
			if (m_down.clicked)
			{
				m_selected = Min<int32>(m_selected + 1, m_choices.size() - 1);
			}
		}

		void clear()
		{
			m_selected = 0;
			m_choices.clear();
		}

		int32 lastSelectedSeekPoint() const
		{
			return m_lastSelectedSeekPoint;
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
			const Color m_selectedColor = Palette::Red;

			m_choiceBox(TextureAsset(m_choiceBoxTexture)).draw();

			for (auto i = 0u; i < m_choices.size(); i++)
			{
				const auto color = static_cast<int32>(i) == m_selected ? m_selectedColor : m_textColor;
				FontAsset(m_textFont)(m_choices[i].text).draw(m_choices[i].position, color);
			}
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

	};
}
