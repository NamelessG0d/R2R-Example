#include <windows.h>
#include<stdio.h>

enum ConsoleColor
{
	//
	// Summary:
	//     The color black.
	Black = 0,
	//
	// Summary:
	//     The color dark blue.
	DarkBlue = 1,
	//
	// Summary:
	//     The color dark green.
	DarkGreen = 2,
	//
	// Summary:
	//     The color dark cyan (dark blue-green).
	DarkCyan = 3,
	//
	// Summary:
	//     The color dark red.
	DarkRed = 4,
	//
	// Summary:
	//     The color dark magenta (dark purplish-red).
	DarkMagenta = 5,
	//
	// Summary:
	//     The color dark yellow (ochre).
	DarkYellow = 6,
	//
	// Summary:
	//     The color gray.
	Gray = 7,
	//
	// Summary:
	//     The color dark gray.
	DarkGray = 8,
	//
	// Summary:
	//     The color blue.
	Blue = 9,
	//
	// Summary:
	//     The color green.
	Green = 10,
	//
	// Summary:
	//     The color cyan (blue-green).
	Cyan = 11,
	//
	// Summary:
	//     The color red.
	Red = 12,
	//
	// Summary:
	//     The color magenta (purplish-red).
	Magenta = 13,
	//
	// Summary:
	//     The color yellow.
	Yellow = 14,
	//
	// Summary:
	//     The color white.
	White = 15
};

void TestCallback(const char* message) {
	printf("Message from Managed : %s\n", message);
}

typedef void (*WriteLineADef)(const char* message);
typedef void (*WriteLineWDef)(const wchar_t* message);
typedef void (*SetForegroundColorDef)(ConsoleColor color);
typedef void (*SetBackgroundColorDef)(ConsoleColor color);
typedef void (*ClearConsoleDef)();
typedef void (*ReadLineWDef)(const wchar_t* buffer, unsigned long long bufferSize);
typedef void (*SetMessageCallbackADef)(decltype(TestCallback));
typedef void (*CallMessageCallbackDef)();

#define Import(handle, name) name##Def name = (name##Def)GetProcAddress(handle, #name);

int main()
{
	printf("======================\n");
	printf("= ReadyToRun Example =\n");
	printf("======================\n\n");

	HINSTANCE handle = LoadLibrary("net8.0-windows\\publish\\win-x64\\R2R-Example.Lib.dll");
	if (handle == nullptr) {
		fprintf(stderr, "Couldn't find ReadyToRun library! Did you publish it?");
		return ERROR_FILE_NOT_FOUND;
	}

	Import(handle, WriteLineA);
	Import(handle, WriteLineW);
	Import(handle, SetForegroundColor);
	Import(handle, SetBackgroundColor);
	Import(handle, ClearConsole);
	Import(handle, ReadLineW);
	Import(handle, SetMessageCallbackA);
	Import(handle, CallMessageCallback);
	
	printf("Let's try to clear the console!\n");
	system("pause");
	ClearConsole();
	printf("Console cleared!\n");

	WriteLineA("Hello from a C++ char pointer");
	WriteLineW(L"Hello from a C++ wchar_t pointer");

	wchar_t buffer[1024] = {};
	printf("Enter text : ");
	ReadLineW(buffer, sizeof(buffer));
	printf("This was given back and is printed from unamanged : %ws\n", buffer);

	printf("Setting the text color to green!\n");
	SetForegroundColor(ConsoleColor::Green);

	printf("Reverting the text color and setting the background color to blue!\n");
	SetBackgroundColor(ConsoleColor::DarkBlue);
	SetForegroundColor(ConsoleColor::White);

	printf("Amazing right?\n");
	SetBackgroundColor(ConsoleColor::Black);

	printf("Passing our callback...\n");
	SetMessageCallbackA(&TestCallback);

	printf("Invoking our callback...\n");
	CallMessageCallback();

	printf("Example completed!\n");

	FreeLibrary(handle);
	return ERROR_SUCCESS;
}
