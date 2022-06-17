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
	ApproachPhase();
}

void Enemy::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachPhase();
		break;
	case Phase::Leave:
		LeavePhase();
		break;
	}

	MatrixCalculation(enemyWorldTransform_);
	enemyWorldTransform_.TransferMatrix();
}

void Enemy::ApproachPhase()
{
	float moveSpeed = 0.2f;
	//移動(ベクトル加算)
	enemyWorldTransform_.translation_ += { 0.0f, 0.0f, -moveSpeed };
	//規定の位置に到達したら離脱
	if (enemyWorldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeavePhase()
{
	float moveSpeed = 0.2f;
	//移動(ベクトル加算)
	enemyWorldTransform_.translation_ += {-moveSpeed, moveSpeed, -moveSpeed };
}



void Enemy::Draw(ViewProjection& viewProjection)
{
	//3Dモデルの描画
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, textureHandle_);
}
