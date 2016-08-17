# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class BGM
	{
	protected:

		String m_bgm;

		double m_time;

		Stopwatch m_stopwatch;

	public:

		BGM() = default;

		BGM(const String& bgm, int32 time) :
			m_bgm(bgm), m_time(time)
		{
			m_stopwatch.start();
		}

		virtual ~BGM() = default;

		virtual void update() const {};

		bool isFinished() const
		{
			return m_stopwatch.ms() > m_time;
		}
	};

	class FadeInBGM : public BGM
	{
	public:

		FadeInBGM() = default;

		FadeInBGM(const String& bgm, int32 time) :
			BGM(bgm, time)
		{
			SoundAsset(m_bgm).play();
		}

		virtual ~FadeInBGM() = default;

		virtual void update() const override
		{
			SoundAsset(m_bgm).setVolume(m_stopwatch.ms() / static_cast<double>(m_time));
		}

	};

	class FadeOutBGM : public BGM
	{
	public:

		FadeOutBGM() = default;

		FadeOutBGM(const String& bgm, int32 time) :
			BGM(bgm, time) {}

		virtual ~FadeOutBGM()
		{
			SoundAsset(m_bgm).stop();
		}

		virtual void update() const override
		{
			SoundAsset(m_bgm).setVolume(1.0 - m_stopwatch.ms() / static_cast<double>(m_time));
		}

	};

}
