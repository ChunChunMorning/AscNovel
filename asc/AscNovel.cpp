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

	const auto EndOfCommand = Array<String>{ L"0", L"-1" };

	class Novel::CNovel
	{
	public:

		bool isUpdating;

		int32 currentLine;

		int32 lastSeekPoint;

		KeyCombination skip;

		Array<Array<String>> commands;

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
			choiceManager.clear();
			messageManager.clear();
			spriteManager.clear();
			timeManager.clear();
		}

		void execute()
		{
			switch (Parse<int32>(commands[currentLine][0]))
			{
			// Point
			case 0:
				isUpdating = false;
				return;

			// Text
			case 1:
			{
				const auto args = commands[currentLine][1].split(L',');
				messageManager.setText(args[0]);
				messageManager.start(args.size() > 1);
				break;
			}

			// Name
			case 2:
				messageManager.setName(commands[currentLine][1]);
				break;

			// Sprite
			case 3:
				//spriteManager.add<Sprite>(commands[currentLine][1]);
				break;

			// FixedSprite
			case 4:
				//spriteManager.add<FixedSprite>(commands[currentLine][1]);
				break;

			// Choice
			case 5:
				choiceManager.start(commands[currentLine][1]);
				break;

			// Jump
			case 6:
				start(Parse<int32>(commands[currentLine][1]));
				return;

			// Play
			case 8:
				soundManager.playBGM(commands[currentLine][1]);
				break;

			// Stop
			case 9:
				soundManager.stopBGM(commands[currentLine][1]);
				break;

			// Lihgt
			case 10:
				spriteManager.lightUp(commands[currentLine][1]);
				break;

			// Spot
			case 11:
				spriteManager.lightUpSpot(commands[currentLine][1]);
				break;

			// Bring
			case 12:
				spriteManager.bring(commands[currentLine][1]);
				break;

			// Erase
			case 13:
				spriteManager.erase(commands[currentLine][1]);
				break;

			// Wait
			case 14:
				timeManager.wait(commands[currentLine][1]);
				break;

			default:
				break;
			}

			currentLine++;
		}

		bool start(int32 seekPoint)
		{
			const auto size = commands.size() - 1U;
			for (auto i = 0u; i < size; i++)
			{
				const auto index = (currentLine + i) % commands.size();

				if (
					Parse<int32>(commands[index][0]) == 0 &&
					Parse<int32>(commands[index][1]) == seekPoint
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

			messageManager.skip();
			timeManager.clear();
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
		pImpl->commands.push_back(line.split(L','));
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
		!pImpl->choiceManager.isUpdating() &&
		!pImpl->messageManager.isUpdating() &&
		!pImpl->timeManager.isUpdating()
		)
	{
		pImpl->choiceManager.lastSelectedSeekPoint().then([&](int32 seekPoint) { start(seekPoint); });
		pImpl->execute();
	}

	pImpl->choiceManager.update();
	pImpl->messageManager.update();
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
	pImpl->choiceManager.setKey(submit, up, down);
	pImpl->messageManager.setKey(submit);

	return *this;
}

asc::Novel& asc::Novel::setButton(std::unique_ptr<IMessageButton>&& button)
{
	pImpl->messageManager.setButton(std::move(button));

	return *this;
}

asc::Novel& asc::Novel::setFont(const FontAssetName& text, const FontAssetName& name)
{
	pImpl->choiceManager.setFont(text);
	pImpl->messageManager.setFont(text, name);

	return *this;
}

asc::Novel& asc::Novel::setColor(const Color& color, const Color& selectedColor)
{
	pImpl->choiceManager.setColor(color, selectedColor);
	pImpl->messageManager.setColor(color);

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
