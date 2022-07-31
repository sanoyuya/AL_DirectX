#pragma once
#include <memory>
#include<sstream>

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include"EnemyBullet.h"
#include <Enemy.h>

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

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData(const char* filepath);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();


private:
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	uint32_t enemyTextureHandle_ = 0;

	// 3Dモデル
	std::shared_ptr<Model> model_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	std::unique_ptr<Player> player_;

	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//天球
	std::unique_ptr<Skydome> modelSkydome_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//待機中フラグ
	bool isStand_ = false;

	//待機タイマー
	int StandTime_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary

	/// <summary>
	/// 衝突判定と応答
	/// </summary
	void CheckAllCollisions();
};