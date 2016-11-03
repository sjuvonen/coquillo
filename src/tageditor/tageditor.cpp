#include "basictags.hpp"
#include "imagetags.hpp"
#include "rawdata.hpp"
#include "tageditor.hpp"

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
            // _tabBasic->setRole()
            _tabBasic->setModel(model);
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
        }

        void TagEditor::setEditorIndex(const QModelIndex & idx) {
            _tabBasic->setEditorIndex(idx);
            _tabBasic->setEnabled(idx.isValid());

            _tabRaw->setEditorIndex(idx);
            _tabRaw->setEnabled(idx.isValid());
        }
    }
}
