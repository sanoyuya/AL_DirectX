﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include"Player.h"
#include"Enemy.h"
#include"PlayerBullet.h"
#include"EnemyBullet.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//ズーム/アウト切り替え
	bool zoomFlag = false;
	bool scopeFlag = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleSprite_ = 0;
	uint32_t textureHandleSprite2_ = 0;

	// 3dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_[9][9];
	//ビュープロジェクション
	ViewProjection viewProjection_;
};
