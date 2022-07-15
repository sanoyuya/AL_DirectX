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
		if (waveflag > 5) {
			waveflag = 0;
		}
	}

	//if (input_->TriggerKey(DIK_H)) {
	//	if (waveflag < 4) {
	//		A = 30.0f;//振幅
	//	}
	//	else {
	//		A = 15.0f;//振幅
	//	}
	//	Aflag = 1;
	//}if (Aflag == 1) {
	//	A -= 0.5f;
	//	if (A == 0) {
	//		Aflag = 0;
	//		//t = 0;
	//	}
	//}

	if (input_->PushKey(DIK_UP)) {
		timeSpeed += 0.001f;
	}if (input_->PushKey(DIK_DOWN)) {
		timeSpeed -= 0.001f;
	}if (input_->PushKey(DIK_LEFT)) {
		direction = 0.1f;
	}if (input_->PushKey(DIK_RIGHT)) {
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
	if (waveflag < 4) {
		A = 30.0f;//振幅
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				float x = i * r;
				float z = j * r;
				float d = sqrt((i * r) * (i * r) + (j * r) * (j * r));
				float centerD = sqrt(((32 - i) * r) * ((32 - i) * r) + ((32 - j) * r) * ((32 - j) * r));
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
	}

	if (waveflag == 4) {
		A = 15.0f;//振幅
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				float centerD1 = sqrt(((10 - i) * r) * ((10 - i) * r) + ((32 - j) * r) * ((32 - j) * r));//左
				float centerD2 = sqrt(((54 - i) * r) * ((54 - i) * r) + ((32 - j) * r) * ((32 - j) * r));//右
				float centerD3 = sqrt(((32 - i) * r) * ((32 - i) * r) + ((54 - j) * r) * ((54 - j) * r));//上
				float centerD4 = sqrt(((32 - i) * r) * ((32 - i) * r) + ((10 - j) * r) * ((10 - j) * r));//下
				y[i][j] = A * sinf(2 * π * (t / T - centerD1 / λ))+ A * sinf(2 * π * (t / T - centerD2 / λ))+ A * sinf(2 * π * (t / T - centerD3 / λ)) + A * sinf(2 * π * (t / T - centerD4 / λ));//中心から
			}
		}
	}

	if (waveflag == 5) {
		A = 15.0f;//振幅
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				float centerD1 = sqrt(((10 - i) * r) * ((10 - i) * r) + ((10 - j) * r) * ((10 - j) * r));//左下
				float centerD2 = sqrt(((32 - i) * r) * ((32 - i) * r) + ((10 - j) * r) * ((10 - j) * r));//真ん中下
				float centerD3 = sqrt(((54 - i) * r) * ((54 - i) * r) + ((10 - j) * r) * ((10 - j) * r));//右下
				float centerD4 = sqrt(((54 - i) * r) * ((54 - i) * r) + ((32 - j) * r) * ((32 - j) * r));//右真ん中
				float centerD5 = sqrt(((54 - i) * r) * ((54 - i) * r) + ((54 - j) * r) * ((54 - j) * r));//右上
				float centerD6 = sqrt(((32 - i) * r) * ((32 - i) * r) + ((54 - j) * r) * ((54 - j) * r));//真ん中上
				float centerD7 = sqrt(((10 - i) * r) * ((10 - i) * r) + ((54 - j) * r) * ((54 - j) * r));//左上
				float centerD8 = sqrt(((10 - i) * r) * ((10 - i) * r) + ((32 - j) * r) * ((32 - j) * r));//左真ん中
				float centerD9 = sqrt(((32 - i) * r) * ((32 - i) * r) + ((32 - j) * r) * ((32 - j) * r));//真ん中
				y[i][j] = A * sinf(2 * π * (t / T - centerD1 / λ)) + A * sinf(2 * π * (t / T - centerD2 / λ)) +
					A * sinf(2 * π * (t / T - centerD3 / λ)) + A * sinf(2 * π * (t / T - centerD4 / λ)) +
					A * sinf(2 * π * (t / T - centerD5 / λ)) + A * sinf(2 * π * (t / T - centerD6 / λ)) +
					A * sinf(2 * π * (t / T - centerD7 / λ)) + A * sinf(2 * π * (t / T - centerD8 / λ)) +
					A * sinf(2 * π * (t / T - centerD9 / λ));
			}
		}
	}
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
			PrimitiveDrawer::GetInstance()->DrawLine3d({ i * r,y[i][j],j * r }, { (i + 1) * r,y[i + 1][j + 1], (j + 1) * r }, { 255,255,255,255 });//斜め線
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
