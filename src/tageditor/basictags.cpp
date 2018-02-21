#include <QAbstractItemModel>
#include <QButtonGroup>
#include <QDataWidgetMapper>

#include "autonumbers.hpp"
#include "basictags.hpp"
#include "metadatachangeindicatordelegate.hpp"
#include "ui_basictags.h"

#include <QDebug>

namespace Coquillo {
    namespace TagEditor {
        BasicTags::BasicTags(QWidget * parent)
        : EditorPageBase(parent) {
            _ui = new Ui::BasicTags;
            _ui->setupUi(this);

            _inputMapper = new QDataWidgetMapper(this);
            _inputMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

            _labelMapper = new QDataWidgetMapper(this);
            _labelMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
            _labelMapper->setItemDelegate(new MetaDataChangeIndicatorDelegate);

            setupMappings();

            QButtonGroup * clone_group = new QButtonGroup(this);

            foreach (QWidget * input, findChildren<QWidget*>(QRegExp("^tag"))) {
                if (input->inherits("QLineEdit")) {
                    connect(qobject_cast<QLineEdit*>(input), &QLineEdit::textEdited, _inputMapper, &QDataWidgetMapper::submit);
                } else if (input->inherits("QSpinBox")) {
                    auto emitter = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged);
                    connect(qobject_cast<QSpinBox*>(input), emitter, _inputMapper, &QDataWidgetMapper::submit);
                }
            }

            foreach (QAbstractButton * button, findChildren<QAbstractButton*>(QRegExp("^clone"))) {
                const QString input_name = button->objectName().replace("clone", "tag");
                int column = _inputMapper->mappedSection(findChild<QWidget*>(input_name));
                clone_group->addButton(button, column);
            }

            connect(clone_group, SIGNAL(buttonClicked(int)), SLOT(cloneField(int)));
            connect(_ui->autoNumbers, SIGNAL(clicked()), SLOT(autoNumberItems()));
        }

        BasicTags::~BasicTags() {
            delete _ui;
        }

        void BasicTags::setModel(QAbstractItemModel * model) {
            EditorPageBase::setModel(model);

            if (model) {
                _inputMapper->setModel(model);
                _labelMapper->setModel(model);
                setupMappings();
            }
        }

        void BasicTags::setEditorIndex(const QModelIndex & idx) {
            EditorPageBase::setEditorIndex(idx);

            _inputMapper->submit();
            _inputMapper->setCurrentModelIndex(idx);
            _labelMapper->setCurrentModelIndex(idx);
        }

        void BasicTags::autoNumberItems() {
            AutoNumbers numbers(model());
            numbers.autoNumberItems(selectionModel()->selectedRows());
        }

        void BasicTags::cloneField(int column) {
            const QVariant value = editorIndex().sibling(editorIndex().row(), column).data();

            foreach (const QModelIndex idx, selectionModel()->selectedRows(column)) {
                model()->setData(idx, value);
            }
        }

        void BasicTags::setupMappings() {
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
    }
}
