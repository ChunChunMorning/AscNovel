# pragma once
# include <Siv3D.hpp>
# include "AscIMessgeButton.hpp"

namespace asc
{
	using namespace s3d;

	using TextureAssetName = String;
	using SoundAssetName = String;
	using FontAssetName = String;

	class Novel
	{
	private:

		class CNovel;

		std::shared_ptr<CNovel> pImpl;

	public:

		Novel();

		virtual ~Novel() = default;

		bool load(const FilePath& path, const Optional<TextEncoding>& encoding = unspecified, bool isAdditive = false);

		void loadByString(const String& scenario, bool isAdditive = false);

		void clear();

		bool start(int32 seekPoint);

		void update();

		bool isUpdating() const;

		int32 seekPoint() const;

		void draw() const;

		Novel& setSpeed(double charPerSecond);

		Novel& setWaitingTime(double second);

		Novel& setKey(const KeyCombination& submit, const KeyCombination& skip);

		Novel& setKey(const KeyCombination& submit, const KeyCombination& skip, const KeyCombination& up, const KeyCombination& down);

		Novel& setButton(std::unique_ptr<IMessageButton>&& button);

		template <class Type>
		Novel& setButton(Type&& button)
		{
			return setButton(static_cast<std::unique_ptr<IMessageButton>>(std::make_unique<Type>(button)));
		}

		Novel& setFont(const FontAssetName& text)
		{
			return setFont(text, text);
		}

		Novel& setFont(const FontAssetName& text, const FontAssetName& name);

		Novel& setColor(const Color& color, const Color& selectedColor = Palette::Red);

		Novel& setMessageTexture(const TextureAssetName& texture, const Rect& region);

		Novel& setMessagePosition(const Point& text, const Point& name = Point::Zero);

		Novel& setChoiceTexture(const TextureAssetName texture, const Rect& region);

		Novel& setChoicePosition(const Point& position);

		Novel& setBGMVolume(double volume);

		Novel& setSEVolume(double volume);

		Novel& setSound(const SoundAssetName& charCount);

		Novel& setSound(const SoundAssetName& charCount, const SoundAssetName& move, const SoundAssetName& submit);

		Novel& setSilentChars(const Array<wchar> silentChars);

	};
}