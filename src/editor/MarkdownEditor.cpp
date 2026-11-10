#include "MarkdownEditor.h"

#include <QDebug>
#include <QKeyEvent>
#include <QTextBlock>
#include <QTextCursor>

namespace {

// start 是文档中的起点，len 是选区长度（两者都按 UTF-16 代码单元计）。
// 通过引用在传入的光标上建立选区；这里不删除文字，也不设置格式。
void selectRange(QTextCursor &cursor, int start, int len)
{
    // 先把光标和锚点一起移到起点，再保留锚点，把光标移到选区末尾。
    cursor.setPosition(start);
    cursor.setPosition(start + len, QTextCursor::KeepAnchor);
}

} // namespace

void MarkdownEditor::keyPressEvent(QKeyEvent *event)
{
    // QTextEdit::keyPressEvent(QKeyEvent *) 返回 void，负责 Qt 原有的编辑行为。
    // 先调用它再检查文档，才能看到这次按键插入的空格；按键类型从 event 读取。
    QTextEdit::keyPressEvent(event);
    if (event->text() == "*") {
        tryHandleBold();
    } else if (event->key() == Qt::Key_Space) {
        tryHandleHeading();
        tryHandleUnorderedList();
    }
}

void MarkdownEditor::tryHandleHeading()
{
    // textCursor() 返回当前光标的副本。移动副本可建立选区，不必先移动界面上的光标；
    // 用副本删除内容或设置格式，仍会修改同一份文档。
    auto cursor = textCursor();
    if (!cursor.atBlockEnd()) {
        return;
    }

    // block() 取的是文档段落，窗口太窄造成的视觉折行不会把它拆成多个段落。
    // 要求光标在段尾且整段恰好是标题标记，因此正文中的 # 和 ### 都会原样保留。
    const auto bText = cursor.block().text();
    int hLevel;
    qreal pSize;
    if (bText == "# ") {
        hLevel = 1;
        pSize = 20;
    } else if (bText == "## ") {
        hLevel = 2;
        pSize = 16;
    } else {
        return;
    }

    // movePosition(op, mode) 返回是否移动成功。StartOfBlock 指定移到段首，
    // KeepAnchor 保留原来的段尾作为选区另一端，因此只选中标记和末尾空格。
    if (!cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor)) {
        return;
    }
    // 删除标记和设置标题级别一起撤销；触发转换的空格仍属于原始输入。
    cursor.beginEditBlock();
    cursor.removeSelectedText();

    // blockFormat() 返回段落格式的副本；修改后要通过 setBlockFormat() 写回。
    // 标题级别记录「这是几级标题」，将来导出时可以据此还原 # 或 ##。
    auto bf = cursor.blockFormat();
    bf.setHeadingLevel(hLevel);
    cursor.setBlockFormat(bf);
    cursor.endEditBlock();

    // 标题级别本身不会让这里输入的文字变大。此时段落为空，
    // setCurrentCharFormat() 设置接下来输入的字符格式，字号单位是磅。
    auto cf = currentCharFormat();
    cf.setFontPointSize(pSize);
    setCurrentCharFormat(cf);
}

void MarkdownEditor::tryHandleBold()
{
    // 入口已经让 QTextEdit 插入了本次按键。这里只检查当前段尾，
    // 暂不处理在段落中间补写闭合标记的情况。
    auto cursor = textCursor();
    if (!cursor.atBlockEnd()) {
        return;
    }

    const auto bText = cursor.block().text();
    if (!bText.endsWith("**")) {
        return;
    }
    // 两个下标都从段首数，分别指向各自 ** 的第一个星号。
    // 搜索开头标记时先排除闭合标记；openPos + 2 是正文起点。
    auto closePos = bText.size() - 2;
    auto openPos = bText.left(closePos).lastIndexOf("**");
    if (openPos == -1 || openPos + 2 >= closePos) {
        return;
    }

    // 先确认前一个字符存在。遇到转义或连续星号时保留原文；
    // 目前只处理简单的粗体，不尝试解析嵌套格式。
    if (openPos > 0) {
        if (bText.at(openPos - 1) == '\\' || bText.at(openPos - 1) == '*') {
            return;
        }
    }

    // 正文检查也要覆盖段首的粗体，因此放在 openPos > 0 分支之外。
    // 保留正文长度，删掉标记后会用它来建立粗体选区。
    auto str = bText.mid(openPos + 2, closePos - (openPos + 2));
    if (str.contains('\\') || str.contains('*')) {
        return;
    }

    // 在修改文档前保存当前输入格式，转换后用它恢复后续输入的样式。
    const auto of = currentCharFormat();

    // QString 下标是段内偏移；selectRange() 要的是文档位置。
    // 加上段首位置得到标记的实际位置。先删右边，左边的位置不会变。
    const auto bPos = cursor.block().position();
    const auto closeStart = bPos + static_cast<int>(closePos);
    selectRange(cursor, closeStart, 2);
    // 删除两端标记和加粗正文一起撤销，避免只恢复一边的星号。
    cursor.beginEditBlock();
    cursor.removeSelectedText();

    // 删除左边标记后，正文移到 openStart，但正文长度不变。
    const auto openStart = bPos + static_cast<int>(openPos);
    selectRange(cursor, openStart, 2);
    cursor.removeSelectedText();

    // 只合并字重，避免把标题字号等其他字符属性覆盖掉。
    QTextCharFormat cf;
    cf.setFontWeight(QFont::Bold);

    selectRange(cursor, openStart, static_cast<int>(str.size()));
    cursor.mergeCharFormat(cf);
    cursor.endEditBlock();

    // 不带 KeepAnchor，收起选区并把光标留在粗体末尾，防止下一次输入覆盖正文。
    // 先把光标交回编辑器，再恢复输入格式，后续文字就不会意外继承粗体。
    cursor.setPosition(openStart + static_cast<int>(str.size()));
    setTextCursor(cursor);
    setCurrentCharFormat(of);
}

void MarkdownEditor::tryHandleUnorderedList()
{
    // 空格已由 QTextEdit 写入。只在段尾检查，避免在正文中间输入空格时触发。
    auto cursor = textCursor();
    if (!cursor.atBlockEnd()) {
        return;
    }

    // 后面会选中整段，因此必须确认整段恰好是 "- "，不能只比较后缀。
    // 例如 "abc - " 应保持原样。
    const auto bText = cursor.block().text();
    if (bText != "- ") {
        return;
    }

    // 从段尾移到段首并保留锚点，正好选中连字符和空格。
    // currentList() 在普通段落返回 nullptr；headingLevel() 在正文中返回 0。
    // 先检查完再删除，避免吞掉标题或已有列表项里作为正文输入的 "- "。
    if (!cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor)
        || cursor.currentList()
        || cursor.blockFormat().headingLevel()) {
        return;
    }
    // 检查通过后才开组，确保删除标记和创建列表项一起撤销。
    cursor.beginEditBlock();
    cursor.removeSelectedText();

    // ListDisc 让 Qt 为当前段绘制实心圆点，圆点本身不是文档里的字符。
    // 将光标交回编辑器，下一次输入就从这个空列表项的开头开始。
    cursor.createList(QTextListFormat::ListDisc);
    cursor.endEditBlock();
    setTextCursor(cursor);
}
