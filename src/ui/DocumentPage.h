#include <QWidget>
#include <QLineEdit>
#include "editor/MarkdownEditor.h"

class DocumentPage : public QWidget {
public:
    explicit DocumentPage(QWidget *parent = nullptr);
    void focusTitle();

private:
    void createEditor();
    void applyPagePalette();
    void createTitleEdit();
    void buildLayout();

    QLineEdit *m_titleEdit = nullptr;
    MarkdownEditor *m_editor = nullptr;

};