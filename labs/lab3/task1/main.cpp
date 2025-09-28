#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include "./Decorator/CCinnamon.h"
#include "./Decorator/CLemon.h"
#include "./Beverage/CCoffee.h"
#include "./Beverage/CCappuccino.h"
#include "./Beverage/CLatte.h"
#include "./Beverage/CTea.h"
#include "./Beverage/CBlackTea.h"
#include "./Beverage/CGreenTea.h"
#include "./Beverage/CBerryTea.h"
#include "./Beverage/COolongTea.h"

using namespace std;

/*
Функциональный объект, создающий лимонную добавку
*/
struct MakeLemon
{
	explicit MakeLemon(const unsigned quantity)
		:m_quantity(quantity)
	{}

	auto operator()(IBeveragePtr && beverage)const
	{
		return make_unique<CLemon>(move(beverage), m_quantity);
	}
private:
	unsigned m_quantity;
};

/*
Функция, возвращающая функцию, создающую коричную добавку
*/
function<IBeveragePtr(IBeveragePtr &&)> MakeCinnamon()
{
	return [] (IBeveragePtr && b) {
		return make_unique<CCinnamon>(move(b));
	};
}

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

unique_ptr<IBeverage> DefineCoffee()
{
	cout << "Type 1 for Cappuccino or 2 for Latte" << endl;
	int coffeeChoice;
	cin >> coffeeChoice;

	unique_ptr<IBeverage> beverage;
	switch (coffeeChoice)
	{
		case 1:
			beverage = make_unique<CCappuccino>();
		break;
		case 2:
			beverage = make_unique<CLatte>();
		break;
		default:
			beverage = make_unique<CCoffee>();
	}
	return beverage;
}

void DialogWithUser()
{
	cout << "Type 1 for coffee or 2 for tea\n";
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
		default: return;
	}

	int condimentChoice;
	for (;;)
	{
		cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << endl;
		cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			//beverage = make_unique<CLemon>(move(beverage));
			beverage = move(beverage) << MakeCondiment<CLemon>(2);
		}
		else if (condimentChoice == 2)
		{
			//beverage = make_unique<CCinnamon>(move(beverage));
			beverage = move(beverage) << MakeCondiment<CCinnamon>();
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
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