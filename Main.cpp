# include <Siv3D.hpp>
# include "asc\AscNovel.hpp"

void Main()
{
	Window::Resize(1280, 720);
	TextureAsset::Register(L"character1", L"Assets/character1.png");
	TextureAsset::Register(L"character2", L"Assets/character2.png");
	TextureAsset::Register(L"character3", L"Assets/character3.png");
	TextureAsset::Register(L"test_message_box", L"Assets/test_message_box.png");
	TextureAsset::Register(L"test_choice_box", L"Assets/test_choice_box.png");
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

		if (Input::Key3.clicked)
			novel.start(3);

		if (Input::Key4.clicked)
			novel.start(4);

		if (Input::Key5.clicked)
			novel.start(5);

		if (Input::Key6.clicked)
			novel.start(6);

		if (Input::Key7.clicked)
			novel.start(7);

		if (Input::Key8.clicked)
			novel.start(8);

		if (Input::Key9.clicked)
			novel.start(9);

		novel.update();

		if(novel.isUpdating())
			novel.draw();
	}
}
