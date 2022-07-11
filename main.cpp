#include "clientWindow.h"
#include <QApplication>

#define IP "127.0.0.1"
#define PORT 2323

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ClientWindow window(IP, PORT);

    window.show();

    return app.exec();
}
