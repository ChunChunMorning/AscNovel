# pragma once
# include <Siv3D.hpp>

namespace asc
{
	class Novel
	{
	public:
		Novel() = default;

		virtual ~Novel() = default;

		void start(uint32 seekPoint) {}

		void update() {}

		void draw() const {}

	private:

	};
}