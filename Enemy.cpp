#include "Enemy.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Enemy::Initialize(Model* model, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	enemyModel_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("oityan.jpg");

	//ワールド変換の初期化
	enemyWorldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	enemyWorldTransform_.translation_ = position;
}

void Enemy::Update()
{
	Move();
}

void Enemy::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	move = { 0.0f,0.0f ,-moveSpeed };

	//座標移動
	enemyWorldTransform_.translation_ += move;

	MatrixCalculation(enemyWorldTransform_);
	enemyWorldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	//3Dモデルの描画
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, textureHandle_);
}
