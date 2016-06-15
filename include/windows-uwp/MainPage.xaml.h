//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once
#include "MainPage.g.h"
#include "langdetect.h"
#include "str-utils.h"
#include <ppltasks.h>
namespace langdetect
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void selectStopFilesBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void detectLanguageBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
