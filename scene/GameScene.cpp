#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	worldTransform_.rotation_ = {0.0f,0.0f, 0.0f};

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	XMFLOAT3 endPoint = {0, 0, 0};

	XMFLOAT3 frontVec = {0, 0, 0};

	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	//正面ベクトル
	endPoint.z = worldTransform_.translation_.z + 1;

	frontVec.z = endPoint.z - worldTransform_.translation_.z;

	endWorldTransform_.translation_ = endPoint;

	//カメラ視点座標を設定
	viewProjection_.eye = {0, 50, -100};

	//カメラ注視点座標を設定
	viewProjection_.target = {0, 0, 0};

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	XMFLOAT3 move = {0, 0, 0};
	XMFLOAT3 move2 = {0, 0, 0};

	const float modelSpeed = 0.2f;
	const float modelRotationSpeed = 0.02f;

	if (input_->PushKey(DIK_UP)) {
		move = {0, 0, modelSpeed};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, 0, -modelSpeed};
	}
	if (input_->PushKey(DIK_LEFT)) {
		move2 = {0, modelRotationSpeed, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move2 = {0, -modelRotationSpeed, 0};
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	worldTransform_.rotation_.x += move2.x;
	worldTransform_.rotation_.y += move2.y;
	worldTransform_.rotation_.z += move2.z;
	endWorldTransform_.rotation_.x += move2.x;
	endWorldTransform_.rotation_.y += move2.y;
	endWorldTransform_.rotation_.z += move2.z;

	//行列の再計算
	worldTransform_.UpdateMatrix();
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
	// translation
	debugText_->SetPos(50, 70);
	debugText_->Printf("translation:(%f,%f,%f)", 10.0f, 10.0f, 10.0f);
	// rotation
	debugText_->SetPos(50, 90);
	debugText_->Printf("rotation:(%f,%f,%f)", 0.785398f, 0.785398f, 0.0f);
	// scale
	debugText_->SetPos(50, 110);
	debugText_->Printf("scale:(%f,%f,%f)", 5.0f, 5.0f, 5.0f);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}