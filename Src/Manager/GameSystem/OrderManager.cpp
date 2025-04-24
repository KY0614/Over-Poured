#include <DxLib.h>
#include "../../Common/DebugDrawFormat.h"
#include "../../Object/Order.h"
#include "OrderManager.h"

OrderManager::OrderManager(void)
{
}

OrderManager::~OrderManager(void)
{
}

void OrderManager::Init(void)
{
	orders_.clear();
	count_ = 1;
}

void OrderManager::Update(void)
{
	// 先頭の注文の制限時間だけを減らす
	if (!orders_.empty()) {
		orders_.front()->Update();
		// 制限時間が切れた注文を削除
		if (orders_.front()->GetOrderTime() < 0.1f) {
			orders_.erase(orders_.begin());
			AddOrder();	//削除したら一番最後に新しく追加
			count_++;
		}
	}
}

void OrderManager::Draw(void)
{
#ifdef _DEBUG

	auto orders = orders_.front()->GetOrder();

	//注文に合わせて四角の色を変える
	int startX = DebugDrawFormat::GetFormatSize("注文 : %d,%d", orders.drink_, orders.sweets_);
	startX = startX * 1.5;//フォントサイズが1.5倍なので
	int scale = 25;
	int endX = startX + scale;
	int startY = 60;
	int endY = startY + scale;
	int drinkCol = GetColor(0, 0, 0);

	if (orders.drink_ == Order::DRINK::HOT)
	{
		drinkCol = GetColor(255, 0, 0);
	}
	else
	{
		drinkCol = GetColor(0, 255, 255);
	}
	//飲み物用
	DrawBox(startX, startY, endX, endY, drinkCol, true);

	int foodCol = GetColor(0, 0, 0);
	switch (orders.sweets_)
	{
	case Order::SWEETS::NONE:
		foodCol = GetColor(0, 0, 0);
		break;

	case Order::SWEETS::CHOCO:
		foodCol = GetColor(132, 98, 68);
		break;

	case Order::SWEETS::STRAWBERRY:
		foodCol = GetColor(255, 198, 244);
		break;
	default:
		break;
	}

	//食べ物用
	DrawBox(endX + scale, startY, endX + (scale * 2), endY, foodCol, true);
#endif // _DEBUG
}

void OrderManager::InitOrder(void)
{
	if (orders_.empty())
	{
		//5つまで生成する
		for (int i = 0; i < MAX_CREATE_NUM; i++)
		{
			CreateSingleOrder();
		}
	}
}

void OrderManager::CreateSingleOrder(void)
{
	//最大注文生成数を超えそうだったらreturn
	if (orders_.size() >= MAX_CREATE_NUM) return;

	std::unique_ptr<Order> order = std::make_unique<Order>();
	order->CreateOrder();		//注文を生成
	orders_.push_back(std::move(order));
}

void OrderManager::AddOrder(void)
{
	//最大注文生成数を超えそうだったらreturn
	if (orders_.size() >= MAX_CREATE_NUM) return;

	//注文が４つ以下になったら１つ追加
	if (orders_.size() <= MAX_CREATE_NUM - 1)
	{
		CreateSingleOrder();
	}
}

void OrderManager::ClearOrder(void)
{
	//先頭の要素を削除
	orders_.erase(orders_.begin());
}
