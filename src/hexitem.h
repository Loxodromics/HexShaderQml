//
//  hexitem.h
//  HexShader
//
//  Created by philipp on 09.02.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//

#pragma once
#include <qsgmaterial.h>
#include <qsgnode.h>

#include <qquickitem.h>

#include <qsgsimplematerial.h>
#include <qsgsimplerectnode.h>

namespace LFD {

struct State {
	QColor color;
	float time;

	int compare(const State* other) const
	{
		uint rgb = color.rgba();
		uint otherRgb = other->color.rgba();

		if (rgb == otherRgb) {
			return 0;
		}
		else if (rgb < otherRgb) {
			return -1;
		}
		else {
			return 1;
		}
	}
};

///
/// ##### HexMaterialShader #####
///

class HexMaterialShader : public QSGSimpleMaterialShader<State> {
public:
	explicit HexMaterialShader();

	QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(HexMaterialShader, State);

public:
	const char* vertexShader() const override;
	const char* fragmentShader() const override;
	QList<QByteArray> attributes() const override;
	void updateState(const State* state, const State*) override;
	void resolveUniforms() override;

private:
	int id_color;
	int id_time;
};

///
/// ##### HexNode #####
///

class HexNode : public QSGGeometryNode {
public:
	explicit HexNode();

	QSGGeometry m_geometry;
};

///
/// ##### HexItem #####
///

class HexItem : public QQuickItem {
	Q_OBJECT

	Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
	explicit HexItem();

	void setColor(const QColor& color);
	QColor color() const { return m_color; }

signals:
	void colorChanged();

private:
	QColor m_color;
	qreal m_time;

public:
	QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData*) override;
};

}
