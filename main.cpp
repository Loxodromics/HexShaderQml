//
//  main.cpp
//  HexShader
//
//  Created by philipp on 09.02.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/hexitem.h"

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	qmlRegisterType<LFD::HexItem>("HexItem", 1, 0, "HexItem");

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(
	  &engine, &QQmlApplicationEngine::objectCreated, &app,
	  [url](QObject* obj, const QUrl& objUrl) {
		  if (!obj && url == objUrl)
			  QCoreApplication::exit(-1);
	  },
	  Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
