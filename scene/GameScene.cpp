#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"

float ChangeRadi(const float& angle) {
	float PI = 3.141592654f;
	float radian = angle * PI / 180;
	return radian;
}

void SetMatScale(Matrix4& m,const Vector3& scale) {
	//スケーリング倍率を行列に設定する
	m.m[0][0] = scale.x;
	m.m[1][1] = scale.y;
	m.m[2][2] = scale.z;
	m.m[3][3] = 1.0f;
}

void SetMatRotX(Matrix4& m,const float radian) {
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

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	//ワールドトランスフォーム初期化
	worldTransform_2.Initialize();

	//スケーリング行列を宣言
	Matrix4 matScale;

	SetMatScale(matScale, { 5.0f,5.0f,5.0f });

	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	SetMatRotX(matRotX, ChangeRadi(45.0f));
	SetMatRotY(matRotY, ChangeRadi(45.0f));
	SetMatRotZ(matRotZ, 0.0f);

	//平行移動行列宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	SetMatTrans(matTrans, { 10,10,10 });

	//worldTransform_.matWorld_に単位行列を代入する
	worldTransform_.matWorld_.m[0][0] = 1.0f;
	worldTransform_.matWorld_.m[1][1] = 1.0f;
	worldTransform_.matWorld_.m[2][2] = 1.0f;
	worldTransform_.matWorld_.m[3][3] = 1.0f;

	//行列の合成
	//各軸の回転行列を合成
	matRot = matRotZ * matRotX * matRotY;
	//worldTransform_.matWorld_にmatScaleを掛け算して代入
	worldTransform_.matWorld_ = matScale * matRot * matTrans;

	//行列の転送
	worldTransform_.TransferMatrix();

	//ビュープロジェクション初期化
	viewProjection_.Initialize();

	//デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	debugCamera_->Update();
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

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	

#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	model_->Draw(worldTransform_2, debugCamera_->GetViewProjection(), textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

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
