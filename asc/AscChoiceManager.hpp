# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	using TextureAssetName = String;
	using FontAssetName = String;

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

		Optional<int32> m_lastSelectedSeekPoint;

		int32 m_selected;

		Array<Choice> m_choices;

		KeyCombination m_keyUp;

		KeyCombination m_keyDown;

		KeyCombination m_keySubmit;

		TextureAssetName m_texture;

		FontAssetName m_font;

		Color m_unselectedColor;

		Color m_selectedColor;

		Rect m_region;

		Point m_position;

		std::function<void()> m_onMove;

		std::function<void()> m_onSubmit;

		int32 select()
		{
			auto selected = m_selected;

			if (m_keyUp.clicked)
			{
				selected = Max<int32>(m_selected - 1, 0);
			}
			if (m_keyDown.clicked)
			{
				selected = Min<int32>(m_selected + 1, m_choices.size() - 1);
			}

			return selected;
		}

	public:

		ChoiceManager(
			std::function<void()> onMove,
			std::function<void()> onSubmit
		) :
			m_isUpdating(false),
			m_lastSelectedSeekPoint(none),
			m_unselectedColor(Palette::Black),
			m_selectedColor(Palette::Red),
			m_onMove(onMove),
			m_onSubmit(onSubmit) {}

		void setKey(const KeyCombination& submit, const KeyCombination& up, const KeyCombination& down)
		{
			m_keySubmit = submit;
			m_keyUp = up;
			m_keyDown = down;
		}

		void setTexture(const TextureAssetName& texture, const Rect& region)
		{
			m_texture = texture;
			m_region = region;
		}

		void setFont(const FontAssetName& font)
		{
			m_font = font;
		}

		void setColor(const Color& unselected, const Color& selected)
		{
			m_unselectedColor = unselected;
			m_selectedColor = selected;
		}

		void setPosition(const Point& position)
		{
			m_position = position;
		}

		void start(String string)
		{
			const auto args = string.split(L',');
			Array<std::pair<int32, String>> choices;

			for (auto i = 0u; i < args.size(); i += 2)
			{
				choices.push_back(std::make_pair(Parse<int32>(args[i]), args[i + 1]));
			}

			start(choices);
		}

		void start(const Array<std::pair<int32, String>>& choices)
		{
			Point position = m_position;

			for (const auto& c : choices)
			{
				m_choices.push_back({c.first, c.second, position});
				position.y += FontAsset(m_font)(c.second).region().h;
			}

			m_isUpdating = true;
		}

		void update()
		{
			if(!m_isUpdating)
				return;

			const KeyCombination m_submit = Input::KeyEnter | Gamepad(0).button(0);

			if (m_submit.clicked)
			{
				m_onSubmit();
				m_lastSelectedSeekPoint = m_choices[m_selected].seekPoint;
				m_isUpdating = false;
				return;
			}

			const auto selected = select();

			if (selected != m_selected)
			{
				m_onMove();
				m_selected = selected;
			}
		}

		void clear()
		{
			m_lastSelectedSeekPoint = none;
			m_selected = 0;
			m_choices.clear();
			m_isUpdating = false;
		}

		Optional<int32> lastSelectedSeekPoint() const
		{
			return m_lastSelectedSeekPoint;
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

		void draw() const
		{
			if(!m_isUpdating)
				return;

			m_region(TextureAsset(m_texture)).draw();

			for (auto i = 0u; i < m_choices.size(); i++)
			{
				const auto color = static_cast<int32>(i) == m_selected ? m_selectedColor : m_unselectedColor;
				FontAsset(m_font)(m_choices[i].text).draw(m_choices[i].position, color);
			}
		}

	};
}
