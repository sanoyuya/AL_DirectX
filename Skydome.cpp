#include "Skydome.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Skydome::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	worldTransform_.scale_ = { 100,100,100 };

	//ワールド変換の初期化
	worldTransform_.Initialize();

	MatrixCalculation(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection& viewProjection_){
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection_);
}
