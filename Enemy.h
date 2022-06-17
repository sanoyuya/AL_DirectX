#pragma once
#include "Model.h"
#include "WorldTransform.h"

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
	void Draw(ViewProjection& viewProjection);
private:
	//ワールド変換データ
	WorldTransform enemyWorldTransform_;
	//モデル
	Model* enemyModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;

	//敵
	std::list<std::unique_ptr<Enemy>>enemy_;

	//行動フェーズ
	enum class Phase {
		Approach,	//接近
		Leave,		//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;
};