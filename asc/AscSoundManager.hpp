# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	using SoundAssetName = String;

	class SoundManager
	{
	private:

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

		void playBGM(const String& string)
		{
			const auto args = string.split(L',');
			playBGM(args[0], Parse<double>(args[1]));
		}

		void playBGM(const String& bgm, double time)
		{
			SoundAsset(bgm).setVolume(m_bgmVolume);
			SoundAsset(bgm).play(static_cast<SecondsF>(time));
		}

		void stopBGM(const String& string)
		{
			const auto args = string.split(L',');
			stopBGM(args[0], Parse<double>(args[1]));
		}

		void stopBGM(const String& bgm, double time)
		{
			SoundAsset(bgm).stop(static_cast<SecondsF>(time));
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
