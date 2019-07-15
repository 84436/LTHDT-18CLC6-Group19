#include "Product.h"

Product::Product(string SellerID, string ID, string Name, string Category, string Description, int32_t Stock, int64_t Price, int64_t ShippingFee)
{
	SetSellerID(SellerID);
	ID(ID);
}

void Product::GetInfo(){
	cout << endl << "\t \t INFORMATION : " << endl;
	cout << "\t \t PROVIDER : " << endl;
	Provider->OutputInfo();
	cout << "\t \t PRODUCT INFORMATION : " << endl;
	cout << "\t ID : \t" << ID << endl;
	cout << "\t Name : \t" << Name << endl;
	cout << "\t Category : \t" << Category << endl;
	cout << "\t Decription : \t" << Decription << endl;
	cout << "\t Stock : \t" << Stock << endl;
	cout << "\t Price : \t" << Price << endl;
	cout << "\t Shipping Fee : \t" << ShippingFee << endl;
	cout << "\t Rating : \t" << (1 * Rating[0] + 2 * Rating[1] + 3 * Rating[2] + 4 * Rating[3] + 5 * Rating[4]) / (Rating[0] + Rating[1] + Rating[2] + Rating[3] + Rating[4]) << endl;
}

void Product::Rate(int){
	int n;
	cout << "How many Stars you want to rate this Product (1 - 5) : ";
	cin >> n;
	Rating[n - 1]++;
}