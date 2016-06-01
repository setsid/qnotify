/*
    qnotify - utility that shows a text message on a screen.

    Copyright (C) 2015-2016 Vadim Kuznetsov <vimusov@gmail.com>

    yacron is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    yacron is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with yacron.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui>
#include <QtWidgets>
#include <stdlib.h>
#include <string.h>


#define WIDTH (300)  // window width
#define HEIGHT (100)  // window height
#define TIMEOUT (10)  // how many seconds to show the window


class CustomLabel: public QLabel
{
protected:
    void mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton)
            QApplication::quit();
        else
            QLabel::mouseReleaseEvent(event);
    }
};


class NotifyWindow: public QMainWindow
{
private:
    CustomLabel message_label;

public:
    NotifyWindow()
    : QMainWindow(0, Qt::SplashScreen | Qt::WindowStaysOnTopHint) {

        QFont font = message_label.font();
        font.setBold(true);
        font.setPointSize(12);
        message_label.setFont(font);

        message_label.setWordWrap(true);
        message_label.setFrameShape(QFrame::WinPanel);
        message_label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        message_label.setGeometry(0, 0, WIDTH, HEIGHT);

        setCentralWidget(&message_label);

        setStyleSheet("background-color: white;");
    }

    void show(const char* message) {
        message_label.setText(QString::fromUtf8(message));

        // resize and move the window to the top right corner
        resize(WIDTH, HEIGHT);
        move(QDesktopWidget().screenGeometry().width() - WIDTH, 0);

        QMainWindow::show();

        startTimer(1000);
    }

protected:
     void timerEvent(QTimerEvent*) {
         static int timeout = 0;
         if (timeout++ > TIMEOUT)
             QApplication::quit();
     }
};


int main(int argc, char** argv)
{
    if (argc != 2) {
        qCritical("Usage: qnotify <message>");
        return EXIT_FAILURE;
    }

    const char *display = getenv("DISPLAY");
    if (display == nullptr || !strlen(display))
        setenv("DISPLAY", ":0", 1);

    QApplication app(argc, argv);

    NotifyWindow win;
    win.show(argv[1]);

    return app.exec();
}
