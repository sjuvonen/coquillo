
#include <QDebug>

#include "autonumbers.hpp"
#include "basictags.hpp"
#include "imagetags.hpp"
#include "rawdata.hpp"
#include "tageditor.hpp"

namespace Coquillo {
    namespace TagEditor {
        TagEditor::TagEditor(QWidget * parent)
        : QTabWidget(parent), _selmodel(0) {
            _basic = new BasicTags;
            _images = new ImageTags;
            _raw = new RawData;

            addTab(_basic, tr("Basic"));
            addTab(_images, tr("Images"));
            addTab(_raw, tr("Raw"));

            connect(_basic, SIGNAL(autoNumberingClicked()), SLOT(onAutoNumbering()));
            connect(_basic, SIGNAL(cloneField(int, QVariant)), SLOT(onCloneField(int, QVariant)));
            connect(_images, SIGNAL(cloneField(int, QVariant)), SLOT(onCloneField(int, QVariant)));

            setEditorIndex(QModelIndex());
        }

        void TagEditor::onAutoNumbering() {
            AutoNumbers::autoNumberItems(model(), selectionModel()->selectedRows());
        }

        void TagEditor::onCloneField(int column, const QVariant & value) {
            foreach (const QModelIndex & idx, selectionModel()->selectedRows(column)) {
                const_cast<QAbstractItemModel*>(idx.model())->setData(idx, value);
            }
        }

        void TagEditor::setEditorIndex(const QModelIndex & idx) {
            _basic->setEditorIndex(idx);
            _images->setEditorIndex(idx);
            _raw->setEditorIndex(idx);

            _basic->setEnabled(idx.isValid());
            _images->setEnabled(idx.isValid());
            _raw->setEnabled(idx.isValid());

            if (_images->imageCount()) {
                setTabText(1, tr("Images (%1)").arg(_images->imageCount()));
            } else {
                setTabText(1, tr("Images"));
            }
        }

        QAbstractItemModel * TagEditor::model() const {
            return _basic->model();
        }

        void TagEditor::setModel(QAbstractItemModel * model) {
            _basic->setModel(model);
            _images->setModel(model);
        }

        void TagEditor::setSelectionModel(QItemSelectionModel * model) {
            if (_selmodel) {
                disconnect(_selmodel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                    this, SLOT(setEditorIndex(QModelIndex)));
            }

            if (model) {
                connect(model, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                    this, SLOT(setEditorIndex(QModelIndex)));
            }

            _selmodel = model;
        }
    }
}
