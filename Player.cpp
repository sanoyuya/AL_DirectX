#include "Player.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update()
{
	Move();
	debugText_->SetPos(50, 50);
	debugText_->Printf("translation_:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Move() {
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move = { -moveSpeed,0.0f ,0.0f };
	}if (input_->PushKey(DIK_RIGHT)) {
		move = { moveSpeed,0.0f ,0.0f };
	}if (input_->PushKey(DIK_UP)) {
		move = { 0.0f,moveSpeed ,0.0f };
	}if (input_->PushKey(DIK_DOWN)) {
		move = { 0.0f,-moveSpeed ,0.0f };
	}

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	//座標移動
	worldTransform_.translation_ += move;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	MatrixCalculation(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection_)
{
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
