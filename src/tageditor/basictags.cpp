#include <QAbstractItemModel>
#include <QButtonGroup>
#include <QDataWidgetMapper>

#include "basictags.hpp"
#include "metadatachangeindicatordelegate.hpp"
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

            QButtonGroup * clone_group = new QButtonGroup(this);
            setModel(0);

            foreach (QWidget * input, findChildren<QWidget*>(QRegExp("^tag"))) {
                if (input->inherits("QLineEdit")) {
                    connect(input, SIGNAL(textChanged(QString)), _inputMapper, SLOT(submit()));
                } else if (input->inherits("QSpinBox")) {
                    connect(input, SIGNAL(valueChanged(int)), _inputMapper, SLOT(submit()));
                }
            }

            foreach (QAbstractButton * button, findChildren<QAbstractButton*>(QRegExp("^clone"))) {
                const QString input_name = button->objectName().replace("clone", "tag");
                int column = _inputMapper->mappedSection(findChild<QWidget*>(input_name));
                clone_group->addButton(button, column);
            }

            connect(clone_group, SIGNAL(buttonClicked(int)), SLOT(emitCloneField(int)));
            connect(_ui->autoNumbers, SIGNAL(clicked()), SIGNAL(autoNumberingClicked()));
        }

        BasicTags::~BasicTags() {
            delete _ui;
        }

        void BasicTags::setModel(QAbstractItemModel * model) {
            if (model) {
                _inputMapper->setModel(model);
                _labelMapper->setModel(model);
            }

            _inputMapper->addMapping(_ui->tagTitle, 0);
            _inputMapper->addMapping(_ui->tagArtist, 1);
            _inputMapper->addMapping(_ui->tagAlbum, 2);
            _inputMapper->addMapping(_ui->tagGenre, 3);
            _inputMapper->addMapping(_ui->tagComment, 4);
            _inputMapper->addMapping(_ui->tagYear, 5);
            _inputMapper->addMapping(_ui->tagNumber, 6);
            _inputMapper->addMapping(_ui->tagTotal, 7);
            _inputMapper->addMapping(_ui->tagDisc, 8);
            _inputMapper->addMapping(_ui->tagOriginalArtist, 9);
            _inputMapper->addMapping(_ui->tagAlbumArtist, 10);
            _inputMapper->addMapping(_ui->tagComposer, 11);
            _inputMapper->addMapping(_ui->tagUrl, 12);
            _inputMapper->addMapping(_ui->tagEncoder, 13);

            _labelMapper->addMapping(_ui->labelTitle, 0);
            _labelMapper->addMapping(_ui->labelArtist, 1);
            _labelMapper->addMapping(_ui->labelAlbum, 2);
            _labelMapper->addMapping(_ui->labelGenre, 3);
            _labelMapper->addMapping(_ui->labelComment, 4);
            _labelMapper->addMapping(_ui->labelYear, 5);
            _labelMapper->addMapping(_ui->labelNumber, 6);
            _labelMapper->addMapping(_ui->labelTotal, 7);
            _labelMapper->addMapping(_ui->labelDisc, 8);
            _labelMapper->addMapping(_ui->labelOriginalArtist, 9);
            _labelMapper->addMapping(_ui->labelAlbumArtist, 10);
            _labelMapper->addMapping(_ui->labelComposer, 11);
            _labelMapper->addMapping(_ui->labelUrl, 12);
            _labelMapper->addMapping(_ui->labelEncoder, 13);
        }

        QAbstractItemModel * BasicTags::model() const {
            return _inputMapper->model();
        }

        void BasicTags::setEditorIndex(const QModelIndex & idx) {
            _inputMapper->setCurrentModelIndex(idx);
            _labelMapper->setCurrentModelIndex(idx);

            // _ui->audioProperties->setData(idx.data(MetaData::AudioPropertiesRole).toMap());
        }

        void BasicTags::emitCloneField(int column) {
            const QVariant value = _inputMapper->model()->index(_inputMapper->currentIndex(), column).data();
            emit cloneField(column, value);
        }
    }
}
