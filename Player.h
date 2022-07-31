#pragma once
#include <cassert>
#include <list>
#include <memory>

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "WorldTransform.h"

#include "myMath.h"
#include "PlayerBullet.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(
		std::shared_ptr<Model> model, uint32_t textureHandle, WorldTransform* parent_,
		const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets_; }

	/// <summary>
	/// 半径を所得
	/// </summary>
	float GetRadius();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	Player() = default;
	~Player() = default;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	std::shared_ptr<Model> model_;

	//テクスチャハンドル
	uint32_t texturehandle_ = 0u;

	//キーボード操作
	Input* input_ = nullptr;

	//デバッグ用表示
	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//半径
	const float radius_ = 1.0f;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 3Dレティクルの計算
	/// </summary>
	void Reticle3D();

	/// <summary>
	/// 2Dレティクルの計算
	/// </summary>
	void Reticle2D(const ViewProjection& viewProjection);
};