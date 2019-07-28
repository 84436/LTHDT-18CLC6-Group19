﻿#include "Seller.h"
#include "AccountProvider.h"
#include "../Product/ProductProvider.h"
#include "../Order/OrderProvider.h"

void Seller::ListOrder_Pending()
{
	list<Order> FilteredOrders = OrderProvider::GetInstance().ListByAccountID(this->ID());

	FilteredOrders.remove_if(OrderProvider::isNotPending_Seller);

	cout << "Total pending order count: " << FilteredOrders.size() << endl;
	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		cout << (*i).ID() << " : " << (*i).Status_String() << endl;
	}
}

void Seller::GetInfo()
{
	Account::GetInfo();
	cout << "Rating        : " << this->GetRate() << endl;
}

void Seller::ListProduct()
{
	list<Product> FilteredProducts = ProductProvider::GetInstance().ListBySellerID(this->ID(), true);
	for (auto i = FilteredProducts.begin(); i != FilteredProducts.end(); ++i)
	{
		cout << (*i).ID() << ": Name = " << (*i).Name() << endl;
	}
}

void Seller::GetProductByID(string _ProductID)
{
	if (!ProductProvider::isRelated(this->ID(), _ProductID))
	{
		cout << "Product not found" << endl;
		return;
	}
	ProductProvider::GetInstance().GetByID(_ProductID)->GetInfo();
}

void Seller::AddProduct()
{
	Product _Product;
	_Product.SellerID(this->ID());
	string s;
	cout << "Name                                         : ";	getline(cin, s); _Product.Name(s);
	cout << "Is R18? [\"true\" = true, otherwise = false]  : ";	getline(cin, s); _Product.isR18(s == "true" ? true : false);
	cout << "Category                                     : ";	getline(cin, s); _Product.Category(s);
	cout << "Description                                  : ";	getline(cin, s); _Product.Description(s);
	cout << "Price                                        : ";	getline(cin, s); _Product.Price(stoi(s));
	cout << "Base stock                                   : ";	getline(cin, s); _Product.AddStock(stoi(s));
	ProductProvider::GetInstance().Add(_Product);
}

void Seller::EditProduct(string _ProductID)
{
	if (!ProductProvider::isRelated(this->ID(), _ProductID))
	{
		cout << "Product not found" << endl;
		return;
	}

	ProductProvider::GetInstance().GetByID(_ProductID)->EditInfo();
}

void Seller::DeleteProduct(string _ProductID)
{
	if (!ProductProvider::isRelated(this->ID(), _ProductID))
	{
		cout << "Product not found" << endl;
		return;
	}

	ProductProvider::GetInstance().Delete(_ProductID);
}

void Seller::AddStock(string _ProductID, int32_t _Amount)
{
	if (!ProductProvider::isRelated(this->ID(), _ProductID))
	{
		cout << "Product not found" << endl;
		return;
	}

	ProductProvider::GetInstance().GetByID(_ProductID)->AddStock(_Amount);
}

void Seller::ListShippers()
{
	list<Shipper*> AvailableShipper = AccountProvider::GetInstance().ListShippers();
	for (auto i = AvailableShipper.begin(); i != AvailableShipper.end(); ++i)
		cout << (**i).ID() << " (" << (**i).Name() << ")" << endl;
}

void Seller::AcceptOrder(string _OrderID)
{
	if (!OrderProvider::isRelated(this->ID(), _OrderID))
	{
		cout << "Order not found" << endl;
		return;
	}

	Order* _Order = OrderProvider::GetInstance().GetByID(_OrderID);
	
	if (_Order->Status() != SELLER_PENDING)
	{
		cout << "This order is waiting for delivery, completed or cancelled." << endl;
		return;
	}

	cout << endl;
	cout << "Available shippers:" << endl;
	ListShippers();

	string _ShipperID; 
	do
	{
		cout << "Enter shipper ID: ";
		getline(cin, _ShipperID);
	}
	while
	(
		(AccountProvider::GetInstance().GetShipper(_ShipperID) == nullptr)
		&& (cout << "Invalid shipper ID." << endl)
	);
	_Order->ShipperID(_ShipperID);

	string _ShippingFee;
	do
	{
		cout << "Shipping fee: ";
		getline(cin, _ShippingFee);
	}
	while
	(
		(stoll(_ShippingFee) < 0)
		&& (cout << "Invalid shipping fee." << endl)
	);
	_Order->ShippingFee(stoll(_ShippingFee));

	string _PriceCoeff;
	do
	{
		cout << "% Sale (0 - 100): ";
		getline(cin, _PriceCoeff);
	}
	while
	(
		(stod(_PriceCoeff) < 0 || stod(_PriceCoeff) > 100)
		&& (cout << "Invalid price coefficient." << endl)
	);
	_Order->PriceCoeff(1 - stod(_PriceCoeff) / 100);

	string _Note;
	cout << "Note (optional): ";
	getline(cin, _Note);
	_Order->Note(_Note);

	_Order->Status(BUYER_PENDING);
}

void Seller::RejectOrder(string _OrderID)
{
	if (!OrderProvider::isRelated(this->ID(), _OrderID))
	{
		cout << "Order not found." << endl;
		return;
	}

	Order* _Order = OrderProvider::GetInstance().GetByID(_OrderID);

	if (_Order->Status() != SELLER_PENDING)
	{
		cout << "This order is waiting for delivery, completed or cancelled." << endl;
		return;
	}

	string _Note;
	cout << "Note (i.e. why you've rejected this order): ";
	getline(cin,_Note);
	_Order->Note(_Note);

	_Order->Status(SELLER_CANCELLED);
}

vector<int16_t> Seller::RatingArray()
{
	return _Rating;
}

void Seller::RatingArray(int16_t _1, int16_t _2, int16_t _3, int16_t _4, int16_t _5)
{
	this->_Rating[0] = _1;
	this->_Rating[1] = _2;
	this->_Rating[2] = _3;
	this->_Rating[3] = _4;
	this->_Rating[4] = _5;
}

void Seller::Rate(int _score)
{
	_Rating[_score - 1]++;
}

float Seller::GetRate()
{
	// No _Rating
	if (_Rating[0] + _Rating[1] + _Rating[2] + _Rating[3] + _Rating[4] == 0) return 0;
	// Normal case
	return (float)(1 * _Rating[0] + 2 * _Rating[1] + 3 * _Rating[2] + 4 * _Rating[3] + 5 * _Rating[4]) / (float)(_Rating[0] + _Rating[1] + _Rating[2] + _Rating[3] + _Rating[4]);
}

void Seller::StatsByMonth(int16_t _Year, int8_t _Month)
{
	if (_Year < 1900 || _Year > Date::Today().Year)
	{
		cout << "Invalid year." << endl;
		return;
	}

	if (_Month < 1 || _Month > 12)
	{
		cout << "Invalid month." << endl;
		return;
	}

	list<Order> FilteredOrders = OrderProvider::GetInstance().ListByAccountID(this->ID());
	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		if ((*i).ShippingDate().Month != _Month)
		{
			FilteredOrders.erase(i);
			i = FilteredOrders.begin();
		}
	}

	cout << "Overview for " << Date::Month_String(_Month) << ":" << endl;
	if (FilteredOrders.size() == 0)
	{
		cout << "There are no orders for this month." << endl;
		return;
	}
	cout << endl;

	// Get stats
	list<ProductStatItem> StatTable;
	for (auto i = FilteredOrders.begin(); i != FilteredOrders.end(); ++i)
	{
		bool Found = false;
		for (auto j = StatTable.begin(); j != StatTable.end(); ++j)
		{
			if ((*j).ProductID == (*i).ID())
			{
				Found = true;
				(*j).Quantity += (*i).Quantity();
				(*j).TotalTotalPrice += (*i).TotalPrice() - (*i).ShippingFee();
				break;
			}
		}
		if (!Found)
		{
			StatTable.insert(StatTable.end(), { (*i).ID(), (*i).Quantity(), (*i).TotalPrice() - (*i).ShippingFee() });
		}
	}

	// Show stats
	for (auto i = StatTable.begin(); i != StatTable.end(); ++i)
	{
		cout << (*i).ProductID;
		cout << " (" << ProductProvider::GetInstance().GetByID((*i).ProductID)->Name() << ") ";
		cout << " : ";
		cout << "Quantity = " << (*i).Quantity << "; ";
		cout << "Revenue = " << (*i).TotalTotalPrice << endl;
	}
	cout << endl;

	// Total revenue
	int64_t Total = 0;
	for (auto i = StatTable.begin(); i != StatTable.end(); ++i)
	{
		Total += (*i).TotalTotalPrice;
	}
	cout << "Total revenue = " << Total << endl;
	cout << endl;

	// Best seller
	ProductStatItem BestSeller;
	for (auto i = StatTable.begin(); i != StatTable.end(); ++i)
	{
		if ((*i).Quantity > BestSeller.Quantity)
		{
			BestSeller = (*i);
		}
	}
	cout << "Best seller of the month:" << endl;
	cout << BestSeller.ProductID;
	cout << " (" << ProductProvider::GetInstance().GetByID(BestSeller.ProductID)->Name() << ") ";
	cout << " : ";
	cout << "Quantity = " << BestSeller.Quantity << "; ";
	cout << "Revenue = " << BestSeller.TotalTotalPrice << endl;
}

void Seller::CancelOrder(string _OrderID)
{
	Order* _Order = OrderProvider::GetInstance().GetByID(_OrderID);
	if (_Order == nullptr) {
		cout << "Order does not exist." << endl;
		return;
	}
	if (_Order->Status() == SELLER_PENDING) {
		_Order->Status(SELLER_CANCELLED);
	}
	else {
		cout << "Can not cancel order now" << endl;
		return;
	}
}
