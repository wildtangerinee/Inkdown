#include <QApplication>
#include <QMainWindow>

#include "editor/MarkdownEditor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.show();

    auto *editor = new MarkdownEditor;
    window.setCentralWidget(editor);

    return app.exec();
}
