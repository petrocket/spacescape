/* 
This source file is part of Spacescape
For the latest info, see http://alexcpeterson.com/spacescape

"He determines the number of the stars and calls them each by name. "
Psalm 147:4

The MIT License

Copyright (c) 2010 Alex Peterson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "QtSpacescapeExportFileDialog.h"
#include <QLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>

#include "QtOgreWidgetOSX.h"

struct QFileDialogArgs
{
    QFileDialogArgs() : parent(0), mode(QFileDialog::AnyFile) {}

    QWidget *parent;
    QString caption;
    QString directory;
    QString selection;
    QString filter;
    QFileDialog::FileMode mode;
    QFileDialog::Options options;
};

/** constructor
*/
QtSpacescapeExportFileDialog::QtSpacescapeExportFileDialog(QWidget *parent, Qt::WindowFlags f) :
    QFileDialog(parent, f)
{
}
QtSpacescapeExportFileDialog::QtSpacescapeExportFileDialog(const QFileDialogArgs &args) :
    QFileDialog(args.parent, args.caption, args.directory, args.filter)
{
}

/** destructor
*/
QtSpacescapeExportFileDialog::~QtSpacescapeExportFileDialog(void)
{
}

QString QtSpacescapeExportFileDialog::getExportFileName(QWidget *parent,
                                   const QString &caption,
                                   const QString &dir,
                                   const QString &filter,
                                   QString *selectedFilter,
                                   Options options,
                                   QString *imageSize,
                                   QString *orientation)
{
    QString path;

    // TODO: allow use of dir as path (must validate)
    path = QDir::currentPath();

    QFileDialogArgs args;
    args.parent = parent;
    args.caption = caption;
    args.directory = (dir == 0 || dir.isNull()) ? path : dir;
    args.selection = "";
    args.filter = filter;
    args.mode = AnyFile;
    args.options = DontUseNativeDialog;
    
    // create a qt dialog
    QtSpacescapeExportFileDialog dialog(args);

    // add our widgets
    QLayout* l = dialog.layout();

    QComboBox*q = new QComboBox(parent);
    q->addItem("512");
    q->addItem("1024");
    q->addItem("2048");
    q->addItem("4096");
    QLabel *sizeLabel = new QLabel("Image Size:", parent);
    if(imageSize && !imageSize->isNull()) {
        q->setCurrentText(*imageSize);
    }
    
    QLabel *orientationLabel = new QLabel("Export For:", parent);
    QComboBox *orientationCombo = new QComboBox(parent);
    orientationCombo->addItem("OGRE 3D");
    orientationCombo->addItem("UNREAL");
    orientationCombo->addItem("UNITY");
    orientationCombo->addItem("SOURCE");
    if(orientation && !orientation->isNull()) {
        orientationCombo->setCurrentText(*orientation);
    }
    
    if (dynamic_cast<QGridLayout*>(l) != 0) {
        QGridLayout* grid = dynamic_cast<QGridLayout*>(l);
        const int numRows = grid->rowCount();
        grid->addWidget(sizeLabel, numRows, 0, 1, 1);
        grid->addWidget(q, numRows, 1, 1, 1);

        grid->addWidget(orientationLabel, numRows + 1, 0, 1, 1);
        grid->addWidget(orientationCombo, numRows + 1, 1, 1, 1);
    }

    dialog.setAcceptMode(AcceptSave);
    if (selectedFilter && !selectedFilter->isNull()) {
        dialog.selectNameFilter(*selectedFilter);
    }
    
    if (dialog.exec() == QDialog::Accepted) {

        if (selectedFilter)
            *selectedFilter = dialog.selectedNameFilter();

        *imageSize = q->currentText();
        *orientation = orientationCombo->currentText();

        delete q;
        delete sizeLabel;
        delete orientationCombo;
        delete orientationLabel;
        return dialog.selectedFiles().value(0);
    }

    delete q;
    delete sizeLabel;
    delete orientationCombo;
    delete orientationLabel;
    
    return QString();
}
