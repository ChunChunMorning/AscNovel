# include <Siv3D.hpp>
# include "asc\AscNovel.hpp"

class MyButton : public asc::IMessageButton
{
private:

	Stopwatch m_stopwatch;

public:

	void onShownAll() override
	{
		m_stopwatch.start();
	}

	void onSubmit() override
	{
		m_stopwatch.reset();
	}

	void onClear() override
	{
		m_stopwatch.reset();
	}

	void update() override {}

	void draw() const override
	{
		if (!m_stopwatch.isActive())
			return;

		const double r = Min<double>(10.0, m_stopwatch.ms() / 10);

		Circle(595, 440, r).draw(Palette::Lightgreen);
	}
};

void Main()
{
	FontManager::Register(L"Exaple/YomogiFont.ttf");

	TextureAssetData message;
	message.onPreload = [](TextureAssetData& data) {
		Image image(640, 160);
		RoundRect(10, 0, 620, 150, 25.0).overwrite(image, Palette::Lightgreen);
		RoundRect(10, 0, 620, 150, 25.0).stretched(-10).overwrite(image, Palette::Lightyellow);
		data.texture = Texture(image);
		return static_cast<bool>(data.texture);
	};

	TextureAsset::Register(L"message", message);
	SoundAsset::Register(L"se", L"Example/Sound.mp3");
	SoundAsset::Register(L"bgm", L"Example/風の丘.mp3");
	FontAsset::Register(L"text", 12, L"よもぎフォント");

	asc::Novel novel;

	novel
		.setFont(L"text")
		.setColor(Palette::Black)
		.setMessageTexture(L"message", Rect(0, 320, 640, 160))
		.setMessagePosition({40, 380}, {30, 340})
		.setSound(L"se", L"se", L"se")
		.setButton(MyButton())
		.setKey(KeyCombination(Input::KeyEnter), KeyCombination(Input::KeySpace), KeyCombination(Input::KeyUp), KeyCombination(Input::KeyDown));

	novel.load(L"scenario.txt");

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
