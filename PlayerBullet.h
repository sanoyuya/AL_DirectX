#pragma once
#include "Model.h"
#include"WorldTransform.h"

/// <summary>
/// 自機の弾
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);
private:
	//ワールド変換データ
	WorldTransform bulletWorldTransform_;
	//モデル
	Model* bulletModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};

