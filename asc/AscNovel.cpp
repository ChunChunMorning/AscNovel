# include "AscNovel.hpp"
# include <Siv3D.hpp>
# include "AscChoiceManager.hpp"
# include "AscMessageManager.hpp"
# include "AscIMessgeButton.hpp"
# include "AscSoundManager.hpp"
# include "AscSpriteManager.hpp"
# include "AscTimeManager.hpp"

namespace asc
{
	using namespace s3d;

	using Commnad = std::pair<int32, String>;

	const Commnad EndOfCommand = std::pair<int32, String>(0, L"-1");

	class Novel::CNovel
	{
	public:

		bool m_isUpdating;

		int32 m_currentLine;

		int32 m_lastSeekPoint;

		KeyCombination m_skip;

		Array<Commnad> m_commands;

		ChoiceManager m_choiceManager;

		MessageManager m_messageManager;

		SoundManager m_soundManager;

		SpriteManager m_spriteManager;

		TimeManager m_timeManager;

		CNovel() :
			m_isUpdating(false),
			m_currentLine(0),
			m_lastSeekPoint(-1),
			m_commands({ EndOfCommand }),
			m_choiceManager(
				[&] { m_soundManager.playMoveSound(); },
				[&] { m_soundManager.playSubmitSound(); }
				),
			m_messageManager(
				[&] { m_soundManager.playCharSound(); }
				) {}

		void loadByString(const String& scenario, bool isAdditive = false)
		{
			isAdditive ? m_commands.pop_back() : m_commands.clear();

			const auto lines = scenario.trim().split(L'\n');

			for (const auto& line : lines)
			{
				const auto pos = line.indexOf(L",");
				m_commands.push_back(std::make_pair(Parse<int32>(line.substr(0U, pos)), line.substr(pos + 1U, line.length)));
			}

			m_commands.push_back(EndOfCommand);
		}

		void clearManager()
		{
			m_messageManager.clear();
			m_spriteManager.clear();
			m_choiceManager.clear();
			m_timeManager.clear();
		}

		void execute()
		{
			switch (m_commands[m_currentLine].first)
			{
				// Point
			case 0:
				m_isUpdating = false;
				return;

				// Text
			case 1:
				m_messageManager.setText(m_commands[m_currentLine].second);
				m_messageManager.start();
				break;

				// Name
			case 2:
				m_messageManager.setName(m_commands[m_currentLine].second);
				break;

				// AddSprite
			case 3:
				m_spriteManager.add<Sprite>(m_commands[m_currentLine].second);
				break;

				// AddFixedSprite
			case 4:
				m_spriteManager.add<FixedSprite>(m_commands[m_currentLine].second);
				break;

				// Choice
			case 5:
				m_choiceManager.start(m_commands[m_currentLine].second);
				break;

				// Jump
			case 6:
				start(Parse<int32>(m_commands[m_currentLine].second));
				return;

				// AutomaticText
			case 7:
				m_messageManager.setText(m_commands[m_currentLine].second);
				m_messageManager.start(true);
				break;

				// Play BGM
			case 8:
				m_soundManager.playBGM(m_commands[m_currentLine].second);
				break;

				// Stop BGM
			case 9:
				m_soundManager.stopBGM(m_commands[m_currentLine].second);
				break;

				// Lihgt Up
			case 10:
				m_spriteManager.lightUp(m_commands[m_currentLine].second);
				break;

				// Lihgt Up Spot
			case 11:
				m_spriteManager.lightUpSpot(m_commands[m_currentLine].second);
				break;

				// Bring
			case 12:
				m_spriteManager.bring(m_commands[m_currentLine].second);
				break;

				// Erase
			case 13:
				m_spriteManager.erase(m_commands[m_currentLine].second);
				break;

				// Wait
			case 14:
				m_timeManager.wait(m_commands[m_currentLine].second);
				break;

			default:
				break;
			}

			m_currentLine++;
		}

		void skip()
		{
			while (m_isUpdating && !m_choiceManager.isUpdating())
			{
				execute();
			}

			m_timeManager.clear();
			m_messageManager.skip();
		}

		bool start(int32 seekPoint)
		{
			const auto size = m_commands.size() - 1;
			for (auto i = 0u; i < size; i++)
			{
				const auto index = (m_currentLine + i) % m_commands.size();

				const auto command = m_commands[index];
				if (
					command.first == 0 &&
					Parse<int32>(command.second) == seekPoint
					)
				{
					clearManager();
					m_currentLine = index + 1;
					m_lastSeekPoint = seekPoint;
					m_isUpdating = true;

					return true;
				}
			}

			return false;
		}

		void update()
		{
			if (m_skip.clicked)
			{
				skip();
			}

			while (
				m_isUpdating &&
				!m_messageManager.isUpdating() &&
				!m_choiceManager.isUpdating() &&
				!m_timeManager.isUpdating()
				)
			{
				m_choiceManager.lastSelectedSeekPoint().then([&](int32 seekPoint) { start(seekPoint); });
				execute();
			}

			m_messageManager.update();
			m_choiceManager.update();
		}

	};
}

asc::Novel::Novel() : pImpl(std::make_shared<CNovel>()) {}

bool asc::Novel::load(const FilePath& path, const Optional<TextEncoding>& encoding, bool isAdditive)
{
	TextReader reader(path, encoding);

	if (!reader.isOpened())
		return false;

	loadByString(reader.readAll(), isAdditive);
	return true;
}

void asc::Novel::loadByString(const String& scenario, bool isAdditive)
{
	pImpl->loadByString(scenario, isAdditive);
}

bool asc::Novel::start(int32 seekPoint)
{
	return pImpl->start(seekPoint);
}

void asc::Novel::update()
{
	pImpl->update();
}

bool asc::Novel::isUpdating() const
{
	return pImpl->m_isUpdating;
}

int32 asc::Novel::seekPoint() const
{
	return pImpl->m_lastSeekPoint;
}

void asc::Novel::draw() const
{
	pImpl->m_spriteManager.draw();
	pImpl->m_messageManager.draw();
	pImpl->m_choiceManager.draw();
}

asc::Novel& asc::Novel::setSpeed(int32 speed)
{
	pImpl->m_messageManager.setSpeed(speed);

	return *this;
}

asc::Novel& asc::Novel::setWaitingTime(int32 time)
{
	pImpl->m_messageManager.setTime(time);

	return *this;
}

asc::Novel& asc::Novel::setKey(const KeyCombination& submit, const KeyCombination& skip)
{
	pImpl->m_skip = skip;
	pImpl->m_messageManager.setKey(submit);

	return *this;
}

asc::Novel& asc::Novel::setKey(const KeyCombination& submit, const KeyCombination& skip, const KeyCombination& up, const KeyCombination& down)
{
	pImpl->m_skip = skip;
	pImpl->m_messageManager.setKey(submit);
	pImpl->m_choiceManager.setKey(submit, up, down);

	return *this;
}

asc::Novel& asc::Novel::setButton(std::unique_ptr<IMessageButton>&& button)
{
	pImpl->m_messageManager.setButton(std::move(button));

	return *this;
}

asc::Novel& asc::Novel::setFont(const FontAssetName& text, const FontAssetName& name)
{
	pImpl->m_messageManager.setFont(text, name);
	pImpl->m_choiceManager.setFont(text);

	return *this;
}

asc::Novel& asc::Novel::setColor(const Color& color, const Color& selectedColor)
{
	pImpl->m_messageManager.setColor(color);
	pImpl->m_choiceManager.setColor(color, selectedColor);

	return *this;
}

asc::Novel& asc::Novel::setMessageTexture(const TextureAssetName& texture, const Rect& region)
{
	pImpl->m_messageManager.setTexture(texture, region);

	return *this;
}

asc::Novel& asc::Novel::setMessagePosition(const Point& text, const Point& name)
{
	pImpl->m_messageManager.setPosition(text, name);

	return *this;
}

asc::Novel& asc::Novel::setChoiceTexture(const TextureAssetName texture, const Rect& region)
{
	pImpl->m_choiceManager.setTexture(texture, region);

	return *this;
}

asc::Novel& asc::Novel::setChoicePosition(const Point& position)
{
	pImpl->m_choiceManager.setPosition(position);

	return *this;
}

asc::Novel& asc::Novel::setBGMVolume(double volume)
{
	pImpl->m_soundManager.setBGMVolume(volume);

	return *this;
}

asc::Novel& asc::Novel::setSEVolume(double volume)
{
	pImpl->m_soundManager.setSEVolume(volume);

	return *this;
}

asc::Novel& asc::Novel::setSound(const SoundAssetName& charCount)
{
	pImpl->m_soundManager.setSE(charCount);

	return *this;
}

asc::Novel& asc::Novel::setSound(const SoundAssetName& charCount, const SoundAssetName& move, const SoundAssetName& submit)
{
	pImpl->m_soundManager.setSE(charCount, move, submit);

	return *this;
}

asc::Novel& asc::Novel::setSilentChars(const Array<wchar> silentChars)
{
	pImpl->m_messageManager.setSilentChars(silentChars);

	return *this;
}
