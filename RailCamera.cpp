#include "RailCamera.h"
using namespace myMath;
RailCamera::RailCamera() {}

RailCamera::~RailCamera() {}

void RailCamera::Initialize(const Vector3& position, const Vector3& rotaion) {
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotaion;

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 400.0f;
	viewProjection_.Initialize();

	//シングルインスタンスを取得する
	debugText_ = DebugText::GetInstance();
}

void RailCamera::Update(Vector3& move, Vector3& rot) {
	using namespace MathUtility;

	//ワールドトランスフォームの数値を加算
	worldTransform_.translation_ -= move;
	worldTransform_.rotation_ += rot;
	//ワールドトランスフォームの更新
	worldTransform_.matWorld_ = WorldForm(worldTransform_);

	viewProjection_.eye = GetWorldPosition(worldTransform_);
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = myMath::VecMatMul(forward, worldTransform_.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = myMath::VecMatMul(up, worldTransform_.matWorld_);
	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//デバッグ用表示
	debugText_->SetPos(0, 120);
	debugText_->Printf(
		"RailCamera.eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(0, 100);
	debugText_->Printf(
		" worldTransform_.translation_:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

ViewProjection RailCamera::GetViewProjection() { return viewProjection_; }

WorldTransform* RailCamera::GetWorldTransformPtr() { return &worldTransform_; }