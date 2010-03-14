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
#include <QProgressDialog>
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
    QProgressDialog *mProgressDialog;
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
        MainWindow->resize(1024, 520);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow, QMenuBar, #centralWidget,  #centralWidget QSplitter::handle, QMenuBar QMenu, QMenuBar QMenu::item, QStatusBar {\n"
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
"QMenu::it"
                        "em {\n"
"padding:3px 15px;\n"
"border:1px solid #333;\n"
"border-radius:2px;\n"
"margin:0;\n"
"}\n"
"QMenu::item:hover,  QMenu::item:selected {\n"
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
"#frame QPushButton {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(104, 104, 104, 255), stop:0.464503 rgba(104, 104, 104, 255), stop:0.46856 rgba(90, 90, 90, 255), stop:1 rgba(45, 45, 45, 255));\n"
"         border:1px solid #777;\n"
"         border-radius: 3px;\n"
"padding:3px 5px;\n"
"text-align:center;\n"
"color: rgb(200, 200, 200);\n"
"}\n"
"\n"
"#frame QPushButton:hover {"
                        "\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(114, 114, 114, 255), stop:0.464503 rgba(114, 114, 114, 255), stop:0.46856 rgba(100, 100, 100, 255), stop:1 rgba(55, 55, 55, 255));\n"
"color: rgb(210,210,210);\n"
"}\n"
"#frame QPushButton:pressed {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(94,94, 94, 255), stop:0.464503 rgba(94,94, 94, 255), stop:0.46856 rgba(80, 80,80, 255), stop:1 rgba(35, 35, 35, 255));\n"
"color: rgb(210,210,210);\n"
"         border-color:#666;\n"
"}\n"
"\n"
"#centralWidget QScrollBar {\n"
"background-color: rgb(50, 50, 50);\n"
"color: rgb(200, 200, 200);\n"
"border:1px solid #333;\n"
"margin:17px 0 17px 0;\n"
"}\n"
"\n"
"#centralWidget QScrollBar::handle:vertical {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(104, 104, 104, 255), stop:0.464503 rgba(104, 104, 104, 255), stop:0.46856 rgba(90, 90, 90, 255), stop:1 rgba(45, 45, 45, 255));\n"
"color: rgb(200, 200, 200);\n"
""
                        "border:1px solid #444;\n"
"border-radius:2px;\n"
"}\n"
"#centralWidget QScrollBar::handle:vertical:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(114, 114, 114, 255), stop:0.464503 rgba(114, 114, 114, 255), stop:0.46856 rgba(100, 100, 100, 255), stop:1 rgba(55, 55, 55, 255));\n"
"border:1px solid #555;\n"
"}\n"
"#centralWidget QScrollBar::handle:vertical:pressed {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(94,94, 94, 255), stop:0.464503 rgba(94,94, 94, 255), stop:0.46856 rgba(80, 80,80, 255), stop:1 rgba(35, 35, 35, 255));\n"
"border:1px solid #555;\n"
"}\n"
"\n"
"#centralWidget QScrollBar::add-line:vertical, #centralWidget QScrollBar::sub-line:vertical   {\n"
"background-color: rgb(50, 50, 50);\n"
"height: 15px;\n"
"subcontrol-position: bottom;\n"
"subcontrol-origin: margin;\n"
"border:1px solid #333;\n"
"border-radius:2px;\n"
"}\n"
"#centralWidget QScrollBar:hover QScrollBar::add-line:vertical, #centralWidget QScrollBar"
                        ":hover QScrollBar::sub-line:vertical  {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(104, 104, 104, 255), stop:0.464503 rgba(104, 104, 104, 255), stop:0.46856 rgba(90, 90, 90, 255), stop:1 rgba(45, 45, 45, 255));\n"
"color: rgb(200, 200, 200);\n"
"border:1px solid #444;\n"
"border-radius:2px;\n"
"}\n"
"\n"
"#centralWidget QScrollBar::sub-line:vertical {\n"
" subcontrol-position: top;\n"
"}\n"
"\n"
"#centralWidget  QScrollBar::add-line:vertical:hover,  #centralWidget QScrollBar::sub-line:vertical:hover  {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(114, 114, 114, 255), stop:0.464503 rgba(114, 114, 114, 255), stop:0.46856 rgba(100, 100, 100, 255), stop:1 rgba(55, 55, 55, 255));\n"
"border:1px solid #555;\n"
"border-radius:2px;\n"
"}\n"
"#centralWidget QScrollBar::add-line:vertical:pressed,  #centralWidget QScrollBar::sub-line:vertical:pressed  {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba("
                        "64,64, 64, 255), stop:0.464503 rgba(64,64,64, 255), stop:0.46856 rgba(50,50,50, 255), stop:1 rgba(40, 40, 40, 255));	\n"
"}\n"
"\n"
"#centralWidget  QScrollBar::up-arrow:vertical, #centralWidget QScrollBar::down-arrow:vertical {\n"
"background:url(:/spacescape/images/bullet_arrow_up.png);\n"
"      width: 15px;\n"
"      height: 15px;\n"
"  }\n"
"#centralWidget QScrollBar::down-arrow:vertical {\n"
"background:url(:/spacescape/images/bullet_arrow_down.png);\n"
"}\n"
"#centralWidget  QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"      background: none;\n"
"  }\n"
"#layerProperties QTreeView {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(22, 22, 22, 255), stop:0.901366 rgba(67, 67, 67, 255), stop:0.960546 rgba(76, 76, 76, 255), stop:1 rgba(88, 88, 88, 255));\n"
"alternate-background-color: #1a1a1a;\n"
"border:1px solid #000;\n"
"color: rgb(200, 200, 200);\n"
"}\n"
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
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
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
        QSizePolicy sizePolicy1b(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1b.setHorizontalStretch(0);
        sizePolicy1b.setVerticalStretch(0);
        sizePolicy1b.setHeightForWidth(layerProperties->sizePolicy().hasHeightForWidth());
        layerProperties->setSizePolicy(sizePolicy1b);
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
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/spacescape/images/page_white_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        newLayer->setIcon(icon);

        horizontalLayout->addWidget(newLayer);

        copyLayer = new QPushButton(frame);
        copyLayer->setObjectName(QString::fromUtf8("copyLayer"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/spacescape/images/page_white_copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        copyLayer->setIcon(icon1);

        horizontalLayout->addWidget(copyLayer);

        moveLayerDown = new QPushButton(frame);
        moveLayerDown->setObjectName(QString::fromUtf8("moveLayerDown"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/spacescape/images/page_white_put.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveLayerDown->setIcon(icon3);

        horizontalLayout->addWidget(moveLayerDown);

        moveLayerUp = new QPushButton(frame);
        moveLayerUp->setObjectName(QString::fromUtf8("moveLayerUp"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/spacescape/images/page_white_get.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveLayerUp->setIcon(icon4);

        horizontalLayout->addWidget(moveLayerUp);

        horizontalSpacer = new QSpacerItem(62, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);       

        deleteLayer = new QPushButton(frame);
        deleteLayer->setObjectName(QString::fromUtf8("deleteLayer"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/spacescape/images/page_white_delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        deleteLayer->setIcon(icon2);

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

        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(100);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ogreWindow->sizePolicy().hasHeightForWidth());
        ogreWindow->setSizePolicy(sizePolicy3);
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

        mProgressDialog = new QProgressDialog(MainWindow);
        mProgressDialog->setMinimum(0);
        mProgressDialog->setMaximum(100);
        mProgressDialog->setWindowModality(Qt::WindowModal);
        mProgressDialog->setCancelButton(0);

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
#ifndef QT_NO_STATUSTIP
        action_New->setStatusTip(QApplication::translate("MainWindow", "Clear the scene and start over.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Open->setText(QApplication::translate("MainWindow", "&Open", 0, QApplication::UnicodeUTF8));
        action_Open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_Open->setStatusTip(QApplication::translate("MainWindow", "Open a file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        action_Save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("MainWindow", "S&ave as...", 0, QApplication::UnicodeUTF8));
        action_Export->setText(QApplication::translate("MainWindow", "&Export Skybox", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_Export->setStatusTip(QApplication::translate("MainWindow", "Export scene to six images for a skybox", 0, QApplication::UnicodeUTF8));
#endif
        actionAbout->setText(QApplication::translate("MainWindow", "About Spacescape", 0, QApplication::UnicodeUTF8));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        layerProperties->setProperty("resizeMode", QVariant(QApplication::translate("MainWindow", "ResizeToContents", 0, QApplication::UnicodeUTF8)));
        newLayer->setText(QApplication::translate("MainWindow", "New Layer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        newLayer->setStatusTip(QApplication::translate("MainWindow", "Create a new layer", 0, QApplication::UnicodeUTF8));
#endif
        copyLayer->setText(QApplication::translate("MainWindow", "Copy Layer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        copyLayer->setStatusTip(QApplication::translate("MainWindow", "Copy the selected layer", 0, QApplication::UnicodeUTF8));
#endif        
        moveLayerDown->setText(QApplication::translate("MainWindow", "Move Down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        moveLayerDown->setStatusTip(QApplication::translate("MainWindow", "Move the selected layer down", 0, QApplication::UnicodeUTF8));
#endif
        moveLayerUp->setText(QApplication::translate("MainWindow", "Move Up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        moveLayerUp->setStatusTip(QApplication::translate("MainWindow", "Move the selected layer up", 0, QApplication::UnicodeUTF8));
#endif

        deleteLayer->setText(QApplication::translate("MainWindow", "Delete Layer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        deleteLayer->setStatusTip(QApplication::translate("MainWindow", "Delete the selected layer", 0, QApplication::UnicodeUTF8));
#endif
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
