# include <Siv3D.hpp>
# include "asc\AscNovel.hpp"

class Button : public asc::IMessageButton
{
private:

	Stopwatch m_stopwatch;

public:

	void init() override
	{
		m_stopwatch.start();
	}

	void update() override {}

	void onClick() override
	{
		m_stopwatch.reset();
	}

	void draw() const override
	{
		const double r = Min<double>(10.0, m_stopwatch.ms() / 100);

		Circle(100, 100, r).draw();
	}
};

void Main()
{
	Window::Resize(1280, 720);
	TextureAsset::Register(L"character1", L"Assets/character1.png");
	TextureAsset::Register(L"character2", L"Assets/character2.png");
	TextureAsset::Register(L"character3", L"Assets/character3.png");
	TextureAsset::Register(L"message", L"Assets/test_message_box.png");
	TextureAsset::Register(L"choice", L"Assets/test_choice_box.png");
	SoundAsset::Register(L"char", L"Example/Sound.mp3");
	SoundAsset::Register(L"move", L"Example/Sound.mp3");
	SoundAsset::Register(L"submit", L"Example/Sound.mp3");
	SoundAsset::Register(L"bgm1", L"Example/風の丘.mp3");
	SoundAsset::Register(L"bgm2", L"Example/風の丘.mp3");
	FontAsset::Register(L"name", 32, L"メイリオ");
	FontAsset::Register(L"text", 30, L"メイリオ");

	asc::Novel novel;

	novel
		.setFont(L"text", L"name")
		.setColor(Palette::Black, Palette::Red)
		.setMessageTexture(L"message", Rect(6, 440, 1268, 285))
		.setMessagePosition({60, 575}, {40, 525})
		.setChoiceTexture(L"choice", Rect(850, 330, 400, 160))
		.setChoicePosition({870, 340})
		.setSound(L"char", L"move", L"submit")
		.setButton(std::make_unique<Button>())
		.setKey(KeyCombination(Input::KeyEnter), KeyCombination(Input::KeySpace), KeyCombination(Input::KeyUp), KeyCombination(Input::KeyDown));

	while (System::Update())
	{
		if (Input::Key0.clicked)
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
