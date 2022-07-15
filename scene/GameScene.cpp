#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"


GameScene::GameScene() {}

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
	viewProjection_.eye = { 310,400,-200 };
	viewProjection_.target = { 310,0,190 };
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);

	////軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	//デバッグ時のみ有効
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_B)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}
#endif
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	//else
	//{
	//	viewProjection_.UpdateMatrix();
	//	viewProjection_.TransferMatrix();
	//}

	if (input_->TriggerKey(DIK_SPACE)) {
		waveflag++;
		if (waveflag > 3) {
			waveflag = 0;
		}
	}

	if (input_->PushKey(DIK_UP) == 1) {
		timeSpeed += 0.001f;
	}if (input_->PushKey(DIK_DOWN) == 1) {
		timeSpeed -= 0.001f;
	}if (input_->PushKey(DIK_LEFT) == 1) {
		direction = 0.1f;
	}if (input_->PushKey(DIK_RIGHT) == 1) {
		direction = -0.1f;
	}
	if (timeSpeed < 0.001f) {
		timeSpeed = 0.001f;
	}if (timeSpeed > 1.01f) {
		timeSpeed = 1.01f;
	}

	t += timeSpeed;
	
	
	λ = 250.0f;
	T = 100.0f;
	f = 1 / T;
	v = f * λ;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			float x = i * 10;
			float z = j * 10;
			float d = sqrt((i * 10) * (i * 10) + (j * 10) * (j * 10));
			float centerD = sqrt(((32 - i) * 10) * ((32 - i) * 10) + ((32 - j) * 10) * ((32 - j) * 10));
			if (waveflag == 0) {
				y[i][j] = A * sinf(2 * π * (t / T - x / λ));//横波
			}if (waveflag == 1) {
				y[i][j] = A * sinf(2 * π * (t / T - z / λ));//縦波
			}if (waveflag == 2) {
				y[i][j] = (A * sinf(2 * π * (t / T - d / λ)));//左端斜め波
			}if (waveflag == 3) {
				y[i][j] = A * sinf(2 * π * (t / T - centerD / λ));//中心から
			}
		}
	}

	//十字
	//for (int i =23; i >= 0; i--) {//左下
	//	for (int j = 23; j >= 0; j--) {
	//		y[i][j] = (A * sinf(2 * π * (t - direction * i / T - x / λ)) + A * sinf(2 * π * (t - direction * j / T - x / λ))) / 2;//横縦波
	//	}
	//}

	//for (int i = 22; i < 46; i++) {//右下
	//	for (int j = 23; j >= 0; j--) {
	//		y[i][j] = (A * sinf(2 * π * (t + direction * i / T - x / λ)) + A * sinf(2 * π * (t + direction * -j / T - x / λ))) / 2;//横縦波
	//	}
	//}

	//for (int i = 22; i >= 0; i--) {//左上
	//	for (int j = 23; j < 46; j++) {
	//		y[i][j] = (-(A * sinf(2 * π * (t - direction * i / T - x / λ))) + A * sinf(2 * π * (t + direction * j / T - x / λ))) / 2;//横縦波
	//	}
	//}

	//for (int i = 22; i < 46; i++) {//右上
	//	for (int j = 22; j < 46; j++) {
	//		y[i][j] = (A * sinf(2 * π * (t + direction * i / T - x / λ)) + A * sinf(2 * π * (t + direction * j / T - x / λ))) / 2;//横縦波
	//	}
	//}
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
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	
	for (int i = 0; i < 63; i++) {
		for (int j = 0; j < 63; j++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d({ i * 10.0f,y[i][j],j * 10.0f }, { (i + 1) * 10.0f,y[i + 1][j + 1], (j + 1) * 10.0f }, { 255,255,255,255 });//斜め線
		}
	}

	//for (int i = 0; i < 45; i++) {
	//	for (int j = 0; j < 45; j++) {
	//		PrimitiveDrawer::GetInstance()->DrawLine3d({ i * 5.0f,y[i][j],x + j * 5.0f }, { i * 5.0f,y[i+1][j+1], (j + 1) * 5.0f }, { 255,255,255,255 });//十字線
	//		PrimitiveDrawer::GetInstance()->DrawLine3d({ i * 5.0f,y[i][j],x + j * 5.0f }, { (i + 1) * 5.0f,y[i+1][j+1], j * 5.0f }, { 255,255,255,255 });
	//	}
	//}


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
	debugText_->SetPos(50, 50);
	/*if (waveflag == 0) {
		debugText_->Printf("yoko wave");
	}
	else if (waveflag == 1) {
		debugText_->Printf("tate wave");
	}
	else {
		debugText_->Printf("naname wave");
	}*/

	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
