#include "EnemyBullet.h"
#include<cassert>
#include <Enemy.h>
#include "MathUtility.h"
using namespace MathUtility;

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
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

void EnemyBullet::Update()
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

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = bulletWorldTransform_.translation_.x;
	worldPos.y = bulletWorldTransform_.translation_.y;
	worldPos.z = bulletWorldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(bulletWorldTransform_, viewProjection, textureHandle_);
}