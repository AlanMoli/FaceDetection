#pragma once

#include <QFile>
#include <QApplication>

class SetStyle
{
public:
	static void setStyle(const QString &style)
	{
		QFile qss(style);
		qss.open(QFile::ReadOnly);
		qApp->setStyleSheet(qss.readAll());
		qss.close();
	}
};