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
	viewProjection_.eye = { 160,500,-160 };
	viewProjection_.target = { 160,0,160 };
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
		if (waveflag > 1) {
			waveflag = 0;
		}
	}



	t += timeSpeed;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			if (waveflag == 0) {
				y[i][j] = A * sinf(2 * π * (t - 0.1 * i / T - x / λ));//横波
			}if (waveflag == 1) {
				y[i][j] = (A * sinf(2 * π * (t - 0.1 * i / T - x / λ)) + A * sinf(2 * π * (t - 0.1 * j / T - x / λ))) / 2;//横縦波
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
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64;j++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d({ i * 5.0f,y[i][j],x + j * 5.0f }, { (i + 1) * 5.0f,y[i][j], (j + 1) * 5.0f }, { 255,255,255,255 });//斜め線
			//PrimitiveDrawer::GetInstance()->DrawLine3d({ i * 5.0f,y[i][j],x + j * 5.0f }, { i * 5.0f,y[i][j], (j + 1) * 5.0f }, { 255,255,255,255 });//十字線
			//PrimitiveDrawer::GetInstance()->DrawLine3d({ i * 5.0f,y[i][j],x + j * 5.0f }, { (i + 1) * 5.0f,y[i][j], j * 5.0f }, { 255,255,255,255 });
		}
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
