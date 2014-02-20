
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QDebug>
#include "basictags.h"
#include "metadatachangeindicatordelegate.h"
#include "ui_basictags.h"

namespace Coquillo {
    namespace TagEditor {
        BasicTags::BasicTags(QWidget * parent)
        : QWidget(parent) {
            _ui = new Ui::BasicTags;
            _ui->setupUi(this);
            _inputMapper = new QDataWidgetMapper(this);
            _inputMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

            _labelMapper = new QDataWidgetMapper(this);
            _labelMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
            _labelMapper->setItemDelegate(new MetaDataChangeIndicatorDelegate);

            QList<QWidget*> inputs = findChildren<QWidget*>(QRegExp("^tag"));

            foreach (QWidget * input, inputs) {
                if (input->inherits("QLineEdit")) {
                    connect(input, SIGNAL(textChanged(QString)), _inputMapper, SLOT(submit()));
                } else if (input->inherits("QSpinBox")) {
                    connect(input, SIGNAL(valueChanged(int)), _inputMapper, SLOT(submit()));
                }
            }
        }

        BasicTags::~BasicTags() {
            delete _ui;
        }

        void BasicTags::setModel(QAbstractItemModel * model) {
            _inputMapper->setModel(model);
            _inputMapper->addMapping(_ui->tagTitle, 1);
            _inputMapper->addMapping(_ui->tagArtist, 2);
            _inputMapper->addMapping(_ui->tagAlbum, 3);
            _inputMapper->addMapping(_ui->tagGenre, 4);
            _inputMapper->addMapping(_ui->tagComment, 5);
            _inputMapper->addMapping(_ui->tagYear, 6);
            _inputMapper->addMapping(_ui->tagNumber, 7);
            _inputMapper->addMapping(_ui->tagTotal, 8);
            _inputMapper->addMapping(_ui->tagDisc, 9);
            _inputMapper->addMapping(_ui->tagOriginalArtist, 10);
            _inputMapper->addMapping(_ui->tagAlbumArtist, 11);
            _inputMapper->addMapping(_ui->tagComposer, 12);
            _inputMapper->addMapping(_ui->tagUrl, 13);
            _inputMapper->addMapping(_ui->tagEncoder, 14);

            _labelMapper->setModel(model);
            _labelMapper->addMapping(_ui->labelTitle, 1);
            _labelMapper->addMapping(_ui->labelArtist, 2);
            _labelMapper->addMapping(_ui->labelAlbum, 3);
            _labelMapper->addMapping(_ui->labelGenre, 4);
            _labelMapper->addMapping(_ui->labelComment, 5);
            _labelMapper->addMapping(_ui->labelYear, 6);
            _labelMapper->addMapping(_ui->labelNumber, 7);
            _labelMapper->addMapping(_ui->labelTotal, 8);
            _labelMapper->addMapping(_ui->labelDisc, 9);
            _labelMapper->addMapping(_ui->labelOriginalArtist, 10);
            _labelMapper->addMapping(_ui->labelAlbumArtist, 11);
            _labelMapper->addMapping(_ui->labelComposer, 12);
            _labelMapper->addMapping(_ui->labelUrl, 13);
            _labelMapper->addMapping(_ui->labelEncoder, 14);
        }

        QAbstractItemModel * BasicTags::model() const {
            return _inputMapper->model();
        }

        void BasicTags::setCurrentIndex(const QModelIndex & idx) {
            _inputMapper->setCurrentModelIndex(idx);
            _labelMapper->setCurrentModelIndex(idx);
            setEnabled(idx.isValid());
        }
    }
}
