#include "qt/qt_gui.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) 
{
	ui->setupUi(this);	
}

MainWindow::~MainWindow()
{
	cleanup();
	delete ui;
}

void MainWindow::on_btnDetectLanguage_clicked()
{
	QString text = ui->textEdit->toPlainText();
	QByteArray ba = text.toUtf8();
	char* utf8text = ba.data();

	detect_language(utf8text);
}

void MainWindow::on_btnSelectDirectory_clicked()
{
 	QFileDialog dialog(this);
	QStringList fileNames;
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	if (dialog.exec())
		fileNames = dialog.selectedFiles();

	QString qs = fileNames.at(0);
	
	// Convert the QString to a UTF8 C String
	QByteArray ba = qs.toUtf8();
	char* path = ba.data();

	initialize(path);
}

void qt_display_dialog(const char* output) 
{
	QMessageBox msg;
	QString str = QString::fromUtf8(output);
	msg.setText(str);
	msg.exec();
}
