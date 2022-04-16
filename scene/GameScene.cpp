#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//キャラクター大元
	worldTransform_[PartID::Root].Initialize();
	//脊椎
	worldTransform_[PartID::Spine].translation_ = {0, 4.5f, 0};
	worldTransform_[PartID::Spine].parent_ = &worldTransform_[PartID::Root];
	worldTransform_[PartID::Spine].Initialize();

	//上半身
	//Chest
	worldTransform_[PartID::Chest].translation_ = {0, 0, 0};
	worldTransform_[PartID::Chest].parent_ = &worldTransform_[PartID::Spine];
	worldTransform_[PartID::Chest].Initialize();

	//Head
	worldTransform_[PartID::Head].translation_ = {0, 4.5, 0};
	worldTransform_[PartID::Head].parent_ = &worldTransform_[PartID::Chest];
	worldTransform_[PartID::Head].Initialize();

	//ArmL
	worldTransform_[PartID::ArmL].translation_ = {-4.5f, 0, 0};
	worldTransform_[PartID::ArmL].parent_ = &worldTransform_[PartID::Chest];
	worldTransform_[PartID::ArmL].Initialize();
	//ArmR
	worldTransform_[PartID::ArmR].translation_ = {4.5f, 0, 0};
	worldTransform_[PartID::ArmR].parent_ = &worldTransform_[PartID::Chest];
	worldTransform_[PartID::ArmR].Initialize();

	//下半身
	//Hip
	worldTransform_[PartID::Hip].translation_ = {0, -4.5f, 0};
	worldTransform_[PartID::Hip].parent_ = &worldTransform_[PartID::Spine];
	worldTransform_[PartID::Hip].Initialize();

	//LegL
	worldTransform_[PartID::LegL].translation_ = {-4.5f, -4.5f, 0};
	worldTransform_[PartID::LegL].parent_ = &worldTransform_[PartID::Hip];
	worldTransform_[PartID::LegL].Initialize();

	//LegR
	worldTransform_[PartID::LegR].translation_ = {4.5f, -4.5f, 0};
	worldTransform_[PartID::LegR].parent_ = &worldTransform_[PartID::Hip];
	worldTransform_[PartID::LegR].Initialize();

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//キャラクター移動処理
	//キャラクターの移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//上半身回転処理
	//上半身の回転速さ[ラジアン/frame]
	const float kChestRotSpeed = 0.05f;
	//下半身回転処理
	//下半身の回転速さ[ラジアン/frame]
	const float kHipRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	if (input_->PushKey(DIK_U)) {
		worldTransform_[PartID::Chest].rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_I)) {
		worldTransform_[PartID::Chest].rotation_.y += kChestRotSpeed;
	}

	if (input_->PushKey(DIK_J)) {
		worldTransform_[PartID::Hip].rotation_.y -= kHipRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_[PartID::Hip].rotation_.y += kHipRotSpeed;
	}

	//注視点移動(ベクトルの加算)
	worldTransform_[PartID::Root].translation_.x += move.x;
	worldTransform_[PartID::Root].translation_.y += move.y;
	worldTransform_[PartID::Root].translation_.z += move.z;

	//行列の再計算
	//大元から順に更新していく
	worldTransform_[PartID::Root].UpdateMatrix();
	worldTransform_[PartID::Spine].UpdateMatrix();
	worldTransform_[PartID::Chest].UpdateMatrix();
	worldTransform_[PartID::Head].UpdateMatrix();
	worldTransform_[PartID::ArmL].UpdateMatrix();
	worldTransform_[PartID::ArmR].UpdateMatrix();
	worldTransform_[PartID::Hip].UpdateMatrix();
	worldTransform_[PartID::LegL].UpdateMatrix();
	worldTransform_[PartID::LegR].UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToRadians(viewProjection_.fovAngleY));
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f",viewProjection_.nearZ);
	debugText_->SetPos(50, 150);
	debugText_->Printf("Root:(%f,%f,%f)", worldTransform_[PartID::Root].translation_.x += move.x,worldTransform_[PartID::Root].translation_.x += move.y,worldTransform_[PartID::Root].translation_.x += move.z);
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
	model_ = Model::Create();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_[Root], viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_[Spine], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegR], viewProjection_, textureHandle_);

	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}