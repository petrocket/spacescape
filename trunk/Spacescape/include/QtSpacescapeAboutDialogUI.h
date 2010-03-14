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
#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *logo;
    QFrame *frame;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *versionLabel;
    QLabel *version;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_3;
    QLabel *aboutTitle;
    QLabel *author;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *releaseDateLabel;
    QLabel *releaseDate;
    QWidget *widget3;
    QVBoxLayout *verticalLayout;
    QLabel *authorUrlLabel;
    QLabel *authorUrl;
    QWidget *widget4;
    QVBoxLayout *verticalLayout_2;
    QLabel *blogUrlLabel;
    QLabel *blogUrl;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *aboutCloseBtn;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(544, 311);
        verticalLayout_4 = new QVBoxLayout(AboutDialog);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        logo = new QLabel(AboutDialog);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/spacescape/images/spacescape-logo.png")));
        logo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        horizontalLayout_3->addWidget(logo);

        frame = new QFrame(AboutDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 50, 68, 15));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        versionLabel = new QLabel(widget);
        versionLabel->setObjectName(QString::fromUtf8("versionLabel"));

        horizontalLayout->addWidget(versionLabel);

        version = new QLabel(widget);
        version->setObjectName(QString::fromUtf8("version"));
        version->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(version);

        widget1 = new QWidget(frame);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(0, 0, 96, 40));
        verticalLayout_3 = new QVBoxLayout(widget1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        aboutTitle = new QLabel(widget1);
        aboutTitle->setObjectName(QString::fromUtf8("aboutTitle"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        aboutTitle->setFont(font);

        verticalLayout_3->addWidget(aboutTitle);

        author = new QLabel(widget1);
        author->setObjectName(QString::fromUtf8("author"));

        verticalLayout_3->addWidget(author);

        widget2 = new QWidget(frame);
        widget2->setObjectName(QString::fromUtf8("widget2"));
        widget2->setGeometry(QRect(0, 70, 120, 15));
        horizontalLayout_2 = new QHBoxLayout(widget2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        releaseDateLabel = new QLabel(widget2);
        releaseDateLabel->setObjectName(QString::fromUtf8("releaseDateLabel"));

        horizontalLayout_2->addWidget(releaseDateLabel);

        releaseDate = new QLabel(widget2);
        releaseDate->setObjectName(QString::fromUtf8("releaseDate"));
        releaseDate->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout_2->addWidget(releaseDate);

        widget3 = new QWidget(frame);
        widget3->setObjectName(QString::fromUtf8("widget3"));
        widget3->setGeometry(QRect(0, 110, 200, 34));
        verticalLayout = new QVBoxLayout(widget3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        authorUrlLabel = new QLabel(widget3);
        authorUrlLabel->setObjectName(QString::fromUtf8("authorUrlLabel"));
        QFont font1;
        font1.setBold(true);
        font1.setUnderline(true);
        font1.setWeight(75);
        authorUrlLabel->setFont(font1);

        verticalLayout->addWidget(authorUrlLabel);

        authorUrl = new QLabel(widget3);
        authorUrl->setObjectName(QString::fromUtf8("authorUrl"));
        authorUrl->setOpenExternalLinks(true);
        authorUrl->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(authorUrl);

        widget4 = new QWidget(frame);
        widget4->setObjectName(QString::fromUtf8("widget4"));
        widget4->setGeometry(QRect(0, 170, 200, 34));
        verticalLayout_2 = new QVBoxLayout(widget4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        blogUrlLabel = new QLabel(widget4);
        blogUrlLabel->setObjectName(QString::fromUtf8("blogUrlLabel"));
        blogUrlLabel->setFont(font1);

        verticalLayout_2->addWidget(blogUrlLabel);

        blogUrl = new QLabel(widget4);
        blogUrl->setObjectName(QString::fromUtf8("blogUrl"));
        blogUrl->setOpenExternalLinks(true);
        blogUrl->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_2->addWidget(blogUrl);


        horizontalLayout_3->addWidget(frame);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(418, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        aboutCloseBtn = new QPushButton(AboutDialog);
        aboutCloseBtn->setObjectName(QString::fromUtf8("aboutCloseBtn"));

        horizontalLayout_4->addWidget(aboutCloseBtn);


        verticalLayout_4->addLayout(horizontalLayout_4);


        retranslateUi(AboutDialog);
        QObject::connect(aboutCloseBtn, SIGNAL(clicked(bool)), AboutDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About Spacescape", 0, QApplication::UnicodeUTF8));
        logo->setText(QString());
        versionLabel->setText(QApplication::translate("AboutDialog", "Version", 0, QApplication::UnicodeUTF8));
        version->setText(QApplication::translate("AboutDialog", "0.2", 0, QApplication::UnicodeUTF8));
        aboutTitle->setText(QApplication::translate("AboutDialog", "Spacescape", 0, QApplication::UnicodeUTF8));
        author->setText(QApplication::translate("AboutDialog", "by Alex Peterson", 0, QApplication::UnicodeUTF8));
        releaseDateLabel->setText(QApplication::translate("AboutDialog", "Release Date", 0, QApplication::UnicodeUTF8));
        releaseDate->setText(QApplication::translate("AboutDialog", "3/13/2010", 0, QApplication::UnicodeUTF8));
        authorUrlLabel->setText(QApplication::translate("AboutDialog", "Developer Website:", 0, QApplication::UnicodeUTF8));
        authorUrl->setText(QApplication::translate("AboutDialog", "http://alexcpeterson.com/spacescape", 0, QApplication::UnicodeUTF8));
        blogUrlLabel->setText(QApplication::translate("AboutDialog", "Blog:", 0, QApplication::UnicodeUTF8));
        blogUrl->setText(QApplication::translate("AboutDialog", "http://petrocket.blogspot.com", 0, QApplication::UnicodeUTF8));
        aboutCloseBtn->setText(QApplication::translate("AboutDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtSpacescapeAboutDialogUI: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
