#pragma once
#include "DebugText.h"
#include "MathUtility.h"
#include "MyMath.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	RailCamera();
	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotaion);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3& move, Vector3& rot);

	/// <summary>
	/// ビュープロジェクションを所得
	/// </summary>
	ViewProjection GetViewProjection();

	/// <summary>
	/// ワールド変換データのアドレスを所得
	/// </summary>
	WorldTransform* GetWorldTransformPtr();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグ用表示
	DebugText* debugText_ = nullptr;
};