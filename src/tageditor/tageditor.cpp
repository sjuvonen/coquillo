
#include "tags/tagdataroles.hpp"
#include "autonumbers.hpp"
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

            connect(_tabBasic, SIGNAL(autoNumberingClicked()), SLOT(autoNumberTracks()));
        }

        QAbstractItemModel * TagEditor::model() const {
            return _tabBasic->model();
        }

        void TagEditor::setModel(QAbstractItemModel * model) {
            // _tabBasic->setRole()
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
        }

        void TagEditor::setEditorIndex(const QModelIndex & idx) {
            _tabBasic->setEditorIndex(idx);
            _tabBasic->setEnabled(idx.isValid());

            _tabImages->setEditorIndex(idx);
            _tabImages->setEnabled(idx.isValid());

            _tabRaw->setEditorIndex(idx);
            _tabRaw->setEnabled(idx.isValid());

            const QString label = idx.sibling(idx.row(), Tags::ImageField).data().toString();

            if (label[0] == "0") {
                setTabText(1, tr("Images"));
            } else {
                setTabText(1, label);
            }

            // if (_tabImages->imageCount()) {
            //     // setTabText(1, tr("Images") + QString(" (%1)").arg(_tabImages->imageCount()));
            //     setTabText(1, idx.sibling(idx.row(), Tags::ImageField).data().toString());
            // } else {
            //     setTabText(1, tr("Images"));
            // }
        }

        void TagEditor::autoNumberTracks() {
            AutoNumbers numbers(model());
            numbers.autoNumberItems(selectionModel()->selectedRows());
        }
    }
}
