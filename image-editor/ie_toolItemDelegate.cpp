#include "ie_toolItemDelegate.h"
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItemV4>


void IE_ToolItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    if (index.column() == 1) {
//        QStyleOptionViewItemV4 opt(option);
//        initStyleOption(&opt, index);
//        QString text = QString("%1").arg(opt.text.toInt(),
//                                         5, 10, QChar('0'));
//        painter->save();
//        painter->setRenderHints(QPainter::Antialiasing|
//                                QPainter::TextAntialiasing);
//        if (opt.state & QStyle::State_Selected) {
//            painter->fillRect(opt.rect, opt.palette.highlight());
//            painter->setPen(opt.palette.highlightedText().color());
//        }
//        else
//            painter->setPen(opt.palette.windowText().color());
//        painter->drawText(opt.rect.adjusted(0, 0, -3, 0), text,
//                QTextOption(Qt::AlignVCenter|Qt::AlignRight));
//        painter->restore();
//    }
//    else
        QStyledItemDelegate::paint(painter, option, index);
}


QWidget *IE_ToolItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{


//        if (index.column() == Zipcode)
//            return new ZipcodeSpinBox(parent);
//        if (index.column() == State) {
//            QComboBox *editor = new QComboBox(parent);
//            editor->addItems(usStates);
//            return editor;
//        }
        return QStyledItemDelegate::createEditor(parent, option, index);
}

void IE_ToolItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//    if (index.column() == Zipcode) {
//           int value = index.model()->data(index).toInt();
//           ZipcodeSpinBox *spinBox =
//                   qobject_cast<ZipcodeSpinBox*>(editor);
//           Q_ASSERT(spinBox);
//           spinBox->setValue(value);
//       }
//       else if (index.column() == State) {
//           QString state = index.model()->data(index).toString();
//           QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
//           Q_ASSERT(comboBox);
//           comboBox->setCurrentIndex(comboBox->findText(state));
//       }
//       else
           QStyledItemDelegate::setEditorData(editor, index);
}

void IE_ToolItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    if (index.column() == Zipcode) {
//            ZipcodeSpinBox *spinBox =
//                    qobject_cast<ZipcodeSpinBox*>(editor);
//            Q_ASSERT(spinBox);
//            spinBox->interpretText();
//            model->setData(index, spinBox->value());
//        }
//        else if (index.column() == State) {
//            QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
//            Q_ASSERT(comboBox);
//            model->setData(index, comboBox->currentText());
//        }
//        else
            QStyledItemDelegate::setModelData(editor, model, index);
}
