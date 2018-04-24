
#include "tags/tagdataroles.hpp"
#include "basictags.hpp"
#include "imagetags.hpp"
#include "rawdata.hpp"
#include "tageditor.hpp"

#include <QDebug>

namespace Coquillo {
    namespace TagEditor {
        TagEditor::TagEditor(QWidget * parent)
        : QTabWidget(parent) {
            _tabBasic = new BasicTags(this);
            _tabImages = new ImageTags(this);
            _tabRaw = new RawData(this);

            addTab(_tabBasic, tr("Basic"));
            addTab(_tabImages, tr("Images"));
            addTab(_tabRaw, tr("Raw"));
        }

        QAbstractItemModel * TagEditor::model() const {
            return _tabBasic->model();
        }

        void TagEditor::setModel(QAbstractItemModel * model) {
            _tabBasic->setModel(model);
            _tabImages->setModel(model);
            _tabRaw->setModel(model);
        }

        void TagEditor::setSelectionModel(QItemSelectionModel * model) {
            if (_selection) {
                disconnect(_selection, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                    this, SLOT(setEditorIndex(QModelIndex)));
            }

            if (model) {
                connect(model, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
                    this, SLOT(setEditorIndex(QModelIndex)));
            }

            _selection = model;
            _tabBasic->setSelectionModel(model);
            _tabImages->setSelectionModel(model);
        }

        void TagEditor::setEditorIndex(const QModelIndex & idx) {
            _tabBasic->setEditorIndex(idx);
            _tabImages->setEditorIndex(idx);
            _tabRaw->setEditorIndex(idx);

            _tabBasic->setEnabled(idx.isValid());
            _tabImages->setEnabled(idx.isValid());
            _tabRaw->setEnabled(idx.isValid());

            const QString label = idx.sibling(idx.row(), Tags::ImageField).data().toString();

            if (label.isEmpty() || label[0] == '0') {
                setTabText(1, tr("Images"));
            } else {
                setTabText(1, label);
            }
        }
    }
}
