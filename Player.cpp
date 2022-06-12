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

	//オブジェクト
	worldTransform.translation_.x = 10;
	worldTransform.translation_.z = 10;
	worldTransform.Initialize();
	worldTransform.translation_ = { 10,0,10 };
	MatrixCalculation(worldTransform);
	worldTransform.TransferMatrix();

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
	viewProjection_.eye.y = 20;

	Vector3 camera(0, 0, 0);
	camera = worldTransform_.translation_ - viewProjection_.eye;

	vlength = Vector3Length(camera);
}

void Player::Update()
{
	Move();
	SelectMode();
	debugText_->SetPos(50, 50);
	debugText_->Printf("translation_:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("viewProjection_.eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("mode:%d", mode_);
}

void Player::Move()
{

	Vector3 move(0, 0, 1);
	Vector3 front(0, 0, 2);
	Vector3 camera(0, 0, 0);
	Vector3 cameraUp(0, 1, 0);
	Vector3 cameraCross(0, 1, 0);


	float rotationSpeed = 0.02f;
	float moveSpeed = 0.05f;
	//視点の移動の速さ
	const float kEyeSpeed = 0.02f;

	// 2πを超えたら0に戻す
	viewAngle_ = fmodf(viewAngle_, 2 * PI);
	camera = worldTransform_.translation_ - viewProjection_.eye;
	camera = Vector3Normalize(camera);

	cameraCross = Vector3Cross(cameraUp, camera);
	cameraCross = Vector3Normalize(cameraCross);

	//視点ベクトルを計算(半径1の円周上の座標)

	if (mode_ == false)
	{
		if (input_->PushKey(DIK_A))
		{
			worldTransform_.rotation_.y -= rotationSpeed;
		}
		if (input_->PushKey(DIK_D))
		{
			worldTransform_.rotation_.y += rotationSpeed;
		}
		// y軸回転行列演算
		move.x = cos(worldTransform_.rotation_.y) * front.x + sin(worldTransform_.rotation_.y) * front.z;
		move.z = -sin(worldTransform_.rotation_.y) * front.x + cos(worldTransform_.rotation_.y) * front.z;

		if (input_->PushKey(DIK_W))
		{
			worldTransform_.translation_.x += moveSpeed * move.x;
			worldTransform_.translation_.z += moveSpeed * move.z;
		}
		if (input_->PushKey(DIK_S))
		{
			worldTransform_.translation_.x -= moveSpeed * move.x;
			worldTransform_.translation_.z -= moveSpeed * move.z;
		}
	}
	else
	{
		viewProjection_.eye = { viewProjection_.target.x + sinf(viewAngle_) * length_, 20.0f,viewProjection_.target.z + cosf(viewAngle_) * length_ };

		if (input_->PushKey(DIK_W))
		{
			camera = worldTransform_.translation_ - viewProjection_.eye;
			camera = Vector3Normalize(camera);

			worldTransform_.translation_.x += moveSpeed * camera.x;
			worldTransform_.translation_.z += moveSpeed * camera.z;
		}
		if (input_->PushKey(DIK_S))
		{
			camera = worldTransform_.translation_ - viewProjection_.eye;
			camera = Vector3Normalize(camera);

			worldTransform_.translation_.x -= moveSpeed * camera.x;
			worldTransform_.translation_.z -= moveSpeed * camera.z;
		}
		if (input_->PushKey(DIK_D))
		{
			worldTransform_.translation_.x += moveSpeed * cameraCross.x;
			worldTransform_.translation_.z += moveSpeed * cameraCross.z;
		}
		if (input_->PushKey(DIK_A))
		{
			worldTransform_.translation_.x -= moveSpeed * cameraCross.x;
			worldTransform_.translation_.z -= moveSpeed * cameraCross.z;
		}

		if (input_->PushKey(DIK_LEFT))
		{
			viewAngle_ -= kEyeSpeed;
		}

		if (input_->PushKey(DIK_RIGHT))
		{
			viewAngle_ += kEyeSpeed;
		}
	}


	worldTransform_.TransferMatrix();
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	MatrixCalculation(worldTransform_);
}

void Player::SelectMode()
{
	if (input_->TriggerKey(DIK_Q))
	{
		if (mode_ == false)
		{
			mode_ = true;
		}
		else
		{
			mode_ = false;
		}
	}
}

void Player::Draw()
{
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform, viewProjection_, textureHandle_);
}