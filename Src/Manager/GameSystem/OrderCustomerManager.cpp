#include <vector>
#include <algorithm>
#include "../../Utility/AsoUtility.h"
#include "../../Common/DebugDrawFormat.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Object/Customer/CustomerManager.h"
#include "../../Object/Order/OrderManager.h"
#include "../../Object/Stage/StageObject.h"
#include "../Generic/SceneManager.h"
#include "OrderCustomerManager.h"

OrderCustomerManager::OrderCustomerManager(void)
{
	orderMng_ = nullptr;
	customerMng_ = nullptr;
	isServe_ = false;
}

OrderCustomerManager::~OrderCustomerManager(void)
{
}

void OrderCustomerManager::Init(void)
{
	//注文
	orderMng_ = std::make_unique<OrderManager>();
	orderMng_->Init();

	//客
	customerMng_ = std::make_shared<CustomerManager>();

	//生成されている注文内容を参照して種類を決めて生成
	CreateCustomersByOrders();
	customerMng_->Init();
	//customerMng_->InitCustomersPos();
	isServe_ = false;
}

void OrderCustomerManager::Update(void)
{
	customerMng_->Update();

	//入口からカウンターまで動かす用
	if (!customerMng_->CheckFirstCustomerCol())
	{
		customerMng_->IsMove();
		if (customerMng_->CheckFirstCustomerCol())customerMng_->IsNotMove();
	}
	
	//注文の制限時間が過ぎたら追加生成を行う
	if (orderMng_->IsFirstOrderTimeOut() || isServe_)
	{
		if(!isServe_)customerMng_->SetCustomerReacton(0);
		//お客を移動させる
		customerMng_->IsMove();

		//お客の移動が終わったら削除と生成を行う
		if (customerMng_->CheckSecondCustomerCol())
		{
			//注文とお客を追加生成
			ClearOrderAndCustomer();

			//追加生成
			AddOrdersAndCustomers();
			isServe_ = false;
		}
	}
	else if(customerMng_->CheckFirstCustomerCol())
	{
		//制限時間を過ぎてなかったら先頭のオーダーだけ更新
		orderMng_->FirstOrderUpdate();
	}
}

void OrderCustomerManager::Draw(void)
{
	orderMng_->Draw();

	customerMng_->Draw();

#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG

}

void OrderCustomerManager::CreateCustomersByOrders(void)
{
	std::vector<Order::DRINK> drinks = orderMng_->GetAllOrderDrink();
	for (Order::DRINK drink : drinks)
	{
		customerMng_->CreateSingleCustomer(drink);
	}
}

void OrderCustomerManager::AddOrdersAndCustomers(void)
{
	//注文とお客を追加生成、位置調整
	orderMng_->AddOrder();
	AddCustomerByOrder();
}

void OrderCustomerManager::ClearOrderAndCustomer(void)
{
	//先頭の注文とお客を削除
	orderMng_->ClearFirstOrder();
	customerMng_->ClearFirstCustomers();
}

void OrderCustomerManager::AddCustomerByOrder(void)
{
	customerMng_->CreateSingleCustomer(orderMng_->GetLastOrderDrink());
	customerMng_->SetLastCustomerPos();
}

int OrderCustomerManager::CheckServeAndOrder(Order::OrderData serve)
{
	Order::DRINK serveDrink = serve.drink_;
	Order::SWEETS serveSweets = serve.sweets_;
	bool serveDrinkLid = serve.lid_;
	int score = 0;

	if(orderMng_->GetFirstOrder().num_ == 1)
	{
		//飲み物と提供品を比較して加点
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 50;
		}

		//飲み物の蓋がされていたら加点
		if (serveDrinkLid)
		{
			score += 20;
		}

		//注文数が１のときはサブオーダーはないので-50点
		if (serveSweets != orderMng_->GetFirstOrder().sweets_)
		{
			score -= 50;
		}

		//オーダーの残り制限時間による加点
		if (orderMng_->GetFirstOrder().time_ > 10.0f)
		{
			score += 50;
		}
		else if (orderMng_->GetFirstOrder().time_ > 3.0f)
		{
			score += 10;
		}	
	}
	else if (orderMng_->GetFirstOrder().num_ == 2)
	{
		//飲み物と提供品を比較して加点
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 50;
		}

		//飲み物の蓋がされていたら加点
		if (serveDrinkLid)
		{
			score += 20;
		}

		//サブオーダーのスイーツとの比較して加点
		if (serveSweets == orderMng_->GetFirstOrder().sweets_)
		{
			score += 30;
		}
		else
		{
			score -= 20;
		}

		//オーダーの残り制限時間による加点
		if (orderMng_->GetFirstOrder().time_ > 12.0f)
		{
			score += 40;
		}
		else if (orderMng_->GetFirstOrder().time_ > 6.0f)
		{
			score += 20;
		}		
	}
	customerMng_->SetCustomerReacton(score);
	return score;
}

int OrderCustomerManager::CheckServeAndOrder(StageObject& obj)
{
	Order::DRINK serveDrink = obj.GetDrinkType();
	Order::SWEETS serveSweets = obj.GetSweetsType();
	int score = 0;

	if (orderMng_->GetFirstOrder().num_ == 1)
	{
		//飲み物と提供品を比較して加点
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 50;
		}
		else
		{
			score -= 50;
		}

		//注文数が１のときはサブオーダーはないので-50点
		if (serveSweets != orderMng_->GetFirstOrder().sweets_)
		{
			score -= 50;
		}

		//オーダーの残り制限時間による加点
		if (orderMng_->GetFirstOrder().time_ > 7.0f)
		{
			score += 50;
		}
		else if (orderMng_->GetFirstOrder().time_ > 3.0f)
		{
			score += 10;
		}
	}
	else
	{
		if (serveDrink == orderMng_->GetFirstOrder().drink_)
		{
			score += 30;
		}

		if (serveSweets == orderMng_->GetFirstOrder().sweets_)
		{
			score += 30;
		}

		if (orderMng_->GetFirstOrder().time_ > 12.0f)
		{
			score += 40;
		}
		else if (orderMng_->GetFirstOrder().time_ > 6.0f)
		{
			score += 20;
		}
	}
	customerMng_->SetCustomerReacton(score);
	return score;
}

bool OrderCustomerManager::GetIsMoving(void)
{
	return customerMng_->GetIsMove();
}

int OrderCustomerManager::GetCustomerNum(void) const
{
	return customerMng_->GetCustomerNum();
}

void OrderCustomerManager::DebugDraw(void)
{
	SetFontSize(24);
	VECTOR pos = customerMng_->GetFirstPos();
	pos.y += 180.0f;
	pos.x += 30.0f;
	VECTOR screenPos = ConvWorldPosToScreenPos(pos);

	VECTOR pos2 = customerMng_->GetFirstPos();
	pos2.y += 150.0f;
	pos2.x += 30.0f;
	VECTOR screenPos2 = ConvWorldPosToScreenPos(pos2);

	// 変換成功
	DrawFormatString(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), GetColor(255, 255, 255),
		L"注文 : %d,%d",
		orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_);	// 変換成功
	DrawFormatString(static_cast<int>(screenPos2.x), static_cast<int>(screenPos2.y), GetColor(255, 255, 255),
		L"注文制限時間 : % 2.f",
		orderMng_->GetFirstOrder().time_);

	SetFontSize(16);

	auto orders = orderMng_->GetFirstOrder();

	//注文に合わせて四角の色を変える
	//int startX = DebugDrawFormat::GetFormatSize(L"注文 : %d,%d", orders.drink_, orders.sweets_);
	//startX = startX * 1.5;//フォントサイズが1.5倍なので
	//int endX = startX + scale;
	//int startY = 30;
	//int endY = startY + scale;
	int drinkCol = GetColor(0, 0, 0);

	//表示ベース位置（Yはちょっと上にオフセット）
	int baseX = static_cast<int>(screenPos.x);
	int baseY = static_cast<int>(screenPos.y) - 80; // 文字のYと合わせる

	// 四角形の描画位置を決定（文字の下に並べる）
	int startX = baseX;
	int startY = baseY + 40; // 文字の下に少し空けて

	int scale = 25;//大きさ
	int gap = 10; // 文字とボックスの隙間

	//飲み物の色決定
	if (orders.drink_ == Order::DRINK::HOT)
	{
		drinkCol = GetColor(255, 0, 0);
	}
	else
	{
		drinkCol = GetColor(0, 255, 255);
	}
	//飲み物用
	DrawBox(startX, startY, startX + scale, startY + scale, drinkCol, true);

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
	}

	//食べ物用
	DrawBox(startX + scale + gap, startY, startX + scale * 2 + gap, startY + scale, foodCol, true);

#ifdef _DEBUG

	int line = 1;	//行
	int lineHeight = 30;	//行

	SetFontSize(24);
	DebugDrawFormat::FormatString(L"注文 : %d,%d", orderMng_->GetFirstOrder().drink_, orderMng_->GetFirstOrder().sweets_, line, lineHeight);
	DebugDrawFormat::FormatString(L"注文制限時間 : %2.f", orderMng_->GetFirstOrder().time_, line, lineHeight);
	DebugDrawFormat::FormatString(L"%d個目", orderMng_->GetCount(), line, lineHeight);
	DebugDrawFormat::FormatString(L"%d人", customerMng_->GetCustomerNum(), line, lineHeight);
	DebugDrawFormat::FormatString(L"isServe : %d", isServe_, line, lineHeight);
	SetFontSize(16);

#endif // _DEBUG
}
