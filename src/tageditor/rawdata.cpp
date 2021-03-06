#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "tags/tagcontainer.hpp"
#include "tags/tagdataroles.hpp"
#include "rawdata.hpp"
#include "ui_rawdata.h"

#include <QDebug>

namespace Coquillo {
    namespace TagEditor {
        RawData::RawData(QWidget * parent)
        : QWidget(parent), _model(0) {
            _ui = new Ui::RawData;
            _ui->setupUi(this);

            auto model = new QStandardItemModel;
            model->setColumnCount(2);
            model->setHorizontalHeaderLabels({ tr("Field"), tr("Value") });
            auto proxy = new QSortFilterProxyModel;
            proxy->setSourceModel(model);
            proxy->sort(0);
            _ui->treeView->setModel(proxy);
        }

        RawData::~RawData() {
            delete _ui;
        }

        Tags::Container RawData::tagContainer() const {
            return qvariant_cast<Tags::Container>(_current.data(Tags::ContainerRole));
        }

        void RawData::onDataChanged(const QModelIndex & tl, const QModelIndex & br) {
            if (tl.row() <= _current.row() and br.row() >= _current.row()) {
                setEditorIndex(_current);
            }
        }

        void RawData::selectTag(const QString & name) {
            auto model = treeModel();
            model->removeRows(0, model->rowCount());

            const auto data = tagContainer();
            const auto tag = data.tag(name);

            foreach (const QString & key, tag.keys()) {
                const QVariantList values = tag.all(key);
                auto label_item = new QStandardItem(key);
                auto value_item = new QStandardItem;

                if (values.count() <= 1) {
                    value_item->setText(values.value(0).toString());
                } else {
                    value_item->setText(tr("%1 values").arg(values.count()));
                    QFont font = value_item->font();
                    font.setItalic(true);
                    value_item->setFont(font);

                    foreach (const QVariant & value, values) {
                        label_item->appendRow({ new QStandardItem, new QStandardItem(value.toString()) });
                    }
                }

                model->appendRow({label_item, value_item});
            }
        }

        void RawData::setEditorIndex(const QModelIndex & idx) {
            _current = idx;
            const auto data = tagContainer();
            _ui->comboBox->clear();
            _ui->comboBox->addItems(data.tagNames());
            _ui->comboBox->setCurrentText(data.primaryTag());
            _ui->comboBox->setEnabled(data.tags().count() > 1);
        }

        void RawData::setModel(QAbstractItemModel * model) {
            if (_model) {
                disconnect(_model, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                    this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
            }
            if (model) {
                connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                    this, SLOT(onDataChanged(QModelIndex, QModelIndex)));
            }
            _model = model;
        }

        QStandardItemModel * RawData::treeModel() const {
            auto proxy = qobject_cast<QSortFilterProxyModel*>(_ui->treeView->model());
            return qobject_cast<QStandardItemModel*>(proxy->sourceModel());
        }
    }
}
