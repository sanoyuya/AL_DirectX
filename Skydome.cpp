#include "Skydome.h"

void Skydome::Initialize() {
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//スケールを設定
	worldTransform_.scale_ = { 250.0f, 250.0f, 250.0f };
	//スケーリング行列
	worldTransform_.matWorld_ = myMath::ScalingForm(worldTransform_.scale_);
	//行列の転送
	worldTransform_.TransferMatrix();

	// 3Dモデル生成
	model_ = Model::CreateFromOBJ("Skydome", true);
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}