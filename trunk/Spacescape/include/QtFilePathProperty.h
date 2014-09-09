//
//  QtFilePathManager.h
//  Spacescape
//
//  Created by Alex on 9/1/14.
//
//

#ifndef __Spacescape__QtFilePathManager__
#define __Spacescape__QtFilePathManager__

#define QT_QTPROPERTYBROWSER_IMPORT

#include <qtpropertybrowser.h>
#include <QWidget>
#include <qlineedit.h>

QT_BEGIN_NAMESPACE

class QtFileEdit : public QWidget
{
	Q_OBJECT
public:
     QtFileEdit(QWidget *parent = 0);
    
     void setFilePath(const QString &filePath) { if (theLineEdit->text() != filePath) theLineEdit->setText(filePath); }
     QString filePath() const { return theLineEdit->text(); }
     void setFilter(const QString &filter) { theFilter = filter; }
     QString filter() const { return theFilter; }
signals:
     void filePathChanged(const QString &filePath);
protected:
     void focusInEvent(QFocusEvent *e);
     void focusOutEvent(QFocusEvent *e);
     void keyPressEvent(QKeyEvent *e);
     void keyReleaseEvent(QKeyEvent *e);
 private slots:
     void buttonClicked();
 private:
     QLineEdit *theLineEdit;
     QString theFilter;
};

class QtFilePathManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    QtFilePathManager(QObject *parent = 0)
    : QtAbstractPropertyManager(parent)
    { }
    
    QString value(const QtProperty *property) const;
    QString filter(const QtProperty *property) const;
    
    public slots:
    void setValue(QtProperty *property, const QString &val);
    void setFilter(QtProperty *property, const QString &fil);
signals:
    void valueChanged(QtProperty *property, const QString &val);
    void filterChanged(QtProperty *property, const QString &fil);
protected:
    virtual QString valueText(const QtProperty *property) const { return value(property); }
    virtual void initializeProperty(QtProperty *property) { theValues[property] = Data(); }
    virtual void uninitializeProperty(QtProperty *property) { theValues.remove(property); }
private:
    
    struct Data
    {
        QString value;
        QString filter;
    };
    
    QMap<const QtProperty *, Data> theValues;
};

class QtFileEditFactory : public QtAbstractEditorFactory<QtFilePathManager>
{
    Q_OBJECT
public:
    QtFileEditFactory(QObject *parent = 0)
    : QtAbstractEditorFactory<QtFilePathManager>(parent)
    { }
    virtual ~QtFileEditFactory();
protected:
    virtual void connectPropertyManager(QtFilePathManager *manager);
    virtual QWidget *createEditor(QtFilePathManager *manager, QtProperty *property,
                                  QWidget *parent);
    virtual void disconnectPropertyManager(QtFilePathManager *manager);
    private slots:
    void slotPropertyChanged(QtProperty *property, const QString &value);
    void slotFilterChanged(QtProperty *property, const QString &filter);
    void slotSetValue(const QString &value);
    void slotEditorDestroyed(QObject *object);
private:
    QMap<QtProperty *, QList<QtFileEdit *> > theCreatedEditors;
    QMap<QtFileEdit *, QtProperty *> theEditorToProperty;
};


QT_END_NAMESPACE

#endif /* defined(__Spacescape__QtFilePathManager__) */
