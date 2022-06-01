#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <algorithm>
#include <vector>

#define MAX_NUMBER_OF_SALEITEM 5

using namespace std;
ofstream fout("write.txt");
ifstream fin("read.txt");

static bool isLogIn = false;	 //현재 로그인 상태 표시
static char logInedID[20] = {0}; // isLogIn이 true일때 로그인된 아이디
static bool isSelected = false;	 // 상품 정보 검색에서 선택된 상품이 현재 있는지 표시
static string selectedItem;		 // 상품 정보 검색에서 검색한 상품

typedef struct MemberInfo
{
	string name;
	int securityNum;
	string ID;
	string password;
	struct MemberInfo *next;
} MemberInfo;

MemberInfo *top = NULL;
MemberInfo *last = NULL;

typedef struct SaleItemInfo
{
	int check;
	char sellerID[20];
	char buyerID[20];
	string productName;
	string coName;
	int satisfaction;
	int price;
	int leftItem;
} SaleItemInfo;

SaleItemInfo SellingItemList[MAX_NUMBER_OF_SALEITEM] = {
	0,
};

bool equal(char a[], char b[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (a[i] != b[i])
			return false;
	}
	return true;
}

#pragma region saleItem
class PrintTotalSalesUI;
class SearchItemUI;
class PurchaseUI;
class SearchBuyItemUI;
class EvaluateItemUI;
class SearchSellingItemUI;
class SearchSoldOutItemUI;

class SaleItem
{
public:
	SaleItem(){};
	void createItem(string productName, string coName, int price, int leftItem)
	{
		for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
		{
			if (SellingItemList[i].check == 0)
			{
				SellingItemList[i].check = 1;
				SellingItemList[i].productName = productName;
				SellingItemList[i].coName = coName;
				SellingItemList[i].price = price;
				SellingItemList[i].leftItem += leftItem;
				copy(logInedID, logInedID + 20, SellingItemList[i].sellerID);

				break;
			}
		}
	}
	void getSoldItemInfo(PrintTotalSalesUI *printTotalSalesUI);
	void getSaleItemInfo(SearchSellingItemUI *searchSellingItemUI);
	void listSoldItem(SearchSoldOutItemUI *pSearchSoldOutItemUI);
	void getSoldOutItemDetails(SearchSoldOutItemUI *searchSoldOutItemUI);
	void findItem(string productName, SearchItemUI *SearchItemUI);
	void processingPurchase(char logInedID[20], string productName, PurchaseUI *PurchaseUI);
	void listBuyedItem(char logInedID[20], SearchBuyItemUI *SearchBuyItemUI);
	void addScore(string productName, int score, EvaluateItemUI *EvaluateItemUI);
};
#pragma endregion

#pragma region member
class Member
{
private:
	MemberInfo *p;
	MemberInfo *q;

public:
	Member() { p, q = NULL; }
	void print()
	{
		p = top;
		while (p != NULL)
		{
			cout << p->name << endl;
			p = p->next;
		}
	}
	bool createMember(string name, int securityNum, string ID, string password)
	{
		p = top;
		while (1)
		{
			if (p == NULL)
			{
				break;
			}

			if (name.compare(p->name) == 0 && securityNum == p->securityNum)
			{
				cout << "이미 등록된 유저입니다" << endl;
				return true; //이미 등록된 상태
			}

			else
			{
				q = p;
				p = q->next;
			}
		}

		MemberInfo *newNode = new MemberInfo;
		newNode->name = name;
		newNode->securityNum = securityNum;
		newNode->ID = ID;
		newNode->password = password;
		newNode->next = NULL;
		if (top == NULL)
		{
			top = newNode;
		}
		else
		{
			last->next = newNode;
		}

		last = newNode;

		return false;
	}
	void removeMember(string ID)
	{
		p = top;
		while (1)
		{
			if (p == NULL)
			{
				cout << "등록된 유저가 없습니다" << endl;
				break;
			}
			if (ID.compare(p->ID) != 0)
			{
				q = p;
				p = q->next;
			}
			else
			{
				if (p == top)
				{
					top = p->next;
				}
				else
				{
					q->next = p->next;
				}
				delete p;
				break;
			}
		}
	}
	bool checkMember(string ID, string password)
	{
		p = top;
		while (1)
		{
			if (p == NULL)
			{
				cout << "해당 유저는 존재하지 않습니다, ID/password 를 다시 확인해주세요" << endl;
				return false;
			}

			if (ID.compare(p->ID) == 0 && password.compare(p->password) == 0)
			{
				strcpy(logInedID, ID.c_str());
				return true;
			}

			else
			{
				q = p;
				p = q->next;
			}
		}
	}
};
#pragma endregion

#pragma region signup
class SignUp;
class SignUpUI
{
private:
	SignUp *pSignUp;
	string name;
	int securityNum = 0;
	string ID;
	string password;

public:
	SignUpUI(SignUp *signUp);
	void readFile();
	void printSignUpUI()
	{
		fout << "1.1. 회원가입" << endl;
		fout << "> " << name << " " << securityNum << " " << ID << " " << password << endl;
		fout << endl;
	}
};

class SignUp
{
private:
	SignUpUI *pSignUpUI = NULL;
	Member *pMember = NULL;
	bool alreadyExist = false;

public:
	SignUp(){};

	void SignUpWork()
	{
		pSignUpUI = new SignUpUI(this);
		pSignUpUI->readFile();
		if (!alreadyExist)
		{
			pSignUpUI->printSignUpUI();
		}
	}

	void addNewMember(string name, int securityNum, string ID, string password)
	{
		pMember = new Member;
		alreadyExist = pMember->createMember(name, securityNum, ID, password);
	}
};
SignUpUI::SignUpUI(SignUp *signUp)
{
	pSignUp = signUp;
}

void SignUpUI::readFile()
{
	fin >> name >> securityNum >> ID >> password;
	pSignUp->addNewMember(name, securityNum, ID, password);
}
#pragma endregion

#pragma region signout
class SignOut;
class SignOutUI
{
private:
	SignOut *pSignOut;

public:
	SignOutUI(SignOut *signOut);
	void printSignOutUI()
	{
		fout << "1.2. 회원탈퇴" << endl;
		fout << "> " << logInedID << endl;
		fout << endl;
	}
};

class SignOut
{
private:
	SignOutUI *pSignOutUI = NULL;
	Member *pMember = NULL;

public:
	SignOut(){};
	void SignOutWork()
	{
		pSignOutUI = new SignOutUI(this);
		pSignOutUI->printSignOutUI();
		isLogIn = false;
		isSelected = false;
	}

	void removeMember(string ID)
	{
		pMember = new Member;
		pMember->removeMember(ID);
	}
};

SignOutUI::SignOutUI(SignOut *signOut)
{
	pSignOut = signOut;
}

#pragma endregion

#pragma region LogIn
class LogIn;
class LogInUI
{
private:
	LogIn *pLogIn;
	string ID;
	string password;

public:
	LogInUI(LogIn *logIn);
	void readFile();
	void printLogInUI()
	{
		fout << "2.1. 로그인" << endl;
		fout << "> " << ID << " " << password << endl;
		fout << endl;
	}
};

class LogIn
{
private:
	LogInUI *pLogInUI = NULL;
	Member *pMember = NULL;
	bool alreadyLogIned = false;

public:
	LogIn(){};
	void LogInWork()
	{
		pLogInUI = new LogInUI(this);
		pLogInUI->readFile();
		if (alreadyLogIned)
		{
			pLogInUI->printLogInUI();
			isLogIn = true; //로그인 활성화
			cout << "로그인 되었습니다" << endl;
		}
	}
	void checkMember(string ID, string password)
	{
		pMember = new Member;
		alreadyLogIned = pMember->checkMember(ID, password);
	}
};

LogInUI::LogInUI(LogIn *logIn)
{
	pLogIn = logIn;
}

void LogInUI::readFile()
{
	fin >> ID >> password;
	pLogIn->checkMember(ID, password);
}
#pragma endregion

#pragma region LogOut
class LogOut;
class LogOutUI
{
private:
	LogOut *pLogOut;

public:
	LogOutUI(LogOut *logOut);
	void printLogOutUI()
	{
		fout << "2.2. 로그아웃" << endl;
		fout << "> " << logInedID << endl;
		fout << endl;
	}
};

class LogOut
{
private:
	LogOutUI *pLogOutUI = NULL;
	Member *pMember = NULL;

public:
	LogOut(){};
	void LogOutWork()
	{
		pLogOutUI = new LogOutUI(this);
		pLogOutUI->printLogOutUI();
		isLogIn = false;
		isSelected = false;
	}
};

LogOutUI::LogOutUI(LogOut *logOut)
{
	pLogOut = logOut;
}

#pragma endregion

#pragma region addsellingitem // 3.1 판매 상품 등록
class AddSellingItem;
class AddSellingItemUI
{
private:
	AddSellingItem *pAddSellingItem;
	string productName;
	string coName;
	int price;
	int leftItem;

public:
	AddSellingItemUI(AddSellingItem *addSellingItme);
	void readFile();
	void printAddSellingItemUI()
	{
		fout << "3.1. 판매 의류 등록" << endl;
		fout << "> " << productName << " " << coName << " " << price << " " << leftItem << endl;
		fout << endl;
	}
};

class AddSellingItem
{
private:
	AddSellingItemUI *pAddSellingItemUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	AddSellingItem(){};
	void AddSellingItemWork()
	{
		pAddSellingItemUI = new AddSellingItemUI(this);
		pAddSellingItemUI->readFile();
		pAddSellingItemUI->printAddSellingItemUI();
	}
	void addNewItem(string productName, string coName, int price, int leftItem)
	{
		pSaleItem = new SaleItem;
		pSaleItem->createItem(productName, coName, price, leftItem);
	}
};

AddSellingItemUI::AddSellingItemUI(AddSellingItem *addSellingItem)
{
	pAddSellingItem = addSellingItem;
}

void AddSellingItemUI::readFile()
{
	fin >> productName >> coName >> price >> leftItem;
	pAddSellingItem->addNewItem(productName, coName, price, leftItem);
}
#pragma endregion
// 3.1 end

#pragma region searchsellingitem
class SearchSellingItem;
class SearchSellingItemUI // 등록 상품 조회 UI 3.2
{
private:
	SearchSellingItem *pSearchSellingItem; //상품명] [제작회사명] [가격] [수량] }
	string productName;
	string coName;
	int price;
	int leftItem;

public:
	SearchSellingItemUI(SearchSellingItem *searchSellingItem);
	void startInterface(string productName, string coName, int price, int leftItem)
	{
		fout << ">"
			 << " " << productName << " " << coName << " " << price << " " << leftItem << endl;
	}
	~SearchSellingItemUI(){};
};

class SearchSellingItem // 등록 상품 조회 3.2
{
private:
	SearchSellingItemUI *pSearchSellingItemUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	SearchSellingItem(){};
	void SearchSellingItemWork();
	~SearchSellingItem(){};
};

#pragma endregion
#pragma region searchsoldoutitem

SearchSellingItemUI::SearchSellingItemUI(SearchSellingItem *searchSellingItem)
{
	pSearchSellingItem = searchSellingItem;
}

void SearchSellingItem::SearchSellingItemWork()
{
	pSearchSellingItemUI = new SearchSellingItemUI(this);
	pSaleItem = new SaleItem;
	pSaleItem->getSaleItemInfo(pSearchSellingItemUI);
}

void SaleItem::getSaleItemInfo(SearchSellingItemUI *searchSellingItemUI)
{
	fout << "3.2. 등록 상품 조회" << endl;

	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{
		if (SellingItemList[i].leftItem == 1 && equal(logInedID, SellingItemList[i].sellerID, 20))
		{
			searchSellingItemUI->startInterface(SellingItemList[i].productName, SellingItemList[i].coName, SellingItemList[i].price, SellingItemList[i].leftItem);
		}
	}
	fout << endl;
}

// 3.2 end

#pragma region searchsoldoutitem

class SearchSoldOutItem;
class SearchSoldOutItemUI // 3.3 판매 완료 상품 조회 UI
{
	typedef struct SoldItemInfo
	{
		string productName;
		string coName;
		int price;
		int soldItem;
		int satisfaction;
	} SoldItemInfo;

	static bool cmp(const SoldItemInfo &a, const SoldItemInfo &b)
	{
		return a.productName < b.productName; // 이름에 대한 정렬
	}

private:
	SearchSoldOutItem *pSearchSoldOutItem;
	vector<SoldItemInfo> SoldItemList;

public:
	SearchSoldOutItemUI(SearchSoldOutItem *searchSoldOutItem);
	void setSoldItemList(string, string, int, int, int);
	void sortItem();
	void startinterface();
};

class SearchSoldOutItem //판매 완료 상품 조회 3.3
{
private:
	SearchSoldOutItemUI *pSearchSoldOutItemUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	SearchSoldOutItem(){};
	void SearchSoldOutItemWork();
	~SearchSoldOutItem(){};
};

#pragma endregion

// 3.3 판매 완료 상품 조회

void SearchSoldOutItemUI::setSoldItemList(string prodcutName, string coName, int price, int soldItem, int satisfaction)
{
	SoldItemInfo soldItemInfo;
	soldItemInfo.productName = prodcutName;
	soldItemInfo.coName = coName;
	soldItemInfo.price = price;
	soldItemInfo.soldItem = soldItem;
	soldItemInfo.satisfaction = satisfaction;
	SoldItemList.push_back(soldItemInfo);
}

void SaleItem::listSoldItem(SearchSoldOutItemUI *pSearchSoldOutItemUI)
{
	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{
		if (SellingItemList[i].leftItem != 0)
			continue;
		pSearchSoldOutItemUI->setSoldItemList(SellingItemList[i].productName, SellingItemList[i].coName,
											  SellingItemList[i].price, SellingItemList[i].leftItem + 1, SellingItemList[i].satisfaction);
	}
}
void SearchSoldOutItemUI::sortItem()
{
	sort(SoldItemList.begin(), SoldItemList.end(), cmp);
}

void SearchSoldOutItemUI::startinterface()
{
	for (int i = 0; i < SoldItemList.size(); i++)
	{
		if (SoldItemList[i].productName.empty())
			continue;
		fout << ">" << SoldItemList[i].productName << " " << SoldItemList[i].coName << " " << SoldItemList[i].price << " " << SoldItemList[i].soldItem << " " << SoldItemList[i].satisfaction << endl;
	}
	fout << endl;
}

SearchSoldOutItemUI::SearchSoldOutItemUI(SearchSoldOutItem *searchSoldOutItem)
{
	pSearchSoldOutItem = searchSoldOutItem;
}
/*
void SearchSoldOutItemUI::sortItem(SaleItemInfo *p)
{

	// sort(p, p + sizeof(p) / sizeof(SaleItemInfo) + 1);
	sort(p, p + sizeof(p) + 1, cmp);
	for (int i = 0; i < sizeof(p); i++)
	{
		this->startinterface(p[i].productName, p[i].coName, p[i].price, p[i].leftItem, p[i].satisfaction);
	}
}*/

void SearchSoldOutItem::SearchSoldOutItemWork()
{
	pSearchSoldOutItemUI = new SearchSoldOutItemUI(this);
	pSaleItem = new SaleItem;
	pSaleItem->getSoldOutItemDetails(pSearchSoldOutItemUI);
	pSearchSoldOutItemUI->startinterface();
}

void SaleItem::getSoldOutItemDetails(SearchSoldOutItemUI *searchSoldOutItemUI)
{
	fout << "3.3. 판매 완료 상품 조회" << endl;
	// SaleItemInfo *p = SellingItemList;
	// int j = 0;
	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{

		if (SellingItemList[i].check == 1 && SellingItemList[i].leftItem == 0 && equal(logInedID, SellingItemList[i].sellerID, 20))
		{
			this->listSoldItem(searchSoldOutItemUI);
			searchSoldOutItemUI->sortItem();
		}
	}
}

#pragma region PrintTotalSales
class PrintTotalSales;
class PrintTotalSalesUI
{
private:
	PrintTotalSales *pPrintTotalSales;
	string productName;
	int price;
	int satisfaction;

public:
	PrintTotalSalesUI(PrintTotalSales *printTotalSales);
	void readFile();
	void startInterface(string productName, int price, int satisfaction)
	{
		fout << "> " << productName << " " << price << " " << satisfaction << endl;
	}
};

class PrintTotalSales
{
private:
	PrintTotalSalesUI *pPrintTotalSalesUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	PrintTotalSales(){};
	void PrintTotalSalesWork()
	{
		pPrintTotalSalesUI = new PrintTotalSalesUI(this);
		pSaleItem = new SaleItem;
		pSaleItem->getSoldItemInfo(pPrintTotalSalesUI);
	}
};

PrintTotalSalesUI::PrintTotalSalesUI(PrintTotalSales *printTotalSales)
{
	pPrintTotalSales = printTotalSales;
}
void SaleItem::getSoldItemInfo(PrintTotalSalesUI *printTotalSalesUI)
{
	{
		fout << "5.1. 판매 상품 통계" << endl;

		for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
		{
			if (SellingItemList[i].leftItem == 0 && equal(logInedID, SellingItemList[i].sellerID, 20))
			{
				printTotalSalesUI->startInterface(SellingItemList[i].productName, SellingItemList[i].price, SellingItemList[i].satisfaction);
			}
		}
		fout << endl;
	}
}
#pragma endregion

class SearchItem;
class SearchItemUI
{
private:
	SearchItem *pSearchItem;
	char sellerID[20];
	string productName;
	string coName;
	int price;
	int leftItem;
	int satisfaction;

public:
	SearchItemUI(SearchItem *searchItem);
	void readFile();
	void SetSearchItemUIAttr(char[20], string, int, int, int);
	void printSearchItemUI();
};

class SearchItem
{
private:
	SearchItemUI *pSearchItemUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	SearchItem(){};
	void SearchItemWork()
	{
		pSearchItemUI = new SearchItemUI(this);
		pSearchItemUI->readFile();
		if (isSelected)
			pSearchItemUI->printSearchItemUI();
	}
	void showItem(string productName)
	{
		pSaleItem = new SaleItem;
		pSaleItem->findItem(productName, pSearchItemUI);
	}
};

SearchItemUI::SearchItemUI(SearchItem *searchItem)
{
	pSearchItem = searchItem;
}

void SearchItemUI::readFile()
{
	fin >> productName;
	selectedItem = productName;
	pSearchItem->showItem(productName);
}

void SearchItemUI::SetSearchItemUIAttr(char chArr[20], string str, int num1, int num2, int num3)
{
	strcpy(sellerID, chArr);
	coName = str;
	price = num1;
	leftItem = num2;
	satisfaction = num3;
}

void SearchItemUI::printSearchItemUI()
{
	fout << "4.1. 상품 정보 검색" << endl;
	fout << "> " << sellerID << " " << productName << " " << coName << " " << price << " " << leftItem << " " << satisfaction << endl;
	fout << endl;
}

void SaleItem::findItem(string productName, SearchItemUI *pSearchItemUI)
{
	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{
		if (productName == SellingItemList[i].productName && SellingItemList[i].leftItem != 0)
		{
			pSearchItemUI->SetSearchItemUIAttr(SellingItemList[i].sellerID, SellingItemList[i].coName,
											   SellingItemList[i].price, SellingItemList[i].leftItem, SellingItemList->satisfaction);
			isSelected = true;
			return;
		}
		else if (productName == SellingItemList[i].productName && SellingItemList[i].leftItem == 0)
		{
			cout << "판매완료된 상품입니다. 상품 정보 검색이 불가능합니다." << endl;
			fout << "4.1. 상품 정보 검색" << endl;
			fout << "> " << endl;
			fout << endl;
			return;
		}
	}
	cout << "입력하신 상품명의 상품이 존재하지 않습니다." << endl;
	fout << "4.1. 상품 정보 검색" << endl;
	fout << "> " << endl;
	fout << endl;
}

class Purchase;
class PurchaseUI
{
private:
	Purchase *pPurchase;
	string productName;
	char sellerID[20];

public:
	PurchaseUI(Purchase *purchase);
	void getProductName();
	void setPurchaseUIAttr(char[20], string);
	void printPurchaseUI();
};

class Purchase
{
private:
	PurchaseUI *pPurchaseUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	Purchase(){};
	void PurchaseWork()
	{
		if (isSelected)
		{
			pPurchaseUI = new PurchaseUI(this);
			pPurchaseUI->getProductName();
			pPurchaseUI->printPurchaseUI();
		}
		else
		{
			cout << "상품 정보 검색을 먼저 해 주십시오" << endl;
			fout << "4.2. 상품 구매" << endl;
			fout << "> " << endl;
			fout << endl;
		}
	}
	void handleItem(string productName);
};

PurchaseUI::PurchaseUI(Purchase *purchase)
{
	pPurchase = purchase;
}
void PurchaseUI::getProductName()
{
	pPurchase->handleItem(selectedItem);
}

void PurchaseUI::setPurchaseUIAttr(char chArr[20], string str)
{
	strcpy(sellerID, chArr);
	productName = str;
}

void PurchaseUI::printPurchaseUI()
{
	fout << "4.2. 상품 구매" << endl;
	fout << "> " << sellerID << " " << productName << endl;
	fout << endl;
}

void Purchase::handleItem(string productName)
{
	pSaleItem = new SaleItem;
	pSaleItem->processingPurchase(logInedID, productName, pPurchaseUI);
}

void SaleItem::processingPurchase(char chArr[20], string str, PurchaseUI *pPurchaseUI)
{
	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{
		if (str == SellingItemList[i].productName)
		{
			if (SellingItemList[i].leftItem > 0)
			{
				strcpy(SellingItemList[i].buyerID, chArr); // 조회한 상품에 대한 구매처리
				SellingItemList[i].leftItem -= 1;
				pPurchaseUI->setPurchaseUIAttr(SellingItemList[i].sellerID, SellingItemList[i].productName); // UI의 attr 설정
			}
		}
	}
}

class SearchBuyItem;
class SearchBuyItemUI
{
	typedef struct BuyItemInfo
	{
		char sellerID[20];
		string productName;
		string coName;
		int price;
		int leftItem;
		int satisfaction;
	} BuyItemInfo;
	static bool cmp(const BuyItemInfo &a, const BuyItemInfo &b)
	{
		return a.productName < b.productName;
	}

private:
	SearchBuyItem *pSearchBuyItem;
	vector<BuyItemInfo> BuyItemList;

public:
	SearchBuyItemUI(SearchBuyItem *searchBuyItem);
	void setBuyItemList(char[20], string, string, int, int, int);
	void sortItem();
	void printBuyItemUI();
};

class SearchBuyItem
{
private:
	SearchBuyItemUI *pSearchBuyItemUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	SearchBuyItem(){};
	void getLogInedID();
	void SearchBuyItemWork()
	{
		if (isLogIn)
		{
			pSearchBuyItemUI = new SearchBuyItemUI(this);
			getLogInedID();
			pSearchBuyItemUI->sortItem();
			pSearchBuyItemUI->printBuyItemUI();
		}
		else
		{
			cout << "로그인을 먼저 해 주십시오" << endl;
			fout << "4.3. 상품 구매 내역 조회" << endl;
			fout << "> " << endl;
			fout << endl;
		}
	}
};

SearchBuyItemUI::SearchBuyItemUI(SearchBuyItem *searchBuyItem)
{
	pSearchBuyItem = searchBuyItem;
}

void SearchBuyItemUI::setBuyItemList(char chArr[20], string str1, string str2, int num1, int num2, int num3)
{
	BuyItemInfo buyItemInfo;
	strcpy(buyItemInfo.sellerID, chArr);
	buyItemInfo.productName = str1;
	buyItemInfo.coName = str2;
	buyItemInfo.price = num1;
	buyItemInfo.leftItem = num2;
	buyItemInfo.satisfaction = num3;
	BuyItemList.push_back(buyItemInfo);
}

void SearchBuyItemUI::sortItem()
{
	sort(BuyItemList.begin(), BuyItemList.end(), cmp);
}

void SearchBuyItemUI::printBuyItemUI()
{
	fout << "4.3. 상품 구매 내역 조회" << endl;
	for (int i = 0; i < BuyItemList.size(); i++)
	{
		fout << "> " << BuyItemList[i].sellerID << " " << BuyItemList[i].productName << " " << BuyItemList[i].coName << " "
			 << BuyItemList[i].price << " " << BuyItemList[i].leftItem << " " << BuyItemList[i].satisfaction << endl;
	}
	fout << endl;
}

void SearchBuyItem::getLogInedID()
{
	pSaleItem = new SaleItem;
	pSaleItem->listBuyedItem(logInedID, pSearchBuyItemUI);
}

void SaleItem::listBuyedItem(char chArr[20], SearchBuyItemUI *pSearchBuyItemUI)
{
	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{
		if (strcmp(SellingItemList[i].buyerID, chArr) == 0)
			pSearchBuyItemUI->setBuyItemList(SellingItemList[i].sellerID, SellingItemList[i].productName,
											 SellingItemList[i].coName, SellingItemList[i].price, SellingItemList[i].leftItem, SellingItemList[i].satisfaction);
	}
}

class EvaluateItem;
class EvaluateItemUI
{
private:
	EvaluateItem *pEvaluateItem;
	char sellerID[20];
	string productName;
	int satisfaction;
	bool isEvaluated = false;

public:
	EvaluateItemUI(EvaluateItem *evaluateItem);
	void readFile();
	void SetEvaluateItemUIAttr(char[20], string, int, bool);
	void printEvaluateItemUI();
};

class EvaluateItem
{
private:
	EvaluateItemUI *pEvaluateItemUI = NULL;
	SaleItem *pSaleItem = NULL;

public:
	EvaluateItem(){};
	void EvaluateItemWork()
	{
		if (isLogIn)
		{
			pEvaluateItemUI = new EvaluateItemUI(this);
			pEvaluateItemUI->readFile();
			pEvaluateItemUI->printEvaluateItemUI();
		}
		else
		{
			cout << "로그인을 먼저 해 주십시오" << endl;
			fout << "4.4. 상품 구매만족도 평가" << endl;
			fout << "> " << endl;
			fout << endl;
		}
	}
	void saveItemSatisfied(string productName, int score);
};

EvaluateItemUI::EvaluateItemUI(EvaluateItem *evaluateItem)
{
	pEvaluateItem = evaluateItem;
}

void EvaluateItemUI::readFile()
{
	fin >> productName >> satisfaction;
	pEvaluateItem->saveItemSatisfied(productName, satisfaction);
}

void EvaluateItemUI::SetEvaluateItemUIAttr(char chArr[20], string str, int num, bool TF)
{
	strcpy(sellerID, chArr);
	productName = str;
	satisfaction = num;
	isEvaluated = TF;
}

void EvaluateItemUI::printEvaluateItemUI()
{
	if (isEvaluated)
	{
		fout << "4.4. 상품 구매만족도 평가" << endl;
		fout << "> " << sellerID << " " << productName << " " << satisfaction << endl;
		fout << endl;
	}
}

void EvaluateItem::saveItemSatisfied(string productName, int satisfaction)
{
	pSaleItem = new SaleItem;
	pSaleItem->addScore(productName, satisfaction, pEvaluateItemUI);
}

void SaleItem::addScore(string productName, int satisfaction, EvaluateItemUI *pEvaluateItemUI)
{
	bool isEvaluated = false;
	for (int i = 0; i < MAX_NUMBER_OF_SALEITEM; i++)
	{
		if (productName == SellingItemList[i].productName && strcmp(SellingItemList[i].buyerID, logInedID) == 0)
		{
			SellingItemList[i].satisfaction = satisfaction;
			isEvaluated = true;
			pEvaluateItemUI->SetEvaluateItemUIAttr(logInedID, productName, satisfaction, isEvaluated);
			return;
		}
		else if (productName == SellingItemList[i].productName && strcmp(SellingItemList[i].buyerID, logInedID) != 0)
		{
			cout << "사용자가 구매한 상품이 아닙니다." << endl;
			fout << "4.4. 상품 구매만족도 평가" << endl;
			fout << "> " << endl;
			fout << endl;
			return;
		}
	}
	cout << "사용자 입력한 상품명의 상품이 없습니다." << endl;
	fout << "4.4. 상품 구매만족도 평가" << endl;
	fout << "> " << endl;
	fout << endl;
}