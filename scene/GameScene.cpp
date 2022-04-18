﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");

	for (int i = 0; i < maxWorldNum; i++) {
		worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};

		worldTransform_[i].rotation_ = {0.0f, 0.0f, 0.0f};

		if (i >= 16) {
			worldTransform_[i].translation_ = {-40.0f + 10 * (i - 16), 20.0f, 0.0f};
		} else {
			worldTransform_[i].translation_ = {-40.0f + 10 * i, -20.0f, 0.0f};
		}
		

		//ワールドトランスフォーム初期化
		worldTransform_[i].Initialize();
	}

	
	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {}

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
	model_ = Model::Create();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < maxWorldNum; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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
	
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}