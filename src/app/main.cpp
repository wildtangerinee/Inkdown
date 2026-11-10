#include <QApplication>
#include <QMainWindow>


#include "ui/DocumentPage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.resize(900, 650);

    auto *page = new DocumentPage;
    window.setCentralWidget(page);
    window.show();
    page->focusTitle();

    return app.exec();
}
