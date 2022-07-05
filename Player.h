#pragma once
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>

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

	void Rot();

	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

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
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
};