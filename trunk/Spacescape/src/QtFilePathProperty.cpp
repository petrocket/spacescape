//
//  QtFilePathManager.cpp
//  Spacescape
//
//  Created by Alex on 9/1/14.
//
//

#include "QtFilePathProperty.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QFocusEvent>

QtFileEdit::QtFileEdit(QWidget *parent)
: QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    theLineEdit = new QLineEdit(this);
    theLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String("..."));
    layout->addWidget(theLineEdit);
    layout->addWidget(button);
    setFocusProxy(theLineEdit);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
    connect(theLineEdit, SIGNAL(textEdited(const QString &)),
            this, SIGNAL(filePathChanged(const QString &)));
    connect(button, SIGNAL(clicked()),
            this, SLOT(buttonClicked()));
}

void QtFileEdit::buttonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose a file"), theLineEdit->text(), theFilter);
    if (filePath.isNull())
        return;
    theLineEdit->setText(filePath);
    emit filePathChanged(filePath);
}

void QtFileEdit::focusInEvent(QFocusEvent *e)
{
    theLineEdit->event(e);
    if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
        theLineEdit->selectAll();
    }
    QWidget::focusInEvent(e);
}

void QtFileEdit::focusOutEvent(QFocusEvent *e)
{
    theLineEdit->event(e);
    QWidget::focusOutEvent(e);
}

void QtFileEdit::keyPressEvent(QKeyEvent *e)
{
    theLineEdit->event(e);
}

void QtFileEdit::keyReleaseEvent(QKeyEvent *e)
{
    theLineEdit->event(e);
}

// ============================================

QString QtFilePathManager::value(const QtProperty *property) const
{
    if (!theValues.contains(property))
        return QString();
    return theValues[property].value;
}

QString QtFilePathManager::filter(const QtProperty *property) const
{
    if (!theValues.contains(property))
        return QString();
    return theValues[property].filter;
}

void QtFilePathManager::setValue(QtProperty *property, const QString &val)
{
    if (!theValues.contains(property))
        return;
    
    Data data = theValues[property];
    
    if (data.value == val)
        return;
    
    data.value = val;
    
    theValues[property] = data;
    
    emit propertyChanged(property);
    emit valueChanged(property, data.value);
}

void QtFilePathManager::setFilter(QtProperty *property, const QString &fil)
{
    if (!theValues.contains(property))
        return;
    
    Data data = theValues[property];
    
    if (data.filter == fil)
        return;
    
    data.filter = fil;
    
    theValues[property] = data;
    
    emit filterChanged(property, data.filter);
}




// ============================================

QtFileEditFactory::~QtFileEditFactory()
{
    QList<QtFileEdit *> editors = theEditorToProperty.keys();
    QListIterator<QtFileEdit *> it(editors);
    while (it.hasNext())
        delete it.next();
}

void QtFileEditFactory::connectPropertyManager(QtFilePathManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
            this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
    connect(manager, SIGNAL(filterChanged(QtProperty *, const QString &)),
            this, SLOT(slotFilterChanged(QtProperty *, const QString &)));
}

QWidget *QtFileEditFactory::createEditor(QtFilePathManager *manager,
                                       QtProperty *property, QWidget *parent)
{
    QtFileEdit *editor = new QtFileEdit(parent);
    editor->setFilePath(manager->value(property));
    editor->setFilter(manager->filter(property));
    theCreatedEditors[property].append(editor);
    theEditorToProperty[editor] = property;
    
    connect(editor, SIGNAL(filePathChanged(const QString &)),
            this, SLOT(slotSetValue(const QString &)));
    connect(editor, SIGNAL(destroyed(QObject *)),
            this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void QtFileEditFactory::disconnectPropertyManager(QtFilePathManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
               this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
    disconnect(manager, SIGNAL(filterChanged(QtProperty *, const QString &)),
               this, SLOT(slotFilterChanged(QtProperty *, const QString &)));
}

void QtFileEditFactory::slotPropertyChanged(QtProperty *property,
                                          const QString &value)
{
    if (!theCreatedEditors.contains(property))
        return;
    
    QList<QtFileEdit *> editors = theCreatedEditors[property];
    QListIterator<QtFileEdit *> itEditor(editors);
    while (itEditor.hasNext())
        itEditor.next()->setFilePath(value);
}

void QtFileEditFactory::slotFilterChanged(QtProperty *property,
                                        const QString &filter)
{
    if (!theCreatedEditors.contains(property))
        return;
    
    QList<QtFileEdit *> editors = theCreatedEditors[property];
    QListIterator<QtFileEdit *> itEditor(editors);
    while (itEditor.hasNext())
        itEditor.next()->setFilter(filter);
}

void QtFileEditFactory::slotSetValue(const QString &value)
{
    QObject *object = sender();
    QMap<QtFileEdit *, QtProperty *>::ConstIterator itEditor =
    theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            QtFilePathManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
        itEditor++;
    }
}

void QtFileEditFactory::slotEditorDestroyed(QObject *object)
{
    QMap<QtFileEdit *, QtProperty *>::ConstIterator itEditor =
    theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtFileEdit *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            theEditorToProperty.remove(editor);
            theCreatedEditors[property].removeAll(editor);
            if (theCreatedEditors[property].isEmpty())
                theCreatedEditors.remove(property);
            return;
        }
        itEditor++;
    }
}

