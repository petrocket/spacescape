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
#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtTreePropertyBrowser>
#include "QtSpacescapeWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New;
    QAction *action_Open;
    QAction *action_Save;
    QAction *actionSave_as;
    QAction *action_Export;
    QAction *actionAbout;
    QAction *actionE_xit;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QtTreePropertyBrowser *layerProperties;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *moveLayerDown;
    QPushButton *moveLayerUp;
    QPushButton *newLayer;
    QPushButton *copyLayer;
    QSpacerItem *horizontalSpacer;
    QPushButton *deleteLayer;
    QLabel *helpText;
    QtSpacescapeWidget *ogreWindow;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 400);
MainWindow->setStyleSheet(QString::fromUtf8("* {\n"
"color: rgb(200, 200, 200);\n"
"background-color: rgb(50, 50, 50);\n"
"}\n"
"\n"
"QMenuBar {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(104, 104, 104, 255), stop:0.464503 rgba(104, 104, 104, 255), stop:0.46856 rgba(90, 90, 90, 255), stop:1 rgba(45, 45, 45, 255));\n"
"}\n"
"QMenuBar::item {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(104, 104, 104, 255), stop:0.464503 rgba(104, 104, 104, 255), stop:0.46856 rgba(90, 90, 90, 255), stop:1 rgba(45, 45, 45, 255));\n"
"}\n"
"QMenuBar::item:hover, QMenuBar::item:selected {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(124, 124, 124, 255), stop:0.464503 rgba(124, 124, 124, 255), stop:0.46856 rgba(110, 110, 110, 255), stop:1 rgba(65, 65, 65, 255));\n"
"border:1px solid #777;\n"
"border-radius:2px;\n"
"}\n"
"\n"
"QMenu::item {\n"
"padding:3px 15px;\n"
"border:1px solid #333;\n"
"border-radius:2px;\n"
"margin:0;\n"
"}\n"
"QMenu::item:hover,  "
                        "QMenu::item:selected {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(74,74, 74, 255), stop:0.464503 rgba(74,74,74, 255), stop:0.46856 rgba(60, 60,60, 255), stop:1 rgba(50, 50, 50, 255));\n"
"border:1px solid #555;\n"
"}\n"
"\n"
"QMenu::separator {\n"
"background:#444;\n"
"height:2px;\n"
"margin:3px 0;\n"
"}\n"
"QMenu, #menu_File, #menu_Help {\n"
"padding:3px;\n"
"border:1px solid #333;\n"
"border-radius:2px;\n"
"margin:-1px;\n"
"}\n"
"\n"
"QPushButton {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(104, 104, 104, 255), stop:0.464503 rgba(104, 104, 104, 255), stop:0.46856 rgba(90, 90, 90, 255), stop:1 rgba(45, 45, 45, 255));\n"
"         border:1px solid #777;\n"
"         border-radius: 3px;\n"
"padding:3px 5px;\n"
"text-align:center;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(114, 114, 114, 255), stop:0.464503 rgba(114, 114, 114, 255), stop:0.46856 rgba(100, "
                        "100, 100, 255), stop:1 rgba(55, 55, 55, 255));\n"
"color: rgb(210,210,210);\n"
"}\n"
"QPushButton:pressed {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(94,94, 94, 255), stop:0.464503 rgba(94,94, 94, 255), stop:0.46856 rgba(80, 80,80, 255), stop:1 rgba(35, 35, 35, 255));\n"
"color: rgb(210,210,210);\n"
"         border-color:#666;\n"
"}\n"
"\n"
"#layerProperties QTreeView{\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(22, 22, 22, 255), stop:0.901366 rgba(67, 67, 67, 255), stop:0.960546 rgba(76, 76, 76, 255), stop:1 rgba(88, 88, 88, 255));\n"
"alternate-background-color: #222;\n"
"border:1px solid #000;\n"
"}\n"
"#layerProperties QTreeView:selected {\n"
"background-color:#f00;\n"
"}\n"
"\n"
"#helpText {\n"
"border:1px solid #444;\n"
"color:#999;\n"
"}"));
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        action_Export = new QAction(MainWindow);
        action_Export->setObjectName(QString::fromUtf8("action_Export"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(4, 0, 0, 0);
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        layerProperties = new QtTreePropertyBrowser(widget);
        layerProperties->setObjectName(QString::fromUtf8("layerProperties"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(layerProperties->sizePolicy().hasHeightForWidth());
        layerProperties->setSizePolicy(sizePolicy);
        layerProperties->setProperty("headerVisible", QVariant(false));

        verticalLayout->addWidget(layerProperties);

        frame = new QFrame(widget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 3, 0, 0);
        newLayer = new QPushButton(frame);
        newLayer->setObjectName(QString::fromUtf8("newLayer"));

        horizontalLayout->addWidget(newLayer);

        copyLayer = new QPushButton(frame);
        copyLayer->setObjectName(QString::fromUtf8("copyLayer"));

        horizontalLayout->addWidget(copyLayer);

        moveLayerDown = new QPushButton(frame);
        moveLayerDown->setObjectName(QString::fromUtf8("moveLayerDown"));

        horizontalLayout->addWidget(moveLayerDown);

        moveLayerUp = new QPushButton(frame);
        moveLayerUp->setObjectName(QString::fromUtf8("moveLayerUp"));

        horizontalLayout->addWidget(moveLayerUp);

        horizontalSpacer = new QSpacerItem(62, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);       

        deleteLayer = new QPushButton(frame);
        deleteLayer->setObjectName(QString::fromUtf8("deleteLayer"));

        horizontalLayout->addWidget(deleteLayer);


        verticalLayout->addWidget(frame);

        splitter->addWidget(widget);
        helpText = new QLabel(splitter);
        helpText->setObjectName(QString::fromUtf8("helpText"));
        helpText->setFrameShape(QFrame::StyledPanel);
        helpText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        helpText->setWordWrap(true);
        helpText->setIndent(2);
        helpText->setBaseSize(0,0);
        helpText->setFixedSize(0,0);
        splitter->addWidget(helpText);
        splitter_2->addWidget(splitter);
        ogreWindow = new QtSpacescapeWidget(splitter_2);
        ogreWindow->setObjectName(QString::fromUtf8("ogreWindow"));
        sizePolicy.setHeightForWidth(ogreWindow->sizePolicy().hasHeightForWidth());
        ogreWindow->setSizePolicy(sizePolicy);
        ogreWindow->setMinimumSize(QSize(256, 256));
        ogreWindow->setProperty("backgroundColor", QVariant(QColor(0, 0, 0)));
        splitter_2->addWidget(ogreWindow);

        horizontalLayout_2->addWidget(splitter_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu_File->addAction(action_New);
        menu_File->addAction(action_Open);
        menu_File->addSeparator();
        menu_File->addAction(action_Save);
        menu_File->addAction(actionSave_as);
        menu_File->addAction(action_Export);
        menu_File->addSeparator();
        menu_File->addAction(actionE_xit);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);
        QObject::connect(actionE_xit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(action_Export, SIGNAL(triggered()), MainWindow, SLOT(onExport()));
        QObject::connect(action_New, SIGNAL(triggered()), MainWindow, SLOT(onNewFile()));
        QObject::connect(newLayer, SIGNAL(clicked()), MainWindow, SLOT(onNewLayerClicked()));
        QObject::connect(copyLayer, SIGNAL(clicked()), MainWindow, SLOT(onCopyLayerClicked()));
        QObject::connect(moveLayerDown, SIGNAL(clicked()), MainWindow, SLOT(onMoveLayerDown()));
        QObject::connect(moveLayerUp, SIGNAL(clicked()), MainWindow, SLOT(onMoveLayerUp()));
        QObject::connect(deleteLayer, SIGNAL(clicked()), MainWindow, SLOT(onDeleteLayerClicked()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Spacescape - untitled.xml*", 0, QApplication::UnicodeUTF8));
        action_New->setText(QApplication::translate("MainWindow", "&New", 0, QApplication::UnicodeUTF8));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_Open->setStatusTip(QApplication::translate("MainWindow", "Open a file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("MainWindow", "S&ave as...", 0, QApplication::UnicodeUTF8));
        action_Export->setText(QApplication::translate("MainWindow", "&Export Skybox", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About Spacescape", 0, QApplication::UnicodeUTF8));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        layerProperties->setProperty("resizeMode", QVariant(QApplication::translate("MainWindow", "ResizeToContents", 0, QApplication::UnicodeUTF8)));
        newLayer->setText(QApplication::translate("MainWindow", "New Layer", 0, QApplication::UnicodeUTF8));
        copyLayer->setText(QApplication::translate("MainWindow", "Copy Layer", 0, QApplication::UnicodeUTF8));
        moveLayerDown->setText(QApplication::translate("MainWindow", "Move Down", 0, QApplication::UnicodeUTF8));
        moveLayerUp->setText(QApplication::translate("MainWindow", "Move Up", 0, QApplication::UnicodeUTF8));
        deleteLayer->setText(QApplication::translate("MainWindow", "Delete Layer", 0, QApplication::UnicodeUTF8));
        helpText->setText(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtSpacescapeUI: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
