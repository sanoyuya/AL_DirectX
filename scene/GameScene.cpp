#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	textureHandle_ = TextureManager::Load("mario.jpg");

	//カメラ視点座標を設定
	viewProjection_.eye = {0, 10, -20};

	//カメラ注視点座標を設定
	viewProjection_.target = {0, 0, 0};

	//上方向ベクトルを指定
	viewProjection_.up = {0.0f, 1.0f, 0.0f};

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	
	XMFLOAT3 move(0, 0, 0);
	XMFLOAT3 front(0, 0, 1);

	//y軸回転行列演算
	move.x =cos(worldTransform_.rotation_.y) * front.x + sin(worldTransform_.rotation_.y) * front.z;
	move.z =(-sin(worldTransform_.rotation_.y)) * front.x + cos(worldTransform_.rotation_.y) * front.z;

	float moveSpeed = 0.05f;
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.translation_.x += moveSpeed * move.x;
		worldTransform_.translation_.z += moveSpeed * move.z;
	}
	if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.x -= moveSpeed * move.x;
		worldTransform_.translation_.z -= moveSpeed * move.z;
	}

	float RotationSpeed = 0.03f;
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= RotationSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += RotationSpeed;
	}

	worldTransform_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	model_ = Model::Create();

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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
