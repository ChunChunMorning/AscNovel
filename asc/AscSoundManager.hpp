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

		void playLetterSound()
		{
			// ToDo Configurable
			const String letterSound = L"letter";

			SoundAsset(letterSound).playMulti();
		}

	};
}
