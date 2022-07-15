#pragma once

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

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3dモデル
	Model* model_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	float v = 0.833f;//速さ[m/s]
	float λ = 50.0f;//波長[m]
	float T = 60.0f;//周期[s]
	float f = 0.25f;//振動数[Hz=回/s]
	float t = 0.0f;//時刻[t]
	float π = 3.141592654f;//円周率
	float A = 0.0f;//振幅
	int num = 0;
	float y[64][64] = {0};//変位
	//float x = 0.0f;//原点からの距離
	float z = 0.0f;

	float r = 10.0f;
	float timeSpeed = 1.0f;
	int Aflag = 0;
	int waveflag = 0;
	float direction = -0.1f;
};
