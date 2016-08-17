# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class SoundManager
	{
	private:

	public:

		SoundManager() = default;

		virtual ~SoundManager() = default;

		void playCharSound()
		{
			// ToDo Configurable
			const String charSound = L"char";

			if (SoundAsset::IsRegistered(charSound))
			{
				SoundAsset(charSound).playMulti();
			}
		}

		void playMoveSound()
		{
			// ToDo Configurable
			const String moveSound = L"move";

			if (SoundAsset::IsRegistered(moveSound))
			{
				SoundAsset(moveSound).playMulti();
			}
		}

		void playSubmitSound()
		{
			// ToDo Configurable
			const String submitSound = L"submit";

			if (SoundAsset::IsRegistered(submitSound))
			{
				SoundAsset(submitSound).playMulti();
			}
		}

	};
}
