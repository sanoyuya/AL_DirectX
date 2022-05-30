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

	//範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォーム初期化
		worldTransform.Initialize();

		//X,Y,Z方向のスケーリングを設定
		SetMatScale(matScale, { 1.0f,1.0f,1.0f });
		//X,Y,Z軸周りの回転角を乱数で設定
		SetMatRotX(matRotX, radian(engine));
		SetMatRotY(matRotY, radian(engine));
		SetMatRotZ(matRotZ, radian(engine));
		//X,Y,Z軸周りの平行移動(座標)を乱数で設定
		SetMatTrans(matTrans, { coordinate(engine) ,coordinate(engine) ,coordinate(engine) });

		//worldTransform_.matWorld_に単位行列を代入する
		worldTransform.matWorld_.m[0][0] = 1.0f;
		worldTransform.matWorld_.m[1][1] = 1.0f;
		worldTransform.matWorld_.m[2][2] = 1.0f;
		worldTransform.matWorld_.m[3][3] = 1.0f;

		//行列の合成
		//各軸の回転行列を合成
		matRot = matRotZ * matRotX * matRotY;
		//worldTransform_.matWorld_にmatScaleを掛け算して代入
		worldTransform.matWorld_ = matScale * matRot * matTrans;

		//行列の転送
		worldTransform.TransferMatrix();
	}

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,0,-10 };
	//カメラ注視点座標を設定
	viewProjection_.target = { 10,0,0 };
	//カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(ChangeRadi(45.0f)),sinf(ChangeRadi(45.0f)),0.0f };
	
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
	//視点の移動ベクトル
	Vector3 move = { 0.0f,0.0f,0.0f };

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move = { 0,0,+kEyeSpeed };
	}
	else if (input_->PushKey(DIK_S)) {
		move = { 0,0,-kEyeSpeed };
	}

	//視点移動（ベクトルの加算）
	viewProjection_.eye += move;

	//注視点移動処理
	//注視点の移動ベクトル
	Vector3 move2 = { 0.0f,0.0f,0.0f };

	//注視点の移動速さ
	const float kTargetSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move2 = { -kTargetSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move2 = { +kTargetSpeed,0,0 };
	}

	//注視点移動（ベクトルの加算）
	viewProjection_.target += move2;

	//上方向回転処理
	//上方向の回転の速さ[ラジアン/frame]
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//2πを超えたら0に戻す
		viewAngle = fmodf(viewAngle, 6.283185308);
	}
	
	//上方向ベクトルを計算(半径1の円周上の座標)
	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバック表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)",viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform,viewProjection_, textureHandle_);
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
