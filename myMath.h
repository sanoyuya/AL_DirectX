#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <cmath>

namespace myMath
{

	//円周率
	const float PI = 3.1415926535f;

	const float PI2 = PI * 2;

	/// <summary>
	/// 度数法から弧度法へ
	/// </summary>
	/// <param name="angle">角度(度数法)</param>
	float ChangeDira(const float& angle);

	/// <summary>
	/// 弧度法から度数法へ
	/// </summary>
	/// <param name="angle">角度(弧度法)</param>
	float ChangeRadi(const float& angle);

	/// <summary>
	/// 単位行列を生成
	/// </summary>
	/// <returns>単位行列</returns>
	Matrix4 IdentityMatrix();

	/// <summary>
	/// スケーリング行列生成
	/// </summary>
	/// <param name="matScal"> : スケーリング行列(単位行列)</param>
	/// <param name="scal"> : X,Y,Zのスケーリング設定</param>
	void ScalingForm(Matrix4& matScal, Vector3& scal);

	/// <summary>
	/// スケーリング行列生成
	/// </summary>
	/// <param name="scal"> : X,Y,Zのスケーリング設定</param>
	Matrix4 ScalingForm(Vector3& scale);

	/// <summary>
	/// X軸回転行列を生成
	/// </summary>
	/// <param name="matRotX"> : X軸回転行列(単位行列)</param>
	/// <param name="angle"> : 回転させる角度(ラジアン)</param>
	void RotationXForm(Matrix4& matRotX, float angle);

	/// <summary>
	/// X軸回転行列を生成
	/// </summary>
	/// <param name="angle"> : 回転させる角度(ラジアン)</param>
	Matrix4 RotationXForm(float angle);

	/// <summary>
	/// Y軸回転行列を生成
	/// </summary>
	/// <param name="matRotY"> : Y軸回転行列(単位行列)</param>
	/// <param name="angle"> : 回転させる角度(ラジアン)</param>
	void RotationYForm(Matrix4& matRotY, float angle);

	/// <summary>
	/// Y軸回転行列を生成
	/// </summary>
	/// <param name="angle"> : 回転させる角度(ラジアン)</param>
	Matrix4 RotationYForm(float angle);

	/// <summary>
	/// Z軸回転行列を生成
	/// </summary>
	/// <param name="matRotZ"> : Z軸回転行列(単位行列)</param>
	/// <param name="angle"> : 回転させる角度(ラジアン)</param>
	void RotationZForm(Matrix4& matRotZ, float angle);

	/// <summary>
	/// Z軸回転行列を生成
	/// </summary>
	/// <param name="angle"> : 回転させる角度(ラジアン)</param>
	Matrix4 RotationZForm(float angle);

	/// <summary>
	/// 回転行列を生成
	/// </summary>
	/// <param name="matRot"> : 回転行列(単位行列)</param>
	/// <param name="rotaion"> : X,Y,Z軸の回転させる角度(ラジアン)</param>
	void RotationForm(Matrix4& matRot, Vector3& rotaion);

	/// <summary>
	/// 回転行列を生成
	/// </summary>
	/// <param name="rotaion"> : X,Y,Z軸の回転させる角度(ラジアン)</param>
	Matrix4 RotationForm(Vector3& rotaion);

	/// <summary>
	/// 平行移動行列を生成
	/// </summary>
	/// /// <param name="matTrans"> : 平行移動行列(単位行列)</param>
	/// <param name="trans"> : X,Y,Zの移動量</param>
	void TransferForm(Matrix4& matTrans, Vector3& trans);

	/// <summary>
	/// 平行移動行列を生成
	/// </summary>
	/// <param name="trans"> : X,Y,Zの移動量</param>
	Matrix4 TransferForm(Vector3& trans);

	/// <summary>
	/// ワールド行列を生成
	/// </summary>
	/// /// <param name="matWorld"> : ワールド行列を生成(単位行列)</param>
	/// <param name="worldTransform"> : ワールドトランスフォーム</param>
	void WorldForm(Matrix4& matWorld, WorldTransform& worldTransform);

	/// <summary>
	/// ワールド行列を生成
	/// </summary>
	/// <param name="worldTransform"> : ワールドトランスフォーム</param>
	Matrix4 WorldForm(WorldTransform& worldTransform);

	/// <summary>
	/// アフィン変換
	/// </summary>
	/// <param name="worldTransform"> : ワールドトランスフォーム</param>
	void AffineTransformation(WorldTransform& worldTransform);

	/// <summary>
	/// 親子関係更新
	/// </summary>
	// <param name="worldTransform"> : ワールドトランスフォーム</param>
	void ParenChildUpdate(WorldTransform& worldTransform);

	/// <summary>
	/// ノルムを求める
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	float Vector3Length(Vector3 vec);

	/// <summary>
	/// 正規化する
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector3& Vector3Normalize(Vector3& vec);


	/// <summary>
	/// 単項演算子
	/// </summary>
	const Vector3 Vector3Sub(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="mat">行列</param>
	/// <returns></returns>
	Vector3 VecMatMul(Vector3& vec, Matrix4& mat);

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <param name="worldTransform_"></param>
	/// <returns></returns>
	Vector3 GetWorldPosition(WorldTransform& worldTransform_);
} // namespace MyMath