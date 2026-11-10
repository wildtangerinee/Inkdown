#pragma once

#include <QTextEdit>

class QKeyEvent;

class MarkdownEditor : public QTextEdit {
protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void tryHandleHeading();
    void tryHandleBold();
    void tryHandleUnorderedList();
    bool tryExitEmptyHeading();
};
