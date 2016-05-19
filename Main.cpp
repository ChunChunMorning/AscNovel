
# include <Siv3D.hpp>
# include "asc\AscNovel.hpp"

void Main()
{
	asc::Novel novel;

	while (System::Update())
	{
		novel.update();
		novel.draw();
	}
}
