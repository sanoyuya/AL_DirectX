#include "PlayerBullet.h"
#include<cassert>
#include <Player.h>
#include "MathUtility.h"
using namespace MathUtility;

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	bulletModel_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("oityan.jpg");

	//ワールド変換の初期化
	bulletWorldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	bulletWorldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	//座標を移動させる(フレーム分の移動量を足しこむ)
	bulletWorldTransform_.translation_ += velocity_;

	MatrixCalculation(bulletWorldTransform_);
	bulletWorldTransform_.TransferMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(bulletWorldTransform_, viewProjection, textureHandle_);
}
