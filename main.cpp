#include "Shoppingsite.h"
using namespace std;

#pragma region doTask
void doTask()
{
	int menu_level_1 = 0, menu_level_2 = 0;
	int is_program_exit = 0;
	string trash;

	while (!is_program_exit)
	{
		fin >> menu_level_1 >> menu_level_2;
		cout << menu_level_1 << menu_level_2 << endl;
		switch (menu_level_1)
		{
		case 1:
		{
			switch (menu_level_2)
			{
			case 1:
			{
				SignUp *signUp = new SignUp;
				signUp->SignUpWork();
				break;
			}

			case 2:
			{
				if (isLogIn)
				{
					SignOut *signOut = new SignOut;
					signOut->SignOutWork();
					cout << "로그아웃 됐습니다" << endl;
				}
				else
				{
					cout << "로그인을 먼저 해주세요" << endl;
				}

				break;
			}
			}
			break;
		}

		case 2:
		{
			switch (menu_level_2)
			{
			case 1:
			{
				if (!isLogIn)
				{
					LogIn *logIn = new LogIn;
					logIn->LogInWork();
				}
				else
				{
					cout << "이미 로그인 상태입니다" << endl;
					getline(fin, trash);
				}

				break;
			}

			case 2:
			{
				if (isLogIn)
				{
					LogOut *logOut = new LogOut;
					logOut->LogOutWork();
				}
				else
				{
					cout << "로그인을 먼저 해주세요" << endl;
				}

				break;
			}
			}
			break;
		}
		case 3:
		{
			switch (menu_level_2)
			{
			case 1:
			{
				AddSellingItem *addSellingItem = new AddSellingItem;
				addSellingItem->AddSellingItemWork();
				break;
			}
			case 2:
			{
				// 등록 상품 조회
				SearchSellingItem *searchSellingItem = new SearchSellingItem;
				searchSellingItem->SearchSellingItemWork();
				break;
			}
			case 3:
			{
				// 판매 완료 상품 조회
				SearchSoldOutItem *searchSoldOutItem = new SearchSoldOutItem;
				searchSoldOutItem->SearchSoldOutItemWork();
				break;
			}
			}
			break;
		}
		case 4:
		{
			switch (menu_level_2)
			{
			case 1:
			{
				SearchItem *searchItem = new SearchItem;
				searchItem->SearchItemWork();
				break;
			}
			case 2:
			{
				Purchase *purchase = new Purchase;
				purchase->PurchaseWork();
				break;
			}
			case 3:
			{
				SearchBuyItem *searchBuyItem = new SearchBuyItem;
				searchBuyItem->SearchBuyItemWork();
				break;
			}
			case 4:
			{
				EvaluateItem *evaluateItem = new EvaluateItem;
				evaluateItem->EvaluateItemWork();
				break;
			}
			}
			break;
		}
		case 5:
		{
			if (menu_level_2 == 1)
			{
				PrintTotalSales *printTotalSales = new PrintTotalSales;
				printTotalSales->PrintTotalSalesWork();
			}
			break;
		}
		case 6:
		{
			if (menu_level_2 == 1)
				is_program_exit = true;
			fout << "6.1. 종료" << endl;
			break;
		}
		}
	}

	return;
}
#pragma endregion

int main()
{
	doTask();
	fout.close();
	fin.close();
	return 0;
}