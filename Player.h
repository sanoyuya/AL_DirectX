#pragma once
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 自機
/// </summary>
class Player
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Model* model, uint32_t textureHandle);

    /// <summary>
    /// 毎フレーム処理
    /// </summary>
    void Update();

    void Move();

    void SelectMode();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();
private:
    Input* input_ = nullptr;
    Audio* audio_ = nullptr;
    DebugText* debugText_ = nullptr;
    //ワールド変換データ
    WorldTransform worldTransform_;
    WorldTransform worldTransform;
    //ビュープロジェクション
    ViewProjection viewProjection_;
    //モデル
    Model* model_ = nullptr;
    //テクスチャハンドル
    uint32_t textureHandle_ = 0u;
    //Qキーで操作切り替え
    bool mode_ = false;
    float viewAngle_ = 3.141592654f;
    float length_ = 30.0f;

    float vlength;
};