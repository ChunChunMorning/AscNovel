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

		bool isUpdating;

		int32 currentLine;

		int32 lastSeekPoint;

		KeyCombination skip;

		Array<Commnad> commands;

		ChoiceManager choiceManager;

		MessageManager messageManager;

		SoundManager soundManager;

		SpriteManager spriteManager;

		TimeManager timeManager;

		CNovel() :
			isUpdating(false),
			currentLine(0),
			lastSeekPoint(-1),
			commands({ EndOfCommand }),
			choiceManager(
				[&] { soundManager.playMoveSound(); },
				[&] { soundManager.playSubmitSound(); }
				),
			messageManager(
				[&] { soundManager.playCharSound(); }
				) {}

		void clearManager()
		{
			messageManager.clear();
			spriteManager.clear();
			choiceManager.clear();
			timeManager.clear();
		}

		void execute()
		{
			switch (commands[currentLine].first)
			{
				// Point
			case 0:
				isUpdating = false;
				return;

				// Text
			case 1:
				messageManager.setText(commands[currentLine].second);
				messageManager.start();
				break;

				// Name
			case 2:
				messageManager.setName(commands[currentLine].second);
				break;

				// AddSprite
			case 3:
				spriteManager.add<Sprite>(commands[currentLine].second);
				break;

				// AddFixedSprite
			case 4:
				spriteManager.add<FixedSprite>(commands[currentLine].second);
				break;

				// Choice
			case 5:
				choiceManager.start(commands[currentLine].second);
				break;

				// Jump
			case 6:
				start(Parse<int32>(commands[currentLine].second));
				return;

				// AutomaticText
			case 7:
				messageManager.setText(commands[currentLine].second);
				messageManager.start(true);
				break;

				// Play BGM
			case 8:
				soundManager.playBGM(commands[currentLine].second);
				break;

				// Stop BGM
			case 9:
				soundManager.stopBGM(commands[currentLine].second);
				break;

				// Lihgt Up
			case 10:
				spriteManager.lightUp(commands[currentLine].second);
				break;

				// Lihgt Up Spot
			case 11:
				spriteManager.lightUpSpot(commands[currentLine].second);
				break;

				// Bring
			case 12:
				spriteManager.bring(commands[currentLine].second);
				break;

				// Erase
			case 13:
				spriteManager.erase(commands[currentLine].second);
				break;

				// Wait
			case 14:
				timeManager.wait(commands[currentLine].second);
				break;

			default:
				break;
			}

			currentLine++;
		}

		bool start(int32 seekPoint)
		{
			const auto size = commands.size() - 1;
			for (auto i = 0u; i < size; i++)
			{
				const auto index = (currentLine + i) % commands.size();

				const auto command = commands[index];
				if (
					command.first == 0 &&
					Parse<int32>(command.second) == seekPoint
					)
				{
					clearManager();
					currentLine = index + 1;
					lastSeekPoint = seekPoint;
					isUpdating = true;

					return true;
				}
			}

			return false;
		}

		void skipCommand()
		{
			while (isUpdating && !choiceManager.isUpdating())
			{
				execute();
			}

			timeManager.clear();
			messageManager.skip();
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
	isAdditive ? pImpl->commands.pop_back() : pImpl->commands.clear();

	const auto lines = scenario.trim().split(L'\n');

	for (const auto& line : lines)
	{
		const auto pos = line.indexOf(L",");
		pImpl->commands.push_back(std::make_pair(Parse<int32>(line.substr(0U, pos)), line.substr(pos + 1U, line.length)));
	}

	pImpl->commands.push_back(EndOfCommand);
}

bool asc::Novel::start(int32 seekPoint)
{
	return pImpl->start(seekPoint);
}

void asc::Novel::update()
{
	if (pImpl->skip.clicked)
	{
		pImpl->skipCommand();
	}

	while (
		pImpl->isUpdating &&
		!pImpl->messageManager.isUpdating() &&
		!pImpl->choiceManager.isUpdating() &&
		!pImpl->timeManager.isUpdating()
		)
	{
		pImpl->choiceManager.lastSelectedSeekPoint().then([&](int32 seekPoint) { start(seekPoint); });
		pImpl->execute();
	}

	pImpl->messageManager.update();
	pImpl->choiceManager.update();
}

bool asc::Novel::isUpdating() const
{
	return pImpl->isUpdating;
}

int32 asc::Novel::seekPoint() const
{
	return pImpl->lastSeekPoint;
}

void asc::Novel::draw() const
{
	pImpl->spriteManager.draw();
	pImpl->messageManager.draw();
	pImpl->choiceManager.draw();
}

asc::Novel& asc::Novel::setSpeed(int32 speed)
{
	pImpl->messageManager.setSpeed(speed);

	return *this;
}

asc::Novel& asc::Novel::setWaitingTime(int32 time)
{
	pImpl->messageManager.setTime(time);

	return *this;
}

asc::Novel& asc::Novel::setKey(const KeyCombination& submit, const KeyCombination& skip)
{
	pImpl->skip = skip;
	pImpl->messageManager.setKey(submit);

	return *this;
}

asc::Novel& asc::Novel::setKey(const KeyCombination& submit, const KeyCombination& skip, const KeyCombination& up, const KeyCombination& down)
{
	pImpl->skip = skip;
	pImpl->messageManager.setKey(submit);
	pImpl->choiceManager.setKey(submit, up, down);

	return *this;
}

asc::Novel& asc::Novel::setButton(std::unique_ptr<IMessageButton>&& button)
{
	pImpl->messageManager.setButton(std::move(button));

	return *this;
}

asc::Novel& asc::Novel::setFont(const FontAssetName& text, const FontAssetName& name)
{
	pImpl->messageManager.setFont(text, name);
	pImpl->choiceManager.setFont(text);

	return *this;
}

asc::Novel& asc::Novel::setColor(const Color& color, const Color& selectedColor)
{
	pImpl->messageManager.setColor(color);
	pImpl->choiceManager.setColor(color, selectedColor);

	return *this;
}

asc::Novel& asc::Novel::setMessageTexture(const TextureAssetName& texture, const Rect& region)
{
	pImpl->messageManager.setTexture(texture, region);

	return *this;
}

asc::Novel& asc::Novel::setMessagePosition(const Point& text, const Point& name)
{
	pImpl->messageManager.setPosition(text, name);

	return *this;
}

asc::Novel& asc::Novel::setChoiceTexture(const TextureAssetName texture, const Rect& region)
{
	pImpl->choiceManager.setTexture(texture, region);

	return *this;
}

asc::Novel& asc::Novel::setChoicePosition(const Point& position)
{
	pImpl->choiceManager.setPosition(position);

	return *this;
}

asc::Novel& asc::Novel::setBGMVolume(double volume)
{
	pImpl->soundManager.setBGMVolume(volume);

	return *this;
}

asc::Novel& asc::Novel::setSEVolume(double volume)
{
	pImpl->soundManager.setSEVolume(volume);

	return *this;
}

asc::Novel& asc::Novel::setSound(const SoundAssetName& charCount)
{
	pImpl->soundManager.setSE(charCount);

	return *this;
}

asc::Novel& asc::Novel::setSound(const SoundAssetName& charCount, const SoundAssetName& move, const SoundAssetName& submit)
{
	pImpl->soundManager.setSE(charCount, move, submit);

	return *this;
}

asc::Novel& asc::Novel::setSilentChars(const Array<wchar> silentChars)
{
	pImpl->messageManager.setSilentChars(silentChars);

	return *this;
}
