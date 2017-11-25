//Headers
#include<Windows.h>
#include "Header.h"
#include <map>
#include<tchar.h>
#define WIDTH 1360
#define HEIGHT 760
using namespace std;

//global function decalrations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MyDialProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CustomerNameDialProc(HWND, UINT, WPARAM, LPARAM);
HWND ghModelessDlg = NULL;
HWND gHwnd = NULL;
HBITMAP ghBitmapDlg;
//CPU comboboxes handlers
HWND ghCPUManufacturerCB = NULL;
HWND ghCPUTypeCB = NULL;
HWND ghCPUModelCB = NULL;
HBITMAP hBitmapBackgroundImage;
//RAM comboboxes handlers
HWND ghRAMManufacturerCB = NULL;
HWND ghRAMSizeCB = NULL;
HINSTANCE hInstance;
//GPU comboboxes handlers
HWND ghGraphicsCardManufacturerCB = NULL;
HWND ghGraphicsCardModelCB = NULL;
HWND ghGraphicsCardSizeCB = NULL;

//Motherboard comboboxes handlers
HWND ghMotherboardCB = NULL;
HWND ghMotherboardModelCB = NULL;

//Storage comboboxed handlers
HWND ghStorageCB = NULL;
HWND ghStorageSizeCB = NULL;

//Keyboard combobox handler
HWND ghKeyboardCB = NULL;

//Mouse combobox handler
HWND ghMouseCB = NULL;

//Monitor combobox handlers
HWND ghMonitorCB = NULL;
HWND ghMonitorResolutionCB = NULL;

//Cabinet combobox handler
HWND ghCabinetCB = NULL;

BOOLEAN gbDisplayReceipt, gbReceiptGenerated;

struct WordText {
	TCHAR name[255];
	int xCord;
	int yCord;
};

struct WordText cpuDetails[7], ramDetails[7], gpuDetails[7], motherboardDetails[7], storageDetails[7], keyboardDetails[7], mouseDetalis[7], monitorDetails[7], cabinetDetails[7];
int giOrderAmountTotal;

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	//variable declarations
	WNDCLASSEX wndClass;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	//code
	//initialization of WNDCLASSESX
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register above class
	RegisterClassEx(&wndClass);

	//create window
	gHwnd = CreateWindow(szAppName, TEXT("My Computer Shop"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(gHwnd, SW_MAXIMIZE);
	UpdateWindow(gHwnd);

	//message loop
	while (GetMessage(&msg, NULL, 0, 0)) {
		//process messages related with only window. Messages related with dialouge box will be sent to OS and then to dialogoue procedure
		if (ghModelessDlg == NULL || (IsDialogMessage(ghModelessDlg, &msg) == 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	MessageBox(NULL, TEXT("Thank you for shopping with us."), TEXT("Thank you!"), MB_OK);
	return ((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	void displayReceipt(HWND);
	
	HDC hDC, hDCBackground;
	PAINTSTRUCT Ps;
	SIZE sz;
	
	BITMAP bmBackground;
	int cxBitmap, cyBitmap, cxClient, cyClient;
	PAINTSTRUCT ps;
	RECT rc;
	//code
	switch (iMsg)
	{
	case WM_CREATE:
		
		break;
	case WM_PAINT:
		if (gbDisplayReceipt) {
			displayReceipt(hwnd);
		}
		break;

	case WM_DESTROY: PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (gbDisplayReceipt) {
			displayReceipt(hwnd);
		}
		else {

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			hDC = BeginPaint(hwnd, &Ps);
			hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
			// Load the bitmap from the resource
			hBitmapBackgroundImage = LoadBitmap(hInstance, MAKEINTRESOURCE(BACKGROUND));
			GetObject(hBitmapBackgroundImage, sizeof(bmBackground), &bmBackground);
			cxBitmap = bmBackground.bmWidth;
			cyBitmap = bmBackground.bmHeight;
			// Create a memory device compatible with the above DC variable
			hDCBackground = CreateCompatibleDC(hDC);
			// Select the new bitmap
			SelectObject(hDCBackground, hBitmapBackgroundImage);

			// Copy the bits from the memory DC into the current dc
			BitBlt(hDC, 0, 0, cxClient, cyClient, hDCBackground, 0, 0, SRCCOPY);
			StretchBlt(hDC, 0, 0, cxClient, cyClient, hDCBackground, 0, 0, cxBitmap, cyBitmap, SRCCOPY);

			//text drawing
			GetClientRect(hwnd, &rc);
			SetBkColor(hDC, RGB(0, 0, 0));
			SetTextColor(hDC, RGB(255, 255, 255));
			//at the center
			DrawText(hDC, TEXT("Press Space bar to continue"), -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		
			EndPaint(hwnd, &ps);
			// Restore the old bitmap
			DeleteDC(hDCBackground);
			DeleteObject(hBitmapBackgroundImage);
			EndPaint(hwnd, &Ps);
		}
		break;
	case WM_LBUTTONDOWN:
		break;

	case WM_KEYDOWN: switch (LOWORD(wParam)) {
	case VK_ESCAPE:
		DestroyWindow(hwnd);
		break;
	case VK_SPACE:
		hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		ghModelessDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_CONTROLSDLG), hwnd, MyDialProc);
		//dialog box error handling
		if (!ghModelessDlg)
			MessageBox(hwnd, TEXT("Could not create dialog box. Exiting now..."), TEXT("Error"), MB_ICONSTOP);
		break;
		case 0x52:
			CreateDialog(hInstance, MAKEINTRESOURCE(ID_CUSTOMERNAMEDLG), hwnd, CustomerNameDialProc);
			break;
	}
					 break;

	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

BOOL CALLBACK CustomerNameDialProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	void generateReceipt(TCHAR [255]);
	void displayReceipt(HWND);
	TCHAR customerName[255];
	
	switch (iMsg)
	{
	
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		case IDCONTINUE:
			SendMessage(GetDlgItem(hDlg, ID_CUSTOMERNAME_EB), WM_GETTEXT, (WPARAM)255, (LPARAM)customerName);
			generateReceipt(customerName);
			InvalidateRect(gHwnd, NULL, TRUE);
			EndDialog(hDlg, 0);
			break;
		
		}
		return TRUE;
		break;

	default:
		break;
	}
	return FALSE;
}

void generateReceipt(TCHAR customerName[255]) {

	FILE *fp;
	TCHAR fileName[255] = TEXT(".txt"), str[255];
	_tcscpy_s(str, customerName);
	wcsncat_s(customerName, 255, fileName, 4);

	_wfopen_s(&fp, customerName, TEXT("w"));
	
	fprintf_s(fp, "\t\t\t ****Order Receipt***** \t\t\t\n");
	fprintf_s(fp, "------------------------------------------------------------------------\n");
	fprintf_s(fp, "Name: %ls\n", str);
	fprintf_s(fp, "------------------------------------------------------------------------\n");
	fprintf_s(fp, "1) CPU: Manufacturer: %ls | Type: %ls | Model: %ls | Price: %ls\n", cpuDetails[1].name, cpuDetails[2].name, cpuDetails[3].name, cpuDetails[6].name);
	fprintf_s(fp, "2) RAM: Manufacturer: %ls | Size: %ls | Price: %ls |\n", ramDetails[1].name, ramDetails[4].name, ramDetails[6].name);
	fprintf_s(fp, "3) Graphics Card: Manufacturer: %ls | Model: %ls | Size: %ls | Price: %ls\n", gpuDetails[1].name, gpuDetails[3].name, gpuDetails[4].name, gpuDetails[6].name);
	fprintf_s(fp, "4) Motherboard: Manufacturer: %ls | Model: %ls | Price: %ls \n", motherboardDetails[1].name, motherboardDetails[3].name, motherboardDetails[6].name);
	fprintf_s(fp, "5) Storage: Manufacturer: %ls | Model: %ls | Price: %ls \n", storageDetails[1].name, storageDetails[3].name, storageDetails[6].name);
	fprintf_s(fp, "6) Keyboard: Manufacturer: %ls | Price: %ls\n", keyboardDetails[1].name, keyboardDetails[6].name);
	fprintf_s(fp, "7) Mouse: Manufacturer: %ls | Price: %ls \n", mouseDetalis[1].name, mouseDetalis[6].name);
	fprintf_s(fp, "8) Monitor: Manufacturer: %ls | Resolution: %ls | Price: %ls \n", monitorDetails[1].name, monitorDetails[5].name, monitorDetails[6].name);
	fprintf_s(fp, "9) Cabinet: Manufacturer: %ls | Price: %ls |\n", cabinetDetails[1].name, cabinetDetails[6].name);
	fclose(fp);
	gbReceiptGenerated = TRUE;
}

BOOL CALLBACK MyDialProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	//function declarations
	void addItemsToCombobox(HWND, int, TCHAR[10][255], int);
	int showHideDependentCombobox(HWND, int, int, HWND);
	void getOrderDetails(HWND);
	void displayOrderDetails();

	PAINTSTRUCT ps;
	HDC hdc, hMemDC;
	BITMAP bmp;
	RECT rect;

	//CPU
	TCHAR cpuManufacturerNameList[3][255] = { TEXT("**Select Manufacturer**"), TEXT("Intel"), TEXT("AMD") };
	TCHAR cpuTypeList[3][255] = { TEXT("**Select Type**"), TEXT("32-bit"), TEXT("64-bit") };
	TCHAR intelItemList[4][255] = { TEXT("**Select Item**"), TEXT("Core i3"), TEXT("Core i5"), TEXT("Core i7") };
	TCHAR amdItemList[4][255] = { TEXT("**Select Item**"), TEXT("Opteron"), TEXT("Phenom FX"), TEXT("Athlon X2") };

	//RAM
	TCHAR ramManufacturerNameList[3][255] = { TEXT("**Select Manufacturer**"), TEXT("Kingstone"), TEXT("Samsung") };
	TCHAR ramSizeList[5][255] = { TEXT("**Select Size**"), TEXT("2 GB"), TEXT("4 GB"), TEXT("8 GB"), TEXT("16 GB") };

	//GPU
	TCHAR graphicCardManufacturerNameList[3][255] = { TEXT("**Select Manufacturer**"), TEXT("AMD"), TEXT("NVIDIA") };
	TCHAR nvidiaItemList[5][255] = { TEXT("**Select Model**"), TEXT("GeForce 210"), TEXT("GeForce GT 610"), TEXT("GeForce GT 630"), TEXT("GeForce GTX 650") };
	TCHAR amdGPUItemList[5][255] = { TEXT("**Select Model**"), TEXT("Radeon HD 5450"), TEXT("Radeon HD 6450"), TEXT("Radeon R7 250"), TEXT("Radeon HD 7850") };
	TCHAR graphicCardSizeList[5][255] = { TEXT("**Select Size**"), TEXT("1 GB"), TEXT("2 GB"), TEXT("4 GB"), TEXT("8 GB") };

	//Motherboard
	TCHAR motherboardManufacturerNameList[5][255] = { TEXT("**Select Manufacturer**"), TEXT("Intel"), TEXT("MSI"), TEXT("Gigabyte"), TEXT("Asus") };
	TCHAR intelMotherboardItemList[6][255] = { TEXT("**Select Model**"), TEXT("DH67BLB3"), TEXT("DG43RK"), TEXT("DG41RQ"), TEXT("DH67CLB3"), TEXT("DB75EN") };
	TCHAR msiMotherboardItemList[7][255] = { TEXT("**Select Model**"), TEXT("Z270-A PRO"), TEXT("B350 PC MATE"), TEXT("B250M PRO-VD"), TEXT("B350 TOMAHAWK"), TEXT("B250M PRO-VDH"), TEXT("B350M GAMING PRO") };
	TCHAR gigabyteMotherboardItemList[4][255] = { TEXT("**Select Model**"), TEXT("GA-H110M-A"), TEXT("GA-B250M-DS3H"), TEXT("GA-Z270P-D3") };
	TCHAR asusMotherboardItemList[6][255] = { TEXT("**Select Model**"), TEXT("STRIX Z270-E GAMING"), TEXT("ROG STRIX Z370-E GAMING"), TEXT("PRIME Z270-A"), TEXT("MAXIMUS IX HERO"), TEXT("ROG Strix Z370-I Gaming") };

	//Storage
	TCHAR storageManufacturerNameList[5][255] = { TEXT("**Select Manufacturer**"), TEXT("Samsung"), TEXT("Seagate"), TEXT("WD"), TEXT("Transcend") };
	TCHAR storageSizeList[5][255] = { TEXT("**Select Size**"), TEXT("250 GB"), TEXT("500 GB"), TEXT("1 TB"), TEXT("2 TB") };

	//Keyboard
	TCHAR keyboardManufacturerNameList[6][255] = { TEXT("**Select Manufacturer**"), TEXT("Cooler Master"), TEXT("Corsair"), TEXT("Logitech"), TEXT("Razer"), TEXT("Redragon") };

	//Mouse
	TCHAR mouseManufacturerNameList[6][255] = { TEXT("**Select Manufacturer**"), TEXT("Corsair"), TEXT("Logitech"), TEXT("Razer"), TEXT("Redragon"), TEXT("SteelSeries") };

	//Monitor
	TCHAR monitorManufacturerNameList[6][255] = { TEXT("**Select Manufacturer**"), TEXT("Acer"), TEXT("AOC"), TEXT("Asus"), TEXT("BenQ"), TEXT("Dell") };
	TCHAR monitorResolutionList[6][255] = { TEXT("**Select Resolution**"), TEXT("1920 x 1080"), TEXT("2560 x 1440"), TEXT("3440 x 1440"), TEXT("3840 x 2160"), TEXT("4096 x 2160") };

	//Cabinet
	TCHAR cabinetManufacturerNameList[5][255] = { TEXT("**Select Manufacturer**"), TEXT("Antec"), TEXT("BitFenix"), TEXT("Corsair"), TEXT("Zebronics") };

	switch (iMsg) {

	
	case WM_INITDIALOG: {

		//add string items to CPU manufacturer list
		addItemsToCombobox(hDlg, ID_CPU_CB, cpuManufacturerNameList, 3);
		ghCPUManufacturerCB = GetDlgItem(hDlg, ID_CPU_CB);

		//add string items to CPU type list
		addItemsToCombobox(hDlg, ID_CPU_TYPE_CB, cpuTypeList, 3);
		ghCPUTypeCB = GetDlgItem(hDlg, ID_CPU_TYPE_CB);
		ghCPUModelCB = GetDlgItem(hDlg, ID_CPU_MODEL_CB);

		//add string items to RAM manufacturer list
		addItemsToCombobox(hDlg, ID_RAM_CB, ramManufacturerNameList, 3);
		ghRAMManufacturerCB = GetDlgItem(hDlg, ID_RAM_CB);

		//add string items to RAM size list
		addItemsToCombobox(hDlg, ID_RAM_SIZE_CB, ramSizeList, 5);
		ghRAMSizeCB = GetDlgItem(hDlg, ID_RAM_SIZE_CB);

		//add string items to Graphics Card manufacturer list
		addItemsToCombobox(hDlg, ID_GRAPHICSCARD_CB, graphicCardManufacturerNameList, 3);
		ghGraphicsCardManufacturerCB = GetDlgItem(hDlg, ID_GRAPHICSCARD_CB);
		ghGraphicsCardModelCB = GetDlgItem(hDlg, ID_GRAPHICSCARD_MODEL_CB);

		//add string items to Graphics Card size list
		addItemsToCombobox(hDlg, ID_GRAPHICSCARD_SIZE_CB, graphicCardSizeList, 5);
		ghGraphicsCardSizeCB = GetDlgItem(hDlg, ID_GRAPHICSCARD_SIZE_CB);

		//add string items to motherboard list
		addItemsToCombobox(hDlg, ID_MOTHERBOARD_CB, motherboardManufacturerNameList, 5);
		ghMotherboardCB = GetDlgItem(hDlg, ID_MOTHERBOARD_CB);
		ghMotherboardModelCB = GetDlgItem(hDlg, ID_MOTHERBOARD_MODEL_CB);

		//add string items to storage item list
		addItemsToCombobox(hDlg, ID_STORAGE_CB, storageManufacturerNameList, 5);
		ghStorageCB = GetDlgItem(hDlg, ID_STORAGE_CB);

		//add string items to storage size list
		addItemsToCombobox(hDlg, ID_STORAGE_SIZE_CB, storageSizeList, 5);
		ghStorageSizeCB = GetDlgItem(hDlg, ID_STORAGE_SIZE_CB);

		//add string items to keyboard list
		addItemsToCombobox(hDlg, ID_KEYBOARD_CB, keyboardManufacturerNameList, 6);
		ghKeyboardCB = GetDlgItem(hDlg, ID_KEYBOARD_CB);

		//add string items to mouse list
		addItemsToCombobox(hDlg, ID_MOUSE_CB, mouseManufacturerNameList, 6);
		ghMouseCB = GetDlgItem(hDlg, ID_MOUSE_CB);

		//add string items to monitor list
		addItemsToCombobox(hDlg, ID_MONITOR_CB, monitorManufacturerNameList, 6);
		ghMonitorCB = GetDlgItem(hDlg, ID_MONITOR_CB);

		//add string items to monitor resolution list
		addItemsToCombobox(hDlg, ID_MONITOR_RESOLUTION_CB, monitorResolutionList, 6);
		ghMonitorResolutionCB = GetDlgItem(hDlg, ID_MONITOR_RESOLUTION_CB);

		//add string items to cabinet list
		addItemsToCombobox(hDlg, ID_CABINET_CB, cabinetManufacturerNameList, 5);
		ghCabinetCB = GetDlgItem(hDlg, ID_CABINET_CB);

		//hide dependent CPU type list
		ShowWindow(ghCPUTypeCB, SW_HIDE);

		//hide CPU item list
		ShowWindow(ghCPUModelCB, SW_HIDE);

		//hide RAM size list
		ShowWindow(ghRAMSizeCB, SW_HIDE);

		//hide GPU item list
		ShowWindow(ghGraphicsCardModelCB, SW_HIDE);

		//hide Graphic Card size list
		ShowWindow(ghGraphicsCardSizeCB, SW_HIDE);

		//hide motherboard model list
		ShowWindow(ghMotherboardModelCB, SW_HIDE);

		//hide storage size list
		ShowWindow(ghStorageSizeCB, SW_HIDE);

		//hide monitor resolution list
		ShowWindow(ghMonitorResolutionCB, SW_HIDE);

		ghBitmapDlg = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(BACKGROUND), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		return TRUE;
	}
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		GetClientRect(hDlg, &rect);
		hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC, ghBitmapDlg);
		GetObject(ghBitmapDlg, sizeof(bmp), &bmp);
		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

		SetBkColor(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 30, 25, TEXT("CPU:"), 4);
		TextOut(hdc, 30, 90, TEXT("RAM:"), 4);
		TextOut(hdc, 30, 160, TEXT("GRAPHICS CARD:"), 14);
		TextOut(hdc, 30, 230, TEXT("MOTHERBOARD:"), 12);
		TextOut(hdc, 30, 300, TEXT("STORAGE:"), 8);
		TextOut(hdc, 30, 370, TEXT("KEYBOARD:"), 9);
		TextOut(hdc, 30, 445, TEXT("MOUSE:"), 6);
		TextOut(hdc, 30, 520, TEXT("MONITOR:"), 8);
		TextOut(hdc, 30, 585, TEXT("CABINET:"), 8);
		EndPaint(hDlg, &ps);
		DeleteDC(hMemDC);
		break;
	case WM_COMMAND: {

		switch (LOWORD(wParam)) {
			//Continue button click handling
		case IDCONTINUE:
			//save selected items
			getOrderDetails(hDlg);
			DestroyWindow(hDlg);
			ghModelessDlg = 0;
			gbDisplayReceipt = TRUE;
			InvalidateRect(gHwnd, NULL, TRUE);
			
			break;
		case IDCANCEL:
			DestroyWindow(hDlg);
			ghModelessDlg = 0;
			break;
		default:
			break;
		}
		switch (HIWORD(wParam)) {

		case CBN_SELCHANGE:
			switch (LOWORD(wParam)) {
				//CPU Manufacturer list handling
			case ID_CPU_CB: {

				//remove current contents from CPU model item list
				SendDlgItemMessage(hDlg, ID_CPU_MODEL_CB, CB_RESETCONTENT, 0, 0);

				//Hide model item lists
				ShowWindow(ghCPUModelCB, SW_HIDE);

				int selectedCPUManufacturerItemIndex = showHideDependentCombobox(hDlg, ID_CPU_CB, ID_CPU_TYPE_CB, ghCPUTypeCB);
				switch (selectedCPUManufacturerItemIndex) {
				case 1:
					addItemsToCombobox(hDlg, ID_CPU_MODEL_CB, intelItemList, 4);
					break;
				case 2:
					addItemsToCombobox(hDlg, ID_CPU_MODEL_CB, amdItemList, 4);
					break;
				default:
					break;
				}
			}
							break;

							//CPU Type list handling
			case ID_CPU_TYPE_CB:
				showHideDependentCombobox(hDlg, ID_CPU_TYPE_CB, ID_CPU_MODEL_CB, ghCPUModelCB);
				break;

				//RAM Manufacturer list handling
			case ID_RAM_CB:
				//get selected index of RAM Manufacturer list. If 0 then hide RAM size list else show RAM size list
				showHideDependentCombobox(hDlg, ID_RAM_CB, ID_RAM_SIZE_CB, ghRAMSizeCB);
				break;

				//Graphics card manufacturer list handling
			case ID_GRAPHICSCARD_CB: {
				//hide GPU size list 
				ShowWindow(ghGraphicsCardSizeCB, SW_HIDE);
				//get selected GPU manufacturer item index
				int selectedGraphicsCardItemIndex = showHideDependentCombobox(hDlg, ID_GRAPHICSCARD_CB, ID_GRAPHICSCARD_MODEL_CB, ghGraphicsCardModelCB);
				//remove all contents of GPU model item list
				SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_MODEL_CB, CB_RESETCONTENT, 0, 0);
				switch (selectedGraphicsCardItemIndex) {
				case 1:
					addItemsToCombobox(hDlg, ID_GRAPHICSCARD_MODEL_CB, amdGPUItemList, 5);
					break;
				case 2:
					addItemsToCombobox(hDlg, ID_GRAPHICSCARD_MODEL_CB, nvidiaItemList, 5);
					break;
				default:
					break;
				}
			}
									 break;

									 //GPU model list handling
			case ID_GRAPHICSCARD_MODEL_CB:
				showHideDependentCombobox(hDlg, ID_GRAPHICSCARD_MODEL_CB, ID_GRAPHICSCARD_SIZE_CB, ghGraphicsCardSizeCB);
				break;

				//Motherboard list handling
			case ID_MOTHERBOARD_CB: {
				//get selected motherboard item index
				int selectedMotherboardItemIndex = showHideDependentCombobox(hDlg, ID_MOTHERBOARD_CB, ID_MOTHERBOARD_MODEL_CB, ghMotherboardModelCB);
				//remove all current contents from model list
				SendDlgItemMessage(hDlg, ID_MOTHERBOARD_MODEL_CB, CB_RESETCONTENT, 0, 0);
				switch (selectedMotherboardItemIndex) {
				case 1:
					//intel items
					addItemsToCombobox(hDlg, ID_MOTHERBOARD_MODEL_CB, intelMotherboardItemList, 6);
					break;
				case 2:
					//MSI items
					addItemsToCombobox(hDlg, ID_MOTHERBOARD_MODEL_CB, msiMotherboardItemList, 7);
					break;
				case 3:
					//Gigabyte items
					addItemsToCombobox(hDlg, ID_MOTHERBOARD_MODEL_CB, gigabyteMotherboardItemList, 4);
					break;
				case 4:
					//ASUS items
					addItemsToCombobox(hDlg, ID_MOTHERBOARD_MODEL_CB, asusMotherboardItemList, 6);
					break;
				}
			}
									break;

									//storage list handling
			case ID_STORAGE_CB:
				showHideDependentCombobox(hDlg, ID_STORAGE_CB, ID_STORAGE_SIZE_CB, ghStorageSizeCB);
				break;

				//monitor list handling
			case ID_MONITOR_CB:
				showHideDependentCombobox(hDlg, ID_MONITOR_CB, ID_MONITOR_RESOLUTION_CB, ghMonitorResolutionCB);
				break;

			default:
				break;
			}
		default:
			break;
		}
		return TRUE;
	}
	case WM_CLOSE:
		DestroyWindow(hDlg);
		ghModelessDlg = 0;
		break;
	default:
		break;
	}
	return FALSE;
}

void displayOrderDetails() {
	
}

void addItemsToCombobox(HWND hDlg, int comboboxID, TCHAR itemList[10][255], int itemListSize) {
	for (int i = 0; i < itemListSize; i++) {
		SendDlgItemMessage(hDlg, comboboxID, CB_ADDSTRING, (WPARAM)0, (LPARAM)itemList[i]);
	}
	//set first entry as default selected
	SendDlgItemMessage(hDlg, comboboxID, CB_SETCURSEL, 0, 0);
}

int showHideDependentCombobox(HWND hDlg, int controllingCBID, int dependentCBID, HWND hDependentCB) {

	//reset selection in dependent list
	SendDlgItemMessage(hDlg, dependentCBID, CB_SETCURSEL, 0, 0);
	//get selected index of controlling list. If 0 then hide dependent list else show dependent list
	int selectedItemIndex = SendDlgItemMessage(hDlg, controllingCBID, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex == 0)
		ShowWindow(hDependentCB, SW_HIDE);
	else
		ShowWindow(hDependentCB, SW_SHOW);
	return selectedItemIndex;
}

void getOrderDetails(HWND hDlg) {
	
	TCHAR str[255];
	int cpuPriceList[4] = { 0, 10648, 17398, 27098 };
	int ramPriceList[5] = { 0, 1000, 1800, 3600, 7200 };
	int gpuPriceList[5] = { 0, 8000, 14000, 20000, 26000 };
	int motherboardPriceList[7] = { 0, 13000, 9000, 15000, 12000, 13500, 14500 };
	int storagePriceList[5] = { 0, 2000, 3000, 4000, 6000 };
	int keyboardPriceList[6] = { 0, 800, 1500, 1100, 1350, 1200 };
	int mousePriceList[6] = { 0, 450, 200, 800, 1200, 1000 };
	int monitorPriceList[6] = { 0, 15000, 20000, 26000, 35000, 45000 };
	int cabinetPriceList[5] = { 0, 3500, 12950, 7200, 1300 };

	//get cpu details
	_tcscpy_s(cpuDetails[0].name, TEXT("1) CPU"));
	cpuDetails[0].xCord = 25;
	cpuDetails[0].yCord = 90;
	int selectedItemIndex = SendDlgItemMessage(hDlg, ID_CPU_MODEL_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get model
		SendDlgItemMessage(hDlg, ID_CPU_MODEL_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(cpuDetails[3].name, str);
		cpuDetails[3].xCord = 625;
		cpuDetails[3].yCord = 90;
		giOrderAmountTotal += cpuPriceList[selectedItemIndex];

		wsprintf(str, TEXT("Rs %d"), cpuPriceList[selectedItemIndex]);
		_tcscpy_s(cpuDetails[6].name, str);
		cpuDetails[6].xCord = 1225;
		cpuDetails[6].yCord = 90;

		//get selected cpu manufacturer 
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_CPU_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_CPU_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(cpuDetails[1].name, str);
		cpuDetails[1].xCord = 225;
		cpuDetails[1].yCord = 90;
		//get selected cpu type
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_CPU_TYPE_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_CPU_TYPE_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(cpuDetails[2].name, str);
		cpuDetails[2].xCord = 425;
		cpuDetails[2].yCord = 90;


		_tcscpy_s(cpuDetails[4].name, TEXT("-"));
		cpuDetails[4].xCord = 825;
		cpuDetails[4].yCord = 90;

		_tcscpy_s(cpuDetails[5].name, TEXT("-"));
		cpuDetails[5].xCord = 1025;
		cpuDetails[5].yCord = 90;
	}

	//get RAM details
	_tcscpy_s(ramDetails[0].name, TEXT("2) RAM"));
	ramDetails[0].xCord = 25;
	ramDetails[0].yCord = 120;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_RAM_SIZE_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {

	
		//get price
		giOrderAmountTotal += ramPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), ramPriceList[selectedItemIndex]);
		_tcscpy_s(ramDetails[6].name, str);
		ramDetails[6].xCord = 1225;
		ramDetails[6].yCord = 120;

		//get size details
		SendDlgItemMessage(hDlg, ID_RAM_SIZE_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(ramDetails[4].name, str);
		ramDetails[4].xCord = 825;
		ramDetails[4].yCord = 120;


		//get manufacturer
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_RAM_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_RAM_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(ramDetails[1].name, str);
		ramDetails[1].xCord = 225;
		ramDetails[1].yCord = 120;

		_tcscpy_s(ramDetails[2].name, TEXT("-"));
		ramDetails[2].xCord = 425;
		ramDetails[2].yCord = 120;

		_tcscpy_s(ramDetails[3].name, TEXT("-"));
		ramDetails[3].xCord = 625;
		ramDetails[3].yCord = 120;

		_tcscpy_s(ramDetails[5].name, TEXT("-"));
		ramDetails[5].xCord = 1025;
		ramDetails[5].yCord = 120;

	}

	//get GPU details
	_tcscpy_s(gpuDetails[0].name, TEXT("2) GRPHICS CARD"));
	gpuDetails[0].xCord = 25;
	gpuDetails[0].yCord = 150;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_SIZE_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get size details
		SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_SIZE_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(gpuDetails[4].name, str);
		gpuDetails[4].xCord = 825;
		gpuDetails[4].yCord = 150;

		//get price
		giOrderAmountTotal += gpuPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), gpuPriceList[selectedItemIndex]);
		_tcscpy_s(gpuDetails[6].name, str);
		gpuDetails[6].xCord = 1225;
		gpuDetails[6].yCord = 150;

		//get manufacturer
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(gpuDetails[1].name, str);
		gpuDetails[1].xCord = 225;
		gpuDetails[1].yCord = 150;

		//get model
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_MODEL_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_GRAPHICSCARD_MODEL_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(gpuDetails[3].name, str);
		gpuDetails[3].xCord = 625;
		gpuDetails[3].yCord = 150;

		_tcscpy_s(gpuDetails[2].name, TEXT("-"));
		gpuDetails[2].xCord = 425;
		gpuDetails[2].yCord = 150;

		_tcscpy_s(gpuDetails[4].name, TEXT("-"));
		gpuDetails[4].xCord = 825;
		gpuDetails[4].yCord = 150;

		_tcscpy_s(gpuDetails[5].name, TEXT("-"));
		gpuDetails[5].xCord = 1025;
		gpuDetails[5].yCord = 150;
	}

	//get motherboard details
	_tcscpy_s(motherboardDetails[0].name, TEXT("4) MOTHERBOARD"));
	motherboardDetails[0].xCord = 25;
	motherboardDetails[0].yCord = 180;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_MOTHERBOARD_MODEL_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get model details
		SendDlgItemMessage(hDlg, ID_MOTHERBOARD_MODEL_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(motherboardDetails[3].name, str);
		motherboardDetails[3].xCord = 625;
		motherboardDetails[3].yCord = 180;

		//get price
		giOrderAmountTotal += motherboardPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), motherboardPriceList[selectedItemIndex]);
		_tcscpy_s(motherboardDetails[6].name, str);
		motherboardDetails[6].xCord = 1225;
		motherboardDetails[6].yCord = 180;


		//get manufacturer
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_MOTHERBOARD_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_MOTHERBOARD_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(motherboardDetails[1].name, str);
		motherboardDetails[1].xCord = 225;
		motherboardDetails[1].yCord = 180;

		_tcscpy_s(motherboardDetails[2].name, TEXT("-"));
		motherboardDetails[2].xCord = 425;
		motherboardDetails[2].yCord = 180;

		_tcscpy_s(motherboardDetails[4].name, TEXT("-"));
		motherboardDetails[4].xCord = 825;
		motherboardDetails[4].yCord = 180;

		_tcscpy_s(motherboardDetails[5].name, TEXT("-"));
		motherboardDetails[5].xCord = 1025;
		motherboardDetails[5].yCord = 180;
	}

	//get storage details
	_tcscpy_s(storageDetails[0].name, TEXT("5) STORAGE"));
	storageDetails[0].xCord = 25;
	storageDetails[0].yCord = 210;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_STORAGE_SIZE_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get size details
		SendDlgItemMessage(hDlg, ID_STORAGE_SIZE_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(storageDetails[4].name, str);
		storageDetails[4].xCord = 825;
		storageDetails[4].yCord = 210;

		//get price
		giOrderAmountTotal += motherboardPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), motherboardPriceList[selectedItemIndex]);
		_tcscpy_s(storageDetails[6].name, str);
		storageDetails[6].xCord = 1225;
		storageDetails[6].yCord = 210;

		//get manufacturer
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_STORAGE_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_STORAGE_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(storageDetails[1].name, str);
		storageDetails[1].xCord = 225;
		storageDetails[1].yCord = 210;

		_tcscpy_s(storageDetails[2].name, TEXT("-"));
		storageDetails[2].xCord = 425;
		storageDetails[2].yCord = 210;

		_tcscpy_s(storageDetails[3].name, TEXT("-"));
		storageDetails[3].xCord = 625;
		storageDetails[3].yCord = 210;

		_tcscpy_s(storageDetails[5].name, TEXT("-"));
		storageDetails[5].xCord = 1025;
		storageDetails[5].yCord = 210;
	}

	//get keyboard details
	_tcscpy_s(keyboardDetails[0].name, TEXT("6) KEYBOARD"));
	keyboardDetails[0].xCord = 25;
	keyboardDetails[0].yCord = 240;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_KEYBOARD_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get manufacturer details
		SendDlgItemMessage(hDlg, ID_KEYBOARD_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(keyboardDetails[1].name, str);
		keyboardDetails[1].xCord = 225;
		keyboardDetails[1].yCord = 240;

		//get price
		giOrderAmountTotal += keyboardPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), keyboardPriceList[selectedItemIndex]);
		_tcscpy_s(keyboardDetails[6].name, str);
		keyboardDetails[6].xCord = 1225;
		keyboardDetails[6].yCord = 240;

		_tcscpy_s(keyboardDetails[2].name, TEXT("-"));
		keyboardDetails[2].xCord = 425;
		keyboardDetails[2].yCord = 240;

		_tcscpy_s(keyboardDetails[3].name, TEXT("-"));
		keyboardDetails[3].xCord = 625;
		keyboardDetails[3].yCord = 240;

		_tcscpy_s(keyboardDetails[4].name, TEXT("-"));
		keyboardDetails[4].xCord = 825;
		keyboardDetails[4].yCord = 240;

		_tcscpy_s(keyboardDetails[5].name, TEXT("-"));
		keyboardDetails[5].xCord = 1025;
		keyboardDetails[5].yCord = 240;
	}

	//get mouse details
	_tcscpy_s(mouseDetalis[0].name, TEXT("7) MOUSE"));
	mouseDetalis[0].xCord = 25;
	mouseDetalis[0].yCord = 270;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_MOUSE_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get manufacturer details
		SendDlgItemMessage(hDlg, ID_MOUSE_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(mouseDetalis[1].name, str);
		mouseDetalis[1].xCord = 225;
		mouseDetalis[1].yCord = 270;

		//get price
		giOrderAmountTotal += mousePriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), mousePriceList[selectedItemIndex]);
		_tcscpy_s(mouseDetalis[6].name, str);
		mouseDetalis[6].xCord = 1225;
		mouseDetalis[6].yCord = 270;

		_tcscpy_s(mouseDetalis[2].name, TEXT("-"));
		mouseDetalis[2].xCord = 425;
		mouseDetalis[2].yCord = 270;

		_tcscpy_s(mouseDetalis[3].name, TEXT("-"));
		mouseDetalis[3].xCord = 625;
		mouseDetalis[3].yCord = 270;

		_tcscpy_s(mouseDetalis[4].name, TEXT("-"));
		mouseDetalis[4].xCord = 825;
		mouseDetalis[4].yCord = 270;

		_tcscpy_s(mouseDetalis[5].name, TEXT("-"));
		mouseDetalis[5].xCord = 1025;
		mouseDetalis[5].yCord = 270;
	}

	//get monitor details
	_tcscpy_s(monitorDetails[0].name, TEXT("8) MONITOR"));
	monitorDetails[0].xCord = 25;
	monitorDetails[0].yCord = 300;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_MONITOR_RESOLUTION_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get resolution details
		SendDlgItemMessage(hDlg, ID_MONITOR_RESOLUTION_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(monitorDetails[5].name, str);
		monitorDetails[5].xCord = 1025;
		monitorDetails[5].yCord = 300;
		
		//get price
		giOrderAmountTotal += monitorPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), monitorPriceList[selectedItemIndex]);
		_tcscpy_s(monitorDetails[6].name, str);
		monitorDetails[6].xCord = 1225;
		monitorDetails[6].yCord = 300;

		//get manufacturer
		selectedItemIndex = SendDlgItemMessage(hDlg, ID_MONITOR_CB, CB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hDlg, ID_MONITOR_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(monitorDetails[1].name, str);
		monitorDetails[1].xCord = 225;
		monitorDetails[1].yCord = 300;

		_tcscpy_s(monitorDetails[2].name, TEXT("-"));
		monitorDetails[2].xCord = 425;
		monitorDetails[2].yCord = 300;

		_tcscpy_s(monitorDetails[3].name, TEXT("-"));
		monitorDetails[3].xCord = 625;
		monitorDetails[3].yCord = 300;

		_tcscpy_s(monitorDetails[4].name, TEXT("-"));
		monitorDetails[4].xCord = 825;
		monitorDetails[4].yCord = 300;
	}

	//get cabinet details
	_tcscpy_s(cabinetDetails[0].name, TEXT("9) CABINET"));
	cabinetDetails[0].xCord = 25;
	cabinetDetails[0].yCord = 330;
	selectedItemIndex = SendDlgItemMessage(hDlg, ID_CABINET_CB, CB_GETCURSEL, 0, 0);
	if (selectedItemIndex > 0) {
		//get manufacturer details
		SendDlgItemMessage(hDlg, ID_CABINET_CB, CB_GETLBTEXT, selectedItemIndex, (LPARAM)str);
		_tcscpy_s(cabinetDetails[1].name, str);
		cabinetDetails[1].xCord = 225;
		cabinetDetails[1].yCord = 330;

		//get price
		giOrderAmountTotal += cabinetPriceList[selectedItemIndex];
		wsprintf(str, TEXT("Rs %d"), cabinetPriceList[selectedItemIndex]);
		_tcscpy_s(cabinetDetails[6].name, str);
		cabinetDetails[6].xCord = 1225;
		cabinetDetails[6].yCord = 330;

		_tcscpy_s(cabinetDetails[2].name, TEXT("-"));
		cabinetDetails[2].xCord = 425;
		cabinetDetails[2].yCord = 330;

		_tcscpy_s(cabinetDetails[3].name, TEXT("-"));
		cabinetDetails[3].xCord = 625;
		cabinetDetails[3].yCord = 330;

		_tcscpy_s(cabinetDetails[4].name, TEXT("-"));
		cabinetDetails[4].xCord = 825;
		cabinetDetails[4].yCord = 330;

		_tcscpy_s(cabinetDetails[5].name, TEXT("-"));
		cabinetDetails[5].xCord = 1025;
		cabinetDetails[5].yCord = 330;
	}

}

void displayReceipt(HWND hwnd) {
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[255];
	int count = 0;

	
	struct WordText header[] = {
		{ TEXT("DEVICE"), 25 },
		{ TEXT("MANUFACTURER"), 225 },
		{ TEXT("TYPE"), 425 },
		{ TEXT("MODEL"), 625 },
		{ TEXT("SIZE"), 825 },
		{ TEXT("RESOLUTION"), 1025 },
		{ TEXT("PRICE"), 1225 }
	};
	
	
	hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rect);
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hdc, hPen);
	MoveToEx(hdc, 20, 30, NULL);
	LineTo(hdc, 20, rect.bottom - 260);

	MoveToEx(hdc, 220, 30, NULL);
	LineTo(hdc, 220, rect.bottom - 300);

	MoveToEx(hdc, 420, 30, NULL);
	LineTo(hdc, 420, rect.bottom - 300);

	MoveToEx(hdc, 620, 30, NULL);
	LineTo(hdc, 620, rect.bottom - 300);

	MoveToEx(hdc, 820, 30, NULL);
	LineTo(hdc, 820, rect.bottom - 300);

	MoveToEx(hdc, 1020, 30, NULL);
	LineTo(hdc, 1020, rect.bottom - 300);

	MoveToEx(hdc, 1220, 30, NULL);
	LineTo(hdc, 1220, rect.bottom - 300);

	MoveToEx(hdc, 1350, 30, NULL);
	LineTo(hdc, 1350, rect.bottom - 260);

	MoveToEx(hdc, 20, 30, NULL);
	LineTo(hdc, rect.right - 10, 30);

	MoveToEx(hdc, 20, 80, NULL);
	LineTo(hdc, rect.right - 10, 80);

	MoveToEx(hdc, 20, rect.bottom - 300, NULL);
	LineTo(hdc, rect.right - 10, rect.bottom - 300);

	MoveToEx(hdc, 20, rect.bottom - 260, NULL);
	LineTo(hdc, rect.right - 10, rect.bottom - 260);

	SetBkColor(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 0));

	TCHAR recieptMessage[255];
	if(gbReceiptGenerated)
		wsprintf(recieptMessage, TEXT("### Invoice created in the current directory. You may now close the window."));
	else
		wsprintf(recieptMessage, TEXT("### Press R to print the receipt."));
	SetBkColor(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 0));
	TextOut(hdc, 30, rect.bottom - 50, recieptMessage, wcslen(recieptMessage));

	wsprintf(str, TEXT("Order Total Rs: %d"), giOrderAmountTotal);
	TextOut(hdc, 1200, rect.bottom - 290, str, wcslen(str));

	for (count = 0; count < 7; count++) {
		// Table Header 
		TextOut(hdc, header[count].xCord, 50, header[count].name, wcslen(header[count].name));
	}

	SetBkColor(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 255, 0));

	for (count = 0; count < 7; count++) {
		TextOut(hdc, cpuDetails[count].xCord, cpuDetails[count].yCord, cpuDetails[count].name, _tcslen(cpuDetails[count].name));
		TextOut(hdc, ramDetails[count].xCord, ramDetails[count].yCord, ramDetails[count].name, _tcslen(ramDetails[count].name));
		TextOut(hdc, gpuDetails[count].xCord, gpuDetails[count].yCord, gpuDetails[count].name, _tcslen(gpuDetails[count].name));
		TextOut(hdc, motherboardDetails[count].xCord, motherboardDetails[count].yCord, motherboardDetails[count].name, _tcslen(motherboardDetails[count].name));
		TextOut(hdc, storageDetails[count].xCord, storageDetails[count].yCord, storageDetails[count].name, _tcslen(storageDetails[count].name));
		TextOut(hdc, keyboardDetails[count].xCord, keyboardDetails[count].yCord, keyboardDetails[count].name, _tcslen(keyboardDetails[count].name));
		TextOut(hdc, mouseDetalis[count].xCord, mouseDetalis[count].yCord, mouseDetalis[count].name, _tcslen(mouseDetalis[count].name));
		TextOut(hdc, monitorDetails[count].xCord, monitorDetails[count].yCord, monitorDetails[count].name, _tcslen(monitorDetails[count].name));
		TextOut(hdc, cabinetDetails[count].xCord, cabinetDetails[count].yCord, cabinetDetails[count].name, _tcslen(cabinetDetails[count].name));

	}

	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_PEN)));

	EndPaint(hwnd, &ps);
}