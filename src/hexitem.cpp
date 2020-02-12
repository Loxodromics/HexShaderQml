//
//  hexitemcpp
//  HexShader
//
//  Created by philipp on 09.02.2020.
//  Copyright (c) 2020 Philipp Engelhard. All rights reserved.
//
#include "hexitem.h"

namespace LFD {

HexMaterialShader::HexMaterialShader() {}

const char* HexMaterialShader::vertexShader() const
{
	return "attribute highp vec4 aVertex;                              \n"
		   "attribute highp vec2 aTexCoord;                            \n"
		   "uniform highp mat4 qt_Matrix;                              \n"
		   "varying highp vec2 texCoord;                               \n"
		   "void main() {                                              \n"
		   "    gl_Position = qt_Matrix * aVertex;                     \n"
		   "    texCoord = aTexCoord;                                  \n"
		   "}";
}

const char* HexMaterialShader::fragmentShader() const
{
	return "uniform lowp float qt_Opacity;                             \n"
		   "uniform lowp vec4 color;                                   \n"
		   "uniform highp float time;                                  \n"
		   "varying highp vec2 texCoord;                               \n"
		   "void main ()                                               \n"
		   "{                                                          \n"
			"float mo = sin(time);"
		   "    gl_FragColor = mo * texCoord.x * color * qt_Opacity;  \n"
		   "    gl_FragColor = vec4(mo) * qt_Opacity;  \n"
		   "}";
}

QList<QByteArray> HexMaterialShader::attributes() const
{
	return QList<QByteArray>() << "aVertex"
							   << "aTexCoord";
}

void HexMaterialShader::updateState(const State* state, const State*)
{
	program()->setUniformValue(id_color, state->color);
	program()->setUniformValue(id_time, state->time);
//	program()->setUniformValue(id_time, (GLfloat)static_cast<QSGSimpleMaterial<dwFxWaterMaterialState>*>(newMaterial)->state()->time);
}

void HexMaterialShader::resolveUniforms()
{
	id_color = program()->uniformLocation("color");
	id_time = program()->uniformLocation("time");
}

HexNode::HexNode()
	: m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
{
	setGeometry(&m_geometry);

	QSGSimpleMaterial<State>* material = HexMaterialShader::createMaterial();
	material->setFlag(QSGMaterial::Blending);
	setMaterial(material);
	setFlag(OwnsMaterial);
}

HexItem::HexItem() { setFlag(ItemHasContents, true); }

void HexItem::setColor(const QColor& color)
{
	if (m_color != color) {
		m_color = color;
		emit colorChanged();
		update();
	}
}

QSGNode* HexItem::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*)
{
	HexNode* n = static_cast<HexNode*>(node);
	if (!node)
		n = new HexNode();

	QSGGeometry::updateTexturedRectGeometry(n->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
	static_cast<QSGSimpleMaterial<State>*>(n->material())->state()->color = m_color;
	static_cast<QSGSimpleMaterial<State>*>(n->material())->state()->time = m_time;

	n->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

	return n;
}

}
