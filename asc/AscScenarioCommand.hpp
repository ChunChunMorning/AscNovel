# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class ScenarioCommand : Uncopyable
	{
	private:

		const Optional<int32> m_seekPoint;

	public:

		ScenarioCommand() : m_seekPoint(none) {}

		ScenarioCommand(int32 seekPoint) : m_seekPoint(seekPoint) {}

		virtual ~ScenarioCommand() = default;

		virtual void execute() = 0;

		Optional<int32> getSeekPoint() const { return m_seekPoint; }

	};
}