#include "PlayerBullet.h"
#include<cassert>
#include <Player.h>
#include "MathUtility.h"
using namespace MathUtility;

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);

	bulletModel_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("oityan.jpg");

	//ワールド変換の初期化
	bulletWorldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	bulletWorldTransform_.translation_ = position;
}

void PlayerBullet::Update()
{
	MatrixCalculation(bulletWorldTransform_);
	bulletWorldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(bulletWorldTransform_, viewProjection, textureHandle_);
}
