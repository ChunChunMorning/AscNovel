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

		int32 m_currentLine;

	public:

		ScenarioController() :
			m_isUpdating(false) {}

		bool start(int32 seekPoint, const Array<std::unique_ptr<ScenarioCommand>>& scenarioCommands)
		{
			for (auto i = 0u; i < scenarioCommands.size() - 1; i++)
			{
				auto index = (m_currentLine + i) % scenarioCommands.size();
				auto point = scenarioCommands[index]->getSeekPoint();

				if (point && point.value() == seekPoint)
				{
					m_isUpdating = true;
					m_currentLine = index + 1;

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

		int32 getCurrentLine() const
		{
			return m_currentLine;
		}

	};
}
