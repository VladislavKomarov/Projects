#include "OShowOptions.h"


OShowOptions::OShowOptions()
{
    pchkRuledSurface = new QCheckBox(QString::fromLocal8Bit("���������� �����������"));

	QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pchkRuledSurface);

	setLayout(pvbxLayout);
}


OShowOptions::~OShowOptions()
{
}

void OShowOptions::closeEvent(QCloseEvent *event)
{
	QWidget::closeEvent(event);
    emit slotCloseEvent();
}
