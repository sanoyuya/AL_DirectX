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
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//引数で受け取った初期座標をセット
	enemyWorldTransform_.translation_ = position;

	//ワールド変換の初期化
	enemyWorldTransform_.Initialize();
}

void Enemy::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	Move();
	ApproachPhase();
	//接近フェーズ初期化
	ApproachPhaseInitialize();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
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

void Enemy::Attack()
{
	//弾の速度
	const float kBulletSpeed = 1.0f;

	Vector3 player = player_->GetWorldPosition();
	Vector3 enemy = GetWorldPosition();
	Vector3 l = player - enemy;
	Vector3Normalize(l);
	Vector3 velocity(l * kBulletSpeed);

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(enemyModel_, enemyWorldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Fire()
{
	Attack();
}

void Enemy::ApproachPhaseInitialize()
{
	//発射タイマーカウントダウン
	timer--;
	//指定時間に達した
	if (timer < 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		timer = kFireInterval;
	}
}



void Enemy::Draw(ViewProjection& viewProjection)
{
	//3Dモデルの描画
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = enemyWorldTransform_.translation_.x;
	worldPos.y = enemyWorldTransform_.translation_.y;
	worldPos.z = enemyWorldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}