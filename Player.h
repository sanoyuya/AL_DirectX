#pragma once
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include"PlayerBullet.h"

/// <summary>
/// 自機
/// </summary>
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void Move();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);
private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//弾
	PlayerBullet* bullet_ = nullptr;
};

