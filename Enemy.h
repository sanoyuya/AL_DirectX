#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"EnemyBullet.h"
#include "Input.h"

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Move();
	void ApproachPhase();
	void LeavePhase();
	void Attack();
	void Fire();
	void ApproachPhaseInitialize();
	void Draw(ViewProjection& viewProjection);

	//発車間隔
	static const int kFireInterval = 60;
private:
	//ワールド変換データ
	WorldTransform enemyWorldTransform_;
	//モデル
	Model* enemyModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	Input* input_ = nullptr;
	//敵
	std::list<std::unique_ptr<Enemy>>enemy_;

	//行動フェーズ
	enum class Phase {
		Approach,	//接近
		Leave,		//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//発射タイマー
	int32_t timer = 0;
};