
#include "test_autonumbers.hpp"
#include "tags/tagdataroles.hpp"

namespace Test {
    using namespace Coquillo::TagEditor::NumberStrategy;

    void AutoNumbers::initTestCase() {
        // _model = new TestTagsModel(nullptr);
    }

    void AutoNumbers::itemOrderStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = ItemOrderStrategy().suggestions(items);

        QVERIFY(result[0] == 1);
        QVERIFY(result[1] == 2);
        QVERIFY(result[2] == 3);
        QVERIFY(result[3] == 4);
        QVERIFY(result[4] == 5);
    }

    void AutoNumbers::fileNumberStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = FileNumberStrategy().suggestions(items);

        QVERIFY(result[0] == 1);
        QVERIFY(result[1] == 2);
        QVERIFY(result[2] == 11);
        QVERIFY(result[3] == 5);
        QVERIFY(result[4] == 8);
    };

    void AutoNumbers::originalNumbersStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = PreserveOriginalNumbers().suggestions(items);

        QVERIFY(result[0] == 1);
        QVERIFY(result[1] == 2);
        QVERIFY(result[4] == 8);
        QVERIFY(result.contains(2) == false);
        QVERIFY(result.contains(3) == false);
    }

    void AutoNumbers::trackNumberingTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        Coquillo::TagEditor::AutoNumbers::autoNumberItems(model, items);

        int col = Coquillo::Tags::NumberField;
        QVERIFY(model->index(0, col).data().toInt() == 1);
        QVERIFY(model->index(1, col).data().toInt() == 2);
        QVERIFY(model->index(2, col).data().toInt() == 11);
        QVERIFY(model->index(3, col).data().toInt() == 5);
        QVERIFY(model->index(4, col).data().toInt() == 8);
    }

    QList<QVariantHash> AutoNumbers::testData() const {
        QList<QVariantHash> items;

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/01. file1.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "My First Track"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                        {"TRCK", 1},
                        // {"TRCK[b]", 10},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/02 - file2.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Song Number Two"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                        {"TRCK", 2},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/11. file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Outro"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/5. file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Composition #5"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/8 - file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Octa"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                        {"TRCK", 8},
                    })}
                })}
            }
        }));

        return items;
    }

    void TestTagsModel::initializeTestData(const QList<QVariantHash> & items) {
        store().add(items);
    }
}

QTEST_MAIN(Test::AutoNumbers)
