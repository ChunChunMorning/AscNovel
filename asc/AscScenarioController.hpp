# pragma once
# include <Siv3D.hpp>
# include "AscScenarioCommand.hpp"

namespace asc
{
	using namespace s3d;

	class ScenarioController
	{
	private:

		bool m_isUpdating;

	public:

		int32 currentLine;

		ScenarioController() :
			m_isUpdating(false),
			currentLine(0) {}

		bool start(int32 seekPoint, const Array<std::unique_ptr<ScenarioCommand>>& scenarioCommands)
		{
			for (auto i = 0u; i < scenarioCommands.size() - 1; i++)
			{
				const auto index = (currentLine + i) % scenarioCommands.size();
				const auto point = scenarioCommands[index]->getSeekPoint();

				if (point && point.value() == seekPoint)
				{
					m_isUpdating = true;
					currentLine = index + 1;

					return true;
				}
			}

			return false;
		}

		void finish()
		{
			m_isUpdating = false;
		}

		bool isUpdating() const
		{
			return m_isUpdating;
		}

	};
}
