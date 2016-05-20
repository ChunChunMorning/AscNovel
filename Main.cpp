# include <Siv3D.hpp>
# include "asc\AscNovel.hpp"

void Main()
{
	Window::Resize(1280, 720);

	TextureAsset::Register(L"test_text_box", L"test_text_box.png");
	FontAsset::Register(L"test_name", 30, L"メイリオ");
	FontAsset::Register(L"test_text", 30, L"メイリオ");

	asc::Novel novel;

	novel.start(0);

	while (System::Update())
	{
		novel.update();
		novel.draw();
	}
}
