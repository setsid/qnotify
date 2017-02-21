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

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

/* Check DISPLAY variable and set it if needed. */
static int set_display_variable(void)
{
    const char *value = getenv("DISPLAY");

    if (value != nullptr && *value != '\0')
        return 0;

    const int ret = setenv("DISPLAY", ":0.0", 1);

    if (ret)
        fprintf(stderr, "setenv(DISPLAY): error '%m'\n");

    return ret;
}

/* Check XAUTHORITY variable and set it if needed. */
static int set_xauthority_variable(void)
{
    const char *value = getenv("XAUTHORITY");

    if (value != nullptr && !access(value, R_OK))
        return 0;

    const uid_t uid = getuid();

    const struct passwd *const user = getpwuid(uid);

    if (user == nullptr) {
        fprintf(stderr, "getpwuid(%u): error '%m'\n", uid);
        return 1;
    }

    char file_path[128];

    snprintf(file_path, sizeof(file_path), "%s/.Xauthority", user->pw_dir);

    if (access(file_path, R_OK)) {
        fprintf(stderr, "access('%s'): error '%m'\n", file_path);
        return 1;
    }

    const int ret = setenv("XAUTHORITY", file_path, 1);

    if (ret)
        fprintf(stderr, "setenv(XAUTHORITY): error '%m'\n");

    return ret;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: qnotify <message>\n");
        return EXIT_FAILURE;
    }

    if (set_display_variable())
        return EXIT_FAILURE;

    if (set_xauthority_variable())
        return EXIT_FAILURE;

    QApplication app(argc, argv);

    NotifyWindow win;
    win.show(argv[1]);

    return app.exec();
}
