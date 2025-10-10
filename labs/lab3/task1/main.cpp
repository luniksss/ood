#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include "./Beverage/CCoffee.h"
#include "./Beverage/CCappuccino.h"
#include "./Beverage/CLatte.h"
#include "./Beverage/CTea.h"
#include "./Beverage/CBlackTea.h"
#include "./Beverage/CGreenTea.h"
#include "./Beverage/CBerryTea.h"
#include "./Beverage/COolongTea.h"
#include "./Beverage/CMilkshake.h"
#include "./Decorator/CCream.h"
#include "./Decorator/CChocolate.h"
#include "./Decorator/CLiquor.h"

using namespace std;

/*
Возвращает функцию, декорирующую напиток определенной добавкой

Параметры шаблона:
	Condiment - класс добавки, конструктор которого в качестве первого аргумента
				принимает IBeveragePtr&& оборачиваемого напитка
	Args - список типов прочих параметров конструктора (возможно, пустой)
*/
template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
	// Возвращаем функцию, декорирующую напиток, переданный ей в качестве аргумента
	// Дополнительные аргументы декоратора, захваченные лямбда-функцией, передаются
	// конструктору декоратора через make_unique
	return [=]<typename T0>(T0 && b) {
		return make_unique<Condiment>(forward<T0>(b), args...);
	};
}

/*
Перегруженная версия оператора <<, которая предоставляет нам синтаксический сахар
для декорирования компонента

Позволяет создать цепочку оборачивающих напиток декораторов следующим образом:
auto beverage = make_unique<CConcreteBeverage>(a, b, c)
					<< MakeCondimentA(d, e, f)
					<< MakeCondimentB(g, h);
*/
template <typename Component, typename Decorator>
auto operator << (Component && component, const Decorator & decorate)
{
	return decorate(forward<Component>(component));
}

bool DefinePortionsAmount()
{
	cout << "Type portions amount: 1 or 2" << endl;
	int portionsAmount;
	cin >> portionsAmount;

	if (portionsAmount < 1 || portionsAmount > 2)
	{
		cout << "Impossible portions amount. Will make default portions amount = 1" << endl;
	}
	return portionsAmount == 2;
}

int DefineChocolatePieces()
{
	cout << "Type chocolate pieces: from 1 to 5" << endl;
	int chocolatePieces;
	cin >> chocolatePieces;

	if (chocolatePieces < 1 || chocolatePieces > 5)
	{
		cout << "Impossible chocolate pieces amount. Will make default portion amount = 1" << endl;
		chocolatePieces = 1;
	}
	return chocolatePieces;
}

eLiquorType DefineLiquorType()
{
	cout << "Type liquor type: 1 - Chocolate, 2 - Nut" << endl;
	int liquorType;
	cin >> liquorType;

	if (liquorType == 1)
	{
		return eLiquorType::Chocolate;
	}
	if (liquorType == 2)
	{
		return eLiquorType::Nut;
	}
	cout << "Wrong liquor type/ Will make with default - Chocolate" << endl;
	return eLiquorType::Chocolate;
}

unique_ptr<IBeverage> DefineTea()
{
	cout << "1 - Black, 2 - Green, 3 - Berry, 4 - Oolong tea" << endl;
	int teaChoice;
	cin >> teaChoice;

	unique_ptr<IBeverage> beverage;
	switch (teaChoice)
	{
		case 1:
			beverage = make_unique<CBlackTea>();
		break;
		case 2:
			beverage = make_unique<CGreenTea>();
		break;
		case 3:
			beverage = make_unique<CBerryTea>();
		break;
		case 4:
			beverage = make_unique<COolongTea>();
		break;
		default:
			beverage = make_unique<CTea>();
	}
	return beverage;
}

unique_ptr<IBeverage> DefineMilkshake()
{
	cout << "1 - Small, 2 - Middle, 3 - Large" << endl;
	int sizeChoice;
	cin >> sizeChoice;

	if (sizeChoice < 1 || sizeChoice > 3)
	{
		cout << "Impossible size choice. Will make default size = Large" << endl;
	}

	eMilkshakeSize size;
	switch (sizeChoice)
	{
		case 1:
			size = eMilkshakeSize::Small;
		break;
		case 2:
			size = eMilkshakeSize::Middle;
		break;
		case 3:
		default:
			size = eMilkshakeSize::Large;
		break;
	}
	return make_unique<CMilkshake>(size);
}

unique_ptr<IBeverage> DefineCoffee()
{
	cout << "Type 1 for Cappuccino or 2 for Latte" << endl;
	int coffeeChoice;
	cin >> coffeeChoice;

	unique_ptr<IBeverage> beverage;
	bool isDouble;
	switch (coffeeChoice)
	{
		case 1:
			isDouble = DefinePortionsAmount();
			beverage = make_unique<CCappuccino>(isDouble);
		break;
		case 2:
			isDouble = DefinePortionsAmount();
			beverage = make_unique<CLatte>(isDouble);
		break;
		default:
			beverage = make_unique<CCoffee>();
	}
	return beverage;
}

void DialogWithUser()
{
	cout << "Type 1 for coffee, 2 for tea or 3 for milkshake\n";
	int beverageChoice;
	cin >> beverageChoice;

	unique_ptr<IBeverage> beverage;
	switch (beverageChoice)
	{
		case 1:
			beverage = DefineCoffee();
		break;
		case 2:
			beverage = DefineTea();
		break;
		case 3:
			beverage = DefineMilkshake();
		break;
		default: return;
	}

	int condimentChoice;
	for (bool done = false; !done;)
	{
		cout << "1 - Cream, 2 - Chocolate, 3 - Liquor, 0 - Checkout" << endl;
		cin >> condimentChoice;

		switch (condimentChoice)
		{
			case 1:
			{
				beverage = move(beverage) << MakeCondiment<CCream>();
				break;
			}
			case 2:
			{
				int chocolatePieces = DefineChocolatePieces();
				beverage = move(beverage) << MakeCondiment<CChocolate>(chocolatePieces);
				break;
			}
			case 3:
			{
				eLiquorType type = DefineLiquorType();
				beverage = move(beverage) << MakeCondiment<CLiquor>(type);
				break;
			}
			case 0:
			{
				done = true;
				break;
			}
			default:
				return;
		}
	}

	cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
}


int main()
{
	DialogWithUser();
	cout << endl;
}