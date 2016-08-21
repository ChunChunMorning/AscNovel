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
	////////////////////
	// アセットの作成

	FontManager::Register(L"Exaple/YomogiFont.ttf");

	TextWriter writer(L"scenario.txt");
	writer.write(L"0,1\n2,システム\n1,サンプルの実行、ありがとうございます。\\n好きな図形をクリックしてください。\n0,2\n3,1,chara1,290,0,350,480\n3,2,chara2,0,0,350,480\n7,2\n2,Siv3D君\n1,このように任意の画像を\\n表示させることができます。\n7,1\n1,キャラの陰影も操作可能で\\n誰のセリフか、はっきりさせることができます。\n8,1\n0,3\n2,システム\n1,ウィンドウの表示を実行時のみにしました。\n0,4\n2,システム\n1,ウィンドウを常に表示します。\n1,消したい場合は再び\\n青い四角をクリックしてください。\n0,5\n2,システム\n1,サンプルを終了しますか？,\n9,6,続ける,7,終了する\n0,6\n0,7\n2,システム\n1,サンプルを終了します。\\n決定ボタンを押してください。");
	writer.close();

	TextureAssetData message;
	message.onPreload = [](TextureAssetData& data) {
		Image image(640, 160);
		RoundRect(10, 0, 620, 150, 25.0).overwrite(image, Palette::Lightgreen);
		RoundRect(10, 0, 620, 150, 25.0).stretched(-10).overwrite(image, Palette::Lightyellow);
		data.texture = Texture(image);
		return static_cast<bool>(data.texture);
	};

	TextureAssetData choice;
	choice.onPreload = [](TextureAssetData& data) {
		Image image(200, 100);
		Rect(0, 0, 200, 100).overwrite(image, Palette::Lightgreen);
		Rect(0, 0, 200, 100).stretched(-5).overwrite(image, Palette::Lightyellow);
		data.texture = Texture(image);
		return static_cast<bool>(data.texture);
	};

	TextureAssetData mirror;
	mirror.onPreload = [](TextureAssetData& data) {
		data.texture = Texture(Image(L"Example/Siv3D-kun.png").mirrored());
		return static_cast<bool>(data.texture);
	};

	TextureAsset::Register(L"message", message);
	TextureAsset::Register(L"choice", choice);
	TextureAsset::Register(L"chara1", L"Example/Siv3D-kun.png");
	TextureAsset::Register(L"chara2", mirror);
	SoundAsset::Register(L"se", L"Example/Sound.mp3");
	SoundAsset::Register(L"bgm", L"Example/風の丘.mp3");
	FontAsset::Register(L"text", 12, L"よもぎフォント");



	////////////////////
	// サンプル用のデータ

	Graphics::SetBackground(Palette::Whitesmoke);
	Circle circle(110, 120, 80);
	Rect rect(120, 260, 200, 150);
	Triangle triangle(500, 180, 200);
	bool isShowNovel = false;



	////////////////////
	// Novelの使用

	asc::Novel novel;

	// 各種項目を設定します.
	novel
		.setFont(L"text")
		.setColor(Palette::Black)
		.setMessageTexture(L"message", { 0, 320, 640, 160 })
		.setMessagePosition({ 40, 380 }, { 30, 340 })
		.setChoiceTexture(L"choice", { 430, 210, 200, 100 })
		.setChoicePosition({ 450, 230 })
		.setSound(L"se", L"se", L"se")
		.setButton(MyButton())
		.setKey(
			Input::KeyEnter | XInput(0).buttonB,
			Input::KeySpace | XInput(0).buttonA,
			Input::KeyUp | XInput(0).buttonUp,
			Input::KeyDown | XInput(0).buttonDown
			);

	// シナリオファイルを読み込みます.
	novel.load(L"scenario.txt");

	// シナリオファイル中の該当する番号を開始します.
	novel.start(1);

	while (System::Update())
	{
		if (circle.leftClicked)
			novel.start(2);

		if (rect.leftClicked)
			novel.start(isShowNovel ? 3 : 4);

		if (triangle.leftClicked)
			novel.start(5);

		// 必ず毎ループ呼び出します.
		novel.update();

		// seekPoint関数で実行中の番号を取得できます.
		if (novel.seekPoint() == 3 || novel.seekPoint() == 4)
		{
			isShowNovel = !isShowNovel;

			// seekPoint関数の戻り値を消去できます.
			novel.clearSeekPoint();
		}
		// シナリオ終了時に実行したい場合, isUpdating関数で終了するのを待ちます.
		else if (!novel.isUpdating() && novel.seekPoint() == 7)
		{
			System::Exit();
		}

		circle.draw(Palette::Red);
		triangle.draw(Palette::Green);
		rect.draw(Palette::Blue);

		// シナリオ終了後も最後の状態を保持しているので, 実行時以外も描画できます.
		if (novel.isUpdating() || isShowNovel)
			novel.draw();
	}
}
