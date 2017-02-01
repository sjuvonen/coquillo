
#include "test_autonumbers.hpp"

namespace Test {
    void AutoNumbers::initTestCase() {
        // _model = new TestTagsModel(nullptr);
    }

    void AutoNumbers::basicText() {
        auto * model = new TestTagsModel;
        model->initializeTestData();

        const QModelIndexList items = {model->index(0, 0), model->index(0, 1)};

        Coquillo::TagEditor::AutoNumbers::autoNumberItems(model, items);

        qDebug() << "TEST RUN" << model->rowCount();
    };

    void AutoNumbers::extendedTest() {
        qDebug() << "SECOND TEST";
    }

    void TestTagsModel::initializeTestData() {
        const QVariantHash item1 = {
            {
                {"path", "/test/music/scooter/file1.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Scooter"},
                        {"TIT2", "Nessaja"},
                        {"TALB", "24 Carat Gold"},
                        {"TCON", "Techno"},
                        // {"TRCK", 1},
                        // {"TRCK[b]", 10},
                    })}
                })}
            }
        };

        const QVariantHash item2 = {
            {
                {"path", "/test/music/scooter/file2.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Scooter"},
                        {"TIT2", "The Logical Song"},
                        {"TALB", "24 Carat Gold"},
                        {"TCON", "Techno"},
                        {"TRCK", 2},
                        {"TRCK[b]", 10},
                    })}
                })}
            }
        };

        store().add({item1, item2});
    }
}

QTEST_MAIN(Test::AutoNumbers)
