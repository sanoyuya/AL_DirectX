#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include<random>

float ChangeRadi(const float& angle) {
	float PI = 3.141592654f;
	float radian = angle * PI / 180;
	return radian;
}

float ChangeDegree(const float& angle) {
	float PI = 3.141592654f;
	float radian = angle * 180 / PI;
	return radian;
}

void SetMatScale(Matrix4& m, const Vector3& scale) {
	//スケーリング倍率を行列に設定する
	m.m[0][0] = scale.x;
	m.m[1][1] = scale.y;
	m.m[2][2] = scale.z;
	m.m[3][3] = 1.0f;
}

void SetMatRotX(Matrix4& m, const float radian) {
	//x軸回転行列の各要素を設定する
	m.m[1][2] = sinf(radian);
	m.m[2][1] = -sinf(radian);
	m.m[2][2] = cosf(radian);
	m.m[0][0] = 1.0f;
	m.m[3][3] = 1.0f;
	m.m[1][1] = cosf(radian);
}

void SetMatRotY(Matrix4& m, const float radian) {
	//y軸回転行列の各要素を設定する
	m.m[0][0] = cosf(radian);
	m.m[0][2] = -sinf(radian);
	m.m[2][0] = sinf(radian);
	m.m[2][2] = cosf(radian);
	m.m[1][1] = 1.0f;
	m.m[3][3] = 1.0f;
}

void SetMatRotZ(Matrix4& m, const float radian) {
	//z軸回転行列の各要素を設定する
	m.m[0][0] = cosf(radian);
	m.m[0][1] = sinf(radian);
	m.m[1][0] = -sinf(radian);
	m.m[1][1] = cosf(radian);
	m.m[2][2] = 1.0f;
	m.m[3][3] = 1.0f;
}

void SetMatTrans(Matrix4& m, const Vector3& trans) {
	//移動量を行列に設定する
	m.m[0][0] = 1.0f;
	m.m[1][1] = 1.0f;
	m.m[2][2] = 1.0f;
	m.m[3][3] = 1.0f;
	m.m[3][0] = trans.x;
	m.m[3][1] = trans.y;
	m.m[3][2] = trans.z;
}

Matrix4 IdentityMatrix() {
	Matrix4 m;
	m.m[0][0] = 1.0f;
	m.m[1][1] = 1.0f;
	m.m[2][2] = 1.0f;
	m.m[3][3] = 1.0f;
	return m;
}

void MatrixCalculation(WorldTransform& worldtransform) {
	//スケーリング行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	//平行移動行列宣言
	Matrix4 matTrans;
	SetMatScale(matScale, worldtransform.scale_);
	SetMatRotX(matRotX, worldtransform.rotation_.x);
	SetMatRotY(matRotY, worldtransform.rotation_.y);
	SetMatRotZ(matRotZ, worldtransform.rotation_.z);
	SetMatTrans(matTrans, worldtransform.translation_);
	//各軸の回転行列を合成
	matRot = matRotZ * matRotX * matRotY;
	//worldTransform_.matWorld_にmatScaleを掛け算して代入
	worldtransform.matWorld_ = matScale * matRot * matTrans;
}



GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//回転角用の乱数範囲を設定
	std::uniform_real_distribution<float>radian(0, 3.141592654);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float matRotation = radian(engine);
	//座標用の乱数範囲を設定
	std::uniform_real_distribution<float>coordinate(-10, 10);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float matTranslation = coordinate(engine);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	//スケーリング行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//キャラクター大元
	worldTransforms_[PartID::kRoot].Initialize();
	//脊椎
	worldTransforms_[PartID::kSpine].translation_ = { 0, 4.5f, 0 };
	worldTransforms_[PartID::kSpine].parent_ = &worldTransforms_[PartID::kRoot];
	worldTransforms_[PartID::kSpine].Initialize();

	//上半身
	//Chest
	worldTransforms_[PartID::kChest].translation_ = { 0, 0, 0 };
	worldTransforms_[PartID::kChest].parent_ = &worldTransforms_[PartID::kSpine];
	worldTransforms_[PartID::kChest].Initialize();

	//Head
	worldTransforms_[PartID::kHead].translation_ = { 0, 4.5, 0 };
	worldTransforms_[PartID::kHead].parent_ = &worldTransforms_[PartID::kChest];
	worldTransforms_[PartID::kHead].Initialize();

	//ArmL
	worldTransforms_[PartID::kArmL].translation_ = { -4.5f, 0, 0 };
	worldTransforms_[PartID::kArmL].parent_ = &worldTransforms_[PartID::kChest];
	worldTransforms_[PartID::kArmL].Initialize();
	//ArmR
	worldTransforms_[PartID::kArmR].translation_ = { 4.5f, 0, 0 };
	worldTransforms_[PartID::kArmR].parent_ = &worldTransforms_[PartID::kChest];
	worldTransforms_[PartID::kArmR].Initialize();

	//下半身
	//Hip
	worldTransforms_[PartID::kHip].translation_ = { 0, -4.5f, 0 };
	worldTransforms_[PartID::kHip].parent_ = &worldTransforms_[PartID::kSpine];
	worldTransforms_[PartID::kHip].Initialize();

	//LegL
	worldTransforms_[PartID::kLegL].translation_ = { -4.5f, -4.5f, 0 };
	worldTransforms_[PartID::kLegL].parent_ = &worldTransforms_[PartID::kHip];
	worldTransforms_[PartID::kLegL].Initialize();

	//LegR
	worldTransforms_[PartID::kLegR].translation_ = { 4.5f, -4.5f, 0 };
	worldTransforms_[PartID::kLegR].parent_ = &worldTransforms_[PartID::kHip];
	worldTransforms_[PartID::kLegR].Initialize();

	//ビュープロジェクション初期化
	viewProjection_.Initialize();

	//ビュープロジェクション初期化
	viewProjection_.Initialize();

	//デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

	//キャラクター移動処理
	//キャラクターの移動ベクトル
	Vector3 move = { 0.0f ,0.0f ,0.0f };
	float moveSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move = { -moveSpeed,0.0f ,0.0f };
	}if (input_->PushKey(DIK_RIGHT)) {
		move = { moveSpeed,0.0f ,0.0f };
	}

	worldTransforms_[PartID::kRoot].translation_ += move;
	MatrixCalculation(worldTransforms_[PartID::kRoot]);
	worldTransforms_[PartID::kRoot].TransferMatrix();
	//上半身回転処理
	//回転の早さ
	const float rotSpeed = 0.05f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransforms_[PartID::kChest].rotation_.y -= rotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransforms_[PartID::kChest].rotation_.y += rotSpeed;
	}
	//下半身回転処理
	if (input_->PushKey(DIK_J)) {
		worldTransforms_[PartID::kHip].rotation_.y -= rotSpeed;
	}
	else if (input_->PushKey(DIK_K)) {
		worldTransforms_[PartID::kHip].rotation_.y += rotSpeed;
	}

	//押した方向で移動ベクトルを変更

	//子の更新大元から順に更新していく
	for (int i = 0; i < PartID::kNumPartID; i++) {
		if (i == 0) {
			continue;
		}
		worldTransforms_[i].ParentChilldrenUpdate();
	}
	
	//デバック表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("translation_:(%f,%f,%f)", worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//for (int i = 0; i < 41; i++) {
	//	if (i < 21) {
	//		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i-10, 0, -9), Vector3(i-10, 0, 10), Vector4(0x00, 0x00, 0xff, 0xff));
	//	}
	//	else {
	//		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(-10, 0, i- 30), Vector3(10, 0, i- 30), Vector4(0xff, 0x00, 0x00, 0xff));
	//	}
	//}
	

#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	for (int i = 0; i < PartID::kNumPartID; i++) {
		if (i < 2) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
		

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
