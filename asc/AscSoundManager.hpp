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

	};
}
