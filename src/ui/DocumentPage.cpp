#include "DocumentPage.h"

#include <QTextCursor>
#include <QTextBlockFormat>
#include <QHBoxLayout>
#include <QWidget>

#include <QVBoxLayout>

namespace {
    int bodyPointSize = 18;
    int titlePointSize = 45;
    int contentMargin = 20;
    int pageTopMargin = 28;
    int pageMaxWidth = 760;
    int listIndentWidth = 31;
    int bodyLineHeightPercent = 130;
}

DocumentPage::DocumentPage(QWidget *parent)
    : QWidget(parent)
{
    createEditor();
    applyPagePalette();
    createTitleEdit();
    buildLayout();

    connect(m_titleEdit, &QLineEdit::returnPressed, m_editor, [this] {
    m_editor->setFocus();
  });
}

void DocumentPage::createEditor()
{
    m_editor = new MarkdownEditor;
    m_editor->setFrameShape(QFrame::NoFrame);
    auto *doc = m_editor->document();

    auto bodyFont = doc->defaultFont();
    bodyFont.setFamily(QStringLiteral("PingFang SC"));
    bodyFont.setPointSize(18);
    doc->setDefaultFont(bodyFont);
    
    doc->setDocumentMargin(contentMargin);
    doc->setIndentWidth(31);

    auto cursor = m_editor->textCursor();
    auto blockFormat = cursor.blockFormat();
    blockFormat.setLineHeight(130, QTextBlockFormat::ProportionalHeight);
    cursor.setBlockFormat(blockFormat);

    doc->clearUndoRedoStacks();
}

void DocumentPage::applyPagePalette()
{
    auto editorPalette = m_editor->palette();
    if (editorPalette.color(QPalette::Base).lightness()>128) {
      editorPalette.setColor(QPalette::Base, QColor("#FAFAFA"));
      editorPalette.setColor(QPalette::Text, QColor("#292929"));
      m_editor->setPalette(editorPalette);
    }

    auto containerPalette = palette();
    containerPalette.setBrush(QPalette::Window, m_editor->palette().brush(QPalette::Base));
    setPalette(containerPalette);
    setAutoFillBackground(true);
}

void DocumentPage::createTitleEdit()
{
    m_titleEdit = new QLineEdit;

    auto titleFont = m_editor->document()->defaultFont();
    titleFont.setPointSize(45);
    titleFont.setWeight(QFont::Bold);
    m_titleEdit->setFont(titleFont);

    m_titleEdit->setPalette(m_editor->palette());
    m_titleEdit->setPlaceholderText(QStringLiteral("输入题目"));
    m_titleEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_titleEdit->setFrame(false);
    m_titleEdit->setTextMargins(20,0,20,0);
}

void DocumentPage::buildLayout()
{
    
    auto *col = new QWidget;
    col->setMaximumWidth(760);
    
    auto *colLayout = new QVBoxLayout(col);
    colLayout->setContentsMargins(0,28,0,0);
    colLayout->setSpacing(0);

    colLayout->addWidget(m_titleEdit);
    colLayout->addWidget(m_editor, 1);
    
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addWidget(col, 1);
    layout->addStretch();
    
}

void DocumentPage::focusTitle()
{
    m_titleEdit->setFocus();
}