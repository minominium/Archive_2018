/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *label_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QTextBrowser *ui_textBrowser;
    QTextBrowser *ui_textBrowserWP;
    QTextBrowser *ui_textBrowserLat;
    QTextBrowser *ui_textBrowserLong;
    QTextBrowser *ui_textBrowserAlt;
    QTextBrowser *ui_textBrowserAlt_2;
    QTextBrowser *ui_textBrowserAlt_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_9;
    QGridLayout *gridLayout_8;
    QGroupBox *ui_group_GPS_Point;
    QWidget *layoutWidget2;
    QGridLayout *gpsSet_Layout;
    QHBoxLayout *horizontalLayout_8;
    QLabel *ui_label_ep;
    QLineEdit *ui_line_ep_lat;
    QLineEdit *ui_line_ep_lng;
    QHBoxLayout *horizontalLayout_7;
    QLabel *ui_label_dp;
    QLineEdit *ui_line_dp_lat;
    QLineEdit *ui_line_dp_lng;
    QHBoxLayout *horizontalLayout_2;
    QLabel *ui_label_wp;
    QLineEdit *ui_line_wp_lat;
    QLineEdit *ui_line_wp_lng;
    QHBoxLayout *horizontalLayout_3;
    QLabel *ui_label_sp;
    QLineEdit *ui_line_sp_lat;
    QLineEdit *ui_line_sp_lng;
    QCheckBox *ui_check_ep;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *ui_label_np;
    QLineEdit *ui_line_np_lat;
    QLineEdit *ui_line_np_lng;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *ui_line_np_rad;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *ui_button_GPSdisplay;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *ui_button_GPSreset;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer;
    QGroupBox *ui_group_Emergency;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout;
    QCheckBox *ui_check_emerge_1;
    QPushButton *ui_button_auto;
    QPushButton *ui_button_manual;
    QCheckBox *ui_check_emerge_2;
    QPushButton *ui_button_takeoff;
    QPushButton *ui_button_landing;
    QGroupBox *ui_group_Gripper;
    QWidget *layoutWidget4;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout_2;
    QDial *ui_dial_gripper;
    QLineEdit *ui_line_gripper;
    QLabel *ui_label_gripper;
    QGroupBox *ui_group_toque;
    QWidget *layoutWidget5;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *ui_radio_on;
    QRadioButton *ui_radio_off;
    QGroupBox *ui_group_port;
    QWidget *layoutWidget6;
    QGridLayout *gridLayout_3;
    QLabel *ui_label_baudrate;
    QPushButton *ui_button_scan;
    QLabel *ui_label_port;
    QPushButton *ui_button_connect;
    QComboBox *ui_comboBox_baudrate;
    QComboBox *ui_comboBox_port;
    QPushButton *ui_button_disconnect;
    QSpacerItem *horizontalSpacer_8;
    QWebView *ui_webView;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_11;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1920, 1080);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        label->setGeometry(QRect(0, 80, 1921, 51));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QStringLiteral("PMingLiU-ExtB"));
        font.setPointSize(30);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        label->setFont(font);
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(1700, 20, 151, 141));
        label_2->setPixmap(QPixmap(QString::fromUtf8("../Robit_Logo_1.png")));
        label_2->setScaledContents(true);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 180, 1861, 1002));
        gridLayout_4 = new QGridLayout(layoutWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 20, 1061, 341));
        gridLayout_6 = new QGridLayout(layoutWidget1);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        ui_textBrowser = new QTextBrowser(layoutWidget1);
        ui_textBrowser->setObjectName(QStringLiteral("ui_textBrowser"));
        ui_textBrowser->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowser);

        ui_textBrowserWP = new QTextBrowser(layoutWidget1);
        ui_textBrowserWP->setObjectName(QStringLiteral("ui_textBrowserWP"));
        ui_textBrowserWP->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowserWP);

        ui_textBrowserLat = new QTextBrowser(layoutWidget1);
        ui_textBrowserLat->setObjectName(QStringLiteral("ui_textBrowserLat"));
        ui_textBrowserLat->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowserLat);

        ui_textBrowserLong = new QTextBrowser(layoutWidget1);
        ui_textBrowserLong->setObjectName(QStringLiteral("ui_textBrowserLong"));
        ui_textBrowserLong->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowserLong);

        ui_textBrowserAlt = new QTextBrowser(layoutWidget1);
        ui_textBrowserAlt->setObjectName(QStringLiteral("ui_textBrowserAlt"));
        ui_textBrowserAlt->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowserAlt);

        ui_textBrowserAlt_2 = new QTextBrowser(layoutWidget1);
        ui_textBrowserAlt_2->setObjectName(QStringLiteral("ui_textBrowserAlt_2"));
        ui_textBrowserAlt_2->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowserAlt_2);

        ui_textBrowserAlt_3 = new QTextBrowser(layoutWidget1);
        ui_textBrowserAlt_3->setObjectName(QStringLiteral("ui_textBrowserAlt_3"));
        ui_textBrowserAlt_3->setMaximumSize(QSize(1020, 200));

        horizontalLayout_6->addWidget(ui_textBrowserAlt_3);


        gridLayout_6->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_4);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_5);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_6);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_7);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_8);

        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_9);

        label_10 = new QLabel(layoutWidget1);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_10);


        gridLayout_6->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout_6->addItem(verticalSpacer_2, 2, 0, 1, 1);


        gridLayout_7->addWidget(groupBox, 3, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout_7->addItem(verticalSpacer_3, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout_7->addItem(verticalSpacer, 2, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(6);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        ui_group_GPS_Point = new QGroupBox(layoutWidget);
        ui_group_GPS_Point->setObjectName(QStringLiteral("ui_group_GPS_Point"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ui_group_GPS_Point->sizePolicy().hasHeightForWidth());
        ui_group_GPS_Point->setSizePolicy(sizePolicy3);
        ui_group_GPS_Point->setMaximumSize(QSize(400, 800));
        layoutWidget2 = new QWidget(ui_group_GPS_Point);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 20, 361, 231));
        gpsSet_Layout = new QGridLayout(layoutWidget2);
        gpsSet_Layout->setSpacing(6);
        gpsSet_Layout->setContentsMargins(11, 11, 11, 11);
        gpsSet_Layout->setObjectName(QStringLiteral("gpsSet_Layout"));
        gpsSet_Layout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        ui_label_ep = new QLabel(layoutWidget2);
        ui_label_ep->setObjectName(QStringLiteral("ui_label_ep"));

        horizontalLayout_8->addWidget(ui_label_ep);

        ui_line_ep_lat = new QLineEdit(layoutWidget2);
        ui_line_ep_lat->setObjectName(QStringLiteral("ui_line_ep_lat"));
        ui_line_ep_lat->setEnabled(false);
        ui_line_ep_lat->setLayoutDirection(Qt::RightToLeft);
        ui_line_ep_lat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(ui_line_ep_lat);

        ui_line_ep_lng = new QLineEdit(layoutWidget2);
        ui_line_ep_lng->setObjectName(QStringLiteral("ui_line_ep_lng"));
        ui_line_ep_lng->setEnabled(false);
        ui_line_ep_lng->setLayoutDirection(Qt::RightToLeft);
        ui_line_ep_lng->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(ui_line_ep_lng);


        gpsSet_Layout->addLayout(horizontalLayout_8, 3, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        ui_label_dp = new QLabel(layoutWidget2);
        ui_label_dp->setObjectName(QStringLiteral("ui_label_dp"));

        horizontalLayout_7->addWidget(ui_label_dp);

        ui_line_dp_lat = new QLineEdit(layoutWidget2);
        ui_line_dp_lat->setObjectName(QStringLiteral("ui_line_dp_lat"));
        ui_line_dp_lat->setEnabled(true);
        ui_line_dp_lat->setLayoutDirection(Qt::RightToLeft);
        ui_line_dp_lat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(ui_line_dp_lat);

        ui_line_dp_lng = new QLineEdit(layoutWidget2);
        ui_line_dp_lng->setObjectName(QStringLiteral("ui_line_dp_lng"));
        ui_line_dp_lng->setEnabled(true);
        ui_line_dp_lng->setLayoutDirection(Qt::RightToLeft);
        ui_line_dp_lng->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(ui_line_dp_lng);


        gpsSet_Layout->addLayout(horizontalLayout_7, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        ui_label_wp = new QLabel(layoutWidget2);
        ui_label_wp->setObjectName(QStringLiteral("ui_label_wp"));

        horizontalLayout_2->addWidget(ui_label_wp);

        ui_line_wp_lat = new QLineEdit(layoutWidget2);
        ui_line_wp_lat->setObjectName(QStringLiteral("ui_line_wp_lat"));
        ui_line_wp_lat->setEnabled(true);
        ui_line_wp_lat->setLayoutDirection(Qt::RightToLeft);
        ui_line_wp_lat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(ui_line_wp_lat);

        ui_line_wp_lng = new QLineEdit(layoutWidget2);
        ui_line_wp_lng->setObjectName(QStringLiteral("ui_line_wp_lng"));
        ui_line_wp_lng->setEnabled(true);
        ui_line_wp_lng->setLayoutDirection(Qt::RightToLeft);
        ui_line_wp_lng->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(ui_line_wp_lng);


        gpsSet_Layout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        ui_label_sp = new QLabel(layoutWidget2);
        ui_label_sp->setObjectName(QStringLiteral("ui_label_sp"));

        horizontalLayout_3->addWidget(ui_label_sp);

        ui_line_sp_lat = new QLineEdit(layoutWidget2);
        ui_line_sp_lat->setObjectName(QStringLiteral("ui_line_sp_lat"));
        ui_line_sp_lat->setEnabled(true);
        ui_line_sp_lat->setLayoutDirection(Qt::RightToLeft);
        ui_line_sp_lat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(ui_line_sp_lat);

        ui_line_sp_lng = new QLineEdit(layoutWidget2);
        ui_line_sp_lng->setObjectName(QStringLiteral("ui_line_sp_lng"));
        ui_line_sp_lng->setEnabled(true);
        ui_line_sp_lng->setLayoutDirection(Qt::RightToLeft);
        ui_line_sp_lng->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(ui_line_sp_lng);


        gpsSet_Layout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        ui_check_ep = new QCheckBox(layoutWidget2);
        ui_check_ep->setObjectName(QStringLiteral("ui_check_ep"));
        ui_check_ep->setLayoutDirection(Qt::RightToLeft);
        ui_check_ep->setChecked(true);

        gpsSet_Layout->addWidget(ui_check_ep, 3, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpsSet_Layout->addItem(horizontalSpacer_2, 2, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        ui_label_np = new QLabel(layoutWidget2);
        ui_label_np->setObjectName(QStringLiteral("ui_label_np"));

        horizontalLayout_5->addWidget(ui_label_np);

        ui_line_np_lat = new QLineEdit(layoutWidget2);
        ui_line_np_lat->setObjectName(QStringLiteral("ui_line_np_lat"));
        ui_line_np_lat->setEnabled(true);
        ui_line_np_lat->setLayoutDirection(Qt::RightToLeft);
        ui_line_np_lat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(ui_line_np_lat);

        ui_line_np_lng = new QLineEdit(layoutWidget2);
        ui_line_np_lng->setObjectName(QStringLiteral("ui_line_np_lng"));
        ui_line_np_lng->setEnabled(true);
        ui_line_np_lng->setLayoutDirection(Qt::RightToLeft);
        ui_line_np_lng->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(ui_line_np_lng);


        gpsSet_Layout->addLayout(horizontalLayout_5, 4, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpsSet_Layout->addItem(horizontalSpacer_3, 1, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        ui_line_np_rad = new QLineEdit(layoutWidget2);
        ui_line_np_rad->setObjectName(QStringLiteral("ui_line_np_rad"));
        ui_line_np_rad->setEnabled(true);
        ui_line_np_rad->setLayoutDirection(Qt::RightToLeft);
        ui_line_np_rad->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(ui_line_np_rad);


        gpsSet_Layout->addLayout(verticalLayout_2, 4, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        ui_button_GPSdisplay = new QPushButton(layoutWidget2);
        ui_button_GPSdisplay->setObjectName(QStringLiteral("ui_button_GPSdisplay"));
        ui_button_GPSdisplay->setAutoFillBackground(false);
        ui_button_GPSdisplay->setIconSize(QSize(15, 15));

        horizontalLayout_4->addWidget(ui_button_GPSdisplay);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        ui_button_GPSreset = new QPushButton(layoutWidget2);
        ui_button_GPSreset->setObjectName(QStringLiteral("ui_button_GPSreset"));
        ui_button_GPSreset->setCheckable(false);
        ui_button_GPSreset->setChecked(false);

        horizontalLayout_4->addWidget(ui_button_GPSreset);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);


        gpsSet_Layout->addLayout(horizontalLayout_4, 5, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpsSet_Layout->addItem(horizontalSpacer, 0, 1, 1, 1);


        gridLayout_8->addWidget(ui_group_GPS_Point, 1, 0, 1, 1);

        ui_group_Emergency = new QGroupBox(layoutWidget);
        ui_group_Emergency->setObjectName(QStringLiteral("ui_group_Emergency"));
        sizePolicy3.setHeightForWidth(ui_group_Emergency->sizePolicy().hasHeightForWidth());
        ui_group_Emergency->setSizePolicy(sizePolicy3);
        ui_group_Emergency->setMinimumSize(QSize(0, 0));
        ui_group_Emergency->setMaximumSize(QSize(400, 100));
        layoutWidget3 = new QWidget(ui_group_Emergency);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(70, 20, 218, 65));
        gridLayout = new QGridLayout(layoutWidget3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        ui_check_emerge_1 = new QCheckBox(layoutWidget3);
        ui_check_emerge_1->setObjectName(QStringLiteral("ui_check_emerge_1"));
        ui_check_emerge_1->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(ui_check_emerge_1, 0, 0, 1, 1);

        ui_button_auto = new QPushButton(layoutWidget3);
        ui_button_auto->setObjectName(QStringLiteral("ui_button_auto"));
        ui_button_auto->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(ui_button_auto->sizePolicy().hasHeightForWidth());
        ui_button_auto->setSizePolicy(sizePolicy4);
        ui_button_auto->setMaximumSize(QSize(300, 300));
        ui_button_auto->setAutoRepeatDelay(300);

        gridLayout->addWidget(ui_button_auto, 0, 1, 1, 1);

        ui_button_manual = new QPushButton(layoutWidget3);
        ui_button_manual->setObjectName(QStringLiteral("ui_button_manual"));
        ui_button_manual->setEnabled(false);

        gridLayout->addWidget(ui_button_manual, 0, 2, 1, 1);

        ui_check_emerge_2 = new QCheckBox(layoutWidget3);
        ui_check_emerge_2->setObjectName(QStringLiteral("ui_check_emerge_2"));
        ui_check_emerge_2->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(ui_check_emerge_2, 1, 0, 1, 1);

        ui_button_takeoff = new QPushButton(layoutWidget3);
        ui_button_takeoff->setObjectName(QStringLiteral("ui_button_takeoff"));
        ui_button_takeoff->setEnabled(false);

        gridLayout->addWidget(ui_button_takeoff, 1, 1, 1, 1);

        ui_button_landing = new QPushButton(layoutWidget3);
        ui_button_landing->setObjectName(QStringLiteral("ui_button_landing"));
        ui_button_landing->setEnabled(false);

        gridLayout->addWidget(ui_button_landing, 1, 2, 1, 1);


        gridLayout_8->addWidget(ui_group_Emergency, 0, 0, 1, 1);

        ui_group_Gripper = new QGroupBox(layoutWidget);
        ui_group_Gripper->setObjectName(QStringLiteral("ui_group_Gripper"));
        ui_group_Gripper->setMaximumSize(QSize(400, 16777215));
        layoutWidget4 = new QWidget(ui_group_Gripper);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(10, 20, 391, 171));
        gridLayout_5 = new QGridLayout(layoutWidget4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_6 = new QSpacerItem(30, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_6, 0, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        ui_dial_gripper = new QDial(layoutWidget4);
        ui_dial_gripper->setObjectName(QStringLiteral("ui_dial_gripper"));

        gridLayout_2->addWidget(ui_dial_gripper, 1, 0, 1, 1);

        ui_line_gripper = new QLineEdit(layoutWidget4);
        ui_line_gripper->setObjectName(QStringLiteral("ui_line_gripper"));
        ui_line_gripper->setEnabled(true);
        ui_line_gripper->setLayoutDirection(Qt::RightToLeft);
        ui_line_gripper->setAlignment(Qt::AlignCenter);
        ui_line_gripper->setReadOnly(false);
        ui_line_gripper->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gridLayout_2->addWidget(ui_line_gripper, 2, 0, 1, 1);

        ui_label_gripper = new QLabel(layoutWidget4);
        ui_label_gripper->setObjectName(QStringLiteral("ui_label_gripper"));
        ui_label_gripper->setLayoutDirection(Qt::RightToLeft);
        ui_label_gripper->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(ui_label_gripper, 0, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 0, 0, 1, 1);

        ui_group_toque = new QGroupBox(layoutWidget4);
        ui_group_toque->setObjectName(QStringLiteral("ui_group_toque"));
        ui_group_toque->setAutoFillBackground(false);
        ui_group_toque->setFlat(false);
        ui_group_toque->setCheckable(true);
        ui_group_toque->setChecked(false);
        layoutWidget5 = new QWidget(ui_group_toque);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(10, 60, 61, 51));
        verticalLayout_3 = new QVBoxLayout(layoutWidget5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        ui_radio_on = new QRadioButton(layoutWidget5);
        ui_radio_on->setObjectName(QStringLiteral("ui_radio_on"));
        ui_radio_on->setEnabled(false);

        verticalLayout_3->addWidget(ui_radio_on);

        ui_radio_off = new QRadioButton(layoutWidget5);
        ui_radio_off->setObjectName(QStringLiteral("ui_radio_off"));
        ui_radio_off->setEnabled(false);

        verticalLayout_3->addWidget(ui_radio_off);


        gridLayout_5->addWidget(ui_group_toque, 0, 2, 1, 1);


        gridLayout_8->addWidget(ui_group_Gripper, 1, 1, 1, 1);

        ui_group_port = new QGroupBox(layoutWidget);
        ui_group_port->setObjectName(QStringLiteral("ui_group_port"));
        sizePolicy3.setHeightForWidth(ui_group_port->sizePolicy().hasHeightForWidth());
        ui_group_port->setSizePolicy(sizePolicy3);
        ui_group_port->setMaximumSize(QSize(400, 150));
        layoutWidget6 = new QWidget(ui_group_port);
        layoutWidget6->setObjectName(QStringLiteral("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 20, 361, 65));
        gridLayout_3 = new QGridLayout(layoutWidget6);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        ui_label_baudrate = new QLabel(layoutWidget6);
        ui_label_baudrate->setObjectName(QStringLiteral("ui_label_baudrate"));
        ui_label_baudrate->setLayoutDirection(Qt::LeftToRight);
        ui_label_baudrate->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(ui_label_baudrate, 1, 0, 1, 1);

        ui_button_scan = new QPushButton(layoutWidget6);
        ui_button_scan->setObjectName(QStringLiteral("ui_button_scan"));

        gridLayout_3->addWidget(ui_button_scan, 1, 2, 1, 1);

        ui_label_port = new QLabel(layoutWidget6);
        ui_label_port->setObjectName(QStringLiteral("ui_label_port"));
        ui_label_port->setLayoutDirection(Qt::LeftToRight);
        ui_label_port->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(ui_label_port, 0, 0, 1, 1);

        ui_button_connect = new QPushButton(layoutWidget6);
        ui_button_connect->setObjectName(QStringLiteral("ui_button_connect"));

        gridLayout_3->addWidget(ui_button_connect, 0, 2, 1, 1);

        ui_comboBox_baudrate = new QComboBox(layoutWidget6);
        ui_comboBox_baudrate->setObjectName(QStringLiteral("ui_comboBox_baudrate"));

        gridLayout_3->addWidget(ui_comboBox_baudrate, 1, 1, 1, 1);

        ui_comboBox_port = new QComboBox(layoutWidget6);
        ui_comboBox_port->setObjectName(QStringLiteral("ui_comboBox_port"));

        gridLayout_3->addWidget(ui_comboBox_port, 0, 1, 1, 1);

        ui_button_disconnect = new QPushButton(layoutWidget6);
        ui_button_disconnect->setObjectName(QStringLiteral("ui_button_disconnect"));

        gridLayout_3->addWidget(ui_button_disconnect, 0, 3, 1, 1);


        gridLayout_8->addWidget(ui_group_port, 0, 1, 1, 1);


        horizontalLayout_9->addLayout(gridLayout_8);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);


        gridLayout_7->addLayout(horizontalLayout_9, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_7, 0, 2, 1, 1);

        ui_webView = new QWebView(layoutWidget);
        ui_webView->setObjectName(QStringLiteral("ui_webView"));
        sizePolicy3.setHeightForWidth(ui_webView->sizePolicy().hasHeightForWidth());
        ui_webView->setSizePolicy(sizePolicy3);
        ui_webView->setMinimumSize(QSize(700, 1000));
        ui_webView->setMaximumSize(QSize(700, 800));
        ui_webView->setProperty("url", QVariant(QUrl(QStringLiteral("about:blank"))));

        gridLayout_4->addWidget(ui_webView, 0, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_9, 0, 1, 1, 1);

        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(30, 40, 361, 111));
        label_11->setPixmap(QPixmap(QString::fromUtf8("../19_5_logo.png")));
        label_11->setScaledContents(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1920, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
#ifndef QT_NO_ACCESSIBILITY
        MainWindow->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_STATUSTIP
        label->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("MainWindow", "Ground Control System For KRAC", 0));
        label_2->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Flight Data", 0));
        label_4->setText(QApplication::translate("MainWindow", "Flight Mode", 0));
        label_5->setText(QApplication::translate("MainWindow", "To Waypoint", 0));
        label_6->setText(QApplication::translate("MainWindow", "Latitude", 0));
        label_7->setText(QApplication::translate("MainWindow", "Longitude", 0));
        label_8->setText(QApplication::translate("MainWindow", "Altitude", 0));
        label_9->setText(QApplication::translate("MainWindow", "Battery - Odroid", 0));
        label_10->setText(QApplication::translate("MainWindow", "Battery - Pixhawk", 0));
        ui_group_GPS_Point->setTitle(QApplication::translate("MainWindow", "GPS Point", 0));
        ui_label_ep->setText(QApplication::translate("MainWindow", "EP", 0));
        ui_line_ep_lat->setText(QString());
        ui_line_ep_lng->setText(QString());
        ui_label_dp->setText(QApplication::translate("MainWindow", "DP", 0));
        ui_line_dp_lat->setText(QString());
        ui_line_dp_lng->setText(QString());
        ui_label_wp->setText(QApplication::translate("MainWindow", "WP", 0));
        ui_line_wp_lat->setText(QString());
        ui_line_wp_lng->setText(QString());
        ui_label_sp->setText(QApplication::translate("MainWindow", "SP", 0));
        ui_line_sp_lat->setText(QString());
        ui_line_sp_lng->setText(QString());
        ui_check_ep->setText(QApplication::translate("MainWindow", "Same SP", 0));
        ui_label_np->setText(QApplication::translate("MainWindow", "NP", 0));
        ui_line_np_lat->setText(QApplication::translate("MainWindow", "37.170000 ", 0));
        ui_line_np_lng->setText(QApplication::translate("MainWindow", "128.472250", 0));
        ui_line_np_rad->setText(QApplication::translate("MainWindow", "30", 0));
        ui_button_GPSdisplay->setText(QApplication::translate("MainWindow", "Display", 0));
        ui_button_GPSdisplay->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0));
        ui_button_GPSreset->setText(QApplication::translate("MainWindow", "Reset", 0));
        ui_button_GPSreset->setShortcut(QString());
        ui_group_Emergency->setTitle(QApplication::translate("MainWindow", "Emergency", 0));
        ui_check_emerge_1->setText(QString());
        ui_button_auto->setText(QApplication::translate("MainWindow", "AUTO.", 0));
        ui_button_manual->setText(QApplication::translate("MainWindow", "MANUAL", 0));
        ui_check_emerge_2->setText(QString());
        ui_button_takeoff->setText(QApplication::translate("MainWindow", "Arm", 0));
        ui_button_landing->setText(QApplication::translate("MainWindow", "Disarm", 0));
        ui_group_Gripper->setTitle(QApplication::translate("MainWindow", "Gripper", 0));
        ui_line_gripper->setText(QApplication::translate("MainWindow", "125", 0));
        ui_label_gripper->setText(QApplication::translate("MainWindow", "Position", 0));
        ui_group_toque->setTitle(QApplication::translate("MainWindow", "Torque", 0));
        ui_radio_on->setText(QApplication::translate("MainWindow", "ON", 0));
        ui_radio_off->setText(QApplication::translate("MainWindow", "OFF", 0));
        ui_group_port->setTitle(QApplication::translate("MainWindow", "Comport connect", 0));
        ui_label_baudrate->setText(QApplication::translate("MainWindow", "Baudrate", 0));
        ui_button_scan->setText(QApplication::translate("MainWindow", "Scan", 0));
        ui_label_port->setText(QApplication::translate("MainWindow", "Port", 0));
        ui_button_connect->setText(QApplication::translate("MainWindow", "Connect", 0));
        ui_button_disconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        label_11->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
