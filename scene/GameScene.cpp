#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"

#include <cassert>
#include <fstream>
#include <random>

#include "Player.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("oityan.jpg");
	//レティクルのテクスチャ
	TextureManager::Load("reticle.png");
	// 3Dモデル生成
	model_.reset(Model::Create());

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

	//天球の生成
	modelSkydome_ = std::make_unique<Skydome>();
	//天球の初期化
	modelSkydome_->Initialize();

	//レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f));

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(
		model_, textureHandle_, railCamera_->GetWorldTransformPtr(), Vector3(0.0f, 0.0f, 30.0f));

	LoadEnemyPopData("EnemyScript.csv");
}

void GameScene::Update() {

	//レールカメラの更新
	Vector3 move(0.0f, 0.0f, -0.00f);
	Vector3 rot(0.0f, 0.0001f, 0.0f);
	railCamera_->Update(move, rot);
	viewProjection_ = railCamera_->GetViewProjection();

	//デバッグ時のみ有効
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_B)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}
#endif // !_DEBUG
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}

	//自キャラの更新
	player_->Update(viewProjection_);

	//敵発生コマンド
	UpdateEnemyPopCommands();

	//デスフラグの立った弾を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });

	//敵キャラの更新
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//天球の表示
	modelSkydome_->Draw(viewProjection_);

	//自キャラの表示
	player_->Draw(viewProjection_);

	//敵キャラの表示
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	//敵キャラの弾表示
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullet();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = myMath::Vector3Length(myMath::Vector3Sub(posA, posB));

		//自キャラと敵弾の半径
		float radius = player_->GetRadius() + bullet->GetRadius();

		//自キャラと敵弾の交差判定
		if (distance <= radius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//自弾の座標
			posB = bullet->GetWorldPosition();
			//敵のキャラの座標
			posA = enemy->GetWorldPosition();

			//座標AとBの距離を求める
			float distance = myMath::Vector3Length(myMath::Vector3Sub(posA, posB));

			//敵キャラと自弾の半径
			float radius = enemy->GetRadius() + bullet->GetRadius();

			//敵キャラと自弾の交差判定
			if (distance <= radius) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	//自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bulletB : enemyBullets_) {

			//自弾の座標
			posB = bulletA->GetWorldPosition();
			//敵弾の座標
			posA = bulletB->GetWorldPosition();

			//座標AとBの距離を求める
			float distance = myMath::Vector3Length(myMath::Vector3Sub(posA, posB));

			//自弾と敵弾の半径
			float radius = bulletB->GetRadius() + bulletA->GetRadius();

			//自弾と敵弾の交差判定
			if (distance <= radius) {
				bulletB->OnCollision();
				bulletA->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData(const char* filepath) {
	//ファイルを開く
	std::ifstream file;
	file.open(filepath);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (isStand_) {
		StandTime_--;
		if (StandTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}

		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		//"//"から始まる行コメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす　
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			//敵キャラの生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//敵キャラの初期化
			newEnemy->Initialize(model_, enemyTextureHandle_, Vector3(x, y, z));

			//敵キャラにアドレスを渡す
			newEnemy->SetPlayer(player_.get());
			newEnemy->SetGameScene(this);

			//リストに登録する
			enemys_.push_back(std::move(newEnemy));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			StandTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}
}