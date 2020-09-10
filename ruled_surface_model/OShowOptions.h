#ifndef _OShowOptions_h_
#define _OShowOptions_h_

#include <QtWidgets>

class OShowOptions :
	public QWidget
{
    Q_OBJECT
public:
	OShowOptions();
	~OShowOptions();

    QCheckBox* 	pchkRuledSurface;
signals:
    void slotCloseEvent();
protected:
	virtual void closeEvent(QCloseEvent * event);
};

#endif  //_OShowOptions_h_
