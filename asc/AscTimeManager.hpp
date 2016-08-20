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

		void wait(double second)
		{
			m_stopwatch.start();
			m_time = static_cast<int32>(second * 1000);
		}

		void clear()
		{
			m_time = -1;
			m_stopwatch.reset();
		}

		bool isUpdating() const
		{
			return m_time > m_stopwatch.ms();
		}

	};
}
