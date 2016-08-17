# pragma once
# include <Siv3D.hpp>
# include "AscSprite.hpp"

namespace asc
{
	using namespace s3d;

	class TimeManager
	{
	private:

		Stopwatch m_stopwatch;

		int32 m_time;

	public:

		TimeManager() :
			m_time(-1) {}

		void wait(const String& string)
		{
			wait(Parse<int32>(string));
		}

		void wait(int32 time)
		{
			m_stopwatch.start();
			m_time = time;
		}

		void clear()
		{
			m_time = -1;
			m_stopwatch.reset();
		}

		bool isUpdating() const
		{
			m_time > m_stopwatch.ms();
		}

	};
}
