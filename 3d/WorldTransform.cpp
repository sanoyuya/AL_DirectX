#include "WorldTransform.h"
#include"GameScene.h"
void WorldTransform::ParentChilldrenUpdate()
{
	MatrixCalculation(*this);
	matWorld_ *= parent_->matWorld_;
	TransferMatrix();
}
