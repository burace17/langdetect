#pragma once 
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "langdetect.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		explicit MainWindow(QWidget* parent = 0);
		~MainWindow();
	private slots:
		void on_btnDetectLanguage_clicked();
		void on_btnSelectDirectory_clicked();
	private:
		Ui::MainWindow* ui;
};
void qt_display_dialog(const char* output);
