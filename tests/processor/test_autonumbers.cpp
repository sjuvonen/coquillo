
#include "test_autonumbers.hpp"
#include "tags/tagdataroles.hpp"

namespace Test {
    using namespace Coquillo::TagEditor::NumberStrategy;

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

        /*
         * NOTE: AutoNumbers does not specifically require to use the right column for indices,
         * but this is done in order to simplify result validation below.
         */
        const QModelIndexList items = {
            model->index(0, Coquillo::Tags::NumberField),
            model->index(1, Coquillo::Tags::NumberField),
            model->index(2, Coquillo::Tags::NumberField),
            model->index(3, Coquillo::Tags::NumberField),
            model->index(4, Coquillo::Tags::NumberField),
        };

        Coquillo::TagEditor::AutoNumbers::autoNumberItems(model, items);

        QVERIFY(items[0].data() == 1);
        QVERIFY(items[1].data() == 2);
        QVERIFY(items[2].data() == 11);
        QVERIFY(items[3].data() == 5);
        QVERIFY(items[4].data() == 8);
    }

    void AutoNumbers::complexTrackNumberingTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(complexTestData());

        /*
         * NOTE: AutoNumbers does not specifically require to use the right column for indices,
         * but this is done in order to simplify result validation below.
         */
        const QModelIndexList items = {
            model->index(0, Coquillo::Tags::NumberField),
            model->index(1, Coquillo::Tags::NumberField),
            model->index(2, Coquillo::Tags::NumberField),
            model->index(3, Coquillo::Tags::NumberField),
            model->index(4, Coquillo::Tags::NumberField),
            model->index(5, Coquillo::Tags::NumberField),
            model->index(6, Coquillo::Tags::NumberField),
            model->index(7, Coquillo::Tags::NumberField),
        };

        Coquillo::TagEditor::AutoNumbers::autoNumberItems(model, items);

        QVERIFY(items[0].data() == 1);
        QVERIFY(items[1].data() == 4);
        QVERIFY(items[2].data() == 3);
        QVERIFY(items[3].data() == 2);
        QVERIFY(items[4].data() == 11);
        QVERIFY(items[5].data() == 5);
        QVERIFY(items[6].data() == 8);
        QVERIFY(items[7].data() == 11);
    }

    void AutoNumbers::fileDiscNumberStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = FileNumberStrategy(FileNumberStrategy::DiscNumberMode).suggestions(items);

        QVERIFY(result[0] == 0);
        QVERIFY(result[1] == 1);
        QVERIFY(result[2] == 2);
        QVERIFY(result[3] == 2);
        QVERIFY(result[4] == 1);
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
                {"path", "/test/music/foobar/102_file2.mp3"},
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
                {"path", "/test/music/foobar/211. file.mp3"},
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
                {"path", "/test/music/foobar/205. file.mp3"},
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
                {"path", "/test/music/foobar/108 - file.mp3"},
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

    QList<QVariantHash> AutoNumbers::complexTestData() const {
        QList<QVariantHash> items = testData();

        items.insert(1, QVariantHash({
            {
                {"path", "/test/music/another/04 - file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Demo #4"},
                        {"TALB", "Second Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.insert(2, QVariantHash({
            {
                {"path", "/test/music/another/3. file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Demo #3"},
                        {"TALB", "Second Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.insert(7, QVariantHash({
            {
                {"path", "/test/music/another/11_file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Demo #11"},
                        {"TALB", "Second Album"},
                        {"TCON", "Techno"},
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
