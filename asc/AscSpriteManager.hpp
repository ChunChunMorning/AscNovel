# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	class SpriteManager
	{
	private:

	public:

		SpriteManager()
		{
			
		}

		virtual ~SpriteManager() = default;

		void draw() const
		{
			Rect(0, 0, 640, 720).draw();
		}
	};
}
