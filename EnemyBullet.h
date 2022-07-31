#pragma once
#include "DebugText.h"
#include "Model.h"
#include "MyMath.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <cassert>

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name = "model">モデル</param>
	/// <param name = "position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name = "viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	///デスフラグゲッター
	bool IsDead() const;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 半径を所得
	/// </summary>
	float GetRadius();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t texturehandle_ = 0u;

	//デバッグ用表示
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	const float radius_ = 1.0f;
};

inline bool EnemyBullet::IsDead() const { return isDead_; }