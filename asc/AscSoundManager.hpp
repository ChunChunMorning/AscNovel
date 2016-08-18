# pragma once
# include <Siv3D.hpp>
# include "AscBGM.hpp"

namespace asc
{
	using namespace s3d;

	using SoundAssetName = String;

	class SoundManager
	{
	private:

		Array<std::unique_ptr<BGM>> m_BGMs;

		double m_bgmVolume;

		double m_seVolume;

		SoundAssetName m_charCount;

		SoundAssetName m_move;

		SoundAssetName m_submit;

	public:

		SoundManager() :
			m_bgmVolume(1.0), m_seVolume(1.0) {};

		void setBGMVolume(double volume)
		{
			m_bgmVolume = volume;

			for (const auto& bgm : m_BGMs)
			{
				bgm->setVolume(m_bgmVolume);
			}
		}

		void setSEVolume(double volume)
		{
			m_seVolume = volume;
		}

		void setSE(const SoundAssetName& charCount)
		{
			m_charCount = charCount;
		}

		void setSE(const SoundAssetName& charCount, const SoundAssetName& move, const SoundAssetName& submit)
		{
			m_charCount = charCount;
			m_move = move;
			m_submit = submit;
		}

		void update()
		{
			for (const auto& bgm : m_BGMs)
			{
				bgm->update();
			}

			Erase_if(m_BGMs, [](const std::unique_ptr<BGM>& bgm){ return bgm->isFinished(); });
		}

		void playBGM(const String& string)
		{
			const auto args = string.split(L',');
			playBGM(args[0], Parse<int32>(args[1]));
		}

		void playBGM(const String& bgm, int32 time)
		{
			if (time == 0)
			{
				SoundAsset(bgm).setVolume(m_bgmVolume);
				SoundAsset(bgm).play();
				return;
			}

			m_BGMs.push_back(std::make_unique<FadeInBGM>(bgm, m_bgmVolume, time));
		}

		void stopBGM(const String& string)
		{
			const auto args = string.split(L',');
			stopBGM(args[0], Parse<int32>(args[1]));
		}

		void stopBGM(const String& bgm, int32 time)
		{
			time == 0 ? SoundAsset(bgm).stop() : m_BGMs.push_back(std::make_unique<FadeOutBGM>(bgm, m_bgmVolume, time));
		}

		void playSE(const SoundAssetName& sound)
		{
			if (!SoundAsset::IsRegistered(sound))
				return;

			SoundAsset(sound).playMulti(m_seVolume);
		}

		void playCharSound()
		{
			playSE(m_charCount);
		}

		void playMoveSound()
		{
			playSE(m_move);
		}

		void playSubmitSound()
		{
			playSE(m_submit);
		}

	};
}
