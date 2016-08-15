# include <Siv3D.hpp>
# include "asc\AscNovel.hpp"

void Main()
{
	Window::Resize(1280, 720);
	TextureAsset::Register(L"character1", L"character1.png");
	TextureAsset::Register(L"character2", L"character2.png");
	TextureAsset::Register(L"character3", L"character3.png");
	TextureAsset::Register(L"test_message_box", L"test_message_box.png");
	TextureAsset::Register(L"test_choice_box", L"test_choice_box.png");
	FontAsset::Register(L"test_name", 30, L"メイリオ");
	FontAsset::Register(L"test_text", 30, L"メイリオ");

	asc::Novel novel;

	while (System::Update())
	{
		if(Input::Key0.clicked)
			novel.start(0);

		if(Input::Key1.clicked)
			novel.start(1);

		if(Input::Key2.clicked)
			novel.start(2);

		novel.update();

		if(novel.isUpdating())
			novel.draw();
	}
}
