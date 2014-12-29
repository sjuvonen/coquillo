
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "metadata/metadatamodel.h"
#include "rawdata.h"
#include "ui_rawdata.h"

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
            _ui->treeView->setModel(proxy);
        }

        RawData::~RawData() {
            delete _ui;
        }

        void RawData::setModel(QAbstractItemModel * model) {
            _model = model;
        }

        void RawData::setCurrentIndex(const QModelIndex & idx) {
            _current = idx;
            const auto data = metaData();
            _ui->comboBox->clear();
            _ui->comboBox->addItems(data.tags().keys());
            _ui->comboBox->setCurrentText(data.primaryTagName());
            _ui->comboBox->setEnabled(data.tags().count() > 1);
        }

        void RawData::selectTag(const QString & name) {
            auto model = treeModel();
            model->removeRows(0, model->rowCount());

            const MetaData::MetaData data = metaData();
            const MetaData::Tag tag = data.tag(name);

            foreach (const QString & key, tag.keys()) {
                QVariantList values = tag[key];
                auto label_item = new QStandardItem(key);
                auto value_item = new QStandardItem;

                if (values.count() > 1) {
                    value_item->setText(tr("%1 values").arg(values.count()));
                } else {
                    value_item->setText(values.value(0).toString());

                    foreach (const QVariant & value, values) {
                        value_item->appendRow(new QStandardItem(value.toString()));
                    }
                }

                model->appendRow((QList<QStandardItem*>){label_item, value_item});
            }
        }

        QStandardItemModel * RawData::treeModel() const {
            auto proxy = qobject_cast<QSortFilterProxyModel*>(_ui->treeView->model());
            return qobject_cast<QStandardItemModel*>(proxy->sourceModel());
        }

        MetaData::MetaData RawData::metaData() const {
            int role = MetaData::MetaDataModel::MetaDataRole;
            return _current.data(role).value<MetaData::MetaData>();
        }
    }
}
