# pragma once
# include <Siv3D.hpp>
# include "AscBGM.hpp"

namespace asc
{
	using namespace s3d;

	class SoundManager
	{
	private:

		Array<std::unique_ptr<BGM>> m_BGMs;

	public:

		SoundManager() = default;

		virtual ~SoundManager() = default;

		void update()
		{
			for (const auto& bgm : m_BGMs)
			{
				bgm->update();
			}

			Erase_if(m_BGMs, [](const std::unique_ptr<BGM>& bgm){ return bgm->isFinished(); });
		}

		void playBGM(const String& bgm, int32 time = 0)
		{
			if (time == 0)
			{
				SoundAsset(bgm).play();
				return;
			}

			m_BGMs.push_back(std::make_unique<FadeInBGM>(bgm, time));
		}

		void stopBGM(const String& bgm, int32 time = 0)
		{
			if (time == 0)
			{
				SoundAsset(bgm).stop();
				return;
			}

			m_BGMs.push_back(std::make_unique<FadeOutBGM>(bgm, time));
		}

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
